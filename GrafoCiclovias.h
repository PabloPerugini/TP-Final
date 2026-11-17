#ifndef GRAFO_CICLOVIAS_H
#define GRAFO_CICLOVIAS_H

#include <string>

enum class ResultadoCamino
{
    Encontrado,
    SinCamino,
    EstacionInexistente
};

class GrafoCiclovias
{
public:
    GrafoCiclovias();
    ~GrafoCiclovias();

    void agregarEstacion(const std::string &codigo);
    void eliminarEstacion(const std::string &codigo);
    void agregarCiclovia(const std::string &origen, const std::string &destino);
    ResultadoCamino buscarCaminoMasCorto(const std::string &origen,
                                         const std::string &destino,
                                         std::string &camino, int &tramos);
    int getCantidadVertices() const;
    void vaciar();

private:
    struct AristaGrafo;

    struct VerticeGrafo
    {
        std::string codigo;
        AristaGrafo *adyacentes;
        VerticeGrafo *siguiente;
        bool visitado;
        VerticeGrafo *anterior;
        int distancia;
        VerticeGrafo(const std::string &valor)
            : codigo(valor), adyacentes(nullptr), siguiente(nullptr),
              visitado(false), anterior(nullptr), distancia(0) {}
    };

    struct AristaGrafo
    {
        VerticeGrafo *destino;
        AristaGrafo *siguiente;
        AristaGrafo(VerticeGrafo *valor)
            : destino(valor), siguiente(nullptr) {}
    };

    VerticeGrafo *cabeza;

    VerticeGrafo *buscarVertice(const std::string &codigo) const;
    bool existeArista(VerticeGrafo *origen, VerticeGrafo *destino) const;
    void agregarArista(VerticeGrafo *origen, VerticeGrafo *destino);
    void liberarAristas(AristaGrafo *arista);
    std::string construirCamino(VerticeGrafo *vertice) const;

    GrafoCiclovias(const GrafoCiclovias &) = delete;
    GrafoCiclovias &operator=(const GrafoCiclovias &) = delete;
};

#endif
