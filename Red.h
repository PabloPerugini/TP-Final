#ifndef RED_H
#define RED_H

#include <string>
#include "Bicicleta.h"
#include "GrafoCiclovias.h"
#include "MinHeap.h"

class TablaHash;
class ListaViajes;
class AbbBicicletas;

enum class ResultadoDevolucionRed
{
    EstacionInexistente,
    AsignadaAEspera,
    AgregadaAEstacion,
    SinLugar
};

class Red
{
public:
    Red();
    ~Red();

    void reiniciar();
    void cargarEstaciones(const std::string &ruta);
    void cargarBicicletas(const std::string &ruta);
    void cargarViajes(const std::string &ruta);
    void cargarCiclovias(const std::string &ruta);

    int getCantidadEstaciones() const;
    int getCantidadBicicletas() const;
    int getCantidadViajes() const;
    int getColisionesHash() const;
    double getFactorCargaHash() const;
    void mostrarEstadisticasHash() const;

    bool mostrarInformacionEstacion(const std::string &codigo) const;
    bool agregarEstacion(const std::string &codigo, const std::string &nombre,
                         const std::string &barrio, int capacidadAnclajes,
                         int biciDisponibles);
    bool eliminarEstacion(const std::string &codigo);
    void listarEstacionesOrdenadas(int criterio) const;
    void listarEstacionesBajaOcupacion() const;
    ResultadoCamino calcularCaminoMasCortoBFS(const std::string &origen,
                                              const std::string &destino,
                                              std::string &camino, int &tramos);

    int contarViajesPorRangoFechas(const std::string &estacionOrigen,
                                   int fechaInicio, int fechaFin) const;
    int buscarViajesUsuario(const std::string &dni) const;
    void listarBicicletasParaRevision(int limiteKilometros) const;
    bool retirarBicicletaEnEstacion(const std::string &codigo,
                                    const std::string &dni);
    ResultadoDevolucionRed devolverBicicletaEnEstacion(
        const std::string &codigo, std::string &dniAsignado);
    bool ingresarBiciTaller(int biciId, int prioridad);
    bool extraerBiciMasUrgenteTaller(EntradaTaller &salida);

    bool insertarBicicletaManual(int biciId, const std::string &codigoEstacion,
                                 TipoBici tipo, EstadoBici estado,
                                 int kilometros);
    bool buscarBicicletaPorId(int biciId) const;
    void listarBicicletasInorder() const;
    int contarBicicletasAveriadas() const;
    int obtenerAlturaABB() const;
    bool eliminarBicicletaABB(int biciId);

    int calcularRedistribucionBacktracking(int capacidadCamion) const;
    int calcularRedistribucionManual(int capacidadCamion) const;

private:
    TablaHash *estaciones;
    GrafoCiclovias *ciclovias;
    ListaViajes *viajes;
    AbbBicicletas *bicicletas;
    MinHeap *taller;

    void crearEstructuras();
    void liberarEstructuras();

    Red(const Red &) = delete;
    Red &operator=(const Red &) = delete;
};

#endif
