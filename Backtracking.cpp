#include "Backtracking.h"
#include <iostream>

int Backtracking::resolver(const std::string* codigos, const int* faltantes,
                           int cantidadEstaciones, int capacidad,
                           bool imprimir) {
    if (codigos == nullptr || faltantes == nullptr ||
        cantidadEstaciones <= 0 || capacidad <= 0) {
        return 0;
    }

    bool* seleccion = new bool[cantidadEstaciones];
    for (int i = 0; i < cantidadEstaciones; ++i) {
        seleccion[i] = false;
    }

    int soluciones = 0;
    buscar(codigos, faltantes, cantidadEstaciones, capacidad, 0, 0,
           seleccion, soluciones, imprimir);
    delete[] seleccion;
    return soluciones;
}

void Backtracking::buscar(const std::string* codigos, const int* faltantes,
                          int cantidadEstaciones, int capacidad, int indice,
                          int sumaActual, bool* seleccion, int& soluciones,
                          bool imprimir) {
    if (sumaActual == capacidad) {
        ++soluciones;
        if (imprimir) {
            imprimirSolucion(codigos, seleccion, cantidadEstaciones,
                             soluciones);
        }
        return;
    }
    if (indice >= cantidadEstaciones || sumaActual > capacidad) {
        return;
    }

    if (faltantes[indice] > 0 &&
        sumaActual + faltantes[indice] <= capacidad) {
        seleccion[indice] = true;
        buscar(codigos, faltantes, cantidadEstaciones, capacidad, indice + 1,
               sumaActual + faltantes[indice], seleccion, soluciones, imprimir);
        seleccion[indice] = false;
    }

    buscar(codigos, faltantes, cantidadEstaciones, capacidad, indice + 1,
           sumaActual, seleccion, soluciones, imprimir);
}

void Backtracking::imprimirSolucion(const std::string* codigos,
                                    const bool* seleccion,
                                    int cantidadEstaciones, int numero) {
    std::cout << "Solucion " << numero << ": {";
    bool primero = true;
    for (int i = 0; i < cantidadEstaciones; ++i) {
        if (seleccion[i]) {
            if (!primero) {
                std::cout << ", ";
            }
            std::cout << codigos[i];
            primero = false;
        }
    }
    std::cout << "}\n";
}
