<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de Agustina Thames Alderete - 111287 - athames@fi.uba.ar

A la hora de correr el programa, yo utilicé la línea para compilar y la línea de ejecución con valgrind, ya que el makefile ya tenía incorporada una forma más eficiente de hacerlo.

- Para ejecutar con valgrind usando mis pruebas:

```bash
make valgrind-alumno
```

- Para ejecutar con valgrind usando el archivo de ejemplo dado por la cátedra:
```bash
make 
```
---
##  Funcionamiento

En este TP, se implementa un Hash cerrado, con direccionamiento abierto. Es un hash **cerrado** porque todos los valores están y tienen que estar guardados en la tabla de hash.

<div align="center">
<img width="70%" src="img/hash_cerrado.drawio.svg">
</div>

Tiene **direccionamiento abierto** porque se busca un lugar libre en la tabla de hash para alojar el valor en el caso de que haya una colisión. Por lo tanto, la implementación de la tabla de hash resuelve las colisiones mediante probing lineal.

### Estructuras de datos definidas
- Se define 'componente_tabla_t' como las entradas que se utilizan en la tabla de hash. Cada una de ellas contiene una clave, un valor, y un booleano que indica si el espacio en la tabla está ocupado o no.
- Se define 'hash_t' como la tabla de hash, que contiene la capacidad que se le asigna a la tabla en su creación, la cantidad de elementos guardados en la tabla, y un puntero a un array de punteros a 'componente_tabla_t'.

### Funciones auxiliares
- 'hash_function': (función sacada de Internet para garantizar un correcto funcionamiento de la misma). calcula el valor hash para una clave dada, utilizando un algoritmo simple de desplazamiento de bits y suma del valor ASCII del caracter. Luego toma el módulo del resultado con la capacidad (puesta en la función como "tamanio") de la tabla para pbtener un índice válido.
- 'hash_capacidad': devuelve la capacidad de la tabla de hash.
- 'buscar_en_tabla'. busca una clave en la tabla de hash, y devuelve un puntero a su valor asociado en caso de que sea encontrado. Dentro de esta función se utiliza 'hash_function' para poder encontrar la posición inicial y resuelve las colisiones mediante probing lineal.
- 'rehash': duplica la capacidad de la tabla d ehash y vuelve a almacenar todas las entradas en la nueva tabla. Esta función es utilizada cuando el factor de carga de la tabla es mayor a 0.7 (valor predefinido en el enunciado del TP).
- 'strdup2': tiene una implementación que similar a la de strdup de la librería <string.h>

### Funciones principales
- 'hash_crear': crea una nueva tabla de hash con la capacidad pasada por parámtero, o la capacidad mínima en caso de que la indicada por el usuario sea menor a 3 (valor predefinido en el enunciado del TP). Se asigna la memoria adecuada y se inicializan los campos correspondientes.
- 'hash_insertar': inserta un nuevo par clave-valor en la tabla de hash. En caso de que la clave ya exista, actauliza el valor. Si el factor de cara supera el porcentaje 0.7 (valor predefinido en el enunciado del TP), se realiza un rehash.
- 'hash_quitar': elimina la clave pasada por parámetro de la tabla de hash.
- 'hash_obtener': devuelve el valor asociado a la clave pasada por parámetro.
- 'hash_contiene': verifica si la clave pasada por parámetro pertenece a la tabla de hash.
- 'hash_cantidad': devuelve la cantidad de pares clave-valor que se encuentren guardados en la tabla de hash.
- 'hash_destruir' y 'hash_destruir_todo': ambas funciones liberan la memoria ocupada por la tabla de hash y sus pares almacenados, con la diferencia que la segunda de ellas aplica una función que el usuario pasa por parámetro para liberar el valor de la clave asocaida.
- 'hash_con_cada_clave': es un iterador interno que aplica una función pasada por parámetro a cada una de las claves de la tabla de hash.

### Diagramas relevantes:

Los siguientes diagramas son un ejemplo de cómo fue pensado el algoritmo del TDA HASH.

Por un lado, un ejemplo de diagrama de flujo (de la función hash_insertar), en donde se muestra paso a paso las decisiones que s evan tomando, según hay colisión, o algunos otros casos especiales abordados.

<div align="center">
<img width="70%" src="img/diagrama_flujo_inserción.drawio.svg">
</div>

Por el otro, un diagrama que grafica la utilización de espacio de memoria en el programa (stack, Heap y Read-Only Memory).

<div align="center">
<img width="70%" src="img/stack_heap_hash.drawio.svg">
</div>

---

## Respuestas a las preguntas teóricas
### 1. Qué es un diccionario - Explicá 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1)
Un diccionario es una estructura de datos que almacena elementos en pares clave-valor [key-value], donde cada clave está asociada a un único valor, lo cual significa que la clave funciona como un índice. Se puede acceder a los valores mediante las claves, lo que permite una búsqueda rápida y eficiente (con complejidad O(1)).

#### Formas de implementar un diccionario
1. Como primera implementación, la **Tabla de Hash**. Una tabla hash es una estructura que contiene valores, los cuales pueden ser encontradas a partir de la clave correspondiente, ya que todas las claves de la tabla hash tienen un sólo valor. Lo que hace esta función es garantizar que las claves se conviertan en posiciones dentro de una tabla.

2. La segunda, **Árboles de Búsqueda Balanceados (AVL ó Árboles Rojo/Negro)**. Los árboles de búsqueda balanceados son TDAs que almacenan pares key-value en nodos, los cuales se ordenan según el valor de la clave. Esto permite realizar una serie de operaciones, tales como búsqueda, inserción y eliminación.

3. Por último, **Listas Enlazadas**. Una lista enlazada es un TDA que también guarda pares key-value en nodos. Lo que los diferencia de las otras implementaciones de diccionario mencionadas es que cada nodo se enlaza al siguiente en la lista. 

### 2. Qué es una función de hash y qué características debe tener
Una función de hash es aquella quue convierte una clave en un valor hash único, lo cual se utiliza frecuentemente para identificar datos asociados con esa clave en una estructura de datos.

Es importante destacar que **no** se puede obtener la clave a partir de un valor de la tabla de hash.

### 3. Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)
Una tabla de hash es una estructura de datos que implementa un diccionario (tal como se explicó anteriormente). Se utiliza una función de hash para mapear claves a posiciones en una tabla, donde se almacenan los valores asociados. 

#### Colisiones
Cuando en una tabla de hash hay más claves que espacios, nos encontramos frente a una colisión. Esto puede significar que dos claves apuntan al mismo valor de la tabla de hash. En el caso de que haya una colisión, sigo recorriendo la tabla de hash hasta encontrar el próximo espacio libre. Para buscar ese espacio libre, existen diferentes métodos:
1. **Probing lineal:** se busca el siguiente espacio libre en la tabla de hash de forma secuencial (es decir, linealmente). Si la siguiente posición está ocupada, se continúa buscando linealmente hasta encontrar una posición vacía.
2. **Probing cuadrático:** se busca un espacio libre en la tabla de hash incrementando la posición de la siguiente manera: (cantidad de intentos fallidos)^2 , lo que quiere decir que se incrementa en potencia al cuadrado. 
3. **Hash doble:** se aplica una segunda función de hash a la clave, para encontrar una posición vacía en la tabla de hash.

En mi implememntación del TDA HASH, se emplea el Probing Lineal de la siguiente manera:

<div align="center">
<img width="70%" src="img/colisión_en_hash_cerrado.drawio.svg">
</div>
