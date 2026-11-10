#include "AbbBicicletas.h"
#include <iostream>

AbbBicicletas::AbbBicicletas() : raiz(nullptr) {}

AbbBicicletas::~AbbBicicletas() {
    vaciar();
}

bool AbbBicicletas::insertar(Bicicleta* bicicleta) {
    if (bicicleta == nullptr) {
        return false;
    }
    return insertarRec(raiz, bicicleta);
}

bool AbbBicicletas::insertarRec(NodoAbb*& nodo, Bicicleta* bicicleta) {
    if (nodo == nullptr) {
        nodo = new NodoAbb(bicicleta);
        return true;
    }
    if (bicicleta->getBiciId() < nodo->dato->getBiciId()) {
        return insertarRec(nodo->izquierdo, bicicleta);
    }
    if (bicicleta->getBiciId() > nodo->dato->getBiciId()) {
        return insertarRec(nodo->derecho, bicicleta);
    }
    return false;
}

Bicicleta* AbbBicicletas::buscar(int id) const {
    NodoAbb* actual = raiz;
    while (actual != nullptr) {
        if (id == actual->dato->getBiciId()) {
            return actual->dato;
        }
        actual = (id < actual->dato->getBiciId())
                     ? actual->izquierdo
                     : actual->derecho;
    }
    return nullptr;
}

void AbbBicicletas::listarInorder() const {
    listarInorderRec(raiz);
}

void AbbBicicletas::listarInorderRec(NodoAbb* nodo) const {
    if (nodo == nullptr) {
        return;
    }
    listarInorderRec(nodo->izquierdo);
    std::cout << "Bici #" << nodo->dato->getBiciId()
              << " | Estacion: " << nodo->dato->getCodigoEstacion()
              << " | Tipo: "
              << (nodo->dato->getTipo() == TipoBici::Electrica
                      ? "Electrica"
                      : "Mecanica")
              << " | Estado: "
              << (nodo->dato->getEstado() == EstadoBici::Averiada
                      ? "Averiada"
                      : "OK")
              << " | Km: " << nodo->dato->getKilometros() << "\n";
    listarInorderRec(nodo->derecho);
}

int AbbBicicletas::contarAveriadas() const {
    return contarAveriadasRec(raiz);
}

int AbbBicicletas::contarAveriadasRec(NodoAbb* nodo) const {
    if (nodo == nullptr) {
        return 0;
    }
    int total = nodo->dato->getEstado() == EstadoBici::Averiada ? 1 : 0;
    return total + contarAveriadasRec(nodo->izquierdo) +
           contarAveriadasRec(nodo->derecho);
}

int AbbBicicletas::altura() const {
    return alturaRec(raiz);
}

int AbbBicicletas::alturaRec(NodoAbb* nodo) const {
    if (nodo == nullptr) {
        return 0;
    }
    int alturaIzquierda = alturaRec(nodo->izquierdo);
    int alturaDerecha = alturaRec(nodo->derecho);
    return 1 + (alturaIzquierda > alturaDerecha ? alturaIzquierda
                                                : alturaDerecha);
}

bool AbbBicicletas::eliminar(int id) {
    bool eliminado = false;
    raiz = eliminarRec(raiz, id, eliminado);
    return eliminado;
}

AbbBicicletas::NodoAbb* AbbBicicletas::eliminarRec(NodoAbb* nodo, int id,
                                                   bool& eliminado) {
    if (nodo == nullptr) {
        return nullptr;
    }

    if (id < nodo->dato->getBiciId()) {
        nodo->izquierdo = eliminarRec(nodo->izquierdo, id, eliminado);
        return nodo;
    }
    if (id > nodo->dato->getBiciId()) {
        nodo->derecho = eliminarRec(nodo->derecho, id, eliminado);
        return nodo;
    }

    eliminado = true;
    if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
        delete nodo->dato;
        delete nodo;
        return nullptr;
    }
    if (nodo->izquierdo == nullptr) {
        NodoAbb* hijoDerecho = nodo->derecho;
        delete nodo->dato;
        delete nodo;
        return hijoDerecho;
    }
    if (nodo->derecho == nullptr) {
        NodoAbb* hijoIzquierdo = nodo->izquierdo;
        delete nodo->dato;
        delete nodo;
        return hijoIzquierdo;
    }

    NodoAbb* sucesor = minimo(nodo->derecho);
    Bicicleta* bicicletaOriginal = nodo->dato;
    nodo->dato = sucesor->dato;
    nodo->derecho = quitarMinimo(nodo->derecho);
    delete bicicletaOriginal;
    return nodo;
}

AbbBicicletas::NodoAbb* AbbBicicletas::quitarMinimo(NodoAbb* nodo) {
    if (nodo->izquierdo == nullptr) {
        NodoAbb* hijoDerecho = nodo->derecho;
        delete nodo;
        return hijoDerecho;
    }
    nodo->izquierdo = quitarMinimo(nodo->izquierdo);
    return nodo;
}

AbbBicicletas::NodoAbb* AbbBicicletas::minimo(NodoAbb* nodo) const {
    while (nodo != nullptr && nodo->izquierdo != nullptr) {
        nodo = nodo->izquierdo;
    }
    return nodo;
}

void AbbBicicletas::listarConKilometrosMayoresA(int limite) const {
    listarMayoresRec(raiz, limite);
}

void AbbBicicletas::listarMayoresRec(NodoAbb* nodo, int limite) const {
    if (nodo == nullptr) {
        return;
    }
    listarMayoresRec(nodo->izquierdo, limite);
    if (nodo->dato->getKilometros() > limite) {
        std::cout << "Bici #" << nodo->dato->getBiciId()
                  << " | Km: " << nodo->dato->getKilometros()
                  << " | Estacion: " << nodo->dato->getCodigoEstacion()
                  << "\n";
    }
    listarMayoresRec(nodo->derecho, limite);
}

void AbbBicicletas::vaciar() {
    liberarPostorder(raiz);
    raiz = nullptr;
}

void AbbBicicletas::liberarPostorder(NodoAbb* nodo) {
    if (nodo == nullptr) {
        return;
    }
    liberarPostorder(nodo->izquierdo);
    liberarPostorder(nodo->derecho);
    delete nodo->dato;
    delete nodo;
}
