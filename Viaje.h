#ifndef VIAJE_H
#define VIAJE_H
#include <string>

class Viaje
{
public:
    Viaje(int viajeId, int biciId, const std::string &dniUsuario,
          const std::string &estacionOrigen, const std::string &estacionDestino,
          int fecha, int duracionMin);

    int getViajeId() const;
    int getBiciId() const;
    const std::string &getDniUsuario() const;
    const std::string &getEstacionOrigen() const;
    const std::string &getEstacionDestino() const;
    int getFecha() const; // formato AAAAMMDD, se compara directo
    int getDuracionMin() const;

private:
    int viajeId;
    int biciId; // asociacion: referencia por ID
    std::string dniUsuario;
    std::string estacionOrigen; // asociacion: referencia por codigo
    std::string estacionDestino;
    int fecha;
    int duracionMin;
};

#endif
