#include "pruebas.h"
#include "MinHeap.h"
#include "AbbBicicletas.h"
#include "GrafoCiclovias.h"
#include "Backtracking.h"

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

void probarBacktracking() {
    const std::string codigos[] = {"PLZ", "CAB", "RET", "CON", "BOE"};
    const int faltantes[] = {3, 7, 5, 2, 5};

    int soluciones = 0;
    std::string salida = capturarSalida([&soluciones, &codigos, &faltantes]() {
        soluciones = Backtracking::resolver(codigos, faltantes, 5, 10, true);
    });

    CHECK(soluciones == 4, "el ejemplo del PDF debe tener 4 soluciones");
    CHECK(Backtracking::resolver(codigos, faltantes, 5, 10, false) == 4,
          "contar sin imprimir debe dar el mismo resultado");

    int impresas = 0;
    std::size_t posicion = salida.find("Solucion");
    while (posicion != std::string::npos) {
        ++impresas;
        posicion = salida.find("Solucion", posicion + 1);
    }
    CHECK(impresas == 4, "deben imprimirse las 4 soluciones");

    CHECK(salida.find("PLZ, CAB") != std::string::npos,
          "debe aparecer la solucion PLZ + CAB");
    CHECK(salida.find("RET, BOE") != std::string::npos,
          "debe aparecer la solucion RET + BOE");
    CHECK(salida.find("PLZ, RET, CON") != std::string::npos,
          "debe aparecer la solucion PLZ + RET + CON");
    CHECK(salida.find("PLZ, CON, BOE") != std::string::npos,
          "debe aparecer la solucion PLZ + CON + BOE");

    CHECK(Backtracking::resolver(codigos, faltantes, 5, 1, false) == 0,
          "una capacidad inalcanzable no tiene soluciones");

    const std::string dosCodigos[] = {"A", "B"};
    const int dosFaltantes[] = {3, 7};
    CHECK(Backtracking::resolver(dosCodigos, dosFaltantes, 2, 7, false) == 1,
          "con 3 y 7 solo el 7 suma exactamente 7");
    CHECK(Backtracking::resolver(nullptr, dosFaltantes, 2, 7, false) == 0,
          "un arreglo de codigos nulo devuelve 0");
    CHECK(Backtracking::resolver(dosCodigos, dosFaltantes, 0, 7, false) == 0,
          "una lista vacia devuelve 0");
    CHECK(Backtracking::resolver(dosCodigos, dosFaltantes, 2, 0, false) == 0,
          "una capacidad cero no tiene soluciones");
}
