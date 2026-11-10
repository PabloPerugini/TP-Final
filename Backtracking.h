#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <string>

class Backtracking {
public:
    static int resolver(const std::string* codigos, const int* faltantes,
                        int cantidadEstaciones, int capacidad,
                        bool imprimir);

private:
    static void buscar(const std::string* codigos, const int* faltantes,
                       int cantidadEstaciones, int capacidad, int indice,
                       int sumaActual, bool* seleccion, int& soluciones,
                       bool imprimir);
    static void imprimirSolucion(const std::string* codigos,
                                 const bool* seleccion,
                                 int cantidadEstaciones, int numero);
};

#endif
