#ifndef LISTA_VIAJES_H
#define LISTA_VIAJES_H

#include "Viaje.h"

struct NodoViaje {
    Viaje* datos;
    NodoViaje* siguiente;
    NodoViaje(Viaje* v, NodoViaje* sig = nullptr) : datos(v), siguiente(sig) {}
};

class ListaViajes {
public:
    ListaViajes() : cabeza(nullptr), cantidad(0) {}
    ~ListaViajes() {
        NodoViaje* actual = cabeza;
        while (actual != nullptr) {
            NodoViaje* aBorrar = actual;
            actual = actual->siguiente;
            delete aBorrar->datos;
            delete aBorrar;
        }
    }

    void insertar(Viaje* v) {
        cabeza = new NodoViaje(v, cabeza);
        cantidad++;
    }

    int contarPorRangoFechas(const std::string& estacionOrigen, int fechaInicio, int fechaFin) const {
        int contador = 0;
        NodoViaje* actual = cabeza;
        while (actual != nullptr) {
            if (actual->datos->getEstacionOrigen() == estacionOrigen &&
                actual->datos->getFecha() >= fechaInicio &&
                actual->datos->getFecha() <= fechaFin) {
                contador++;
            }
            actual = actual->siguiente;
        }
        return contador;
    }

    void buscarPorDni(const std::string& dni) const {
        int totalMinutos = 0;
        int viajesEncontrados = 0;
        NodoViaje* actual = cabeza;

        std::cout << "\n=== VIAJES DEL USUARIO DNI: " << dni << " ===\n";
        while (actual != nullptr) {
            if (actual->datos->getDniUsuario() == dni) {
                std::cout << "- Viaje #" << actual->datos->getViajeId()
                          << " | Bici #" << actual->datos->getBiciId()
                          << " | Origen: " << actual->datos->getEstacionOrigen()
                          << " -> Destino: " << actual->datos->getEstacionDestino()
                          << " | Fecha: " << actual->datos->getFecha()
                          << " | Duracion: " << actual->datos->getDuracionMin() << " min\n";
                totalMinutos += actual->datos->getDuracionMin();
                viajesEncontrados++;
            }
            actual = actual->siguiente;
        }
        std::cout << "Total de viajes: " << viajesEncontrados 
                  << " | Tiempo total pedaleado: " << totalMinutos << " minutos.\n";
    }

private:
    NodoViaje* cabeza;
    int cantidad;
};

#endif