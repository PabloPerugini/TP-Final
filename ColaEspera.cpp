#include "ColaEspera.h"

ColaEspera::ColaEspera() : frente(nullptr), fin(nullptr), cantidad(0) {}

ColaEspera::~ColaEspera()
{
    vaciar();
}

void ColaEspera::encolar(const std::string &dni)
{
    Nodo *nuevo = new Nodo(dni);
    if (fin == nullptr)
    {
        frente = nuevo;
    }
    else
    {
        fin->siguiente = nuevo;
    }
    fin = nuevo;
    ++cantidad;
}

bool ColaEspera::desencolar(std::string &dni)
{
    if (frente == nullptr)
    {
        return false;
    }

    Nodo *aBorrar = frente;
    dni = frente->dni;
    frente = frente->siguiente;
    if (frente == nullptr)
    {
        fin = nullptr;
    }

    delete aBorrar;
    --cantidad;
    return true;
}

bool ColaEspera::estaVacia() const
{
    return frente == nullptr;
}

int ColaEspera::tamanio() const
{
    return cantidad;
}

void ColaEspera::vaciar()
{
    Nodo *actual = frente;
    while (actual != nullptr)
    {
        Nodo *aBorrar = actual;
        actual = actual->siguiente;
        delete aBorrar;
    }
    frente = nullptr;
    fin = nullptr;
    cantidad = 0;
}
