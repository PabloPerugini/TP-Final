#include "pruebasRedComun.h"
#include "pruebas.h"
#include "Red.h"

void cargarRedCompleta(Red& red) {
    std::string salida = capturarSalida([&red]() {
        red.cargarEstaciones("estaciones.txt");
        red.cargarBicicletas("bicicletas.txt");
        red.cargarViajes("viajes.txt");
        red.cargarCiclovias("ciclovias.txt");
    });
    CHECK(salida.find("Estaciones cargadas: 10") != std::string::npos,
          "la carga debe leer 10 estaciones");
    CHECK(salida.find("Bicicletas cargadas: 12") != std::string::npos,
          "la carga debe leer 12 bicicletas");
    CHECK(salida.find("Viajes cargados: 20") != std::string::npos,
          "la carga debe leer 20 viajes");
    CHECK(salida.find("Ciclovias cargadas: 8") != std::string::npos,
          "la carga debe leer 8 ciclovias");
}
