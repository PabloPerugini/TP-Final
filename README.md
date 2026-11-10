# Sistema de Bicicletas Públicas — TP de Algoritmos

Trabajo Práctico de **Algoritmos (768)** implementado en **C++17** con orientación a objetos, memoria dinámica y estructuras de datos hechas a mano.

El sistema carga estaciones, bicicletas, viajes y ciclovías desde archivos de texto y permite probar todas las consignas de las secciones A, B, C y D desde un menú por consola.

## Restricciones respetadas

- No se usan `vector`, `map`, `unordered_map`, `queue`, `priority_queue` ni `set`.
- Se usa `std::string` y la STL únicamente para archivos y tareas auxiliares.
- Todas las estructuras pedidas están implementadas con nodos, punteros o arreglos dinámicos propios: tabla de hash, cola FIFO, min-heap, ABB y grafo.
- Los nombres de clases usan PascalCase y los métodos/variables camelCase.
- El proyecto compila con `g++ -std=c++17 -Wall -Wextra` sin warnings.
- La liberación de memoria se hace con RAII y destructores; el ABB se libera en postorder.

## Compilación y ejecución

### Windows

Desde la carpeta del proyecto:

```bat
compilar.bat
tp.exe
```

### Linux o macOS

```bash
g++ -std=c++17 -Wall -Wextra -o tp \
    Bicicleta.cpp ColaEspera.cpp Estacion.cpp Viaje.cpp TablaHash.cpp \
    ListaViajes.cpp MinHeap.cpp AbbBicicletas.cpp GrafoCiclovias.cpp \
    Backtracking.cpp Red.cpp RedEstaciones.cpp RedViajes.cpp \
    RedBicicletas.cpp RedRedistribucion.cpp interfaz.cpp main.cpp
./tp
```

El programa lee los archivos con rutas relativas, así que debe ejecutarse desde la carpeta que contiene `estaciones.txt`, `bicicletas.txt`, `viajes.txt` y `ciclovias.txt`.

## Archivos de entrada

Cada línea es un registro con campos separados por espacios.

| Archivo | Campos |
|---|---|
| `estaciones.txt` | `codigo nombre barrio capacidad_anclajes bicis_disponibles` |
| `bicicletas.txt` | `bici_id codigo_estacion tipo estado kilometros` |
| `viajes.txt` | `viaje_id bici_id dni_usuario estacion_origen estacion_destino fecha duracion_min` |
| `ciclovias.txt` | `estacion_a estacion_b` |

`tipo` es `Mecanica` o `Electrica`; `estado` es `OK` o `Averiada`; las fechas usan el formato `AAAAMMDD`. Las conexiones de `ciclovias.txt` son bidireccionales.

## Menú

| Opción | Funcionalidad |
|---|---|
| 1 | Cargar o recargar los cuatro archivos |
| 2 | Mostrar información de una estación |
| 3 | Agregar una estación |
| 4 | Eliminar una estación sin reubicar bicicletas |
| 5 | Listar estaciones por anclajes o por disponibles |
| 6 | Listar estaciones con menos del 20% de ocupación |
| 7 | Calcular el camino con menos tramos por ciclovías (BFS) |
| 8 | Contar viajes de una estación en un rango de fechas |
| 9 | Buscar viajes de un DNI y sumar minutos |
| 10 | Listar bicicletas con más de X kilómetros |
| 11 | Retirar bicicleta o encolar al usuario si no hay disponibles |
| 12 | Devolver bicicleta y asignarla al primero de la cola |
| 13 | Taller de reparaciones: insertar o extraer la bici más urgente |
| 14 | Insertar una bicicleta manualmente en el ABB |
| 15 | Buscar una bicicleta por ID |
| 16 | Listar todas las bicicletas en inorder |
| 17 | Contar bicicletas averiadas |
| 18 | Calcular la altura del ABB |
| 19 | Eliminar una bicicleta del ABB (opcional) |
| 20 | Redistribución con backtracking |
| 21 | Mostrar estadísticas de la tabla hash |
| 0 | Salir |

La opción 11 implementa el flujo real de la consigna: si hay bicicletas disponibles se descuenta una; si no hay, se encola el DNI. No existe una opción separada para encolar, porque el encolado es la consecuencia del retiro sin disponibilidad.

## Módulos

| Módulo | Responsabilidad |
|---|---|
| `Bicicleta` | Datos de una bicicleta y su estación por código |
| `Viaje` | Datos de un viaje; referencia bicicleta y estaciones por ID/código |
| `Estacion` | Datos, disponibilidad y cola de espera propia |
| `ColaEspera` | Cola FIFO enlazada de DNIs |
| `TablaHash` | Índice de estaciones por código, con encadenamiento |
| `GrafoCiclovias` | Lista de adyacencia no dirigida y BFS |
| `ListaViajes` | Lista enlazada simple de viajes |
| `AbbBicicletas` | Árbol binario de búsqueda ordenado por `bici_id` |
| `MinHeap` | Cola de prioridad del taller con arreglo dinámico |
| `Backtracking` | Subconjuntos de faltantes que suman exactamente C |
| `Red` | Fachada y raíz de composición de todas las estructuras |
| `interfaz` / `main` | Menú, lectura validada y llamadas a `Red` |

`Red` es dueña de la tabla de hash, el grafo, la lista de viajes, el ABB y el min-heap. Su destructor los libera y `reiniciar()` permite recargar los archivos sin duplicar datos.

## Decisiones de diseño

### Disponibilidad de estaciones y ABB

Los archivos de ejemplo no son consistentes entre sí: `PAL` declara 35 bicicletas disponibles pero solo dos bicicletas del ABB apuntan a `PAL`; `BEL` declara 44 y solo tiene la 510. El PDF no pide sincronizar esos dos archivos.

Por eso se usan dos fuentes de verdad con responsabilidades separadas:

- `biciDisponibles` de `estaciones.txt` manda para la cola de espera y para la baja ocupación.
- El ABB manda para los datos de cada bicicleta, la búsqueda por ID, el listado inorder y el filtro por kilómetros.
- Retirar o devolver una bicicleta ajusta el contador y la cola, pero no modifica una `Bicicleta` concreta del ABB.
- Insertar o eliminar una bicicleta del ABB no modifica `biciDisponibles`.

Esta decisión evita inventar datos que el PDF no pide y se puede explicar en la defensa.

### Tabla de hash fija

La tabla tiene 13 buckets y usa la función:

```text
hash(codigo) = suma(codigo[i] * (i + 1)) mod 13
```

Se mantiene fija para conservar las métricas pedidas. La resolución de colisiones es por encadenamiento, así que la tabla sigue funcionando aunque se agreguen más estaciones.

### Árbol sin balanceo

El ABB no se balancea. Es correcto para la consigna y permite responder la pregunta de análisis sobre la carga ordenada. La contrapartida es que la búsqueda puede degradarse a O(n).

### Taller

Cada bicicleta existente puede ingresarse al taller con prioridad 1 a 3. Las eléctricas se fuerzan siempre a prioridad 1. Si la bicicleta no figura como averiada se muestra un aviso, pero se permite el ingreso como mantenimiento preventivo; la consigna describe a las averiadas y no prohíbe el caso. Si la bicicleta ya está en el taller, el ingreso se rechaza.

## Respuestas a las consignas de análisis

### A. Gestión de estaciones

**Tabla de hash.** Con las 10 estaciones de `estaciones.txt` se producen **3 colisiones** y el factor de carga final es:

```text
10 / 13 = 0.769230... ≈ 0.7692
```

La búsqueda, inserción y eliminación son **O(1) promedio** y **O(n) en el peor caso**, cuando todas las claves caen en el mismo bucket. El encadenamiento mantiene la correctitud y evita tener que redimensionar la tabla.

**Ordenamiento.** Se usa inserción sobre un arreglo de punteros: **O(n²)** en el peor caso y **O(n)** si el arreglo ya está casi ordenado. Se eligió porque es un algoritmo de ordenamiento implementado a mano, estable para este uso y suficientemente simple para n = 10.

**Baja ocupación.** Se recorre la tabla y se compara `disponibles / capacidad < 0.20`: **O(n)**. Con los datos de prueba quedan `CAB`, `CON`, `BOE` e `ISL`.

**BFS.** El grafo se representa con lista de adyacencia y el recorrido usa una cola propia. El costo es **O(V + E)**, que para una red dispersa es mejor que una matriz de adyacencia. Con los datos de prueba, `PLZ` a `CON` tiene 4 tramos, por ejemplo `PLZ -> CAB -> PAL -> BOE -> CON`.

### B. Viajes y colas

1. **Cantidad de viajes por estación y rango de fechas:** se recorre la lista completa de viajes y se comparan origen y fecha inclusive. Complejidad **O(m)**, con `m` viajes. La lista enlazada es adecuada porque las consultas pedidas son recorridos completos y la inserción es O(1).

2. **Viajes por DNI y total de minutos:** se recorre la lista, se muestran los viajes del DNI y se acumulan las duraciones. Complejidad **O(m)**. No se eligió un índice por DNI porque el PDF pide una búsqueda y la lista simple evita una estructura adicional.

3. **Bicicletas con más de X kilómetros:** se hace un recorrido inorder del ABB y se filtra por kilómetros. Complejidad **O(n)**, con `n` bicicletas. El ABB ya está ordenado por ID y permite reutilizar el mismo recorrido para el listado completo.

4. **Cola de espera por estación:** cada estación tiene una cola FIFO enlazada. Encolar y desencolar son **O(1)**. Se eligió una cola porque el orden de atención debe ser el de llegada y no se necesita acceso aleatorio.

5. **Taller con min-heap:** el heap binario guarda `(biciId, prioridad)` en un arreglo dinámico. Insertar y extraer el mínimo son **O(log n)** y consultar la raíz es O(1). Se eligió un min-heap porque siempre se necesita la bicicleta más urgente, no un orden completo.

### C. ABB de bicicletas

Inorder, búsqueda, conteo de averiadas, altura y eliminación de los tres casos son las operaciones implementadas. El listado inorder es **O(n)**; la búsqueda es **O(h)**, donde `h` es la altura, con promedio **O(log n)** y peor caso **O(n)**. Contar averiadas y calcular la altura también son **O(n)**. Eliminar es **O(h)**: hoja y un hijo se resuelven desenganchando el nodo; dos hijos usan el sucesor mínimo del subárbol derecho. La liberación final es postorder y cuesta **O(n)**.

**Ejemplo de 5 bicicletas cargadas ordenadas:**

```text
501 -> 502 -> 503 -> 504 -> 505
```

El árbol queda degenerado como una cadena. Si la altura se cuenta por niveles, como hace el programa, es **5**; si se cuenta por aristas, es **4**. En ese caso la búsqueda de la 505 recorre los 5 nodos y cuesta **O(n)** en lugar de O(log n). Con los 12 registros ascendentes de `bicicletas.txt` el árbol real tiene altura **12**; al insertar manualmente la 999 pasa a **13**.

### D. Redistribución con backtracking

Se incluye o se excluye cada estación, se poda cuando la suma supera C y se imprimen todos los subconjuntos cuya suma es exactamente C. En el peor caso hay que explorar los `2^n` subconjuntos, así que la complejidad temporal es **O(2^n)** y la memoria auxiliar es **O(n)** por la selección y la pila de recursión. La poda mejora el caso promedio, pero no cambia la cota del peor caso.

Ejemplo del PDF con `C = 10` y faltantes `PLZ 3`, `CAB 7`, `RET 5`, `CON 2`, `BOE 5`:

```text
{PLZ, CAB}
{RET, BOE}
{PLZ, RET, CON}
{PLZ, CON, BOE}
```

Son exactamente **4 soluciones**.

## Verificación

- `compilar.bat` compila `tp.exe` con `g++ -std=c++17 -Wall -Wextra`, sin warnings.
- El código se revisó con `cppcheck --enable=warning,performance,portability --inconclusive --std=c++17 *.cpp` sin hallazgos.
- En Linux se recomienda verificar con Valgrind:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./tp
```

En este entorno Windows no están disponibles Valgrind ni ASan/UBSan para MinGW; por eso la verificación se apoya en RAII, destructores, revisión de código y `cppcheck`. Todas las estructuras liberan sus nodos y el ABB libera en postorder.

## UML

El diagrama de clases está en [`docs/diagrama-clases.md`](docs/diagrama-clases.md), con las composiciones y asociaciones explicadas. El archivo usa Mermaid, que GitHub y la mayoría de los visores Markdown renderizan directamente.

## Estructura del repositorio

```text
.
├── main.cpp / interfaz.*        # menú y entrada por consola
├── Red.* / Red*.cpp             # fachada y secciones A-D
├── TablaHash.*                  # hash de estaciones
├── Estacion.* / ColaEspera.*    # estación y cola FIFO
├── Bicicleta.* / AbbBicicletas.*# bicicletas y ABB
├── MinHeap.*                    # taller
├── ListaViajes.* / Viaje.*      # viajes
├── GrafoCiclovias.*             # grafo y BFS
├── Backtracking.*               # redistribución
├── compilar.bat                 # compilación en Windows
├── estaciones.txt, bicicletas.txt, viajes.txt, ciclovias.txt
├── README.md
├── TP2C2026.pdf                 # enunciado del trabajo práctico
└── docs/diagrama-clases.md
```
