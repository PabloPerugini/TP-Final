#ifndef RED_H
#define RED_H

#include <string>
#include "TablaHash.h"
#include "AbbBicicletas.h"
#include "ListaViajes.h"
#include "GrafoCiclovias.h"
#include "MinHeap.h"
#include "Cola.h"

class Red {
public:
    Red();
    ~Red();

    // Carga de archivos
    void cargarEstaciones(const std::string& ruta);
    void cargarBicicletas(const std::string& ruta);
    void cargarViajes(const std::string& ruta);
    void cargarCiclovias(const std::string& ruta);

    // Sección A
    void mostrarInformacionEstacion(const std::string& codigo) const;
    bool agregarEstacion(const std::string& codigo, const std::string& nombre,
                         const std::string& barrio, int capacidad, int disponibles);
    bool eliminarEstacion(const std::string& codigo);
    void listarEstacionesOrdenadas(bool porAnclajes);
    void listarEstacionesBajaOcupacion() const;
    void calcularCaminoMasCortoBFS(const std::string& origen, const std::string& destino) const;

    // Sección B
    int contarViajesPorRangoFechas(const std::string& estacion, int fechaInicio, int fechaFin) const;
    void buscarViajesUsuario(const std::string& dni) const;
    void listarBicicletasParaRevision(int kmLimite) const;
    void encolarUsuarioEnEstacion(const std::string& codigoEstacion, const std::string& dni);
    void devolverBicicletaEnEstacion(const std::string& codigoEstacion);
    void ingresarBiciTaller(int idBici, int prioridad);
    void extraerBiciMasUrgenteTaller();

    // Sección C
    void buscarBicicletaPorId(int id) const;
    void listarBicicletasInorder() const;
    int contarBicicletasAveriadas() const;
    int obtenerAlturaABB() const;
    void eliminarBicicletaABB(int id);

    // Sección D
    void calcularRedistribucionBacktracking(int capacidadCamion) const;

private:
    TablaHash* estaciones;
    AbbBicicletas* bicicletas;
    ListaViajes* viajes;
    GrafoCiclovias* ciclovias;
    MinHeap* taller;

    // Colas de espera mapeadas a las estaciones
    struct NodoColaEstacion {
        std::string codigoEstacion;
        Cola* cola;
        NodoColaEstacion* siguiente;
        NodoColaEstacion(const std::string& cod) : codigoEstacion(cod), cola(new Cola()), siguiente(nullptr) {}
        ~NodoColaEstacion() { delete cola; }
    };
    NodoColaEstacion* colasEspera;

    Cola* obtenerOMostarColaEstacion(const std::string& codigo);
};

#endif