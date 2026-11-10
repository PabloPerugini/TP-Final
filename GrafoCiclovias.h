#ifndef GRAFO_CICLOVIAS_H
#define GRAFO_CICLOVIAS_H

#include <string>
#include "ArrayDinamico.h"

struct AristaGrafo {
    std::string destino;
    AristaGrafo* siguiente;
    AristaGrafo(const std::string& d, AristaGrafo* sig = nullptr) : destino(d), siguiente(sig) {}
};

struct VerticeGrafo {
    std::string codigoEstacion;
    AristaGrafo* adyacentes;
    VerticeGrafo* siguiente;
    VerticeGrafo(const std::string& cod) : codigoEstacion(cod), adyacentes(nullptr), siguiente(nullptr) {}
};

class GrafoCiclovias {
public:
    GrafoCiclovias();
    ~GrafoCiclovias();

    void agregarEstacion(const std::string& codigo);
    void agregarCiclovia(const std::string& origen, const std::string& destino);
    void buscarCaminoMasCortoBFS(const std::string& origen, const std::string& destino) const;

private:
    VerticeGrafo* cabeza;
    VerticeGrafo* buscarVertice(const std::string& codigo) const;
    void liberarMemoria();
};

#endif