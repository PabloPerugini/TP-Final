#include "pruebas.h"
#include "ColaEspera.h"
#include "Estacion.h"
#include "TablaHash.h"
#include "ListaViajes.h"

void probarColaEspera() {
    ColaEspera cola;
    std::string dni;

    CHECK(cola.estaVacia(), "la cola nueva debe estar vacia");
    CHECK(cola.tamanio() == 0, "la cola nueva debe tener tamanio 0");
    CHECK(!cola.desencolar(dni), "desencolar en cola vacia debe devolver false");

    cola.encolar("301");
    cola.encolar("302");
    CHECK(!cola.estaVacia(), "la cola con elementos no debe estar vacia");
    CHECK(cola.tamanio() == 2, "la cola debe tener dos elementos");

    CHECK(cola.desencolar(dni) && dni == "301", "FIFO: primero debe salir 301");
    CHECK(cola.desencolar(dni) && dni == "302", "FIFO: segundo debe salir 302");
    CHECK(!cola.desencolar(dni), "la cola quedo vacia");
    CHECK(cola.estaVacia(), "la cola debe estar vacia al final");

    cola.encolar("999");
    cola.vaciar();
    CHECK(cola.estaVacia() && cola.tamanio() == 0, "vaciar debe dejar la cola vacia");
}

void probarEstacion() {
    Estacion estacion("PLZ", "PlazaDeMayo", "Monserrat", 2, 0);
    std::string dni;

    CHECK(estacion.getCodigo() == "PLZ", "codigo de estacion");
    CHECK(estacion.getBiciDisponibles() == 0, "disponibles iniciales");
    CHECK(!estacion.retirarBicicleta(), "no se puede retirar sin disponibles");
    CHECK(estacion.getBiciDisponibles() == 0, "no debe quedar negativo");

    estacion.encolarUsuario("301");
    CHECK(estacion.getUsuariosEnEspera() == 1, "debe haber un usuario en espera");

    CHECK(estacion.devolverBicicleta(dni) == ResultadoDevolucion::AsignadaAEspera,
          "la devolucion debe asignarse al primero de la cola");
    CHECK(dni == "301", "el DNI asignado debe ser 301");
    CHECK(estacion.getBiciDisponibles() == 0,
          "la bici asignada no queda como disponible");
    CHECK(estacion.getUsuariosEnEspera() == 0, "la cola debe quedar vacia");

    CHECK(estacion.devolverBicicleta(dni) == ResultadoDevolucion::AgregadaAEstacion,
          "sin cola, la bici se agrega a la estacion");
    CHECK(estacion.getBiciDisponibles() == 1, "disponibles debe subir a 1");

    CHECK(estacion.devolverBicicleta(dni) == ResultadoDevolucion::AgregadaAEstacion,
          "segunda bici agregada");
    CHECK(estacion.getBiciDisponibles() == 2, "disponibles debe subir a 2");

    CHECK(estacion.devolverBicicleta(dni) == ResultadoDevolucion::SinLugar,
          "estacion llena debe informar SinLugar");
    CHECK(estacion.getBiciDisponibles() == 2, "no debe superar la capacidad");

    Estacion otra("X", "X", "X", 4, 2);
    CHECK(otra.porcentajeOcupacion() == 50.0, "porcentaje de ocupacion 50%");
}

void probarTablaHash() {
    TablaHash tabla;
    struct DatoEstacion {
        const char* codigo;
        const char* nombre;
        const char* barrio;
        int capacidad;
        int disponibles;
    };

    const DatoEstacion datos[10] = {
        {"PLZ", "PlazaDeMayo", "Monserrat", 30, 12},
        {"PAL", "ParqueTresDeFebrero", "Palermo", 40, 35},
        {"CAB", "PrimeraJunta", "Caballito", 25, 4},
        {"RET", "Retiro", "Retiro", 50, 45},
        {"CON", "Congreso", "Congreso", 20, 3},
        {"BOE", "Boca", "Boca", 15, 2},
        {"BAL", "Balvanera", "Balvanera", 35, 10},
        {"FLO", "Flores", "Flores", 28, 22},
        {"BEL", "Belgrano", "Belgrano", 45, 44},
        {"ISL", "IslaDatos", "PuertoMadero", 18, 0}
    };

    for (int i = 0; i < 10; ++i) {
        CHECK(tabla.insertar(datos[i].codigo,
                             new Estacion(datos[i].codigo, datos[i].nombre,
                                          datos[i].barrio, datos[i].capacidad,
                                          datos[i].disponibles)),
              "insertar estacion de prueba");
    }

    CHECK(tabla.getCantidad() == 10, "la tabla debe tener 10 estaciones");
    CHECK(tabla.getColisiones() == 3, "la tabla debe registrar 3 colisiones");
    CHECK(tabla.factorDeCarga() > 0.7692 && tabla.factorDeCarga() < 0.7693,
          "el factor de carga debe ser 10/13");

    Estacion* plz = tabla.obtener("PLZ");
    CHECK(plz != nullptr && plz->getNombre() == "PlazaDeMayo",
          "obtener PLZ debe devolver la estacion correcta");
    CHECK(tabla.obtener("XXX") == nullptr, "obtener una clave inexistente da nullptr");

    Estacion* duplicada = new Estacion("PLZ", "Duplicada", "X", 1, 0);
    CHECK(!tabla.insertar("PLZ", duplicada), "no se debe insertar un codigo duplicado");
    delete duplicada;
    CHECK(tabla.getCantidad() == 10, "el duplicado no cambia la cantidad");

    Estacion* buffer[10];
    CHECK(tabla.volcarEstaciones(buffer, 10) == 10, "volcar debe devolver 10");
    bool todas = true;
    for (int i = 0; i < 10; ++i) {
        if (buffer[i] == nullptr) {
            todas = false;
        }
    }
    CHECK(todas, "volcar no debe dejar punteros nulos");

    CHECK(tabla.eliminar("PLZ"), "eliminar PLZ debe funcionar");
    CHECK(tabla.getCantidad() == 9, "la cantidad baja a 9");
    CHECK(tabla.obtener("PLZ") == nullptr, "PLZ ya no debe existir");
    CHECK(!tabla.eliminar("PLZ"), "eliminar dos veces devuelve false");

    tabla.vaciar();
    CHECK(tabla.getCantidad() == 0, "vaciar deja la tabla sin estaciones");
    CHECK(tabla.getColisiones() == 0, "vaciar resetea las colisiones");

    CHECK(tabla.insertar("AAA", new Estacion("AAA", "A", "A", 1, 0)),
          "insertar despues de vaciar");
    CHECK(tabla.getCantidad() == 1 && tabla.getColisiones() == 0,
          "la tabla queda utilizable despues de vaciar");
}

void probarListaViajes() {
    ListaViajes lista;

    CHECK(lista.getCantidad() == 0, "la lista nueva debe estar vacia");

    lista.insertar(new Viaje(1, 501, "301", "PLZ", "PAL", 20260310, 25));
    lista.insertar(new Viaje(2, 502, "302", "PLZ", "CAB", 20260312, 35));
    lista.insertar(new Viaje(3, 503, "301", "PAL", "RET", 20260311, 40));
    lista.insertar(new Viaje(4, 504, "301", "PLZ", "BOE", 20260312, 15));

    CHECK(lista.getCantidad() == 4, "la lista debe tener 4 viajes");
    CHECK(lista.contarPorRangoFechas("PLZ", 20260310, 20260312) == 3,
          "deben contarse 3 viajes desde PLZ en el rango");
    CHECK(lista.contarPorRangoFechas("PLZ", 20260311, 20260311) == 0,
          "el rango de un dia sin viajes debe dar 0");
    CHECK(lista.contarPorRangoFechas("XXX", 20260101, 20261231) == 0,
          "una estacion inexistente no tiene viajes");
    CHECK(lista.buscarPorDni("301") == 80, "el DNI 301 debe sumar 80 minutos");
    CHECK(lista.buscarPorDni("999") == 0, "un DNI inexistente suma 0");

    lista.vaciar();
    CHECK(lista.getCantidad() == 0, "vaciar deja la lista sin viajes");
}
