#ifndef __HASH_ESTRUCTURA_PRIVADA_H__
#define __HASH_ESTRUCTURA_PRIVADA_H__

#include "hash.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct componente_tabla componente_tabla_t;

/*
 * La función hash_function fue obtenida de Internet, y la acompañaba esta definición:
 * El origen de este tipo de función de dispersión es el algoritmo de hash conocido 
 * como djb2, que fue propuesto por Daniel J. Bernstein, un conocido científico de 
 * la computación y matemático. El djb2 es un algoritmo de hash simple pero efectivo,
 * usado ampliamente debido a su eficiencia y buena distribución de claves.

 * Función de hash que calcula el índice de una clave en la tabla de hash.
 *
 * Devuelve el índice calculado, o 0 en caso de errores
*/
size_t hash_function(const char *clave, size_t tamanio);

/*
 * Devuelve la capacidad actual de la tabla de hash, o 0 en caso de errores
 */
size_t hash_capacidad(hash_t *hash);

/*
 * Busca una clave en la tabla de hash y devuelve el elemento asociado.
 *
 * Devuelve un puntero al elemento asociado a la clave si se encuentra,
 * o NULL en caso de errores
 */
componente_tabla_t *buscar_en_tabla(hash_t *hash, const char *clave);

/*
 * Rehashea (redimensiona) la tabla de hash y redistribuye los elementos.
 */
void rehash(hash_t *hash);

#endif /* __HASH_ESTRUCTURA_PRIVADA_H__ */