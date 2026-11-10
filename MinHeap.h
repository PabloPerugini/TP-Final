#ifndef MIN_HEAP_H
#define MIN_HEAP_H

struct EntradaTaller {
    int biciId;
    int prioridad;
};

class MinHeap {
public:
    MinHeap();
    ~MinHeap();

    void insertar(int biciId, int prioridad);
    bool extraerMin(EntradaTaller& salida);
    bool contiene(int biciId) const;
    bool estaVacio() const;
    int tamanio() const;
    void vaciar();

private:
    EntradaTaller* datos;
    int cantidad;
    int capacidad;

    void redimensionar();
    void subir(int indice);
    void bajar(int indice);

    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;
};

#endif
