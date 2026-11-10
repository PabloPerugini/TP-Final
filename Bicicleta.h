#ifndef BICICLETA_H
#define BICICLETA_H
#include <string>

enum class TipoBici { Mecanica, Electrica };
enum class EstadoBici { OK, Averiada };

class Bicicleta {
public:
    Bicicleta(int biciId, const std::string& codigoEstacion,
              TipoBici tipo, EstadoBici estado, int kilometros);

    int getBiciId() const;
    const std::string& getCodigoEstacion() const;
    TipoBici getTipo() const;
    EstadoBici getEstado() const;
    int getKilometros() const;

    void setCodigoEstacion(const std::string& codigo);

private:
    int biciId;
    std::string codigoEstacion;
    TipoBici tipo;
    EstadoBici estado;
    int kilometros;
};
#endif
