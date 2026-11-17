#include "Red.h"
#include "TablaHash.h"
#include "Estacion.h"
#include "ListaViajes.h"
#include "AbbBicicletas.h"
#include <iostream>

int Red::contarViajesPorRangoFechas(const std::string &estacionOrigen,
                                    int fechaInicio, int fechaFin) const
{
    return viajes->contarPorRangoFechas(estacionOrigen, fechaInicio, fechaFin);
}

int Red::buscarViajesUsuario(const std::string &dni) const
{
    return viajes->buscarPorDni(dni);
}

void Red::listarBicicletasParaRevision(int limiteKilometros) const
{
    std::cout << "=== BICICLETAS CON MAS DE " << limiteKilometros
              << " KM ===\n";
    bicicletas->listarConKilometrosMayoresA(limiteKilometros);
}

bool Red::retirarBicicletaEnEstacion(const std::string &codigo,
                                     const std::string &dni)
{
    Estacion *estacion = estaciones->obtener(codigo);
    if (estacion == nullptr)
    {
        std::cout << "No existe la estacion " << codigo << ".\n";
        return false;
    }
    if (dni.empty())
    {
        std::cout << "El DNI no puede estar vacio.\n";
        return false;
    }

    if (estacion->retirarBicicleta())
    {
        std::cout << "Retiro habilitado en " << codigo << " para DNI " << dni
                  << ".\n";
    }
    else
    {
        estacion->encolarUsuario(dni);
        std::cout << "No hay bicicletas disponibles en " << codigo
                  << ". El DNI " << dni << " quedo en espera.\n";
    }
    return true;
}

ResultadoDevolucionRed Red::devolverBicicletaEnEstacion(
    const std::string &codigo, std::string &dniAsignado)
{
    dniAsignado.clear();
    Estacion *estacion = estaciones->obtener(codigo);
    if (estacion == nullptr)
    {
        return ResultadoDevolucionRed::EstacionInexistente;
    }

    ResultadoDevolucion resultado = estacion->devolverBicicleta(dniAsignado);
    if (resultado == ResultadoDevolucion::AsignadaAEspera)
    {
        return ResultadoDevolucionRed::AsignadaAEspera;
    }
    if (resultado == ResultadoDevolucion::AgregadaAEstacion)
    {
        return ResultadoDevolucionRed::AgregadaAEstacion;
    }
    return ResultadoDevolucionRed::SinLugar;
}

bool Red::ingresarBiciTaller(int biciId, int prioridad)
{
    if (prioridad < 1 || prioridad > 3)
    {
        std::cout << "La prioridad debe estar entre 1 y 3.\n";
        return false;
    }

    Bicicleta *bicicleta = bicicletas->buscar(biciId);
    if (bicicleta == nullptr)
    {
        std::cout << "No existe la bicicleta " << biciId << ".\n";
        return false;
    }

    if (taller->contiene(biciId))
    {
        std::cout << "La bicicleta " << biciId << " ya esta en el taller.\n";
        return false;
    }

    int prioridadFinal = prioridad;
    if (bicicleta->getTipo() == TipoBici::Electrica)
    {
        prioridadFinal = 1;
    }
    if (bicicleta->getEstado() != EstadoBici::Averiada)
    {
        std::cout << "Aviso: la bicicleta " << biciId
                  << " no figura averiada; se ingresa igual al taller.\n";
    }

    taller->insertar(biciId, prioridadFinal);
    std::cout << "Bicicleta " << biciId << " ingresada al taller con prioridad "
              << prioridadFinal << ".\n";
    return true;
}

bool Red::extraerBiciMasUrgenteTaller(EntradaTaller &salida)
{
    return taller->extraerMin(salida);
}
