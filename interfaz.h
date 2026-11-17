#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <string>

class Red;

namespace interfaz
{
    void mostrarMenu();
    bool leerEntero(const std::string &mensaje, int &valor);
    bool leerTexto(const std::string &mensaje, std::string &valor);
    void pausar();
    void mostrarCamino(Red &red);
    void ejecutarTaller(Red &red);
    void ejecutarRedistribucion(Red &red);
    void insertarBicicletaManual(Red &red);
} // namespace interfaz

#endif
