#include "Red.h"
#include "TablaHash.h"
#include "Estacion.h"
#include <iostream>

namespace
{
    bool debeIrAntes(Estacion *primero, Estacion *segundo, int criterio)
    {
        int valorPrimero = criterio == 2 ? primero->getBiciDisponibles()
                                         : primero->getCapacidadAnclajes();
        int valorSegundo = criterio == 2 ? segundo->getBiciDisponibles()
                                         : segundo->getCapacidadAnclajes();
        if (valorPrimero != valorSegundo)
        {
            return valorPrimero > valorSegundo;
        }
        return primero->getCodigo() < segundo->getCodigo();
    }

    void ordenarEstaciones(Estacion **arreglo, int cantidad, int criterio)
    {
        for (int i = 1; i < cantidad; ++i)
        {
            Estacion *actual = arreglo[i];
            int j = i - 1;
            while (j >= 0 && debeIrAntes(actual, arreglo[j], criterio))
            {
                arreglo[j + 1] = arreglo[j];
                --j;
            }
            arreglo[j + 1] = actual;
        }
    }

    void imprimirFilaEstacion(const Estacion *estacion)
    {
        std::cout << "[" << estacion->getCodigo() << "] "
                  << estacion->getNombre() << " | "
                  << estacion->getBarrio() << " | Anclajes: "
                  << estacion->getCapacidadAnclajes() << " | Disponibles: "
                  << estacion->getBiciDisponibles() << " | Ocupacion: "
                  << estacion->porcentajeOcupacion() << "%\n";
    }
} // namespace

bool Red::mostrarInformacionEstacion(const std::string &codigo) const
{
    Estacion *estacion = estaciones->obtener(codigo);
    if (estacion == nullptr)
    {
        std::cout << "No existe la estacion " << codigo << ".\n";
        return false;
    }

    std::cout << "=== INFORMACION DE LA ESTACION " << codigo << " ===\n";
    std::cout << "Nombre: " << estacion->getNombre() << "\n";
    std::cout << "Barrio: " << estacion->getBarrio() << "\n";
    std::cout << "Capacidad de anclajes: " << estacion->getCapacidadAnclajes()
              << "\n";
    std::cout << "Bicicletas disponibles: " << estacion->getBiciDisponibles()
              << "\n";
    std::cout << "Ocupacion: " << estacion->porcentajeOcupacion() << "%\n";
    std::cout << "Usuarios en espera: " << estacion->getUsuariosEnEspera()
              << "\n";
    return true;
}

bool Red::agregarEstacion(const std::string &codigo, const std::string &nombre,
                          const std::string &barrio, int capacidadAnclajes,
                          int biciDisponibles)
{
    if (codigo.empty() || capacidadAnclajes <= 0 || biciDisponibles < 0 ||
        biciDisponibles > capacidadAnclajes)
    {
        std::cout << "Datos invalidos para la estacion.\n";
        return false;
    }
    if (estaciones->obtener(codigo) != nullptr)
    {
        std::cout << "Ya existe una estacion con el codigo " << codigo << ".\n";
        return false;
    }

    Estacion *nueva = new Estacion(codigo, nombre, barrio, capacidadAnclajes,
                                   biciDisponibles);
    if (!estaciones->insertar(codigo, nueva))
    {
        delete nueva;
        return false;
    }
    ciclovias->agregarEstacion(codigo);
    return true;
}

bool Red::eliminarEstacion(const std::string &codigo)
{
    Estacion *estacion = estaciones->obtener(codigo);
    if (estacion == nullptr)
    {
        std::cout << "No existe la estacion " << codigo << ".\n";
        return false;
    }

    int usuariosEnEspera = estacion->getUsuariosEnEspera();
    if (usuariosEnEspera > 0)
    {
        std::cout << "Aviso: se pierden " << usuariosEnEspera
                  << " usuarios en espera de la estacion " << codigo << ".\n";
    }

    bool eliminada = estaciones->eliminar(codigo);
    if (eliminada)
    {
        ciclovias->eliminarEstacion(codigo);
    }
    return eliminada;
}

void Red::listarEstacionesOrdenadas(int criterio) const
{
    int cantidad = estaciones->getCantidad();
    if (cantidad == 0)
    {
        std::cout << "No hay estaciones cargadas.\n";
        return;
    }

    Estacion **arreglo = new Estacion *[cantidad];
    int volcadas = estaciones->volcarEstaciones(arreglo, cantidad);
    ordenarEstaciones(arreglo, volcadas, criterio);

    if (criterio == 2)
    {
        std::cout << "=== ESTACIONES ORDENADAS POR BICICLETAS DISPONIBLES ===\n";
    }
    else
    {
        std::cout << "=== ESTACIONES ORDENADAS POR CAPACIDAD DE ANCLAJES ===\n";
    }
    for (int i = 0; i < volcadas; ++i)
    {
        imprimirFilaEstacion(arreglo[i]);
    }
    delete[] arreglo;
}

void Red::listarEstacionesBajaOcupacion() const
{
    int cantidad = estaciones->getCantidad();
    if (cantidad == 0)
    {
        std::cout << "No hay estaciones cargadas.\n";
        return;
    }

    Estacion **arreglo = new Estacion *[cantidad];
    int volcadas = estaciones->volcarEstaciones(arreglo, cantidad);

    std::cout << "=== ESTACIONES CON MENOS DEL 20% DE OCUPACION ===\n";
    bool encontrada = false;
    for (int i = 0; i < volcadas; ++i)
    {
        if (arreglo[i]->porcentajeOcupacion() < 20.0)
        {
            imprimirFilaEstacion(arreglo[i]);
            encontrada = true;
        }
    }
    if (!encontrada)
    {
        std::cout << "No hay estaciones con baja ocupacion.\n";
    }
    delete[] arreglo;
}

ResultadoCamino Red::calcularCaminoMasCortoBFS(const std::string &origen,
                                               const std::string &destino,
                                               std::string &camino,
                                               int &tramos)
{
    return ciclovias->buscarCaminoMasCorto(origen, destino, camino, tramos);
}
