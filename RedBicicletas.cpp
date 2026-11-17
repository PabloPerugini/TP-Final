#include "Red.h"
#include "TablaHash.h"
#include "AbbBicicletas.h"
#include <iostream>

bool Red::insertarBicicletaManual(int biciId,
                                  const std::string &codigoEstacion,
                                  TipoBici tipo, EstadoBici estado,
                                  int kilometros)
{
    if (biciId <= 0 || kilometros < 0 || codigoEstacion.empty())
    {
        std::cout << "Datos invalidos para la bicicleta.\n";
        return false;
    }
    if (estaciones->obtener(codigoEstacion) == nullptr)
    {
        std::cout << "No existe la estacion " << codigoEstacion << ".\n";
        return false;
    }
    if (bicicletas->buscar(biciId) != nullptr)
    {
        std::cout << "Ya existe una bicicleta con el ID " << biciId << ".\n";
        return false;
    }

    Bicicleta *nueva = new Bicicleta(biciId, codigoEstacion, tipo, estado,
                                     kilometros);
    if (!bicicletas->insertar(nueva))
    {
        delete nueva;
        return false;
    }
    return true;
}

bool Red::buscarBicicletaPorId(int biciId) const
{
    Bicicleta *bicicleta = bicicletas->buscar(biciId);
    if (bicicleta == nullptr)
    {
        std::cout << "No existe la bicicleta " << biciId << ".\n";
        return false;
    }

    std::cout << "=== BICICLETA " << biciId << " ===\n";
    std::cout << "Estacion: " << bicicleta->getCodigoEstacion() << "\n";
    std::cout << "Tipo: "
              << (bicicleta->getTipo() == TipoBici::Electrica ? "Electrica"
                                                              : "Mecanica")
              << "\n";
    std::cout << "Estado: "
              << (bicicleta->getEstado() == EstadoBici::Averiada ? "Averiada"
                                                                 : "OK")
              << "\n";
    std::cout << "Kilometros: " << bicicleta->getKilometros() << "\n";
    return true;
}

void Red::listarBicicletasInorder() const
{
    if (bicicletas->getCantidad() == 0)
    {
        std::cout << "No hay bicicletas cargadas.\n";
        return;
    }
    std::cout << "=== BICICLETAS ORDENADAS POR ID ===\n";
    bicicletas->listarInorder();
}

int Red::contarBicicletasAveriadas() const
{
    return bicicletas->contarAveriadas();
}

int Red::obtenerAlturaABB() const
{
    return bicicletas->altura();
}

bool Red::eliminarBicicletaABB(int biciId)
{
    return bicicletas->eliminar(biciId);
}
