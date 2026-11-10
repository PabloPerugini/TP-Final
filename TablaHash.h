#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <string>

// Forward declaration: evita acoplamiento y acelera la compilación
class Estacion;

class TablaHash {
public:
    TablaHash();
    ~TablaHash();

    // Contrato de la interfaz pública
    bool insertar(const std::string& codigo, Estacion* e);
    Estacion* obtener(const std::string& codigo) const;
    bool eliminar(const std::string& codigo);
    void listarTodas() const;
    int getCantidad() const;
    int getColisiones() const;
    double factorDeCarga() const;

private:
    static const int TAMANIO = 13;

    struct Nodo {
        Estacion* datos;
        Nodo* siguiente;
        Nodo(Estacion* e, Nodo* sig = nullptr) : datos(e), siguiente(sig) {}
    };

    int calcularIndice(const std::string& codigo) const;

    Nodo* tabla[TAMANIO];
    int cantidad;
    int colisiones;
};

#endif // TABLA_HASH_H