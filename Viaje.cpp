#include "Viaje.h"

// Constructor utilizando lista de inicialización de miembros
Viaje::Viaje(int viajeId, int biciId, const std::string &dniUsuario,
             const std::string &estacionOrigen, const std::string &estacionDestino,
             int fecha, int duracionMin)
    : viajeId(viajeId),
      biciId(biciId),
      dniUsuario(dniUsuario),
      estacionOrigen(estacionOrigen),
      estacionDestino(estacionDestino),
      fecha(fecha),
      duracionMin(duracionMin) {}

// Getters
int Viaje::getViajeId() const
{
    return viajeId;
}

int Viaje::getBiciId() const
{
    return biciId;
}

const std::string &Viaje::getDniUsuario() const
{
    return dniUsuario;
}

const std::string &Viaje::getEstacionOrigen() const
{
    return estacionOrigen;
}

const std::string &Viaje::getEstacionDestino() const
{
    return estacionDestino;
}

int Viaje::getFecha() const
{
    return fecha;
}

int Viaje::getDuracionMin() const
{
    return duracionMin;
}
