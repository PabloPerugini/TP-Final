#include "pruebas.h"
#include "Red.h"
#include <sstream>

namespace {
void cargarRedCompleta(Red& red) {
    std::string salida = capturarSalida([&red]() {
        red.cargarEstaciones("estaciones.txt");
        red.cargarBicicletas("bicicletas.txt");
        red.cargarViajes("viajes.txt");
        red.cargarCiclovias("ciclovias.txt");
    });
    CHECK(salida.find("Estaciones cargadas: 10") != std::string::npos,
          "la carga debe leer 10 estaciones");
    CHECK(salida.find("Bicicletas cargadas: 12") != std::string::npos,
          "la carga debe leer 12 bicicletas");
    CHECK(salida.find("Viajes cargados: 20") != std::string::npos,
          "la carga debe leer 20 viajes");
    CHECK(salida.find("Ciclovias cargadas: 8") != std::string::npos,
          "la carga debe leer 8 ciclovias");
}
}  // namespace

void probarRedCarga() {
    Red red;

    CHECK(red.getCantidadEstaciones() == 0, "Red nueva sin estaciones");
    CHECK(red.getCantidadBicicletas() == 0, "Red nueva sin bicicletas");
    CHECK(red.getCantidadViajes() == 0, "Red nueva sin viajes");
    CHECK(red.getColisionesHash() == 0, "Red nueva sin colisiones");
    CHECK(red.getFactorCargaHash() == 0.0, "Red nueva con factor 0");

    cargarRedCompleta(red);
    CHECK(red.getCantidadEstaciones() == 10, "quedan 10 estaciones");
    CHECK(red.getCantidadBicicletas() == 12, "quedan 12 bicicletas");
    CHECK(red.getCantidadViajes() == 20, "quedan 20 viajes");
    CHECK(red.getColisionesHash() == 3, "el hash registra 3 colisiones");
    CHECK(red.getFactorCargaHash() > 0.7692 &&
              red.getFactorCargaHash() < 0.7693,
          "el factor de carga es 10/13");

    std::string estadisticas =
        capturarSalida([&red]() { red.mostrarEstadisticasHash(); });
    CHECK(estadisticas.find("Colisiones registradas: 3") !=
              std::string::npos,
          "las estadisticas muestran las colisiones");
    CHECK(estadisticas.find("0.7692") != std::string::npos,
          "las estadisticas muestran el factor con 4 decimales");
    CHECK(estadisticas.find("(10/13)") != std::string::npos,
          "las estadisticas muestran la fraccion 10/13");

    red.reiniciar();
    CHECK(red.getCantidadEstaciones() == 0 &&
              red.getCantidadBicicletas() == 0 &&
              red.getCantidadViajes() == 0,
          "reiniciar deja la red vacia");

    std::string error = capturarSalida(
        [&red]() { red.cargarEstaciones("no_existe.txt"); });
    CHECK(error.find("No se pudo abrir el archivo") != std::string::npos,
          "un archivo inexistente debe informarse");
}

void probarRedEstaciones() {
    Red red;
    cargarRedCompleta(red);

    std::string info =
        capturarSalida([&red]() { red.mostrarInformacionEstacion("PLZ"); });
    CHECK(info.find("PlazaDeMayo") != std::string::npos,
          "la info de PLZ muestra el nombre");
    CHECK(info.find("Capacidad de anclajes: 30") != std::string::npos,
          "la info de PLZ muestra la capacidad");
    CHECK(info.find("Bicicletas disponibles: 12") != std::string::npos,
          "la info de PLZ muestra las disponibles");
    CHECK(info.find("Usuarios en espera: 0") != std::string::npos,
          "la info de PLZ muestra la cola vacia");

    bool existe = true;
    std::string inexistente = capturarSalida(
        [&red, &existe]() { existe = red.mostrarInformacionEstacion("XXX"); });
    CHECK(!existe, "mostrar una estacion inexistente devuelve false");
    CHECK(inexistente.find("No existe") != std::string::npos,
          "se informa la estacion inexistente");

    CHECK(red.agregarEstacion("TST", "EstacionTest", "Test", 10, 5),
          "agregar una estacion valida");
    CHECK(red.getCantidadEstaciones() == 11, "la red queda con 11 estaciones");

    bool duplicada = true;
    capturarSalida([&red, &duplicada]() {
        duplicada = red.agregarEstacion("TST", "Otra", "Test", 10, 5);
    });
    CHECK(!duplicada, "no se puede agregar un codigo duplicado");

    bool invalida = true;
    capturarSalida([&red, &invalida]() {
        invalida = red.agregarEstacion("BAD", "Bad", "Test", 10, 11);
    });
    CHECK(!invalida, "no se puede agregar una estacion con mas bicis que anclajes");

    CHECK(red.eliminarEstacion("TST"), "eliminar la estacion agregada");
    CHECK(red.getCantidadEstaciones() == 10, "vuelven a quedar 10 estaciones");

    std::string porAnclajes =
        capturarSalida([&red]() { red.listarEstacionesOrdenadas(1); });
    CHECK(porAnclajes.find("[RET]") < porAnclajes.find("[BEL]") &&
              porAnclajes.find("[BEL]") < porAnclajes.find("[PAL]"),
          "el orden por anclajes es descendente");

    std::string porDisponibles =
        capturarSalida([&red]() { red.listarEstacionesOrdenadas(2); });
    CHECK(porDisponibles.find("[RET]") < porDisponibles.find("[BEL]") &&
              porDisponibles.find("[BEL]") < porDisponibles.find("[PAL]"),
          "el orden por disponibles es descendente");

    std::string baja =
        capturarSalida([&red]() { red.listarEstacionesBajaOcupacion(); });
    CHECK(baja.find("[CAB]") != std::string::npos &&
              baja.find("[CON]") != std::string::npos &&
              baja.find("[BOE]") != std::string::npos &&
              baja.find("[ISL]") != std::string::npos,
          "la baja ocupacion incluye CAB, CON, BOE e ISL");
    CHECK(baja.find("[PLZ]") == std::string::npos,
          "PLZ no tiene baja ocupacion");

    std::string camino;
    int tramos = -1;
    CHECK(red.calcularCaminoMasCortoBFS("PLZ", "CON", camino, tramos) ==
              ResultadoCamino::Encontrado,
          "BFS encuentra PLZ -> CON");
    CHECK(tramos == 4, "PLZ -> CON tiene 4 tramos");
    CHECK(red.calcularCaminoMasCortoBFS("BEL", "PLZ", camino, tramos) ==
              ResultadoCamino::SinCamino,
          "BEL no tiene camino a PLZ");
    CHECK(red.calcularCaminoMasCortoBFS("PLZ", "XXX", camino, tramos) ==
              ResultadoCamino::EstacionInexistente,
          "BFS informa una estacion inexistente");

    CHECK(red.eliminarEstacion("CAB"), "eliminar CAB");
    CHECK(red.calcularCaminoMasCortoBFS("PLZ", "PAL", camino, tramos) ==
              ResultadoCamino::SinCamino,
          "sin CAB no hay camino PLZ -> PAL");
}

void probarRedViajes() {
    Red red;
    cargarRedCompleta(red);

    CHECK(red.contarViajesPorRangoFechas("PLZ", 20260310, 20260321) == 2,
          "PLZ tiene 2 viajes en el rango pedido");
    CHECK(red.contarViajesPorRangoFechas("PLZ", 20260310, 20260310) == 1,
          "el rango de un dia es inclusivo");

    int minutos = -1;
    std::string porDni = capturarSalida(
        [&red, &minutos]() { minutos = red.buscarViajesUsuario("30123456"); });
    CHECK(minutos == 182, "el DNI 30123456 suma 182 minutos");
    CHECK(porDni.find("182") != std::string::npos,
          "la salida muestra el total de minutos");

    std::string revision =
        capturarSalida([&red]() { red.listarBicicletasParaRevision(3000); });
    CHECK(revision.find("Bici #502") != std::string::npos &&
              revision.find("Bici #505") != std::string::npos &&
              revision.find("Bici #510") != std::string::npos,
          "la revision incluye 502, 505 y 510");
    CHECK(revision.find("Bici #501") == std::string::npos,
          "la revision no incluye la 501");

    std::string retiro1 = capturarSalida(
        [&red]() { red.retirarBicicletaEnEstacion("ISL", "111"); });
    CHECK(retiro1.find("espera") != std::string::npos,
          "en ISL sin bicis se encola el primer usuario");
    capturarSalida(
        [&red]() { red.retirarBicicletaEnEstacion("ISL", "222"); });
    std::string infoCola =
        capturarSalida([&red]() { red.mostrarInformacionEstacion("ISL"); });
    CHECK(infoCola.find("Usuarios en espera: 2") != std::string::npos,
          "ISL tiene 2 usuarios en espera");

    std::string dni;
    CHECK(red.devolverBicicletaEnEstacion("ISL", dni) ==
              ResultadoDevolucionRed::AsignadaAEspera &&
              dni == "111",
          "la primera devolucion asigna al DNI 111");
    CHECK(red.devolverBicicletaEnEstacion("ISL", dni) ==
              ResultadoDevolucionRed::AsignadaAEspera &&
              dni == "222",
          "la segunda devolucion asigna al DNI 222");
    CHECK(red.devolverBicicletaEnEstacion("ISL", dni) ==
              ResultadoDevolucionRed::AgregadaAEstacion,
          "sin cola, la devolucion agrega un anclaje");
    CHECK(red.devolverBicicletaEnEstacion("XXX", dni) ==
              ResultadoDevolucionRed::EstacionInexistente,
          "devolver en una estacion inexistente se informa");

    std::string infoIsl =
        capturarSalida([&red]() { red.mostrarInformacionEstacion("ISL"); });
    CHECK(infoIsl.find("Bicicletas disponibles: 1") != std::string::npos,
          "ISL queda con una bicicleta disponible");

    std::string retiroPlz = capturarSalida(
        [&red]() { red.retirarBicicletaEnEstacion("PLZ", "333"); });
    CHECK(retiroPlz.find("Retiro habilitado") != std::string::npos,
          "PLZ tiene bicis y habilita el retiro");
    std::string infoPlz =
        capturarSalida([&red]() { red.mostrarInformacionEstacion("PLZ"); });
    CHECK(infoPlz.find("Bicicletas disponibles: 11") != std::string::npos,
          "retirar en PLZ descuenta una bicicleta");

    CHECK(red.ingresarBiciTaller(502, 3), "ingresar la electrica 502");
    CHECK(red.ingresarBiciTaller(505, 2), "ingresar la averiada 505");
    CHECK(red.ingresarBiciTaller(503, 3),
          "ingresar una bici no averiada con aviso");

    EntradaTaller urgente;
    CHECK(red.extraerBiciMasUrgenteTaller(urgente) &&
              urgente.biciId == 502 && urgente.prioridad == 1,
          "la electrica 502 sale primero con prioridad 1");
    CHECK(red.extraerBiciMasUrgenteTaller(urgente) &&
              urgente.biciId == 505 && urgente.prioridad == 2,
          "luego sale la 505 con prioridad 2");
    CHECK(red.extraerBiciMasUrgenteTaller(urgente) &&
              urgente.biciId == 503 && urgente.prioridad == 3,
          "al final sale la 503 con prioridad 3");
    CHECK(!red.extraerBiciMasUrgenteTaller(urgente),
          "el taller queda vacio");
}

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

    std::istringstream entrada("5 PLZ 3 CAB 7 RET 5 CON 2 BOE 5");
    std::streambuf* anterior = std::cin.rdbuf(entrada.rdbuf());
    int manuales = -1;
    std::string manual = capturarSalida([&red, &manuales]() {
        manuales = red.calcularRedistribucionManual(10);
    });
    std::cin.rdbuf(anterior);

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
