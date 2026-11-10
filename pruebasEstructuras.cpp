#include "pruebas.h"
#include "ColaEspera.h"
#include "Estacion.h"

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
