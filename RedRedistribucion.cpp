#include "Red.h"
#include "TablaHash.h"
#include "Estacion.h"
#include "Backtracking.h"
#include <iostream>
#include <sstream>

namespace {
int resolverYMostrar(const std::string* codigos, const int* faltantes,
                     int cantidad, int capacidad) {
    std::cout << "Capacidad del camion: " << capacidad << "\n";
    std::cout << "Estaciones consideradas: " << cantidad << "\n";
    int soluciones =
        Backtracking::resolver(codigos, faltantes, cantidad, capacidad, true);
    if (soluciones == 0) {
        std::cout << "No hay subconjuntos que sumen exactamente la capacidad.\n";
    } else {
        std::cout << "Soluciones encontradas: " << soluciones << "\n";
    }
    return soluciones;
}
}  // namespace

int Red::calcularRedistribucionBacktracking(int capacidadCamion) const {
    std::cout << "=== REDISTRIBUCION CON FALTANTES DEL SISTEMA ===\n";
    if (capacidadCamion <= 0) {
        std::cout << "La capacidad del camion debe ser mayor que cero.\n";
        return 0;
    }

    int cantidad = estaciones->getCantidad();
    if (cantidad == 0) {
        std::cout << "No hay estaciones cargadas.\n";
        return 0;
    }

    Estacion** arreglo = new Estacion*[cantidad];
    int volcadas = estaciones->volcarEstaciones(arreglo, cantidad);
    std::string* codigos = new std::string[volcadas];
    int* faltantes = new int[volcadas];
    int consideradas = 0;

    for (int i = 0; i < volcadas; ++i) {
        int faltante = arreglo[i]->getCapacidadAnclajes() -
                       arreglo[i]->getBiciDisponibles();
        if (faltante > 0) {
            codigos[consideradas] = arreglo[i]->getCodigo();
            faltantes[consideradas] = faltante;
            ++consideradas;
        }
    }

    int soluciones =
        resolverYMostrar(codigos, faltantes, consideradas, capacidadCamion);

    delete[] arreglo;
    delete[] codigos;
    delete[] faltantes;
    return soluciones;
}

int Red::calcularRedistribucionManual(int capacidadCamion) const {
    std::cout << "=== REDISTRIBUCION CON LISTA MANUAL ===\n";
    if (capacidadCamion <= 0) {
        std::cout << "La capacidad del camion debe ser mayor que cero.\n";
        return 0;
    }

    int cantidad = 0;
    std::string linea;
    std::cout << "Cantidad de estaciones: ";
    if (!std::getline(std::cin, linea)) {
        return 0;
    }
    std::istringstream issCantidad(linea);
    int leido;
    std::string sobrante;
    if (!(issCantidad >> leido) || (issCantidad >> sobrante)) {
        std::cout << "Entrada invalida: la cantidad debe ser un numero "
                     "entero.\n";
        return 0;
    }
    cantidad = leido;
    if (cantidad <= 0) {
        std::cout << "La cantidad debe ser mayor que cero.\n";
        return 0;
    }

    std::string* codigos = new std::string[cantidad];
    int* faltantes = new int[cantidad];
    for (int i = 0; i < cantidad; ++i) {
        std::cout << "Estacion " << (i + 1) << " (codigo faltante): ";
        if (!std::getline(std::cin, linea)) {
            delete[] codigos;
            delete[] faltantes;
            return 0;
        }
        std::istringstream iss(linea);
        std::string codigo;
        int faltante = 0;
        std::string sobrante;
        if (!(iss >> codigo >> faltante) || (iss >> sobrante)) {
            delete[] codigos;
            delete[] faltantes;
            std::cout << "Entrada invalida: se esperaba un codigo y un "
                         "numero entero de faltantes.\n";
            return 0;
        }
        codigos[i] = codigo;
        faltantes[i] = faltante;
    }

    int soluciones = resolverYMostrar(codigos, faltantes, cantidad,
                                      capacidadCamion);
    delete[] codigos;
    delete[] faltantes;
    return soluciones;
}
