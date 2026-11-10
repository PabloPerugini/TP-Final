#ifndef PRUEBAS_H
#define PRUEBAS_H

#include <iostream>
#include <sstream>
#include <string>

extern int fallos;

#define CHECK(condicion, mensaje)                                            \
    do {                                                                     \
        if (!(condicion)) {                                                  \
            std::cout << "FALLO: " << mensaje << " (" << __FILE__ << ":"    \
                      << __LINE__ << ")\n";                                  \
            ++fallos;                                                        \
        }                                                                    \
    } while (false)

template <typename Funcion>
std::string capturarSalida(Funcion funcion) {
    std::ostringstream salida;
    std::streambuf* anterior = std::cout.rdbuf(salida.rdbuf());
    funcion();
    std::cout.rdbuf(anterior);
    return salida.str();
}

void probarColaEspera();
void probarEstacion();
void probarTablaHash();
void probarListaViajes();
void probarMinHeap();

#endif
