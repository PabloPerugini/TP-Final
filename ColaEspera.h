#ifndef COLA_ESPERA_H
#define COLA_ESPERA_H

#include <string>

class ColaEspera {
public:
    ColaEspera();
    ~ColaEspera();

    void encolar(const std::string& dni);
    bool desencolar(std::string& dni);
    bool estaVacia() const;
    int tamanio() const;
    void vaciar();

private:
    struct Nodo {
        std::string dni;
        Nodo* siguiente;
        Nodo(const std::string& valor, Nodo* sig = nullptr)
            : dni(valor), siguiente(sig) {}
    };

    Nodo* frente;
    Nodo* fin;
    int cantidad;

    ColaEspera(const ColaEspera&) = delete;
    ColaEspera& operator=(const ColaEspera&) = delete;
};

#endif
