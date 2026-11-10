@echo off
setlocal
g++ -std=c++17 -Wall -Wextra -o pruebas.exe ^
    Bicicleta.cpp ColaEspera.cpp Estacion.cpp Viaje.cpp TablaHash.cpp ^
    ListaViajes.cpp MinHeap.cpp AbbBicicletas.cpp GrafoCiclovias.cpp ^
    Backtracking.cpp Red.cpp RedEstaciones.cpp RedViajes.cpp ^
    RedBicicletas.cpp RedRedistribucion.cpp pruebas.cpp ^
    pruebasEstructuras.cpp pruebasArboles.cpp pruebasRedComun.cpp ^
    pruebasRed.cpp pruebasRedBicicletas.cpp
if errorlevel 1 (
    echo Error de compilacion de las pruebas.
    exit /b 1
)
pruebas.exe
if errorlevel 1 exit /b 1
endlocal
