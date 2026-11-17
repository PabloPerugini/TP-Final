#include "Bicicleta.h"

Bicicleta::Bicicleta(int biciId, const std::string &codigoEstacion,
                     TipoBici tipo, EstadoBici estado, int kilometros)
    : biciId(biciId),
      codigoEstacion(codigoEstacion),
      tipo(tipo),
      estado(estado),
      kilometros(kilometros) {}

// Getters
int Bicicleta::getBiciId() const
{
    return biciId;
}

const std::string &Bicicleta::getCodigoEstacion() const
{
    return codigoEstacion;
}

TipoBici Bicicleta::getTipo() const
{
    return tipo;
}

EstadoBici Bicicleta::getEstado() const
{
    return estado;
}

int Bicicleta::getKilometros() const
{
    return kilometros;
}

// Setters
void Bicicleta::setCodigoEstacion(const std::string &codigo)
{
    codigoEstacion = codigo;
}