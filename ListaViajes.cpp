#include "ListaViajes.h"
#include <iostream>

ListaViajes::ListaViajes() : cabeza(nullptr), cantidad(0) {}

ListaViajes::~ListaViajes() {
    vaciar();
}

void ListaViajes::insertar(Viaje* viaje) {
    if (viaje == nullptr) {
        return;
    }
    cabeza = new NodoViaje(viaje, cabeza);
    ++cantidad;
}

int ListaViajes::contarPorRangoFechas(const std::string& estacionOrigen,
                                      int fechaInicio, int fechaFin) const {
    int contador = 0;
    NodoViaje* actual = cabeza;
    while (actual != nullptr) {
        const Viaje* viaje = actual->dato;
        if (viaje->getEstacionOrigen() == estacionOrigen &&
            viaje->getFecha() >= fechaInicio &&
            viaje->getFecha() <= fechaFin) {
            ++contador;
        }
        actual = actual->siguiente;
    }
    return contador;
}

int ListaViajes::buscarPorDni(const std::string& dni) const {
    int totalMinutos = 0;
    int encontrados = 0;
    NodoViaje* actual = cabeza;

    std::cout << "\n=== VIAJES DEL USUARIO DNI: " << dni << " ===\n";
    while (actual != nullptr) {
        const Viaje* viaje = actual->dato;
        if (viaje->getDniUsuario() == dni) {
            std::cout << "- Viaje #" << viaje->getViajeId()
                      << " | Bici #" << viaje->getBiciId()
                      << " | Origen: " << viaje->getEstacionOrigen()
                      << " -> Destino: " << viaje->getEstacionDestino()
                      << " | Fecha: " << viaje->getFecha()
                      << " | Duracion: " << viaje->getDuracionMin() << " min\n";
            totalMinutos += viaje->getDuracionMin();
            ++encontrados;
        }
        actual = actual->siguiente;
    }

    std::cout << "Total de viajes: " << encontrados
              << " | Tiempo total pedaleado: " << totalMinutos << " minutos.\n";
    return totalMinutos;
}

int ListaViajes::getCantidad() const {
    return cantidad;
}

void ListaViajes::vaciar() {
    NodoViaje* actual = cabeza;
    while (actual != nullptr) {
        NodoViaje* aBorrar = actual;
        actual = actual->siguiente;
        delete aBorrar->dato;
        delete aBorrar;
    }
    cabeza = nullptr;
    cantidad = 0;
}
