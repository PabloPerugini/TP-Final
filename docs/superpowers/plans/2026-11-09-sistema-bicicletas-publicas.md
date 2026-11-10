# Sistema de Bicicletas Públicas — Plan de Implementación

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Completar el TP de Algoritmos con todas las consignas del PDF, menú funcional, pruebas, README y UML.

**Architecture:** `Red` es la raíz de composición y dueña de la tabla de hash, el grafo, la lista de viajes, el ABB y el min-heap. Cada estructura vive en su propia clase con memoria dinámica y destructor propio. `Estacion` compone su `ColaEspera`.

**Tech Stack:** C++17, `g++`, `std::string` y STL solo para archivos/auxiliares. Sin `vector`, `map`, `unordered_map`, `queue`, `priority_queue` ni `set`.

**Spec:** `docs/superpowers/specs/2026-11-09-sistema-bicicletas-publicas-design.md`

**PDF:** `TP2C2026.pdf` — fuente de verdad. Si hay conflicto, gana el PDF.

## Global Constraints

- C++17; compilar con `g++ -std=c++17 -Wall -Wextra` sin warnings.
- Sin contenedores STL de los prohibidos; `std::string` permitido.
- Variables y métodos en camelCase; clases en PascalCase.
- Sin fugas de memoria: RAII, destructores y liberación postorder del ABB.
- Archivos de código de menos de 300 líneas; `Red` se reparte por sección y las pruebas se reparten en archivos.
- `biciDisponibles` es independiente del ABB (ver diseño 6.5).
- Menú final de 21 opciones + salir; cada consigna debe poder probarse.
- Mensajes de consola sin acentos; README en UTF-8.
- Tabla de hash fija de 13 buckets; reportar 3 colisiones y factor 10/13 ≈ 0.7692.
- Fechas `AAAAMMDD` comparadas como enteros; rangos inclusivos.
- Altura del ABB por niveles: vacío 0, un nodo 1.

## Review Focus

- Archivos faltantes o vacíos: el programa avisa y no rompe; las estructuras vacías responden con mensajes.
- Cola/disponibilidad: retirar con 0 disponibles encola; devolver asigna al primero; estación llena informa; nunca quedar negativo.
- Eliminar estación: libera su cola, borra sus aristas y no toca las bicicletas del ABB.
- ABB: eliminar hoja, un hijo y dos hijos; inorder, altura y destructor postorder correctos.
- Backtracking: C=0, sin solución, faltantes ≤ 0, listas vacías y ejemplo de 4 soluciones.
- BFS: origen=destino, sin camino, estación inexistente y estaciones aisladas.

---

### Task 0: Repositorio y limpieza inicial

**Files:**
- Create: `.gitignore`
- Delete: `MinHeap.h`, `AbbBicicletas.h`, `ArrayDinamico.h`, `backtracking.cpp` (vacíos); se reemplazan `GrafoCiclovias.h/.cpp`, `ListaViajes.h`, `Red.h/.cpp`
- Keep: `Estacion.*`, `Bicicleta.*`, `Viaje.*`, `TablaHash.*`, `.txt`, PDF y `docs/`

**Interfaces:**
- Consumes: nada.
- Produces: repositorio git con el estado inicial y sin archivos a medio escribir.

- [ ] **Step 1: Inicializar git y registrar el estado inicial**

Run:
```powershell
git init
```
Crear `.gitignore` con `*.exe`, `*.o`, `*.obj`, `*.stackdump`, `build/`, `.vscode/`, `.idea/`.
```powershell
git add .
git commit -m "chore: estado inicial del TP"
```
Si falta identidad, configurar solo en el repo con un nombre local antes de commitear.

- [ ] **Step 2: Borrar los archivos vacíos o corruptos que se van a reemplazar**

Borrar `MinHeap.h`, `AbbBicicletas.h`, `ArrayDinamico.h`, `backtracking.cpp`, `GrafoCiclovias.h`, `GrafoCiclovias.cpp`, `ListaViajes.h`, `Red.h`, `Red.cpp`. No borrar las clases de datos, la tabla de hash, los `.txt` ni el PDF.

- [ ] **Step 3: Commit de limpieza**

```powershell
git add -A
git commit -m "chore: limpiar archivos a medio escribir"
```

---

### Task 1: Cola de espera y estación

**Files:**
- Create: `ColaEspera.h`, `ColaEspera.cpp`
- Modify: `Estacion.h`, `Estacion.cpp`
- Create: `pruebas.h`, `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
class ColaEspera {
public:
    ColaEspera();
    ~ColaEspera();
    void encolar(const std::string& dni);
    bool desencolar(std::string& dni); // false si está vacía
    bool estaVacia() const;
    int tamanio() const;
    void vaciar();
private:
    struct Nodo { std::string dni; Nodo* siguiente; };
    Nodo* frente; Nodo* fin;
    // copia y asignación privadas/delete
};

enum class ResultadoDevolucion { AsignadaAEspera, AgregadaAEstacion, SinLugar };

class Estacion {
    // constructor y getters existentes
    bool retirarBicicleta(); // descuenta si hay; false si no
    ResultadoDevolucion devolverBicicleta(std::string& dniAsignado);
    void encolarUsuario(const std::string& dni);
    int getUsuariosEnEspera() const;
    // private: ColaEspera colaEspera;
};
```

**Tests:**
- `probarColaEspera`: cola vacía; encolar 301 y 302; FIFO; `desencolar` en vacía devuelve false; `vaciar`.
- `probarEstacion`: capacidad 2, disponibles 0; retirar false; encolar 301; devolver asigna 301 y deja disponibles 0; devolver sin cola incrementa a 1 y 2; tercera devolución `SinLugar`; `porcentajeOcupacion` correcto.

- [ ] **Step 1: Escribir las pruebas que fallan**

En `pruebas.h`: `CHECK(cond, msg)`, contador `fallos`, plantilla `capturarSalida` con `std::ostringstream`. En `pruebas.cpp`: `int fallos = 0;` y `main` llamando a `probarColaEspera()` y `probarEstacion()`. En `pruebasEstructuras.cpp`: implementar ambas pruebas con aserciones.

- [ ] **Step 2: Verificar que fallan**

Run: `g++ -std=c++17 -Wall -Wextra -o pruebas.exe ColaEspera.cpp Estacion.cpp pruebas.cpp pruebasEstructuras.cpp`
Expected: error por `ColaEspera.h` inexistente.

- [ ] **Step 3: Implementar `ColaEspera` y modificar `Estacion`**

Nodos enlazados con `frente`/`fin`. `Estacion` guarda `ColaEspera colaEspera` por valor. `devolverBicicleta` primero desencola; si no hay cola, incrementa si hay lugar; si está llena, `SinLugar`.

- [ ] **Step 4: Verificar que pasan**

Run: `g++ -std=c++17 -Wall -Wextra -o pruebas.exe ColaEspera.cpp Estacion.cpp pruebas.cpp pruebasEstructuras.cpp && .\pruebas.exe`
Expected: sin fallos, salida `OK`.

- [ ] **Step 5: Commit**

```powershell
git add ColaEspera.* Estacion.* pruebas.*
git commit -m "feat: cola de espera y operaciones de estacion"
```

---

### Task 2: Tabla de hash completa

**Files:**
- Modify: `TablaHash.h`, `TablaHash.cpp`
- Modify: `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
int volcarEstaciones(Estacion** destino, int capacidad) const; // llena y devuelve cantidad
void vaciar();       // libera estaciones y nodos, resetea contadores
int getCapacidad() const; // TAMANIO
```

**Tests:**
- Insertar las 10 estaciones del archivo en orden: cantidad 10, colisiones 3, factor 10.0/13.0.
- `obtener("PLZ")` devuelve la estación correcta; `obtener("XXX")` nullptr.
- Insertar duplicado devuelve false y no cambia la cantidad; el test libera el duplicado.
- `volcarEstaciones` llena 10 punteros.
- `eliminar("PLZ")` true, cantidad 9, luego false.
- `vaciar` deja cantidad 0 y colisiones 0; insertar una estación después funciona.

- [ ] **Step 1: Escribir `probarTablaHash` y agregarla a `main`**
- [ ] **Step 2: Compilar y ver fallar** (métodos nuevos inexistentes)
- [ ] **Step 3: Implementar `volcarEstaciones`, `vaciar` y `getCapacidad`**
- [ ] **Step 4: Compilar y ejecutar; esperar todo OK**
- [ ] **Step 5: Commit** `feat: operaciones de volcado y vaciado de hash`

---

### Task 3: Lista de viajes

**Files:**
- Replace: `ListaViajes.h`
- Create: `ListaViajes.cpp`
- Modify: `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
class ListaViajes {
public:
    ListaViajes(); ~ListaViajes();
    void insertar(Viaje* v);
    int contarPorRangoFechas(const std::string& estacion, int desde, int hasta) const;
    int buscarPorDni(const std::string& dni) const; // imprime y devuelve minutos
    int getCantidad() const;
    void vaciar();
private:
    struct NodoViaje { Viaje* dato; NodoViaje* siguiente; };
    NodoViaje* cabeza; int cantidad;
};
```

**Tests:** insertar 4 viajes conocidos; rango inclusivo por estación; suma por DNI; DNI inexistente devuelve 0; `vaciar` deja 0.

- [ ] **Step 1: Escribir `probarListaViajes` y agregarla a `main`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar la lista y sus recorridos**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: lista enlazada de viajes`

---

### Task 4: Min-heap del taller

**Files:**
- Create: `MinHeap.h`, `MinHeap.cpp`
- Modify: `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
struct EntradaTaller { int biciId; int prioridad; };

class MinHeap {
public:
    MinHeap(); ~MinHeap();
    void insertar(int biciId, int prioridad);
    bool extraerMin(EntradaTaller& salida);
    bool estaVacio() const;
    int tamanio() const;
    void vaciar();
private:
    EntradaTaller* datos; int cantidad; int capacidad;
    void redimensionar();
};
```

**Tests:** insertar prioridades 3,1,2,1; extraer cuatro en orden no decreciente y con todos los IDs; vacío devuelve false; insertar 20 para probar crecimiento; `vaciar`.

- [ ] **Step 1: Escribir `probarMinHeap` y agregarla a `main`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar heap binario con arreglo dinámico**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: min-heap del taller`

---

### Task 5: Árbol binario de búsqueda de bicicletas

**Files:**
- Create: `AbbBicicletas.h`, `AbbBicicletas.cpp`
- Modify: `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
class AbbBicicletas {
public:
    AbbBicicletas(); ~AbbBicicletas();
    bool insertar(Bicicleta* b);        // toma ownership si inserta
    Bicicleta* buscar(int id) const;
    void listarInorder() const;
    int contarAveriadas() const;
    int altura() const;                 // niveles
    bool eliminar(int id);
    void listarConKilometrosMayoresA(int limite) const;
    void vaciar();                      // postorder
private:
    struct NodoAbb { Bicicleta* dato; NodoAbb* izq; NodoAbb* der; };
    NodoAbb* raiz;
};
```

**Tests:** insertar 501..505 ascendentes; `buscar(503)`; duplicado false; `contarAveriadas`; `altura()==5`; inorder y filtro de km por salida capturada; probar borrado con la secuencia 501,500,502,503: dos hijos, dos hijos, un hijo, hoja; `vaciar` deja altura 0.

- [ ] **Step 1: Escribir `probarAbb` y agregarla a `main`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar inserción, búsqueda, recorridos, altura y borrado de tres casos**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: ABB de bicicletas con eliminacion opcional`

---

### Task 6: Grafo de ciclovías y BFS

**Files:**
- Create: `GrafoCiclovias.h`, `GrafoCiclovias.cpp`
- Modify: `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
enum class ResultadoCamino { Encontrado, SinCamino, EstacionInexistente };

class GrafoCiclovias {
public:
    GrafoCiclovias(); ~GrafoCiclovias();
    void agregarEstacion(const std::string& codigo);
    void eliminarEstacion(const std::string& codigo);
    void agregarCiclovia(const std::string& a, const std::string& b); // no duplica
    ResultadoCamino buscarCaminoMasCorto(const std::string& origen,
        const std::string& destino, std::string& camino, int& tramos); // no const: usa índices de trabajo
    int getCantidadVertices() const;
    void vaciar();
private:
    struct AristaGrafo; struct VerticeGrafo;
    VerticeGrafo* cabeza;
};
```

**Tests:** cargar las estaciones y aristas del archivo; PLZ→CON `Encontrado` con 4 tramos; BEL→PLZ `SinCamino`; PLZ→XXX `EstacionInexistente`; PLZ→PLZ 0 tramos; eliminar CAB y verificar que PLZ→PAL queda sin camino; `vaciar` deja 0 vértices.

- [ ] **Step 1: Escribir `probarGrafo` y agregarla a `main`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar lista de adyacencia, BFS con cola local y reconstrucción de camino**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: grafo de ciclovias y BFS`

---

### Task 7: Backtracking de redistribución

**Files:**
- Create: `Backtracking.h`, `Backtracking.cpp`
- Modify: `pruebas.cpp`, `pruebasEstructuras.cpp`

**Interfaces:**
```cpp
class Backtracking {
public:
    static int resolver(const std::string* codigos, const int* faltantes,
                        int n, int capacidad, bool imprimir);
};
```

**Tests:** ejemplo C=10 con PLZ3, CAB7, RET5, CON2, BOE5 devuelve 4; C=1 devuelve 0; C=7 con 3,7 devuelve 1; lista vacía devuelve 0; con `imprimir=true` se ven las cuatro soluciones.

- [ ] **Step 1: Escribir `probarBacktracking` y agregarla a `main`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar recursión incluir/excluir con poda `suma > C`**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: backtracking de redistribucion`

---

### Task 8: Red, ciclo de vida, cargas y estadísticas

**Files:**
- Replace: `Red.h`
- Create: `Red.cpp`
- Create: `pruebasRed.cpp`
- Modify: `pruebas.cpp` (llamar a `probarRedCarga`)

**Interfaces:**
```cpp
class Red {
public:
    Red(); ~Red();
    void reiniciar();
    void cargarEstaciones(const std::string& ruta);
    void cargarBicicletas(const std::string& ruta);
    void cargarViajes(const std::string& ruta);
    void cargarCiclovias(const std::string& ruta);
    int getCantidadEstaciones() const;
    int getCantidadBicicletas() const;
    int getCantidadViajes() const;
    int getColisionesHash() const;
    double getFactorCargaHash() const;
    void mostrarEstadisticasHash() const;
    // A-D declaradas según el diseño; en esta tarea pueden quedar stubs temporales
private:
    TablaHash* estaciones; GrafoCiclovias* ciclovias; ListaViajes* viajes;
    AbbBicicletas* bicicletas; MinHeap* taller;
    // copia y asignación delete
};
```

**Tests:** Red vacía con contadores 0; cargar los cuatro archivos; 10 estaciones, 12 bicicletas, 20 viajes, 3 colisiones, factor 0.7692; `reiniciar` vuelve a 0; cargar `no_existe.txt` imprime error y no rompe.

- [ ] **Step 1: Escribir `probarRedCarga` y agregarla a `main`**
- [ ] **Step 2: Compilar solo pruebas y ver fallar** (Red incompleta)
- [ ] **Step 3: Implementar `Red.h`, constructor, destructor, `reiniciar`, cargas, getters y stats; stubs temporales para A-D**
- [ ] **Step 4: Compilar pruebas con todas las fuentes menos `main.cpp`; esperar OK**
- [ ] **Step 5: Commit** `feat: red, cargas y estadisticas de hash`

---

### Task 9: Red — sección A

**Files:**
- Create: `RedEstaciones.cpp`
- Modify: `Red.cpp` (quitar stubs de A)
- Modify: `pruebasRed.cpp`

**Interfaces:** métodos A del diseño: `mostrarInformacionEstacion`, `agregarEstacion`, `eliminarEstacion`, `listarEstacionesOrdenadas`, `listarEstacionesBajaOcupacion`, `calcularCaminoMasCortoBFS`.

**Tests:** agregar TST y duplicado; info capturada; orden por anclajes y disponibles; baja ocupación contiene CAB, CON, BOE, ISL y no PLZ; BFS PLZ→CON 4 tramos y BEL→PLZ sin camino; eliminar CAB y verificar que el grafo ya no conecta PLZ con PAL.

- [ ] **Step 1: Escribir `probarRedEstaciones`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar A; ordenar con inserción y desempate por código**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: gestion de estaciones en Red`

---

### Task 10: Red — sección B

**Files:**
- Create: `RedViajes.cpp`
- Modify: `Red.cpp` (quitar stubs de B)
- Modify: `pruebasRed.cpp`

**Interfaces:** `contarViajesPorRangoFechas`, `buscarViajesUsuario`, `listarBicicletasParaRevision`, `retirarBicicletaEnEstacion`, `devolverBicicletaEnEstacion`, `ingresarBiciTaller`, `extraerBiciMasUrgenteTaller`.

**Tests:** PLZ 20260310–20260321 = 2; DNI 30123456 total 182; km > 3000 contiene 502, 505, 510; ISL (0 disponibles) encola 111 y 222, dos devoluciones asignan 111 y 222, tercera agrega anclaje; PLZ retira y baja a 11; taller: 502 eléctrica con prioridad 3 sale primero como prioridad 1, luego 505 prioridad 2, luego 503 prioridad 3; cola vacía informa.

- [ ] **Step 1: Escribir `probarRedViajes`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar B; el taller fuerza prioridad 1 a eléctricas**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: viajes, colas y taller en Red`

---

### Task 11: Red — sección C

**Files:**
- Create: `RedBicicletas.cpp`
- Modify: `Red.cpp` (quitar stubs de C)
- Modify: `pruebasRed.cpp`

**Interfaces:** `insertarBicicletaManual`, `buscarBicicletaPorId`, `listarBicicletasInorder`, `contarBicicletasAveriadas`, `obtenerAlturaABB`, `eliminarBicicletaABB`.

**Tests:** insertar 999 manual y buscarla; duplicado falla; estación inexistente falla; km negativo falla; inorder empieza en 501 y termina en 999; averiadas = 2; altura inicial 12 y 13 tras insertar 999 (el archivo es ascendente); eliminar 999 y buscar de nuevo.

- [ ] **Step 1: Escribir `probarRedBicicletas`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar C**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: ABB de bicicletas en Red`

---

### Task 12: Red — sección D y estadísticas

**Files:**
- Create: `RedRedistribucion.cpp`
- Modify: `Red.cpp` (quitar stubs de D/stats)
- Modify: `pruebasRed.cpp`

**Interfaces:**
```cpp
int calcularRedistribucionBacktracking(int capacidadCamion) const;
int calcularRedistribucionManual(int capacidadCamion) const; // pide datos por cin
void mostrarEstadisticasHash() const;
```

**Tests:** con los faltantes del sistema y C=10 hay una solución {PAL, RET}; `mostrarEstadisticasHash` muestra 10, 3 y 0.7692; el modo manual se prueba en la integración del menú.

- [ ] **Step 1: Escribir `probarRedRedistribucion`**
- [ ] **Step 2: Compilar y ver fallar**
- [ ] **Step 3: Implementar D; arreglos dinámicos de códigos/faltantes y llamada a `Backtracking::resolver`**
- [ ] **Step 4: Compilar y ejecutar; esperar OK**
- [ ] **Step 5: Commit** `feat: redistribucion y estadisticas en Red`

---

### Task 13: Menú, scripts de compilación y prueba de integración

**Files:**
- Replace: `main.cpp`
- Replace: `compilar.bat`
- Create: `probar.bat`
- Create: `Makefile`
- Create: `entradaPrueba.txt` (smoke test)

**Interfaces:** menú de 21 opciones + 0; `leerEntero` y `pausar`; carga con `reiniciar`; submenú de taller y de backtracking.

**Tests:**
- `compilar.bat` compila `tp.exe` sin warnings.
- `probar.bat` compila `pruebas.exe` y todas las pruebas pasan.
- `tp.exe < entradaPrueba.txt` recorre carga, stats, BFS, DNI, cola, taller, ABB manual, backtracking manual y salida; verificar substrings: `PLZ ->`, `182`, `111`, `502`, `999`, `{PLZ, CAB}`.

- [ ] **Step 1: Escribir `main.cpp` con las 21 opciones y validación de enteros**
- [ ] **Step 2: Crear `compilar.bat`, `probar.bat` y `Makefile` con todas las fuentes**
- [ ] **Step 3: Compilar y corregir warnings**
- [ ] **Step 4: Crear `entradaPrueba.txt` y ejecutar la integración**
- [ ] **Step 5: Commit** `feat: menu completo y scripts de compilacion`

---

### Task 14: README, UML y verificación final

**Files:**
- Create: `README.md`
- Create: `docs/diagrama-clases.md`
- Optional: `docs/diagrama-clases.png`
- Modify: `.gitignore` si hace falta

**Content:**
- Compilación Windows/Linux, ejecución y carga de datos.
- Descripción de cada módulo y opción del menú.
- Complejidades y justificación de cada estructura; B1–B5 con 2–3 oraciones cada uno.
- Hash: función, 3 colisiones, factor 10/13 = 0.7692.
- ABB: ejemplo 501–505, altura 5 niveles / 4 aristas; archivo real ascendente altura 12; búsqueda O(n) en el peor caso.
- Backtracking: ejemplo de 4 soluciones y O(2^n).
- UML Mermaid con composición y asociación; explicar la diferencia.
- Memoria: `valgrind --leak-check=full --show-leak-kinds=all ./tp` en Linux; en Windows no hay Valgrind/ASan, usar `cppcheck` y revisión; el diseño usa RAII.
- Decisiones: `biciDisponibles` independiente del ABB; tabla fija; ABB degenerado; menú de 21 opciones.

**Verification:**
- `compilar.bat` sin warnings.
- `probar.bat` todo OK.
- `cppcheck --enable=warning,performance,portability --inconclusive --std=c++17 <fuentes>` sin errores graves.
- Integración con `entradaPrueba.txt`.
- `git status` limpio.

- [ ] **Step 1: Escribir README y diagrama UML**
- [ ] **Step 2: Compilar y ejecutar todas las pruebas**
- [ ] **Step 3: Correr cppcheck y la integración; corregir lo que aparezca**
- [ ] **Step 4: Revisar la matriz de trazabilidad del diseño contra el PDF**
- [ ] **Step 5: Commit final** `docs: README, UML y verificacion final`
