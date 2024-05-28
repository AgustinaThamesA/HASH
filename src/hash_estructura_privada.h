#ifndef __HASH_ESTRUCTURA_PRIVADA_H__
#define __HASH_ESTRUCTURA_PRIVADA_H__

#include "hash.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct tabla tabla_t;

/*
El origen de este tipo de función de dispersión es el algoritmo de hash conocido 
como djb2, que fue propuesto por Daniel J. Bernstein, un conocido científico de 
la computación y matemático. El djb2 es un algoritmo de hash simple pero efectivo,
usado ampliamente debido a su eficiencia y buena distribución de claves.
*/

size_t hash_function(const char *clave, size_t tamanio);

size_t hash_capacidad(hash_t *hash);

tabla_t *buscar_en_tabla(hash_t *hash, const char *clave);

void rehash(hash_t *hash);

#endif /* __HASH_ESTRUCTURA_PRIVADA_H__ */