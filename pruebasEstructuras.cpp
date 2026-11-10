#include "pruebas.h"
#include "ColaEspera.h"
#include "Estacion.h"
#include "TablaHash.h"
#include "ListaViajes.h"
#include "MinHeap.h"
#include "AbbBicicletas.h"
#include "GrafoCiclovias.h"

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

void probarMinHeap() {
    MinHeap heap;
    EntradaTaller salida;

    CHECK(heap.estaVacio(), "el heap nuevo debe estar vacio");
    CHECK(heap.tamanio() == 0, "el heap nuevo debe tener tamanio 0");
    CHECK(!heap.extraerMin(salida), "extraer de un heap vacio debe devolver false");

    heap.insertar(10, 3);
    heap.insertar(20, 1);
    heap.insertar(30, 2);
    heap.insertar(40, 1);
    CHECK(heap.tamanio() == 4, "el heap debe tener 4 elementos");

    CHECK(heap.extraerMin(salida) && salida.prioridad == 1,
          "primero debe salir una prioridad 1");
    int primero = salida.biciId;
    CHECK(heap.extraerMin(salida) && salida.prioridad == 1,
          "segundo debe salir una prioridad 1");
    int segundo = salida.biciId;
    CHECK((primero == 20 && segundo == 40) || (primero == 40 && segundo == 20),
          "deben salir las dos prioridades 1");
    CHECK(heap.extraerMin(salida) && salida.biciId == 30 && salida.prioridad == 2,
          "luego debe salir la prioridad 2");
    CHECK(heap.extraerMin(salida) && salida.biciId == 10 && salida.prioridad == 3,
          "al final debe salir la prioridad 3");
    CHECK(!heap.extraerMin(salida), "el heap debe quedar vacio");

    for (int i = 0; i < 20; ++i) {
        heap.insertar(i, (i % 3) + 1);
    }
    CHECK(heap.tamanio() == 20, "el heap debe crecer a 20 elementos");
    int prioridadAnterior = 0;
    for (int i = 0; i < 20; ++i) {
        CHECK(heap.extraerMin(salida), "debe extraerse cada elemento");
        CHECK(salida.prioridad >= prioridadAnterior,
              "las prioridades deben salir en orden no decreciente");
        prioridadAnterior = salida.prioridad;
    }
    CHECK(heap.estaVacio(), "el heap debe quedar vacio despues de extraer todo");

    heap.insertar(1, 2);
    heap.vaciar();
    CHECK(heap.estaVacio() && heap.tamanio() == 0, "vaciar deja el heap vacio");
}

void probarAbb() {
    AbbBicicletas abb;

    CHECK(abb.altura() == 0, "un ABB vacio tiene altura 0");
    CHECK(abb.contarAveriadas() == 0, "un ABB vacio no tiene averiadas");
    CHECK(abb.buscar(501) == nullptr, "buscar en un ABB vacio devuelve nullptr");

    CHECK(abb.insertar(new Bicicleta(501, "PLZ", TipoBici::Mecanica,
                                     EstadoBici::OK, 1000)),
          "insertar 501");
    CHECK(abb.insertar(new Bicicleta(502, "PAL", TipoBici::Electrica,
                                     EstadoBici::Averiada, 3500)),
          "insertar 502");
    CHECK(abb.insertar(new Bicicleta(503, "CAB", TipoBici::Mecanica,
                                     EstadoBici::OK, 200)),
          "insertar 503");
    CHECK(abb.insertar(new Bicicleta(504, "RET", TipoBici::Electrica,
                                     EstadoBici::Averiada, 4200)),
          "insertar 504");
    CHECK(abb.insertar(new Bicicleta(505, "CON", TipoBici::Mecanica,
                                     EstadoBici::OK, 500)),
          "insertar 505");

    Bicicleta* duplicada = new Bicicleta(503, "XXX", TipoBici::Mecanica,
                                         EstadoBici::OK, 0);
    CHECK(!abb.insertar(duplicada), "no se debe insertar un ID duplicado");
    delete duplicada;

    CHECK(abb.buscar(503) != nullptr &&
              abb.buscar(503)->getCodigoEstacion() == "CAB",
          "buscar 503 devuelve la bicicleta correcta");
    CHECK(abb.buscar(999) == nullptr, "buscar un ID inexistente devuelve nullptr");
    CHECK(abb.contarAveriadas() == 2, "deben contarse 2 bicicletas averiadas");
    CHECK(abb.altura() == 5,
          "la carga ascendente 501..505 debe formar una cadena de altura 5");

    std::string inorder = capturarSalida([&abb]() { abb.listarInorder(); });
    std::size_t pos501 = inorder.find("Bici #501");
    std::size_t pos503 = inorder.find("Bici #503");
    std::size_t pos505 = inorder.find("Bici #505");
    CHECK(pos501 != std::string::npos && pos503 != std::string::npos &&
              pos505 != std::string::npos && pos501 < pos503 && pos503 < pos505,
          "el recorrido inorder debe salir en orden creciente");

    std::string mayores =
        capturarSalida([&abb]() { abb.listarConKilometrosMayoresA(3000); });
    CHECK(mayores.find("Bici #502") != std::string::npos,
          "el filtro debe incluir la 502");
    CHECK(mayores.find("Bici #504") != std::string::npos,
          "el filtro debe incluir la 504");
    CHECK(mayores.find("Bici #501") == std::string::npos,
          "el filtro no debe incluir la 501");

    abb.vaciar();
    CHECK(abb.altura() == 0, "vaciar deja el ABB con altura 0");
    CHECK(abb.buscar(501) == nullptr, "vaciar elimina todas las bicicletas");

    const int ids[] = {501, 401, 601, 301, 451, 551, 701};
    for (int id : ids) {
        CHECK(abb.insertar(new Bicicleta(id, "PLZ", TipoBici::Mecanica,
                                         EstadoBici::OK, 0)),
              "insertar bicicleta del arbol de borrado");
    }
    CHECK(abb.altura() == 3, "el arbol de borrado tiene altura 3");

    // 501 tiene dos hijos; el sucesor 551 ocupa su lugar.
    CHECK(abb.eliminar(501), "eliminar la raiz con dos hijos");
    CHECK(abb.buscar(501) == nullptr && abb.buscar(551) != nullptr,
          "el sucesor debe quedar en el arbol");
    CHECK(abb.altura() == 3, "la altura se mantiene tras el primer borrado");

    // 401 tiene dos hijos.
    CHECK(abb.eliminar(401), "eliminar un nodo interno con dos hijos");
    CHECK(abb.buscar(401) == nullptr && abb.buscar(451) != nullptr,
          "el sucesor 451 debe quedar en el arbol");

    // 601 tiene un solo hijo (701).
    CHECK(abb.eliminar(601), "eliminar un nodo con un hijo");
    CHECK(abb.buscar(601) == nullptr && abb.buscar(701) != nullptr,
          "el hijo 701 debe quedar en el arbol");

    // 701 es una hoja.
    CHECK(abb.eliminar(701), "eliminar una hoja");
    CHECK(abb.buscar(701) == nullptr, "la hoja eliminada ya no existe");
    CHECK(!abb.eliminar(701), "eliminar dos veces devuelve false");
    CHECK(abb.buscar(301) != nullptr && abb.buscar(451) != nullptr &&
              abb.buscar(551) != nullptr,
          "las bicicletas restantes siguen en el arbol");
    CHECK(abb.altura() == 3, "el arbol restante conserva altura 3");

    abb.vaciar();
    CHECK(abb.altura() == 0 && abb.buscar(551) == nullptr,
          "vaciar deja el arbol vacio despues de los borrados");
}

void probarGrafo() {
    GrafoCiclovias grafo;
    const char* codigos[] = {"PLZ", "PAL", "CAB", "RET", "CON",
                             "BOE", "BAL", "FLO", "BEL", "ISL"};
    for (const char* codigo : codigos) {
        grafo.agregarEstacion(codigo);
    }

    CHECK(grafo.getCantidadVertices() == 10, "el grafo debe tener 10 vertices");
    grafo.agregarEstacion("PLZ");
    CHECK(grafo.getCantidadVertices() == 10,
          "agregar una estacion duplicada no cambia el grafo");

    grafo.agregarCiclovia("PLZ", "CAB");
    grafo.agregarCiclovia("CAB", "PAL");
    grafo.agregarCiclovia("PAL", "RET");
    grafo.agregarCiclovia("RET", "CON");
    grafo.agregarCiclovia("CON", "BOE");
    grafo.agregarCiclovia("PAL", "BOE");
    grafo.agregarCiclovia("BAL", "PLZ");
    grafo.agregarCiclovia("FLO", "BAL");

    std::string camino;
    int tramos = -1;
    CHECK(grafo.buscarCaminoMasCorto("PLZ", "CON", camino, tramos) ==
              ResultadoCamino::Encontrado,
          "debe existir camino entre PLZ y CON");
    CHECK(tramos == 4, "PLZ a CON debe tener 4 tramos");
    CHECK(camino.find("PLZ") == 0, "el camino debe empezar en PLZ");
    CHECK(camino.size() >= 3 &&
              camino.substr(camino.size() - 3) == "CON",
          "el camino debe terminar en CON");

    CHECK(grafo.buscarCaminoMasCorto("PLZ", "PLZ", camino, tramos) ==
              ResultadoCamino::Encontrado,
          "el camino de una estacion a si misma existe");
    CHECK(tramos == 0 && camino == "PLZ",
          "el camino de PLZ a PLZ tiene 0 tramos");

    CHECK(grafo.buscarCaminoMasCorto("BEL", "PLZ", camino, tramos) ==
              ResultadoCamino::SinCamino,
          "BEL esta aislada y no llega a PLZ");
    CHECK(grafo.buscarCaminoMasCorto("PLZ", "XXX", camino, tramos) ==
              ResultadoCamino::EstacionInexistente,
          "una estacion inexistente debe informarse");

    grafo.eliminarEstacion("CAB");
    CHECK(grafo.getCantidadVertices() == 9, "eliminar CAB deja 9 vertices");
    CHECK(grafo.buscarCaminoMasCorto("PLZ", "PAL", camino, tramos) ==
              ResultadoCamino::SinCamino,
          "sin CAB ya no hay camino entre PLZ y PAL");

    grafo.vaciar();
    CHECK(grafo.getCantidadVertices() == 0, "vaciar deja el grafo sin vertices");
}
