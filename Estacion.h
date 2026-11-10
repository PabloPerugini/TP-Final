#ifndef ESTACION_H
#define ESTACION_H
#include <string>

class Estacion {
public:
    Estacion(const std::string& codigo, const std::string& nombre,
             const std::string& barrio, int capacidadAnclajes,
             int biciDisponibles);
    ~Estacion();

    const std::string& getCodigo() const;
    const std::string& getNombre() const;
    const std::string& getBarrio() const;
    int getCapacidadAnclajes() const;
    int getBiciDisponibles() const;

    double porcentajeOcupacion() const;

private:
    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capacidadAnclajes;
    int biciDisponibles;

};

#endif
