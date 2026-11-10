@echo off
setlocal
g++ -std=c++17 -Wall -Wextra -o tp.exe ^
    Bicicleta.cpp ColaEspera.cpp Estacion.cpp Viaje.cpp TablaHash.cpp ^
    ListaViajes.cpp MinHeap.cpp AbbBicicletas.cpp GrafoCiclovias.cpp ^
    Backtracking.cpp Red.cpp RedEstaciones.cpp RedViajes.cpp ^
    RedBicicletas.cpp RedRedistribucion.cpp interfaz.cpp main.cpp
if errorlevel 1 (
    echo Error de compilacion.
    exit /b 1
)
echo Compilacion exitosa: tp.exe
endlocal
