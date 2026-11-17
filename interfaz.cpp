#include <iostream>
#include <string>
#include "interfaz.h"
#include "Red.h"
#include "Bicicleta.h"

namespace interfaz
{
    void mostrarMenu()
    {
        std::cout << "\n===================================================\n";
        std::cout << "      SISTEMA DE BICICLETAS PUBLICAS (CABA)\n";
        std::cout << "===================================================\n";
        std::cout << "--- CARGA DE DATOS ---\n";
        std::cout << " 1. Cargar o recargar los archivos .txt\n\n";
        std::cout << "--- SECCION A: ESTACIONES ---\n";
        std::cout << " 2. Mostrar informacion de una estacion\n";
        std::cout << " 3. Agregar una estacion\n";
        std::cout << " 4. Eliminar una estacion\n";
        std::cout << " 5. Listar estaciones ordenadas\n";
        std::cout << " 6. Listar estaciones con menos del 20% de ocupacion\n";
        std::cout << " 7. Calcular camino con menos tramos (BFS)\n\n";
        std::cout << "--- SECCION B: VIAJES Y COLAS ---\n";
        std::cout << " 8. Contar viajes por estacion y rango de fechas\n";
        std::cout << " 9. Buscar viajes por DNI y total de minutos\n";
        std::cout << "10. Listar bicicletas con mas de X kilometros\n";
        std::cout << "11. Retirar bicicleta (o encolar si no hay)\n";
        std::cout << "12. Devolver bicicleta en una estacion\n";
        std::cout << "13. Taller de reparaciones (min-heap)\n\n";
        std::cout << "--- SECCION C: ABB DE BICICLETAS ---\n";
        std::cout << "14. Insertar bicicleta manualmente en el ABB\n";
        std::cout << "15. Buscar bicicleta por ID\n";
        std::cout << "16. Listar bicicletas ordenadas por ID (inorder)\n";
        std::cout << "17. Contar bicicletas averiadas\n";
        std::cout << "18. Calcular la altura del ABB\n";
        std::cout << "19. Eliminar bicicleta del ABB (opcional)\n\n";
        std::cout << "--- SECCION D: REDISTRIBUCION ---\n";
        std::cout << "20. Redistribucion con backtracking\n\n";
        std::cout << "--- METRICAS ---\n";
        std::cout << "21. Mostrar estadisticas de la tabla hash\n\n";
        std::cout << " 0. Salir\n";
        std::cout << "===================================================\n";
    }

    bool convertirEntero(const std::string &texto, int &valor)
    {
        try
        {
            std::size_t posicion = 0;
            int convertido = std::stoi(texto, &posicion);
            if (posicion != texto.size())
            {
                return false;
            }
            valor = convertido;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    bool leerEntero(const std::string &mensaje, int &valor)
    {
        std::string linea;
        while (true)
        {
            std::cout << mensaje;
            if (!std::getline(std::cin, linea))
            {
                return false;
            }
            if (convertirEntero(linea, valor))
            {
                return true;
            }
            std::cout << "Entrada invalida: ingrese un numero entero.\n";
        }
    }

    bool leerTexto(const std::string &mensaje, std::string &valor)
    {
        std::cout << mensaje;
        return static_cast<bool>(std::getline(std::cin, valor));
    }

    void pausar()
    {
        std::string linea;
        std::cout << "\nPresione ENTER para continuar...";
        std::getline(std::cin, linea);
    }

    void mostrarCamino(Red &red)
    {
        std::string origen;
        std::string destino;
        if (!leerTexto("Codigo de origen: ", origen) ||
            !leerTexto("Codigo de destino: ", destino))
        {
            return;
        }

        std::string camino;
        int tramos = 0;
        ResultadoCamino resultado =
            red.calcularCaminoMasCortoBFS(origen, destino, camino, tramos);
        if (resultado == ResultadoCamino::Encontrado)
        {
            std::cout << "Camino: " << camino << "\n";
            std::cout << "Cantidad de tramos: " << tramos << "\n";
        }
        else if (resultado == ResultadoCamino::SinCamino)
        {
            std::cout << "No existe un camino por ciclovias entre las estaciones.\n";
        }
        else
        {
            std::cout << "Una de las estaciones no existe.\n";
        }
    }

    void ejecutarTaller(Red &red)
    {
        std::cout << "\n--- TALLER DE REPARACIONES ---\n";
        std::cout << "1. Insertar bicicleta\n";
        std::cout << "2. Extraer la bicicleta mas urgente\n";
        int opcion = 0;
        if (!leerEntero("Seleccion: ", opcion))
        {
            return;
        }

        if (opcion == 1)
        {
            int biciId = 0;
            int prioridad = 0;
            if (!leerEntero("ID de bicicleta: ", biciId) ||
                !leerEntero("Prioridad (1 a 3): ", prioridad))
            {
                return;
            }
            red.ingresarBiciTaller(biciId, prioridad);
        }
        else if (opcion == 2)
        {
            EntradaTaller entrada;
            if (red.extraerBiciMasUrgenteTaller(entrada))
            {
                std::cout << "Bicicleta reparada: " << entrada.biciId
                          << " (prioridad " << entrada.prioridad << ")\n";
            }
            else
            {
                std::cout << "El taller no tiene bicicletas pendientes.\n";
            }
        }
        else
        {
            std::cout << "Opcion de taller invalida.\n";
        }
    }

    void ejecutarRedistribucion(Red &red)
    {
        std::cout << "\n--- REDISTRIBUCION CON BACKTRACKING ---\n";
        std::cout << "1. Usar los faltantes del sistema\n";
        std::cout << "2. Ingresar una lista manual\n";
        int modo = 0;
        int capacidad = 0;
        if (!leerEntero("Seleccion: ", modo) ||
            !leerEntero("Capacidad C del camion: ", capacidad))
        {
            return;
        }

        if (modo == 1)
        {
            red.calcularRedistribucionBacktracking(capacidad);
        }
        else if (modo == 2)
        {
            red.calcularRedistribucionManual(capacidad);
        }
        else
        {
            std::cout << "Modo de redistribucion invalido.\n";
        }
    }

    void insertarBicicletaManual(Red &red)
    {
        int biciId = 0;
        int kilometros = 0;
        std::string codigoEstacion;
        std::string tipoTexto;
        std::string estadoTexto;
        if (!leerEntero("ID de bicicleta: ", biciId) ||
            !leerTexto("Codigo de estacion: ", codigoEstacion) ||
            !leerTexto("Tipo (Mecanica/Electrica): ", tipoTexto) ||
            !leerTexto("Estado (OK/Averiada): ", estadoTexto) ||
            !leerEntero("Kilometros: ", kilometros))
        {
            return;
        }

        TipoBici tipo;
        if (tipoTexto == "Mecanica")
        {
            tipo = TipoBici::Mecanica;
        }
        else if (tipoTexto == "Electrica")
        {
            tipo = TipoBici::Electrica;
        }
        else
        {
            std::cout << "Tipo invalido.\n";
            return;
        }

        EstadoBici estado;
        if (estadoTexto == "OK")
        {
            estado = EstadoBici::OK;
        }
        else if (estadoTexto == "Averiada")
        {
            estado = EstadoBici::Averiada;
        }
        else
        {
            std::cout << "Estado invalido.\n";
            return;
        }

        if (red.insertarBicicletaManual(biciId, codigoEstacion, tipo, estado,
                                        kilometros))
        {
            std::cout << "Bicicleta insertada en el ABB.\n";
        }
    }
} // namespace interfaz
