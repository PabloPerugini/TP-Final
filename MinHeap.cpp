#include "MinHeap.h"

MinHeap::MinHeap() : datos(new EntradaTaller[4]), cantidad(0), capacidad(4) {}

MinHeap::~MinHeap() {
    delete[] datos;
}

void MinHeap::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    EntradaTaller* nuevos = new EntradaTaller[nuevaCapacidad];
    for (int i = 0; i < cantidad; ++i) {
        nuevos[i] = datos[i];
    }
    delete[] datos;
    datos = nuevos;
    capacidad = nuevaCapacidad;
}

void MinHeap::subir(int indice) {
    while (indice > 0) {
        int padre = (indice - 1) / 2;
        if (datos[padre].prioridad <= datos[indice].prioridad) {
            break;
        }
        EntradaTaller temporal = datos[padre];
        datos[padre] = datos[indice];
        datos[indice] = temporal;
        indice = padre;
    }
}

void MinHeap::bajar(int indice) {
    while (true) {
        int hijoIzquierdo = 2 * indice + 1;
        int hijoDerecho = 2 * indice + 2;
        int menor = indice;

        if (hijoIzquierdo < cantidad &&
            datos[hijoIzquierdo].prioridad < datos[menor].prioridad) {
            menor = hijoIzquierdo;
        }
        if (hijoDerecho < cantidad &&
            datos[hijoDerecho].prioridad < datos[menor].prioridad) {
            menor = hijoDerecho;
        }
        if (menor == indice) {
            break;
        }

        EntradaTaller temporal = datos[indice];
        datos[indice] = datos[menor];
        datos[menor] = temporal;
        indice = menor;
    }
}

void MinHeap::insertar(int biciId, int prioridad) {
    if (cantidad == capacidad) {
        redimensionar();
    }
    datos[cantidad].biciId = biciId;
    datos[cantidad].prioridad = prioridad;
    subir(cantidad);
    ++cantidad;
}

bool MinHeap::extraerMin(EntradaTaller& salida) {
    if (cantidad == 0) {
        return false;
    }

    salida = datos[0];
    --cantidad;
    if (cantidad > 0) {
        datos[0] = datos[cantidad];
        bajar(0);
    }
    return true;
}

bool MinHeap::contiene(int biciId) const {
    for (int i = 0; i < cantidad; ++i) {
        if (datos[i].biciId == biciId) {
            return true;
        }
    }
    return false;
}

bool MinHeap::estaVacio() const {
    return cantidad == 0;
}

int MinHeap::tamanio() const {
    return cantidad;
}
