#include "pruebas.h"

int fallos = 0;

int main() {
    probarColaEspera();
    probarEstacion();
    probarTablaHash();
    probarListaViajes();
    probarMinHeap();
    probarAbb();
    probarGrafo();
    probarBacktracking();
    probarRedCarga();
    probarRedEstaciones();
    probarRedViajes();
    probarRedBicicletas();
    probarRedRedistribucion();

    if (fallos == 0) {
        std::cout << "OK: todas las pruebas pasaron.\n";
        return 0;
    }

    std::cout << "FALLOS: " << fallos << "\n";
    return 1;
}
