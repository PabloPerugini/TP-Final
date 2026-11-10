# Diseño — Sistema de Bicicletas Públicas (TP Algoritmos)

**Fecha:** 2026-11-09
**Estado:** propuesto, pendiente de aprobación
**Enfoque elegido:** A — conservar la base y separar una clase por estructura.

## 1. Intención

Terminar el TP integrador de Algoritmos en C++17 para que quede entregable y defendible oralmente: menú completo para las secciones A–D, estructuras implementadas a mano, compilación sin warnings con `-Wall -Wextra`, sin fugas de memoria, README con las respuestas de análisis y diagrama UML.

El objetivo no es solo que compile: cada estructura debe poder explicarse por separado y el UML debe reflejar la composición real del sistema.

## 2. Objetivos

- Implementar todas las consignas obligatorias de las secciones A, B, C y D.
- Implementar el opcional de eliminación en el ABB (tres casos).
- Mantener el menú por consola y permitir probar cada funcionalidad con los archivos de ejemplo.
- Implementar a mano tabla de hash, cola, min-heap, ABB, grafo y backtracking. Sin `vector`, `map`, `unordered_map`, `queue`, `priority_queue` ni `set`.
- Entregar README, UML, pruebas automatizadas y scripts de compilación.
- Verificar compilación limpia y liberación de memoria.

## 3. No objetivos

- Persistencia en base de datos o archivos de salida.
- Interfaz gráfica.
- Reubicar bicicletas al eliminar una estación (la consigna lo exime).
- Redimensionar la tabla de hash. Se mantiene fija en 13 buckets para conservar las métricas pedidas; el encadenamiento mantiene la correctitud si se agregan estaciones.
- Prevenir DNIs duplicados en la cola de espera; no es un requisito.
- Balancear el ABB (AVL/rojo-negro). El caso degenerado se analiza en el README.

## 4. Restricciones

- C++17, compilador `g++`.
- `g++ -std=c++17 -Wall -Wextra` sin warnings.
- Sin pérdidas de memoria (RAII y destructores; verificación con Valgrind/ASan cuando esté disponible).
- Variables y métodos en camelCase; clases en PascalCase.
- Clases propias y originales.
- Archivos de código de menos de 300 líneas; si `Red` crece, se reparte en archivos por sección.
- Los archivos de entrada están bien formados, pero el programa debe avisar si no puede abrirlos o si un dato no existe.

## 4.1 Trazabilidad con el PDF (fuente de verdad)

El PDF `TP2C2026.pdf` manda sobre este diseño. Si una decisión de este documento entra en conflicto con el PDF, se corrige el diseño y se descarta el extra. Los agregados (pruebas, estadísticas de hash, modo manual de backtracking, PNG) son apoyo y no reemplazan ninguna consigna.

| Consigna del PDF | Implementación | Evidencia |
|---|---|---|
| Estructuras a mano; sin `vector`, `map`, `queue`, etc. | Clases propias con nodos/arreglos dinámicos | Código y README |
| Lectura de los cuatro archivos | Cargas de `Red` | Menú opción 1 |
| A: mostrar estación | `Red::mostrarInformacionEstacion` | Menú 2 |
| A: agregar estación | `Red::agregarEstacion` | Menú 3 |
| A: eliminar estación sin reubicar bicis | `Red::eliminarEstacion` | Menú 4 |
| A: ordenar por anclajes o disponibles, a mano | Inserción sobre arreglo de punteros | Menú 5 |
| A: estaciones con menos del 20% | `Red::listarEstacionesBajaOcupacion` | Menú 6 |
| A: BFS de menor cantidad de tramos | `GrafoCiclovias::buscarCaminoMasCorto` | Menú 7 |
| A: hash con colisiones y métricas en README | `TablaHash` + README | README |
| B1: viajes por estación y rango de fechas | `ListaViajes::contarPorRangoFechas` | Menú 8 |
| B2: viajes por DNI y total de minutos | `ListaViajes::buscarPorDni` | Menú 9 |
| B3: bicis con más de X km | `AbbBicicletas::listarConKilometrosMayoresA` | Menú 10 |
| B4: cola de espera por estación | `Red::retirarBicicletaEnEstacion` + `devolverBicicletaEnEstacion` | Menú 11 y 12 |
| B5: min-heap del taller | `MinHeap` | Menú 13 |
| B: Big O y justificación por punto | README, sección de análisis | README |
| C: ABB por `bici_id`, insertar/buscar/inorder | `AbbBicicletas` + inserción manual | Menú 14, 15, 16 |
| C: contar averiadas y altura | `AbbBicicletas` | Menú 17, 18 |
| C: liberar en postorder | Destructor de `AbbBicicletas` | Código |
| C: análisis de carga ordenada, ejemplo 5 | README | README |
| C: eliminación opcional (3 casos) | `AbbBicicletas::eliminar` | Menú 19 |
| D: backtracking con C y faltantes | `Backtracking` | Menú 20 |
| D: todos los subconjuntos o aviso | `Backtracking::resolver` | Menú 19 |
| D: complejidad peor caso | README | README |
| Menú por consola | `main.cpp` | Ejecutable |
| UML con asociación y composición | `docs/diagrama-clases.md` | Documento |
| camelCase / PascalCase | Revisión de código | Código |
| Compilar sin warnings y sin fugas | `compilar.bat`, pruebas, Valgrind/ASan | Verificación |

## 5. Estado actual y limpieza

Ya funcionan `Estacion`, `Bicicleta`, `Viaje` y `TablaHash` (hash con encadenamiento, función `suma(codigo[i] * (i+1)) % 13`). `Red` es un esqueleto de stubs. Hay archivos a medio escribir o vacíos de una sesión anterior: `MinHeap.h`, `AbbBicicletas.h`, `ArrayDinamico.h`, `backtracking.cpp` y `Red.cpp` con el contenido de `Red.h`.

Se eliminan o reemplazan esos archivos. `ArrayDinamico` no se usa: cada estructura maneja su propia memoria para no sumar una abstracción genérica que el TP no pide. `Cola.h` no existe todavía y se crea como `ColaEspera`.

La `TablaHash` existente se conserva y se le agregan operaciones de volcado y vaciado. La función hash y el tamaño 13 no cambian.

## 6. Arquitectura

### 6.1 Red como raíz de composición

`Red` es dueña de:

- `TablaHash* estaciones`
- `GrafoCiclovias* ciclovias`
- `ListaViajes* viajes`
- `AbbBicicletas* bicicletas`
- `MinHeap* taller`

El constructor crea las cinco estructuras y el destructor las libera. `reiniciar()` las destruye y vuelve a crear para que la opción 1 del menú recargue los archivos sin duplicar datos. `Red` no se copia (constructor de copia y asignación privados) para evitar doble liberación.

### 6.2 Clases y responsabilidades

| Clase | Responsabilidad | Memoria |
|---|---|---|
| `Red` | Fachada, cargas, menú y coordinación | Dueña de las cinco estructuras |
| `Estacion` | Datos, disponibilidad, cola de espera | Dueña de su `ColaEspera` por valor |
| `ColaEspera` | Cola FIFO de DNIs | Nodos enlazados; libera en destructor |
| `TablaHash` | Índice de estaciones por código | Dueña de `Estacion*`; encadenamiento |
| `GrafoCiclovias` | Red de ciclovías y BFS | Vértices y aristas enlazados |
| `ListaViajes` | Registro de viajes | Nodos enlazados con `Viaje*` |
| `AbbBicicletas` | Registro ordenado por `biciId` | Nodos con `Bicicleta*`; liberación postorder |
| `MinHeap` | Taller de reparaciones | Arreglo dinámico de `EntradaTaller` |
| `Backtracking` | Subconjuntos que suman C | Arreglos auxiliares; sin ownership |

`Bicicleta` y `Viaje` son clases de datos. `Viaje` referencia `biciId` y códigos de estación por asociación, no por composición. `Bicicleta` referencia su estación por código.

### 6.3 Decisiones de estructuras

- **Estaciones:** tabla de hash con encadenamiento. Búsqueda promedio O(1); la tabla fija de 13 buckets es la que ya existe y permite reportar colisiones y factor de carga.
- **Ciclovías:** grafo no dirigido con lista de adyacencia. La red es dispersa; BFS recorre O(V+E) y se reconstruye el camino con punteros al padre.
- **Espera por estación:** cola FIFO enlazada dentro de `Estacion`. Encolar y desencolar O(1), respeta el orden de llegada.
- **Taller:** min-heap binario en arreglo dinámico. Insertar y extraer el mínimo O(log n); la prioridad 1 es la más urgente.
- **Bicicletas:** ABB ordenado por `biciId`. Inorder entrega el listado ordenado; búsqueda promedio O(log n), peor O(n) si degenera.
- **Viajes:** lista enlazada simple. Inserción O(1); las consultas pedidas son recorridos completos O(m), suficiente y simple.
- **Redistribución:** backtracking con poda cuando la suma supera C. Peor caso O(2^n).

### 6.4 Flujo de carga

La opción 1 del menú llama a `Red::reiniciar()` y luego carga, en orden: estaciones, bicicletas, viajes y ciclovías. Si un archivo no se puede abrir, se informa y se continúa con lo demás. Las estaciones se insertan en la tabla y se registran como vértices del grafo; las bicicletas se insertan en el ABB; los viajes se agregan a la lista; las ciclovías agregan aristas entre estaciones existentes.

### 6.5 Disponibilidad y ABB: dos fuentes de verdad

El archivo de estaciones trae `bici_disponibles` como un contador independiente, y el archivo de bicicletas trae la estación de cada bici. Los datos de ejemplo no coinciden entre sí (PAL declara 35 disponibles pero solo dos bicis apuntan a PAL; BEL declara 44 y solo tiene la 510; el ejemplo del PDF tiene la misma inconsistencia). Por eso:

- `biciDisponibles` es la fuente de verdad para la cola de espera y para la baja ocupación. Sale de `estaciones.txt`.
- El ABB es la fuente de verdad para los datos de cada bicicleta, la búsqueda por ID, el listado inorder y el filtro por kilómetros.
- Retirar o devolver una bicicleta ajusta el contador y la cola; no elige ni modifica una `Bicicleta` concreta del ABB.
- Insertar o eliminar una bicicleta del ABB no cambia `biciDisponibles`.
- Al eliminar una estación, sus bicicletas quedan en el ABB (la consigna no pide reubicarlas).

Esta separación evita contradecir los archivos de entrada y no agrega una sincronización que el PDF no pide. Se explicará en el README para la defensa.

## 7. Comportamiento por sección

### A. Estaciones

- `mostrarInformacionEstacion`: busca en la tabla y muestra código, nombre, barrio, capacidad, disponibles, porcentaje y usuarios en espera.
- `agregarEstacion`: valida código no vacío, capacidad > 0, disponibles entre 0 y capacidad y que no exista. Inserta en hash y agrega el vértice aislado al grafo.
- `eliminarEstacion`: elimina de la tabla (la estación libera su cola) y del grafo con sus aristas. Las bicicletas quedan en el ABB, como permite la consigna. Si había usuarios esperando, se avisa antes de borrar.
- `listarEstacionesOrdenadas`: vuelca los punteros de la tabla a un arreglo, ordena con inserción por capacidad o disponibles (desempate por código) y muestra la tabla. O(n²).
- `listarEstacionesBajaOcupacion`: recorre las estaciones y muestra las que tienen `disponibles / capacidad < 0.20` (la consigna define la ocupación como disponibles sobre capacidad). O(n).
- `calcularCaminoMasCortoBFS`: pide origen y destino, ejecuta BFS y muestra camino y cantidad de tramos; si no hay camino o falta una estación, lo informa.

### B. Viajes y colas

- Contar viajes de una estación en un rango de fechas inclusivo: recorrido de la lista O(m).
- Buscar viajes por DNI: recorre la lista, muestra cada viaje y el total de minutos; O(m).
- Bicicletas con más de X km: recorrido inorder del ABB, filtro por kilómetros; O(n).
- **Retirar bicicleta (opción 11):** pide código de estación y DNI. Si la estación tiene disponibles, descuenta una y asigna el retiro; si no tiene, encola el DNI. No hay una opción separada de "encolar": el encolado es la consecuencia de un retiro sin disponibilidad, tal como pide el PDF. La firma es `bool retirarBicicletaEnEstacion(codigo, dni)`; devuelve `false` solo si la estación no existe o el DNI está vacío. O(1) promedio.
- **Devolver bicicleta (opción 12):** si hay cola, asigna la bici al primero y muestra el DNI; si no hay cola y hay anclaje, incrementa la disponibilidad; si la estación está llena, informa que no hay lugar. `Estacion::devolverBicicleta` devuelve `AsignadaAEspera`, `AgregadaAEstacion` o `SinLugar`. O(1).
- **Taller (opción 13):** se ingresa una bicicleta existente y una prioridad 1–3. Las eléctricas se fuerzan a prioridad 1; las mecánicas usan la prioridad ingresada. Si la bicicleta no figura como averiada, se advierte, pero se permite el ingreso (el PDF describe a las averiadas y no prohíbe el mantenimiento preventivo). Insertar y extraer son O(log n).

El README dedica a cada uno de los cinco puntos de B un párrafo de dos o tres oraciones con su complejidad Big O y la justificación de la estructura elegida, además de la tabla resumen.

### C. ABB de bicicletas

- **Insertar (opción 14):** además de la carga desde archivo, el menú permite insertar una bicicleta manualmente (ID, estación, tipo, estado, kilómetros). Valida ID único, estación existente, tipo/estado válidos y kilómetros ≥ 0. Inserta en el ABB; no modifica `biciDisponibles`.
- **Buscar (opción 15), listar inorder (16), contar averiadas (17), altura (18) y eliminar (19, opcional)** con los tres casos: hoja, un hijo y dos hijos.
- La altura se define como cantidad de niveles: árbol vacío 0, un nodo 1. En el README se aclara la equivalencia en aristas (un árbol con 5 nodos en cadena tiene altura 5 por niveles y 4 aristas), para que la respuesta de análisis no dependa de la convención.
- El destructor libera en postorder: primero hijos, luego nodo y bicicleta.

### D. Redistribución

El menú (opción 20) ofrece dos modos:

1. **Faltantes del sistema:** usa `capacidad - disponibles` de cada estación cargada, ignorando faltantes ≤ 0.
2. **Lista manual:** pide la cantidad de estaciones y luego pares código/faltante, para poder reproducir el ejemplo de la consigna.

En ambos casos se ejecuta el mismo backtracking: incluir/excluir cada estación, podar si la suma supera C, imprimir cada subconjunto que sume exactamente C y devolver la cantidad de soluciones. Si no hay soluciones, se informa.

## 8. Complejidades

| Operación | Estructura | Tiempo |
|---|---|---|
| Buscar/insertar/eliminar estación | Hash con encadenamiento | O(1) promedio, O(n) peor |
| Listar ordenado | Arreglo + inserción | O(n²) |
| Baja ocupación | Recorrido de hash | O(n) |
| BFS ciclovías | Lista de adyacencia | O(V+E) |
| Viajes por estación/fecha | Lista | O(m) |
| Viajes por DNI | Lista | O(m) |
| Bicis por km | ABB | O(n) |
| Encolar/desencolar espera | Cola | O(1) |
| Insertar/extraer taller | Min-heap | O(log n) |
| Buscar bici | ABB | O(h), promedio O(log n), peor O(n) |
| Listar bicis | ABB inorder | O(n) |
| Contar averiadas / altura | ABB | O(n) |
| Eliminar bici | ABB | O(h) |
| Redistribución | Backtracking | O(2^n) peor caso, O(n) espacio |

## 9. Entregables

- Código fuente modular y compilable.
- `README.md` con compilación, ejecución, módulos, complejidades y respuestas de análisis.
- `docs/diagrama-clases.md` con UML en Mermaid y explicación de composiciones/asociaciones. Si la herramienta lo permite, se exporta también a PNG.
- `pruebas.cpp` con pruebas de las estructuras y `probar.bat` para compilarlas/ejecutarlas.
- `compilar.bat` actualizado y, si aporta claridad, `Makefile` para Linux.

## 10. Verificación

1. Compilar con `g++ -std=c++17 -Wall -Wextra` y revisar que no haya warnings.
2. Ejecutar `pruebas.exe`: cola FIFO, estación (retiro, devolución, cola), hash (inserción, duplicados, colisiones, borrado), heap (orden de extracción), ABB (altura, inorder, averiadas, tres casos de borrado), grafo (camino, sin camino, estación inexistente), lista de viajes (rango y DNI) y backtracking (ejemplo con 4 soluciones).
3. Prueba de integración manual con los archivos de ejemplo: recorrer las 21 opciones del menú.
4. Verificar memoria con ASan si el compilador lo soporta; documentar el comando de Valgrind para Linux. El diseño usa RAII en todas las estructuras.
5. Revisar el README contra la consigna: colisiones (3), factor de carga final (10/13 ≈ 0.77), ejemplo de ABB con 5 bicicletas y complejidad del backtracking.

## 11. Riesgos y decisiones resueltas

- **Archivos concurrentes:** se eliminan los archivos vacíos/corruptos de la sesión anterior; no se conserva `ArrayDinamico`.
- **B4 sin opción de encolar cruda:** la opción 11 es retirar-o-encolar, que es exactamente el flujo del PDF. El encolado solo ocurre cuando no hay disponibilidad, así que una cola no vacía implica disponibilidad cero.
- **Disponibilidad independiente del ABB:** `biciDisponibles` manda para la cola y la baja ocupación; el ABB manda para los datos de bicicletas. Está justificado por la inconsistencia de los archivos de ejemplo y se explica en 6.5.
- **Inserción manual en el ABB:** se agrega la opción 14 para poder probar la operación de inserción del árbol desde el menú, además de la carga desde archivo.
- **Tabla fija:** si se agregan muchas estaciones, el factor de carga sube. Se documenta como limitación; el encadenamiento evita fallos.
- **ABB degenerado:** el archivo de prueba trae IDs ascendentes, así que el árbol quedará como una cadena. Es correcto para un ABB sin balanceo y es justamente lo que responde la pregunta de análisis.
- **Formato de salida:** se usará texto claro en consola; los acentos se evitan en los mensajes de código para no depender de la codificación de la consola. El README sí puede usar UTF-8.
- **UML:** Mermaid es el formato principal; el PNG es deseable pero no bloquea la entrega.

## 12. Criterios de aceptación

- Compila sin warnings con `g++ -std=c++17 -Wall -Wextra`.
- Las pruebas automatizadas pasan.
- Todas las opciones del menú (21) funcionan con los archivos de ejemplo y cubren cada consigna del PDF.
- No hay fugas de memoria en los recorridos normales y en la salida.
- El README responde las consignas de análisis con números concretos.
- El UML muestra composiciones y asociaciones coherentes con el código.
- El proyecto se puede explicar y defender oralmente estructura por estructura.
