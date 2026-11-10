#ifndef ABB_BICICLETAS_H
#define ABB_BICICLETAS_H

#include "Bicicleta.h"

class AbbBicicletas {
public:
    AbbBicicletas();
    ~AbbBicicletas();

    bool insertar(Bicicleta* bicicleta);
    Bicicleta* buscar(int id) const;
    void listarInorder() const;
    int contarAveriadas() const;
    int getCantidad() const;
    int altura() const;
    bool eliminar(int id);
    void listarConKilometrosMayoresA(int limite) const;
    void vaciar();

private:
    struct NodoAbb {
        Bicicleta* dato;
        NodoAbb* izquierdo;
        NodoAbb* derecho;
        NodoAbb(Bicicleta* valor)
            : dato(valor), izquierdo(nullptr), derecho(nullptr) {}
    };

    NodoAbb* raiz;

    bool insertarRec(NodoAbb*& nodo, Bicicleta* bicicleta);
    NodoAbb* eliminarRec(NodoAbb* nodo, int id, bool& eliminado);
    NodoAbb* quitarMinimo(NodoAbb* nodo);
    NodoAbb* minimo(NodoAbb* nodo) const;
    void listarInorderRec(NodoAbb* nodo) const;
    void listarMayoresRec(NodoAbb* nodo, int limite) const;
    int contarAveriadasRec(NodoAbb* nodo) const;
    int contarNodosRec(NodoAbb* nodo) const;
    int alturaRec(NodoAbb* nodo) const;
    void liberarPostorder(NodoAbb* nodo);

    AbbBicicletas(const AbbBicicletas&) = delete;
    AbbBicicletas& operator=(const AbbBicicletas&) = delete;
};

#endif
