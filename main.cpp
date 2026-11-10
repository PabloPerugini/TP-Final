#include <iostream>
#include <string>
#include <limits>
#include "Red.h"
#include "Estacion.h"
#include "Bicicleta.h"
#include "Viaje.h"

void mostrarMenu() {
    std::cout << "\n===================================================\n";
    std::cout << "      SISTEMA DE BICICLETAS PUBLICAS (CABA)        \n";
    std::cout << "===================================================\n";
    std::cout << "--- CARGA DE DATOS ---\n";
    std::cout << " 1. Cargar datos desde archivos (.txt)\n\n";

    std::cout << "--- SECCION A: GESTION DE ESTACIONES ---\n";
    std::cout << " 2. Buscar estacion por codigo\n";
    std::cout << " 3. Agregar nueva estacion\n";
    std::cout << " 4. Eliminar estacion\n";
    std::cout << " 5. Listar estaciones ordenadas (Anclajes / Disponibles)\n";
    std::cout << " 6. Listar estaciones con menos del 20% de ocupacion\n";
    std::cout << " 7. Calcular recorrido mas corto por ciclovias (BFS)\n\n";

    std::cout << "--- SECCION B: VIAJES Y COLAS DE ESPERA ---\n";
    std::cout << " 8. Contar viajes desde una estacion en rango de fechas\n";
    std::cout << " 9. Buscar viajes de un usuario (DNI) y min totales\n";
    std::cout << "10. Listar bicicletas con mas de X kilometros\n";
    std::cout << "11. Encolar usuario en cola de espera de estacion\n";
    std::cout << "12. Devolver bici en estacion (desencolar usuario)\n";
    std::cout << "13. Taller (Min-Heap): Insertar bici averiada / Extraer mas urgente\n\n";

    std::cout << "--- SECCION C: REGISTRO DE BICICLETAS (ABB) ---\n";
    std::cout << "14. Buscar bicicleta por ID\n";
    std::cout << "15. Listar todas las bicicletas (Inorder)\n";
    std::cout << "16. Contar bicicletas averiadas\n";
    std::cout << "17. Consultar altura del arbol ABB\n";
    std::cout << "18. Eliminar bicicleta del ABB (Opcional)\n\n";

    std::cout << "--- SECCION D: REDISTRIBUCION ---\n";
    std::cout << "19. Calcular redistribucion de bicicletas (Backtracking)\n\n";

    std::cout << " 0. Salir del programa\n";
    std::cout << "===================================================\n";
    std::cout << "Ingrese una opcion: ";
}

void pausar() {
    std::cout << "\nPresione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    // La clase Red gestiona toda la memoria dinamica de forma centralizada (RAII)
    Red redSistema;
    bool datosCargados = false;
    int opcion = -1;

    while (opcion != 0) {
        mostrarMenu();
        if (!(std::cin >> opcion)) {
            std::cout << "\n[!] Entrada invalida. Por favor ingrese un numero.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pausar();
            continue;
        }

        // Guardia: las opciones 2-19 necesitan los datos ya cargados
        // (tambien usa datosCargados, evitando el warning de variable sin leer).
        if (opcion != 0 && opcion != 1 && !datosCargados) {
            std::cout << "\n[!] Primero carga los datos con la opcion 1.\n";
            pausar();
            continue;
        }

        switch (opcion) {
            case 1: {
                std::cout << "\n---> Carga de Archivos\n";
                redSistema.cargarEstaciones("estaciones.txt");
                redSistema.cargarBicicletas("bicicletas.txt");
                redSistema.cargarViajes("viajes.txt");
                redSistema.cargarCiclovias("ciclovias.txt");
                datosCargados = true;
                std::cout << "[+] Archivos procesados e ingresados a las estructuras.\n";
                break;
            }
            case 2: {
                std::string codigo;
                std::cout << "\nIngrese codigo de estacion (ej. PLZ): ";
                std::cin >> codigo;
                redSistema.mostrarInformacionEstacion(codigo);
                break;
            }
            case 3: {
                std::string cod, nom, barrio;
                int cap, disp;
                std::cout << "\n--- Alta de Estacion ---\n";
                std::cout << "Codigo: "; std::cin >> cod;
                std::cout << "Nombre: "; std::cin >> nom;
                std::cout << "Barrio: "; std::cin >> barrio;
                std::cout << "Capacidad Anclajes: "; std::cin >> cap;
                std::cout << "Bicis Disponibles: "; std::cin >> disp;

                if (redSistema.agregarEstacion(cod, nom, barrio, cap, disp)) {
                    std::cout << "[+] Estacion agregada con exito.\n";
                } else {
                    std::cout << "[!] Error: Ya existe una estacion con ese codigo.\n";
                }
                break;
            }
            case 4: {
                std::string codigo;
                std::cout << "\nIngrese codigo de estacion a eliminar: ";
                std::cin >> codigo;
                if (redSistema.eliminarEstacion(codigo)) {
                    std::cout << "[+] Estacion eliminada correctamente.\n";
                } else {
                    std::cout << "[!] No se encontro la estacion especificada.\n";
                }
                break;
            }
            case 5: {
                int criterio;
                std::cout << "\nOrdenar por:\n 1. Capacidad de Anclajes\n 2. Bicis Disponibles\nSeleccion: ";
                std::cin >> criterio;
                redSistema.listarEstacionesOrdenadas(criterio == 1);
                break;
            }
            case 6: {
                std::cout << "\n--- Estaciones con < 20% de Ocupacion ---\n";
                redSistema.listarEstacionesBajaOcupacion();
                break;
            }
            case 7: {
                std::string orig, dest;
                std::cout << "\nCodigo Estacion Origen: "; std::cin >> orig;
                std::cout << "Codigo Estacion Destino: "; std::cin >> dest;
                redSistema.calcularCaminoMasCortoBFS(orig, dest);
                break;
            }
            case 8: {
                std::string cod;
                int fInicio, fFin;
                std::cout << "\nCodigo de Estacion Origen: "; std::cin >> cod;
                std::cout << "Fecha Desde (AAAAMMDD): "; std::cin >> fInicio;
                std::cout << "Fecha Hasta (AAAAMMDD): "; std::cin >> fFin;
                int total = redSistema.contarViajesPorRangoFechas(cod, fInicio, fFin);
                std::cout << "-> Cantidad de viajes registrados: " << total << std::endl;
                break;
            }
            case 9: {
                std::string dni;
                std::cout << "\nIngrese DNI del usuario: "; std::cin >> dni;
                redSistema.buscarViajesUsuario(dni);
                break;
            }
            case 10: {
                int kmLimite;
                std::cout << "\nIngrese limite de kilometros (X): "; std::cin >> kmLimite;
                redSistema.listarBicicletasParaRevision(kmLimite);
                break;
            }
            case 11: {
                std::string codEst, dni;
                std::cout << "\nCodigo de Estacion: "; std::cin >> codEst;
                std::cout << "DNI del Usuario: "; std::cin >> dni;
                redSistema.encolarUsuarioEnEstacion(codEst, dni);
                break;
            }
            case 12: {
                std::string codEst;
                std::cout << "\nCodigo de Estacion donde se devuelve bici: "; std::cin >> codEst;
                redSistema.devolverBicicletaEnEstacion(codEst);
                break;
            }
            case 13: {
                int subOp;
                std::cout << "\n--- TALLER DE REPARACIONES ---\n";
                std::cout << "1. Ingresar bici averiada al taller\n";
                std::cout << "2. Extraer bici mas urgente para reparar\n";
                std::cout << "Seleccion: "; std::cin >> subOp;

                if (subOp == 1) {
                    int idBici, prio;
                    std::cout << "ID Bici: "; std::cin >> idBici;
                    std::cout << "Prioridad (1-Alta, 2-Media, 3-Baja): "; std::cin >> prio;
                    redSistema.ingresarBiciTaller(idBici, prio);
                } else if (subOp == 2) {
                    redSistema.extraerBiciMasUrgenteTaller();
                }
                break;
            }
            case 14: {
                int id;
                std::cout << "\nIngrese ID de bicicleta a buscar: "; std::cin >> id;
                redSistema.buscarBicicletaPorId(id);
                break;
            }
            case 15: {
                std::cout << "\n--- Listado Completo de Bicicletas (Inorder) ---\n";
                redSistema.listarBicicletasInorder();
                break;
            }
            case 16: {
                std::cout << "\nCantidad total de bicicletas averiadas: "
                          << redSistema.contarBicicletasAveriadas() << std::endl;
                break;
            }
            case 17: {
                std::cout << "\nAltura del arbol binario de bicicletas (ABB): "
                          << redSistema.obtenerAlturaABB() << std::endl;
                break;
            }
            case 18: {
                int id;
                std::cout << "\nID de bicicleta a eliminar del ABB: "; std::cin >> id;
                redSistema.eliminarBicicletaABB(id);
                break;
            }
            case 19: {
                int capacidadCamion;
                std::cout << "\nIngrese la capacidad C del camion: "; std::cin >> capacidadCamion;
                redSistema.calcularRedistribucionBacktracking(capacidadCamion);
                break;
            }
            case 0:
                std::cout << "\n[!] Cerrando el sistema y liberando memoria dinamica...\n";
                break;
            default:
                std::cout << "\n[!] Opcion no valida. Reintente.\n";
                break;
        }

        if (opcion != 0) {
            pausar();
        }
    }

    std::cout << "[+] Memoria liberada. Programa finalizado correctamente.\n";
    return 0;
}
