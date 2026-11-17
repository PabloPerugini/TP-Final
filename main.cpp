#include <iostream>
#include <string>
#include "Red.h"
#include "Bicicleta.h"
#include "interfaz.h"

using namespace interfaz;

int main()
{
    Red red;
    bool datosCargados = false;

    while (true)
    {
        mostrarMenu();
        int opcion = 0;
        if (!leerEntero("Ingrese una opcion: ", opcion))
        {
            std::cout << "\nEntrada finalizada.\n";
            break;
        }
        if (opcion != 0 && opcion != 1 && !datosCargados)
        {
            std::cout << "Primero cargue los datos con la opcion 1.\n";
        }
        else
        {
            switch (opcion)
            {
            case 1:
                red.reiniciar();
                red.cargarEstaciones("estaciones.txt");
                red.cargarBicicletas("bicicletas.txt");
                red.cargarViajes("viajes.txt");
                red.cargarCiclovias("ciclovias.txt");
                datosCargados = true;
                break;
            case 2:
            {
                std::string codigo;
                if (leerTexto("Codigo de estacion: ", codigo))
                {
                    red.mostrarInformacionEstacion(codigo);
                }
                break;
            }
            case 3:
            {
                std::string codigo;
                std::string nombre;
                std::string barrio;
                int capacidad = 0;
                int disponibles = 0;
                if (leerTexto("Codigo: ", codigo) &&
                    leerTexto("Nombre: ", nombre) &&
                    leerTexto("Barrio: ", barrio) &&
                    leerEntero("Capacidad de anclajes: ", capacidad) &&
                    leerEntero("Bicicletas disponibles: ", disponibles))
                {
                    if (red.agregarEstacion(codigo, nombre, barrio,
                                            capacidad, disponibles))
                    {
                        std::cout << "Estacion agregada.\n";
                    }
                }
                break;
            }
            case 4:
            {
                std::string codigo;
                if (leerTexto("Codigo a eliminar: ", codigo) &&
                    red.eliminarEstacion(codigo))
                {
                    std::cout << "Estacion eliminada.\n";
                }
                break;
            }
            case 5:
            {
                int criterio = 0;
                if (leerEntero("1. Anclajes / 2. Disponibles: ", criterio))
                {
                    if (criterio == 1 || criterio == 2)
                    {
                        red.listarEstacionesOrdenadas(criterio);
                    }
                    else
                    {
                        std::cout << "Criterio invalido.\n";
                    }
                }
                break;
            }
            case 6:
                red.listarEstacionesBajaOcupacion();
                break;
            case 7:
                mostrarCamino(red);
                break;
            case 8:
            {
                std::string codigo;
                int desde = 0;
                int hasta = 0;
                if (leerTexto("Estacion de origen: ", codigo) &&
                    leerEntero("Fecha desde (AAAAMMDD): ", desde) &&
                    leerEntero("Fecha hasta (AAAAMMDD): ", hasta))
                {
                    std::cout << "Viajes encontrados: "
                              << red.contarViajesPorRangoFechas(
                                     codigo, desde, hasta)
                              << "\n";
                }
                break;
            }
            case 9:
            {
                std::string dni;
                if (leerTexto("DNI del usuario: ", dni))
                {
                    red.buscarViajesUsuario(dni);
                }
                break;
            }
            case 10:
            {
                int limite = 0;
                if (leerEntero("Limite de kilometros: ", limite))
                {
                    red.listarBicicletasParaRevision(limite);
                }
                break;
            }
            case 11:
            {
                std::string codigo;
                std::string dni;
                if (leerTexto("Estacion: ", codigo) &&
                    leerTexto("DNI del usuario: ", dni))
                {
                    red.retirarBicicletaEnEstacion(codigo, dni);
                }
                break;
            }
            case 12:
            {
                std::string codigo;
                if (leerTexto("Estacion donde se devuelve: ", codigo))
                {
                    std::string dni;
                    ResultadoDevolucionRed resultado =
                        red.devolverBicicletaEnEstacion(codigo, dni);
                    if (resultado ==
                        ResultadoDevolucionRed::AsignadaAEspera)
                    {
                        std::cout << "Bicicleta asignada al DNI " << dni
                                  << ".\n";
                    }
                    else if (resultado ==
                             ResultadoDevolucionRed::AgregadaAEstacion)
                    {
                        std::cout << "Bicicleta agregada a la estacion.\n";
                    }
                    else if (resultado ==
                             ResultadoDevolucionRed::SinLugar)
                    {
                        std::cout << "La estacion esta llena.\n";
                    }
                    else
                    {
                        std::cout << "No existe la estacion.\n";
                    }
                }
                break;
            }
            case 13:
                ejecutarTaller(red);
                break;
            case 14:
                insertarBicicletaManual(red);
                break;
            case 15:
            {
                int biciId = 0;
                if (leerEntero("ID de bicicleta: ", biciId))
                {
                    red.buscarBicicletaPorId(biciId);
                }
                break;
            }
            case 16:
                red.listarBicicletasInorder();
                break;
            case 17:
                std::cout << "Bicicletas averiadas: "
                          << red.contarBicicletasAveriadas() << "\n";
                break;
            case 18:
                std::cout << "Altura del ABB: " << red.obtenerAlturaABB()
                          << "\n";
                break;
            case 19:
            {
                int biciId = 0;
                if (leerEntero("ID a eliminar: ", biciId))
                {
                    if (red.eliminarBicicletaABB(biciId))
                    {
                        std::cout << "Bicicleta eliminada del ABB.\n";
                    }
                    else
                    {
                        std::cout << "No existe esa bicicleta.\n";
                    }
                }
                break;
            }
            case 20:
                ejecutarRedistribucion(red);
                break;
            case 21:
                red.mostrarEstadisticasHash();
                break;
            case 0:
                std::cout << "Cerrando el sistema...\n";
                break;
            default:
                std::cout << "Opcion invalida.\n";
                break;
            }
        }

        if (opcion == 0)
        {
            break;
        }
        pausar();
    }

    std::cout << "Memoria liberada. Programa finalizado.\n";
    return 0;
}
