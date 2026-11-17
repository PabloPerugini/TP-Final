# Sistema de Bicicletas Públicas — TP Final de Algoritmos (768)

Trabajo Práctico Final de **Algoritmos (768)** — **UNAHUR**, implementado en **C++17** bajo el paradigma de orientación a objetos, gestión manual de memoria dinámica y estructuras de datos desarrolladas desde cero[cite: 5, 16].

El sistema modela una red urbana de bicicletas públicas (estaciones, flota, historial de viajes, red de ciclovías, taller de reparaciones y logística de redistribución) y permite probar todas las funcionalidades de las **secciones A, B, C y D** mediante un menú interactivo por consola[cite: 5, 16].

---

## Aspectos técnicos y restricciones cumplidas

* **Sin contenedores prohibitivos de la STL:** no se utilizan `vector`, `map`, `unordered_map`, `queue`, `priority_queue` ni `set`[cite: 5, 16]. Se emplea `std::string` y flujos (`fstream`, `iostream`) únicamente para entrada/salida y manejo de texto[cite: 5, 15, 16].
* **Estructuras de datos propias:** tabla de hash con encadenamiento, cola FIFO enlazada, lista simplemente enlazada, min-heap sobre arreglo dinámico, árbol binario de búsqueda (ABB) y grafo no dirigido con listas de adyacencia[cite: 16, 25].
* **Gestión segura de memoria (RAII):** toda memoria reservada con `new` / `new[]` se libera en los destructores correspondientes; el ABB libera sus nodos mediante recorrido *postorder* y `Red::reiniciar()` permite recargar los archivos sin fugas ni duplicados[cite: 15, 16].
* **Estándar y convenciones:** clases en `PascalCase`, métodos y variables en `camelCase`, y compilación limpia sin warnings con `g++ -std=c++17 -Wall -Wextra`[cite: 7, 16].

---

## Estructura del repositorio

```text
TP-Final/
├── main.cpp                         # Punto de entrada y bucle principal del menú[cite: 16, 19]
├── interfaz.h / interfaz.cpp        # Validación de entradas y formateo por consola[cite: 16, 23]
├── Red.h / Red.cpp                  # Fachada principal, ciclo de vida y carga de archivos[cite: 15, 16]
├── RedEstaciones.cpp                # Operaciones de la Sección A (estaciones y BFS)[cite: 12]
├── RedViajes.cpp                    # Operaciones de la Sección B (viajes, retiros, devoluciones y taller)[cite: 10]
├── RedBicicletas.cpp                # Operaciones de la Sección C (gestión del ABB)[cite: 13]
├── RedRedistribucion.cpp            # Operaciones de la Sección D (redistribución con backtracking)[cite: 11]
├── TablaHash.h / TablaHash.cpp      # Índice hash de estaciones (M = 13, encadenamiento)[cite: 8, 9, 16]
├── Estacion.h / Estacion.cpp        # Modelo de estación, ocupación y cola de espera propia[cite: 16, 27, 28]
├── ColaEspera.h / ColaEspera.cpp    # Cola FIFO enlazada de DNIs en espera[cite: 16, 29, 30]
├── GrafoCiclovias.h / .cpp          # Grafo no dirigido de ciclovías y búsqueda BFS[cite: 16, 24, 25]
├── ListaViajes.h / .cpp             # Lista enlazada simple del historial de viajes[cite: 16, 20, 21]
├── Viaje.h / Viaje.cpp              # Modelo de viaje (asociación por ID y códigos)[cite: 3, 4, 16]
├── AbbBicicletas.h / .cpp           # Árbol Binario de Búsqueda ordenado por bici_id[cite: 16, 37, 38]
├── Bicicleta.h / Bicicleta.cpp      # Modelo de bicicleta (tipo, estado, kilometraje)[cite: 33, 34]
├── MinHeap.h / MinHeap.cpp          # Cola de prioridad del taller sobre arreglo dinámico[cite: 16, 17, 18]
├── Backtracking.h / .cpp            # Búsqueda exhaustiva con poda para redistribución[cite: 35, 36]
├── compilar.bat                     # Script de compilación en Windows[cite: 16]
├── estaciones.txt                   # Lote de prueba de estaciones[cite: 26]
├── bicicletas.txt                   # Lote de prueba de bicicletas[cite: 32]
├── viajes.txt                       # Lote de prueba de viajes[cite: 2]
├── ciclovias.txt                    # Lote de prueba de conexiones entre estaciones[cite: 31]
├── docs/
│   └── RedDeBicicletas.drawio.jpg   # Diagrama de clases UML[cite: 40]
└── README.md
```

> **Modularización de `Red`:** aunque `Red` es una única clase fachada, su implementación se dividió en cinco archivos `.cpp` (`Red.cpp`, `RedEstaciones.cpp`, `RedViajes.cpp`, `RedBicicletas.cpp` y `RedRedistribucion.cpp`) para mantener archivos cortos, legibles y alineados con cada sección del enunciado[cite: 10, 11, 12, 13, 15, 16].

---

## Compilación y ejecución

El programa abre los archivos de datos mediante rutas relativas, por lo que debe ejecutarse desde el directorio raíz donde se encuentran `estaciones.txt`, `bicicletas.txt`, `viajes.txt` y `ciclovias.txt`[cite: 16].

### Windows (MinGW / g++)

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

---

## Archivos de entrada

Cada archivo contiene un registro por línea con campos separados por espacios[cite: 5, 16]:

| Archivo | Formato de campos | Notas |
| :--- | :--- | :--- |
| `estaciones.txt` | `codigo nombre barrio capacidad_anclajes bicis_disponibles`[cite: 5, 16] | `codigo` actúa como clave única en el hash y el grafo[cite: 9, 25]. |
| `bicicletas.txt` | `bici_id codigo_estacion tipo estado kilometros`[cite: 5, 16] | `tipo`: `Mecanica` \| `Electrica`. `estado`: `OK` \| `Averiada`[cite: 5, 16]. |
| `viajes.txt` | `viaje_id bici_id dni_usuario estacion_origen estacion_destino fecha duracion_min`[cite: 5, 16] | `fecha` en formato entero `AAAAMMDD`[cite: 3, 5, 16]. |
| `ciclovias.txt` | `estacion_a estacion_b`[cite: 6, 16] | Aristas no dirigidas (conexión bidireccional)[cite: 6, 16]. |

---

## Menú de funcionalidades

| Sección | Opción | Funcionalidad |
| :--- | :---: | :--- |
| **Sistema** | **1** | Cargar o recargar los cuatro archivos `.txt` (reinicia estructuras previas)[cite: 16, 19, 23] |
| **A — Estaciones** | **2** | Mostrar información detallada de una estación por código[cite: 12, 16, 23] |
| **A — Estaciones** | **3** | Agregar una estación nueva al sistema (hash + vértice en grafo)[cite: 12, 16, 23] |
| **A — Estaciones** | **4** | Eliminar una estación existente (sin reubicar bicicletas)[cite: 12, 16, 23] |
| **A — Estaciones** | **5** | Listar estaciones ordenadas por capacidad de anclajes o disponibles[cite: 12, 16, 23] |
| **A — Estaciones** | **6** | Listar estaciones con menos del 20% de ocupación[cite: 12, 16, 23] |
| **A — Estaciones** | **7** | Calcular camino con menor cantidad de tramos por ciclovías (BFS)[cite: 16, 23, 25] |
| **B — Viajes y Colas** | **8** | Contar viajes salientes de una estación en un rango de fechas[cite: 10, 16, 23] |
| **B — Viajes y Colas** | **9** | Buscar viajes por DNI de usuario y sumar minutos pedaleados[cite: 10, 16, 23] |
| **B — Viajes y Colas** | **10** | Listar bicicletas con más de $X$ kilómetros para revisión[cite: 10, 16, 23] |
| **B — Viajes y Colas** | **11** | Retirar bicicleta (o encolar DNI en espera si no hay disponibles)[cite: 10, 16, 23] |
| **B — Viajes y Colas** | **12** | Devolver bicicleta (asigna al primero de la cola o incrementa anclajes)[cite: 16, 23, 28] |
| **B — Viajes y Colas** | **13** | Taller de reparaciones: ingresar bicicleta o extraer la más urgente[cite: 16, 23] |
| **C — ABB Bicicletas** | **14** | Insertar una bicicleta manualmente en el ABB[cite: 13, 16, 23] |
| **C — ABB Bicicletas** | **15** | Buscar una bicicleta por ID[cite: 13, 16, 23] |
| **C — ABB Bicicletas** | **16** | Listar todas las bicicletas ordenadas por ID (recorrido *inorder*)[cite: 13, 16, 23] |
| **C — ABB Bicicletas** | **17** | Contar bicicletas averiadas[cite: 13, 16, 23] |
| **C — ABB Bicicletas** | **18** | Calcular la altura actual del ABB[cite: 13, 16, 23] |
| **C — ABB Bicicletas** | **19** | Eliminar una bicicleta del ABB (hoja, un hijo o dos hijos)[cite: 16, 23, 38] |
| **D — Redistribución** | **20** | Calcular combinaciones exactas de redistribución con backtracking[cite: 11, 16, 23] |
| **Métricas** | **21** | Mostrar estadísticas de la tabla hash (colisiones, factor de carga, buckets)[cite: 15, 16, 23] |
| **Sistema** | **0** | Liberar memoria y salir[cite: 16, 19, 23] |

> **Nota sobre la opción 11:** el encolado de usuarios no tiene una opción aislada porque modela la regla real del dominio: al intentar retirar una bicicleta en una estación sin unidades disponibles, el sistema registra automáticamente el DNI en la `ColaEspera` de esa estación[cite: 10, 16].

---

## Arquitectura y Diagrama UML

![Diagrama de Clases UML — Red de Bicicletas Públicas](docs/RedDeBicicletas.drawio.jpg)

> **Versión interactiva (zoom y edición):** [Abrir `RedDeBicicletas.drawio` en Google Drive / diagrams.net](https://drive.google.com/file/d/1GkzTnbcySADKTKrQYvGB1vd8pk5nFUGV/view?usp=sharing)

El diagrama refleja el patrón **Fachada** centralizado en `Red` y distingue tres tipos de relaciones entre las clases del sistema[cite: 39, 40]:

* **Composición (`*--`):** `Red` crea, administra y destruye las instancias de `TablaHash`, `GrafoCiclovias`, `ListaViajes`, `AbbBicicletas` y `MinHeap`[cite: 15, 39, 40]. A su vez, `Estacion` compone por valor su `ColaEspera`, `MinHeap` administra su arreglo dinámico de `EntradaTaller`, y cada contenedor libera en su destructor los objetos dinámicos que almacena (`Estacion`, `Viaje` y `Bicicleta`)[cite: 9, 18, 21, 27, 38, 39, 40].
* **Asociación (`-->`):** `Bicicleta` referencia a su estación mediante `codigoEstacion`, y `Viaje` referencia bicicleta (`biciId`) y estaciones (`origen / destino`) por clave lógica, sin asumir propiedad de memoria[cite: 3, 33, 39, 40].
* **Dependencia (`..>`):** `Red` utiliza los métodos estáticos de `Backtracking` entregándole arreglos auxiliares temporales para resolver la redistribución[cite: 11, 35, 39, 40].

---

## Decisiones de diseño

### 1. Independencia entre disponibilidad de estaciones y ABB de bicicletas
Los archivos de prueba presentan una inconsistencia de origen: por ejemplo, `PAL` declara 35 bicicletas disponibles en `estaciones.txt`, pero en `bicicletas.txt` solo dos registros apuntan a `PAL` (y `BEL` declara 44 frente a 1 registro en `bicicletas.txt`)[cite: 16, 26, 32]. Dado que el enunciado no solicita sincronizar ambos archivos, se mantuvieron dos fuentes de verdad desacopladas[cite: 16]:
* `Estacion::biciDisponibles` gobierna el cálculo de ocupación, los retiros, las devoluciones y la cola de espera[cite: 16, 28].
* `AbbBicicletas` gobierna el inventario individual de bicicletas (búsqueda por ID, recorrido *inorder*, conteo de averiadas y filtro por kilometraje)[cite: 16, 38].

De este modo se evita fabricar datos ficticios y cada estructura mantiene una responsabilidad única y coherente[cite: 16].

### 2. Tabla de hash de tamaño fijo ($M = 13$)
Se utiliza una tabla de 13 buckets (número primo) con la función de dispersión[cite: 8, 9, 16]:

```text
hash(codigo) = suma(codigo[i] * (i + 1)) mod 13
```

Mantener $M = 13$ fijo permite reproducir exactamente las métricas de colisiones y factor de carga solicitadas, mientras que la resolución de colisiones por **encadenamiento** garantiza que el sistema siga operando correctamente ante cualquier cantidad de inserciones[cite: 9, 16].

### 3. ABB sin auto-balanceo
El árbol binario de búsqueda no implementa rotaciones (AVL/Red-Black)[cite: 16, 38]. Esta decisión responde directamente al alcance del práctico y permite observar experimentalmente cómo la inserción de claves ordenadas degrada la altura del árbol[cite: 16].

### 4. Reglas del taller de reparaciones (`MinHeap`)
El taller admite prioridades del `1` (máxima urgencia) al `3`[cite: 7, 10, 16]. Si la bicicleta ingresada es de tipo `Electrica`, el sistema fuerza automáticamente la prioridad a `1`[cite: 7, 10, 16]. Además, se valida que la bicicleta exista en el ABB y no esté duplicada en el taller; si su estado figura como `OK`, se emite un aviso pero se permite el ingreso en concepto de mantenimiento preventivo[cite: 10, 16].

---

## Respuestas a las consignas de análisis

### Resumen de complejidades temporales

| Operación / Estructura | Estructura utilizada | Caso promedio | Peor caso |
| :--- | :--- | :---: | :---: |
| Búsqueda / inserción / baja de estación | `TablaHash` ($M = 13$, encadenamiento)[cite: 9, 16] | $O(1)$[cite: 16] | $O(n)$[cite: 16] |
| Ordenamiento de estaciones | Arreglo dinámico + Inserción[cite: 12, 16] | $O(n^2)$[cite: 16] | $O(n^2)$[cite: 16] |
| Filtro de estaciones con $< 20\%$ ocupación | Recorrido completo de `TablaHash`[cite: 12, 16] | $O(n)$[cite: 16] | $O(n)$[cite: 16] |
| Camino mínimo por ciclovías | `GrafoCiclovias` + BFS con cola[cite: 16, 25] | $O(V + E)$[cite: 16] | $O(V + E)$[cite: 16] |
| Viajes por estación/fecha o por DNI | `ListaViajes` (lista enlazada simple)[cite: 16, 21] | $O(m)$[cite: 16] | $O(m)$[cite: 16] |
| Bicicletas con más de $X$ kilómetros | `AbbBicicletas` (recorrido *inorder*)[cite: 16, 38] | $O(n)$[cite: 16] | $O(n)$[cite: 16] |
| Encolar / desencolar usuario en espera | `ColaEspera` (cola FIFO enlazada)[cite: 16, 30] | $O(1)$[cite: 16] | $O(1)$[cite: 16] |
| Ingresar / extraer urgencia en taller | `MinHeap` (arreglo dinámico)[cite: 16, 18] | $O(\log n)$[cite: 16] | $O(\log n)$[cite: 16] |
| Búsqueda / eliminación por ID en ABB | `AbbBicicletas` (altura $h$)[cite: 16, 38] | $O(\log n)$[cite: 16] | $O(n)$[cite: 16] |
| Redistribución exacta de capacidad $C$ | `Backtracking` con poda[cite: 16, 36] | $O(2^n)$[cite: 16] | $O(2^n)$[cite: 16] |

---

### Sección A — Gestión de estaciones

* **Tabla de hash:** con las 10 estaciones de `estaciones.txt` se registran **3 colisiones** y el factor de carga final es[cite: 16]:

  $$\alpha = \frac{10}{13} \approx 0.7692$$

  La búsqueda, inserción y eliminación son **$O(1)$ en promedio** y **$O(n)$ en el peor caso** si todas las claves colisionaran en el mismo bucket[cite: 16].
* **Ordenamiento manual:** se implementó **ordenamiento por inserción** sobre un arreglo dinámico de punteros (`Estacion**`)[cite: 12, 16]. Tiene complejidad **$O(n^2)$** en el peor caso y **$O(n)$** si el arreglo ya está casi ordenado; es estable y adecuado para conjuntos pequeños de estaciones[cite: 16].
* **Baja ocupación ($< 20\%$):** recorre todos los buckets de la tabla comparando `disponibles / capacidad < 0.20` en **$O(n)$**[cite: 12, 16]. Con los datos de prueba identifica cuatro estaciones: `CAB` (16%), `CON` (15%), `BOE` (13.33%) e `ISL` (0%)[cite: 16, 26].
* **Camino más corto (BFS):** el grafo no dirigido usa listas de adyacencia y una cola FIFO auxiliar[cite: 16, 25]. Su costo es **$O(V + E)$**, óptimo en memoria y tiempo para redes urbanas dispersas frente a una matriz de adyacencia ($O(V^2)$)[cite: 16]. Con los datos de prueba, ir de `PLZ` a `CON` requiere **4 tramos** (por ejemplo: `PLZ -> CAB -> PAL -> BOE -> CON`)[cite: 16].

---

### Sección B — Viajes y colas de espera

1. **Viajes por estación y rango de fechas — $O(m)$:** recorre los $m$ nodos de `ListaViajes` filtrando por estación de origen y fecha inclusiva (`AAAAMMDD`)[cite: 16, 21]. La lista enlazada permite inserciones en $O(1)$ y recorre secuencialmente todo el historial cuando se consulta[cite: 16, 21].
2. **Viajes por DNI y suma de minutos — $O(m)$:** recorre la lista completa acumulando la duración de cada coincidencia[cite: 16, 21]. No se agregó un índice secundario por DNI para evitar duplicar estructuras frente a una consulta puntual[cite: 16].
3. **Bicicletas con más de $X$ kilómetros — $O(n)$:** reutiliza el recorrido *inorder* del ABB visitando los $n$ nodos y filtrando por kilometraje[cite: 16, 38].
4. **Cola de espera por estación — $O(1)$:** cada estación posee una `ColaEspera` con punteros a `frente` y `fin`, garantizando atención estricta por orden de llegada (FIFO) en tiempo constante tanto al encolar como al desencolar[cite: 16, 29, 30].
5. **Taller de reparaciones — $O(\log n)$:** el `MinHeap` mantiene en la raíz la bicicleta con menor valor numérico de prioridad (mayor urgencia)[cite: 16, 18]. Insertar y extraer el mínimo requieren reorganizar a lo sumo la altura del árbol binario completo ($\lfloor \log_2 n \rfloor$), mientras que consultar el elemento más urgente es $O(1)$[cite: 16, 18].

---

### Sección C — ABB de bicicletas

* El recorrido *inorder*, el conteo de averiadas, el cálculo de altura y la liberación *postorder* visitan cada nodo una vez, por lo que tienen complejidad **$O(n)$**[cite: 16, 38].
* La búsqueda y la eliminación dependen de la altura $h$ del árbol: **$O(h)$** ($O(\log n)$ en un árbol balanceado y **$O(n)$** en un árbol degenerado)[cite: 16, 38]. Para eliminar un nodo con dos hijos se reemplaza su dato por el **sucesor inorder** (mínimo del subárbol derecho) y se desengancha dicho sucesor[cite: 16, 38].

**Análisis de carga ordenada (ejemplo con 5 bicicletas):**
Si se cargan las bicicletas `501, 502, 503, 504, 505` en orden ascendente, cada inserción se encadena siempre por el subárbol derecho[cite: 16]:

```text
501 -> 502 -> 503 -> 504 -> 505
```

El ABB degenera en una lista enlazada con **altura 5** medida por niveles (o **4** medida por aristas)[cite: 16]. En consecuencia, buscar la bicicleta `505` obliga a recorrer los 5 nodos, degradando el costo de búsqueda de $O(\log n)$ a **$O(n)$**[cite: 16]. En el lote real de `bicicletas.txt` (12 bicicletas ordenadas de `501` a `512`), la altura inicial es **12**, y al insertar manualmente la bicicleta `999` pasa a **13**[cite: 16, 32].

---

### Sección D — Redistribución con backtracking

El algoritmo explora recursivamente el árbol binario de decisión (incluir o excluir cada estación con faltantes), aplicando poda inmediata cuando `sumaActual > capacidad` o cuando no quedan estaciones suficientes[cite: 16, 36].

* **Complejidad temporal:** **$O(2^n)$** en el peor caso, ya que existen $2^n$ subconjuntos posibles entre $n$ estaciones con faltantes[cite: 16]. La poda reduce drásticamente las ramas exploradas en la práctica, pero no altera la cota asintótica superior[cite: 16].
* **Complejidad espacial:** **$O(n)$** debido al arreglo booleano de selección y a la profundidad máxima de la pila de llamadas recursivas[cite: 16, 36].

Con el ejemplo de referencia ($C = 10$ y faltantes `PLZ: 3`, `CAB: 7`, `RET: 5`, `CON: 2`, `BOE: 5`), el algoritmo encuentra exactamente **4 soluciones**[cite: 7, 16]:

```text
Solucion 1: {PLZ, CAB}
Solucion 2: {RET, BOE}
Solucion 3: {PLZ, RET, CON}
Solucion 4: {PLZ, CON, BOE}
```

---

## Verificación de calidad y memoria

* **Compilación estricta:** probado con `g++ -std=c++17 -Wall -Wextra` sin advertencias[cite: 16].
* **Análisis estático:** verificado con `cppcheck --enable=warning,performance,portability --inconclusive --std=c++17 *.cpp` sin hallazgos[cite: 16].
* **Gestión de memoria dinámica:** todas las estructuras implementan destructores completos y constructor de copia/asignación deshabilitados (`= delete`) para evitar copias superficiales accidentales[cite: 14, 16, 17, 20, 24, 29, 37]. En entornos Linux puede auditarse en tiempo de ejecución mediante[cite: 16]:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./tp
```