#include "Red.h"
#include "TablaHash.h"
#include "Estacion.h"
#include "Backtracking.h"
#include <iostream>
#include <limits>

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
    std::cout << "Cantidad de estaciones: ";
    if (!(std::cin >> cantidad)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida: la cantidad debe ser un numero "
                     "entero.\n";
        return 0;
    }
    if (cantidad <= 0) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "La cantidad debe ser mayor que cero.\n";
        return 0;
    }

    std::string* codigos = new std::string[cantidad];
    int* faltantes = new int[cantidad];
    for (int i = 0; i < cantidad; ++i) {
        std::cout << "Estacion " << (i + 1) << " (codigo faltante): ";
        if (!(std::cin >> codigos[i] >> faltantes[i])) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                            '\n');
            std::cout << "Entrada invalida: se esperaba un codigo y un "
                         "numero entero de faltantes.\n";
            delete[] codigos;
            delete[] faltantes;
            return 0;
        }
    }

    int soluciones = resolverYMostrar(codigos, faltantes, cantidad,
                                      capacidadCamion);
    delete[] codigos;
    delete[] faltantes;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return soluciones;
}
