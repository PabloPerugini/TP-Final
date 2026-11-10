#include "pruebas.h"
#include "Red.h"
#include "pruebasRedComun.h"
#include <sstream>

void probarRedBicicletas() {
    Red red;
    cargarRedCompleta(red);

    CHECK(red.obtenerAlturaABB() == 12,
          "las 12 bicis cargadas en orden ascendente forman altura 12");
    CHECK(red.contarBicicletasAveriadas() == 2,
          "hay 2 bicicletas averiadas");

    CHECK(red.insertarBicicletaManual(999, "PLZ", TipoBici::Mecanica,
                                      EstadoBici::Averiada, 5000),
          "insertar una bicicleta manual");
    CHECK(red.getCantidadBicicletas() == 13,
          "la red queda con 13 bicicletas");
    CHECK(red.obtenerAlturaABB() == 13,
          "insertar el ID 999 al final alarga la cadena");

    bool encontrada = false;
    std::string datos = capturarSalida(
        [&red, &encontrada]() { encontrada = red.buscarBicicletaPorId(999); });
    CHECK(encontrada, "buscar la bicicleta 999 la encuentra");
    CHECK(datos.find("5000") != std::string::npos,
          "la busqueda muestra los kilometros");

    bool duplicada = true;
    capturarSalida([&red, &duplicada]() {
        duplicada = red.insertarBicicletaManual(
            999, "PLZ", TipoBici::Mecanica, EstadoBici::OK, 10);
    });
    CHECK(!duplicada, "no se puede insertar un ID duplicado");

    bool estacionInexistente = true;
    capturarSalida([&red, &estacionInexistente]() {
        estacionInexistente = red.insertarBicicletaManual(
            1000, "XXX", TipoBici::Mecanica, EstadoBici::OK, 10);
    });
    CHECK(!estacionInexistente, "no se puede insertar en una estacion inexistente");

    bool kilometrosInvalidos = true;
    capturarSalida([&red, &kilometrosInvalidos]() {
        kilometrosInvalidos = red.insertarBicicletaManual(
            1000, "PLZ", TipoBici::Mecanica, EstadoBici::OK, -1);
    });
    CHECK(!kilometrosInvalidos, "no se puede insertar con kilometros negativos");

    std::string inorder =
        capturarSalida([&red]() { red.listarBicicletasInorder(); });
    std::size_t primera = inorder.find("Bici #501");
    std::size_t ultima = inorder.find("Bici #999");
    CHECK(primera != std::string::npos && ultima != std::string::npos &&
              primera < ultima,
          "el inorder empieza en 501 y termina en 999");

    CHECK(red.eliminarBicicletaABB(999), "eliminar la bicicleta 999");
    CHECK(red.getCantidadBicicletas() == 12, "vuelven a quedar 12 bicicletas");
    CHECK(red.obtenerAlturaABB() == 12, "la altura vuelve a 12");
    CHECK(!red.eliminarBicicletaABB(999), "eliminar dos veces devuelve false");
}

void probarRedRedistribucion() {
    Red red;
    cargarRedCompleta(red);

    int soluciones = -1;
    std::string sistema = capturarSalida([&red, &soluciones]() {
        soluciones = red.calcularRedistribucionBacktracking(10);
    });
    CHECK(soluciones == 1,
          "con los faltantes del sistema y C=10 hay una solucion");
    CHECK((sistema.find("{PAL, RET}") != std::string::npos ||
           sistema.find("{RET, PAL}") != std::string::npos),
          "la solucion del sistema es PAL + RET");

    std::istringstream entrada("5 PLZ 3 CAB 7 RET 5 CON 2 BOE 5\n");
    std::streambuf* anterior = std::cin.rdbuf(entrada.rdbuf());
    int manuales = -1;
    std::string manual = capturarSalida([&red, &manuales]() {
        manuales = red.calcularRedistribucionManual(10);
    });
    std::cin.rdbuf(anterior);
    std::cin.clear();

    CHECK(manuales == 4, "el ejemplo manual del PDF tiene 4 soluciones");
    CHECK(manual.find("PLZ, CAB") != std::string::npos &&
              manual.find("RET, BOE") != std::string::npos &&
              manual.find("PLZ, RET, CON") != std::string::npos &&
              manual.find("PLZ, CON, BOE") != std::string::npos,
          "se imprimen las cuatro soluciones del ejemplo");

    std::string estadisticas =
        capturarSalida([&red]() { red.mostrarEstadisticasHash(); });
    CHECK(estadisticas.find("Estaciones cargadas: 10") != std::string::npos &&
              estadisticas.find("Colisiones registradas: 3") !=
                  std::string::npos &&
              estadisticas.find("0.7692") != std::string::npos,
          "las estadisticas tienen los valores pedidos");
}
