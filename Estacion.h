#ifndef ESTACION_H
#define ESTACION_H

#include <string>
#include "ColaEspera.h"

enum class ResultadoDevolucion
{
    AsignadaAEspera,
    AgregadaAEstacion,
    SinLugar
};

class Estacion
{
public:
    Estacion(const std::string &codigo, const std::string &nombre,
             const std::string &barrio, int capacidadAnclajes,
             int biciDisponibles);
    ~Estacion();

    const std::string &getCodigo() const;
    const std::string &getNombre() const;
    const std::string &getBarrio() const;
    int getCapacidadAnclajes() const;
    int getBiciDisponibles() const;

    double porcentajeOcupacion() const;

    bool retirarBicicleta();
    ResultadoDevolucion devolverBicicleta(std::string &dniAsignado);
    void encolarUsuario(const std::string &dni);
    int getUsuariosEnEspera() const;

private:
    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capacidadAnclajes;
    int biciDisponibles;
    ColaEspera colaEspera;
};

#endif
