#include "GrafoCiclovias.h"

GrafoCiclovias::GrafoCiclovias() : cabeza(nullptr) {}

GrafoCiclovias::~GrafoCiclovias() {
    vaciar();
}

GrafoCiclovias::VerticeGrafo* GrafoCiclovias::buscarVertice(
    const std::string& codigo) const {
    VerticeGrafo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->codigo == codigo) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void GrafoCiclovias::agregarEstacion(const std::string& codigo) {
    if (codigo.empty() || buscarVertice(codigo) != nullptr) {
        return;
    }
    VerticeGrafo* nuevo = new VerticeGrafo(codigo);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

void GrafoCiclovias::liberarAristas(AristaGrafo* arista) {
    while (arista != nullptr) {
        AristaGrafo* aBorrar = arista;
        arista = arista->siguiente;
        delete aBorrar;
    }
}

void GrafoCiclovias::eliminarEstacion(const std::string& codigo) {
    VerticeGrafo* objetivo = buscarVertice(codigo);
    if (objetivo == nullptr) {
        return;
    }

    for (VerticeGrafo* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) {
        if (actual == objetivo) {
            continue;
        }
        AristaGrafo** enlace = &actual->adyacentes;
        while (*enlace != nullptr) {
            if ((*enlace)->destino == objetivo) {
                AristaGrafo* aBorrar = *enlace;
                *enlace = aBorrar->siguiente;
                delete aBorrar;
            } else {
                enlace = &(*enlace)->siguiente;
            }
        }
    }

    VerticeGrafo** enlaceVertice = &cabeza;
    while (*enlaceVertice != nullptr) {
        if (*enlaceVertice == objetivo) {
            VerticeGrafo* aBorrar = *enlaceVertice;
            *enlaceVertice = aBorrar->siguiente;
            liberarAristas(aBorrar->adyacentes);
            delete aBorrar;
            return;
        }
        enlaceVertice = &(*enlaceVertice)->siguiente;
    }
}

bool GrafoCiclovias::existeArista(VerticeGrafo* origen,
                                  VerticeGrafo* destino) const {
    for (AristaGrafo* actual = origen->adyacentes; actual != nullptr;
         actual = actual->siguiente) {
        if (actual->destino == destino) {
            return true;
        }
    }
    return false;
}

void GrafoCiclovias::agregarArista(VerticeGrafo* origen,
                                   VerticeGrafo* destino) {
    AristaGrafo* nueva = new AristaGrafo(destino);
    nueva->siguiente = origen->adyacentes;
    origen->adyacentes = nueva;
}

void GrafoCiclovias::agregarCiclovia(const std::string& origen,
                                     const std::string& destino) {
    if (origen == destino) {
        return;
    }
    VerticeGrafo* verticeOrigen = buscarVertice(origen);
    VerticeGrafo* verticeDestino = buscarVertice(destino);
    if (verticeOrigen == nullptr || verticeDestino == nullptr) {
        return;
    }
    if (!existeArista(verticeOrigen, verticeDestino)) {
        agregarArista(verticeOrigen, verticeDestino);
    }
    if (!existeArista(verticeDestino, verticeOrigen)) {
        agregarArista(verticeDestino, verticeOrigen);
    }
}

ResultadoCamino GrafoCiclovias::buscarCaminoMasCorto(
    const std::string& origen, const std::string& destino,
    std::string& camino, int& tramos) {
    camino.clear();
    tramos = 0;

    VerticeGrafo* verticeOrigen = buscarVertice(origen);
    VerticeGrafo* verticeDestino = buscarVertice(destino);
    if (verticeOrigen == nullptr || verticeDestino == nullptr) {
        return ResultadoCamino::EstacionInexistente;
    }

    for (VerticeGrafo* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) {
        actual->visitado = false;
        actual->anterior = nullptr;
        actual->distancia = 0;
    }

    int total = getCantidadVertices();
    VerticeGrafo** cola = new VerticeGrafo*[total];
    int frente = 0;
    int fin = 0;

    verticeOrigen->visitado = true;
    cola[fin++] = verticeOrigen;

    bool encontrado = false;
    while (frente < fin && !encontrado) {
        VerticeGrafo* actual = cola[frente++];
        if (actual == verticeDestino) {
            encontrado = true;
            break;
        }
        for (AristaGrafo* arista = actual->adyacentes; arista != nullptr;
             arista = arista->siguiente) {
            VerticeGrafo* vecino = arista->destino;
            if (!vecino->visitado) {
                vecino->visitado = true;
                vecino->anterior = actual;
                vecino->distancia = actual->distancia + 1;
                cola[fin++] = vecino;
            }
        }
    }

    delete[] cola;

    if (!verticeDestino->visitado) {
        return ResultadoCamino::SinCamino;
    }

    tramos = verticeDestino->distancia;
    camino = construirCamino(verticeDestino);
    return ResultadoCamino::Encontrado;
}

std::string GrafoCiclovias::construirCamino(VerticeGrafo* vertice) const {
    if (vertice->anterior == nullptr) {
        return vertice->codigo;
    }
    return construirCamino(vertice->anterior) + " -> " + vertice->codigo;
}

int GrafoCiclovias::getCantidadVertices() const {
    int cantidad = 0;
    for (VerticeGrafo* actual = cabeza; actual != nullptr;
         actual = actual->siguiente) {
        ++cantidad;
    }
    return cantidad;
}

void GrafoCiclovias::vaciar() {
    VerticeGrafo* actual = cabeza;
    while (actual != nullptr) {
        VerticeGrafo* aBorrar = actual;
        actual = actual->siguiente;
        liberarAristas(aBorrar->adyacentes);
        delete aBorrar;
    }
    cabeza = nullptr;
}
