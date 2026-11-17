#include "Red.h"
#include "TablaHash.h"
#include "Estacion.h"
#include "ListaViajes.h"
#include "AbbBicicletas.h"
#include "Viaje.h"
#include <fstream>
#include <iostream>
#include <iomanip>

Red::Red()
{
    crearEstructuras();
}

Red::~Red()
{
    liberarEstructuras();
}

void Red::crearEstructuras()
{
    estaciones = new TablaHash();
    ciclovias = new GrafoCiclovias();
    viajes = new ListaViajes();
    bicicletas = new AbbBicicletas();
    taller = new MinHeap();
}

void Red::liberarEstructuras()
{
    delete taller;
    taller = nullptr;
    delete bicicletas;
    bicicletas = nullptr;
    delete viajes;
    viajes = nullptr;
    delete ciclovias;
    ciclovias = nullptr;
    delete estaciones;
    estaciones = nullptr;
}

void Red::reiniciar()
{
    liberarEstructuras();
    crearEstructuras();
}

void Red::cargarEstaciones(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo: " << ruta << "\n";
        return;
    }

    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capacidad = 0;
    int disponibles = 0;
    int cargadas = 0;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> disponibles)
    {
        Estacion *estacion =
            new Estacion(codigo, nombre, barrio, capacidad, disponibles);
        if (estaciones->insertar(codigo, estacion))
        {
            ciclovias->agregarEstacion(codigo);
            ++cargadas;
        }
        else
        {
            delete estacion;
            std::cout << "Estacion duplicada ignorada: " << codigo << "\n";
        }
    }
    std::cout << "Estaciones cargadas: " << cargadas << "\n";
}

void Red::cargarBicicletas(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo: " << ruta << "\n";
        return;
    }

    int biciId = 0;
    int kilometros = 0;
    std::string codigoEstacion;
    std::string tipoTexto;
    std::string estadoTexto;
    int cargadas = 0;

    while (archivo >> biciId >> codigoEstacion >> tipoTexto >> estadoTexto >>
           kilometros)
    {
        TipoBici tipo = tipoTexto == "Electrica" ? TipoBici::Electrica
                                                 : TipoBici::Mecanica;
        EstadoBici estado = estadoTexto == "Averiada" ? EstadoBici::Averiada
                                                      : EstadoBici::OK;
        Bicicleta *bicicleta =
            new Bicicleta(biciId, codigoEstacion, tipo, estado, kilometros);
        if (bicicletas->insertar(bicicleta))
        {
            ++cargadas;
        }
        else
        {
            delete bicicleta;
            std::cout << "Bicicleta duplicada ignorada: " << biciId << "\n";
        }
    }
    std::cout << "Bicicletas cargadas: " << cargadas << "\n";
}

void Red::cargarViajes(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo: " << ruta << "\n";
        return;
    }

    int viajeId = 0;
    int biciId = 0;
    int fecha = 0;
    int duracion = 0;
    std::string dni;
    std::string origen;
    std::string destino;
    int cargados = 0;

    while (archivo >> viajeId >> biciId >> dni >> origen >> destino >> fecha >>
           duracion)
    {
        viajes->insertar(new Viaje(viajeId, biciId, dni, origen, destino,
                                   fecha, duracion));
        ++cargados;
    }
    std::cout << "Viajes cargados: " << cargados << "\n";
}

void Red::cargarCiclovias(const std::string &ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo: " << ruta << "\n";
        return;
    }

    std::string origen;
    std::string destino;
    int cargadas = 0;
    while (archivo >> origen >> destino)
    {
        ciclovias->agregarCiclovia(origen, destino);
        ++cargadas;
    }
    std::cout << "Ciclovias cargadas: " << cargadas << "\n";
}

int Red::getCantidadEstaciones() const
{
    return estaciones->getCantidad();
}

int Red::getCantidadBicicletas() const
{
    return bicicletas->getCantidad();
}

int Red::getCantidadViajes() const
{
    return viajes->getCantidad();
}

int Red::getColisionesHash() const
{
    return estaciones->getColisiones();
}

double Red::getFactorCargaHash() const
{
    return estaciones->factorDeCarga();
}

void Red::mostrarEstadisticasHash() const
{
    std::cout << "=== ESTADISTICAS DE LA TABLA HASH ===\n";
    std::cout << "Estaciones cargadas: " << getCantidadEstaciones() << "\n";
    std::cout << "Bicicletas registradas: " << getCantidadBicicletas() << "\n";
    std::cout << "Viajes registrados: " << getCantidadViajes() << "\n";
    std::cout << "Capacidad de la tabla: " << estaciones->getCapacidad() << "\n";
    std::cout << "Colisiones registradas: " << getColisionesHash() << "\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Factor de carga final: " << getFactorCargaHash() << " ("
              << getCantidadEstaciones() << "/" << estaciones->getCapacidad()
              << ")\n";
    std::cout.unsetf(std::ios::floatfield);
    std::cout << std::setprecision(6);
    std::cout << "Contenido de la tabla (por bucket):\n";
    estaciones->listarTodas();
}
