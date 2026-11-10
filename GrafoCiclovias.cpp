#include "GrafoCiclovias.h"
#include "Cola.h"
#include <iostream>

GrafoCiclovias::GrafoCiclovias() : cabeza(nullptr) {}

GrafoCiclovias::~GrafoCiclovias() {
    liberarMemoria();
}

void GrafoCiclovias::liberarMemoria() {
    VerticeGrafo* vActual = cabeza;
    while (vActual != nullptr) {
        AristaGrafo* aActual = vActual->adyacentes;
        while (aActual != nullptr) {
            AristaGrafo* aBorrar = aActual;
            aActual = aActual->siguiente;
            delete aBorrar;
        }
        VerticeGrafo* vBorrar = vActual;
        vActual = vActual->siguiente;
        delete vBorrar;
    }
    cabeza = nullptr;
}

VerticeGrafo* GrafoCiclovias::buscarVertice(const std::string& codigo) const {
    VerticeGrafo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->codigoEstacion == codigo) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

void GrafoCiclovias::agregarEstacion(const std::string& codigo) {
    if (buscarVertice(codigo) != nullptr) return;
    VerticeGrafo* nuevo = new VerticeGrafo(codigo);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

void GrafoCiclovias::agregarCiclovia(const std::string& origen, const std::string& destino) {
    agregarEstacion(origen);
    agregarEstacion(destino);

    VerticeGrafo* vOrigen = buscarVertice(origen);
    VerticeGrafo* vDestino = buscarVertice(destino);

    // Conexión bidireccional
    vOrigen->adyacentes = new AristaGrafo(destino, vOrigen->adyacentes);
    vDestino->adyacentes = new AristaGrafo(origen, vDestino->adyacentes);
}

void GrafoCiclovias::buscarCaminoMasCortoBFS(const std::string& origen, const std::string& destino) const {
    VerticeGrafo* vOrig = buscarVertice(origen);
    VerticeGrafo* vDest = buscarVertice(destino);

    if (!vOrig || !vDest) {
        std::cout << "[!] Una o ambas estaciones no existen en la red de ciclovías.\n";
        return;
    }

    ArrayDinamico<std::string> visitados;
    ArrayDinamico<std::string> padreOrigen;
    ArrayDinamico<std::string> padreDestino;

    Cola Q;
    Q.encolar(origen);
    visitados.agregar(origen);

    bool encontrado = false;

    while (!Q.esVacia()) {
        std::string actual = Q.desencolar();
        if (actual == destino) {
            encontrado = true;
            break;
        }

        VerticeGrafo* vAct = buscarVertice(actual);
        if (!vAct) continue;

        AristaGrafo* arista = vAct->adyacentes;
        while (arista != nullptr) {
            bool yaVisitado = false;
            for (int i = 0; i < visitados.tamano(); ++i) {
                if (visitados[i] == arista->destino) {
                    yaVisitado = true;
                    break;
                }
            }

            if (!yaVisitado) {
                visitados.agregar(arista->destino);
                padreOrigen.agregar(actual);
                padreDestino.agregar(arista->destino);
                Q.encolar(arista->destino);
            }
            arista = arista->siguiente;
        }
    }

    if (!encontrado) {
        std::cout << "[!] No existe un recorrido por ciclovías entre " << origen << " y " << destino << ".\n";
        return;
    }

    // Reconstruir el camino desde el destino hasta el origen
    ArrayDinamico<std::string> camino;
    std::string paso = destino;
    camino.agregar(paso);

    while (paso != origen) {
        for (int i = 0; i < padreDestino.tamano(); ++i) {
            if (padreDestino[i] == paso) {
                paso = padreOrigen[i];
                camino.agregar(paso);
                break;
            }
        }
    }

    std::cout << "\n=== RECORRIDO MAS CORTO (BFS) ===\n";
    std::cout << "Tramos totales: " << camino.tamano() - 1 << "\nCamino: ";
    for (int i = camino.tamano() - 1; i >= 0; --i) {
        std::cout << camino[i] << (i == 0 ? "" : " -> ");
    }
    std::cout << "\n";
}