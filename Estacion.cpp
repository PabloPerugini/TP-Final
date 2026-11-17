#include "Estacion.h"

// Constructor
Estacion::Estacion(const std::string &codigo, const std::string &nombre,
                   const std::string &barrio, int capacidadAnclajes,
                   int biciDisponibles)
    : codigo(codigo),
      nombre(nombre),
      barrio(barrio),
      capacidadAnclajes(capacidadAnclajes),
      biciDisponibles(biciDisponibles) {}

// Destructor: la cola de espera libera sus nodos en su propio destructor.
Estacion::~Estacion()
{
}

// Getters
const std::string &Estacion::getCodigo() const
{
    return codigo;
}

const std::string &Estacion::getNombre() const
{
    return nombre;
}

const std::string &Estacion::getBarrio() const
{
    return barrio;
}

int Estacion::getCapacidadAnclajes() const
{
    return capacidadAnclajes;
}

int Estacion::getBiciDisponibles() const
{
    return biciDisponibles;
}

// Método de cálculo de ocupación (devuelve valor en porcentaje de 0.0 a 100.0)
double Estacion::porcentajeOcupacion() const
{
    if (capacidadAnclajes <= 0)
    {
        return 0.0;
    }
    return (static_cast<double>(biciDisponibles) / capacidadAnclajes) * 100.0;
}

bool Estacion::retirarBicicleta()
{
    if (biciDisponibles <= 0)
    {
        return false;
    }
    --biciDisponibles;
    return true;
}

ResultadoDevolucion Estacion::devolverBicicleta(std::string &dniAsignado)
{
    if (colaEspera.desencolar(dniAsignado))
    {
        return ResultadoDevolucion::AsignadaAEspera;
    }

    if (biciDisponibles < capacidadAnclajes)
    {
        ++biciDisponibles;
        return ResultadoDevolucion::AgregadaAEstacion;
    }

    return ResultadoDevolucion::SinLugar;
}

void Estacion::encolarUsuario(const std::string &dni)
{
    colaEspera.encolar(dni);
}

int Estacion::getUsuariosEnEspera() const
{
    return colaEspera.tamanio();
}
