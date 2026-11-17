#include "TablaHash.h"
#include "Estacion.h"
#include <iostream>

// Constructor: inicializa los buckets en nullptr y resetea contadores
TablaHash::TablaHash() : cantidad(0), colisiones(0)
{
    for (int i = 0; i < TAMANIO; ++i)
    {
        tabla[i] = nullptr;
    }
}

// Destructor: libera la memoria de cada Estacion y de cada Nodo (Dueño total)
TablaHash::~TablaHash()
{
    vaciar();
}

// Libera todo el contenido y deja la tabla lista para reutilizarse.
void TablaHash::vaciar()
{
    for (int i = 0; i < TAMANIO; ++i)
    {
        Nodo *actual = tabla[i];
        while (actual != nullptr)
        {
            Nodo *aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar->datos;
            delete aBorrar;
        }
        tabla[i] = nullptr;
    }
    cantidad = 0;
    colisiones = 0;
}

// Función hash: suma ponderada por posición 1-indexed codificada a módulo 13
int TablaHash::calcularIndice(const std::string &codigo) const
{
    int h = 0;
    for (size_t i = 0; i < codigo.length(); ++i)
    {
        h += static_cast<int>(codigo[i]) * static_cast<int>(i + 1);
    }
    return h % TAMANIO;
}

// Inserción al frente (O(1)) controlando duplicados y métrica de colisiones
bool TablaHash::insertar(const std::string &codigo, Estacion *e)
{
    if (e == nullptr)
        return false;

    // Borde ②: Evitar códigos duplicados
    if (obtener(codigo) != nullptr)
    {
        return false;
    }

    int indice = calcularIndice(codigo);

    // Métrica de colisión: si el bucket ya tenía al menos un elemento
    if (tabla[indice] != nullptr)
    {
        colisiones++;
    }

    // Inserción al frente de la lista enlazada
    Nodo *nuevoNodo = new Nodo(e, tabla[indice]);
    tabla[indice] = nuevoNodo;
    cantidad++;

    return true;
}

// Búsqueda por clave (código)
Estacion *TablaHash::obtener(const std::string &codigo) const
{
    int indice = calcularIndice(codigo);
    Nodo *actual = tabla[indice];

    while (actual != nullptr)
    {
        if (actual->datos != nullptr && actual->datos->getCodigo() == codigo)
        {
            return actual->datos;
        }
        actual = actual->siguiente;
    }

    return nullptr; // Borde ①: No encontrado
}

// Eliminación de un elemento desenganchando el nodo y borrando su memoria
bool TablaHash::eliminar(const std::string &codigo)
{
    int indice = calcularIndice(codigo);
    Nodo *actual = tabla[indice];
    Nodo *anterior = nullptr;

    while (actual != nullptr)
    {
        if (actual->datos != nullptr && actual->datos->getCodigo() == codigo)
        {
            // Desenganche de la lista
            if (anterior == nullptr)
            {
                tabla[indice] = actual->siguiente;
            }
            else
            {
                anterior->siguiente = actual->siguiente;
            }

            delete actual->datos; // Libera la instancia de Estacion
            delete actual;        // Libera el nodo de la lista
            cantidad--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

// Imprime todas las estaciones organizadas por bucket
void TablaHash::listarTodas() const
{
    for (int i = 0; i < TAMANIO; ++i)
    {
        Nodo *actual = tabla[i];
        if (actual != nullptr)
        {
            std::cout << "Bucket " << i << ":\n";
            while (actual != nullptr)
            {
                std::cout << "  - [" << actual->datos->getCodigo() << "] "
                          << actual->datos->getNombre() << " ("
                          << actual->datos->getBarrio() << ")\n";
                actual = actual->siguiente;
            }
        }
    }
}

int TablaHash::volcarEstaciones(Estacion **destino, int capacidad) const
{
    if (destino == nullptr || capacidad <= 0)
    {
        return 0;
    }

    int escritos = 0;
    for (int i = 0; i < TAMANIO && escritos < capacidad; ++i)
    {
        Nodo *actual = tabla[i];
        while (actual != nullptr && escritos < capacidad)
        {
            destino[escritos] = actual->datos;
            ++escritos;
            actual = actual->siguiente;
        }
    }
    return escritos;
}

int TablaHash::getCantidad() const
{
    return cantidad;
}

int TablaHash::getColisiones() const
{
    return colisiones;
}

double TablaHash::factorDeCarga() const
{
    return static_cast<double>(cantidad) / TAMANIO;
}

int TablaHash::getCapacidad() const
{
    return TAMANIO;
}