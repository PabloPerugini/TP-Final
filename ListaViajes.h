#ifndef LISTA_VIAJES_H
#define LISTA_VIAJES_H

#include <string>
#include "Viaje.h"

class ListaViajes
{
public:
    ListaViajes();
    ~ListaViajes();

    void insertar(Viaje *viaje);
    int contarPorRangoFechas(const std::string &estacionOrigen,
                             int fechaInicio, int fechaFin) const;
    int buscarPorDni(const std::string &dni) const;
    int getCantidad() const;
    void vaciar();

private:
    struct NodoViaje
    {
        Viaje *dato;
        NodoViaje *siguiente;
        NodoViaje(Viaje *valor, NodoViaje *sig = nullptr)
            : dato(valor), siguiente(sig) {}
    };

    NodoViaje *cabeza;
    int cantidad;

    ListaViajes(const ListaViajes &) = delete;
    ListaViajes &operator=(const ListaViajes &) = delete;
};

#endif
