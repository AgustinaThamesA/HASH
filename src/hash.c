#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#include "hash.h"
#include "hash_estructura_privada.h"

#define FACTOR_CARGA_MAXIMA 0.7
#define CAPACIDAD_MINIMA 3

struct tabla {
	const char* clave;
	void* valor;
	bool ocupado;
};

struct hash {
	size_t capacidad;
	size_t cantidad;
	tabla_t** tabla;
};

// ================== FUNCIONES AUXILIARES ====================

size_t hash_function(const char *clave, size_t tamanio) {
	size_t hash = 0;
	while (*clave) {
		hash = ((hash << 5) + (unsigned char)(*clave)); // Desplazamiento de bits a la izquierda y suma del valor ASCII del carácter
		clave++;
	}
	return hash % tamanio; // Tomamos el módulo con el tamaño de la tabla para obtener un índice válido
}

size_t hash_capacidad(hash_t *hash)
{
	if (hash == NULL) return 0;
	return hash->capacidad;
}

tabla_t* buscar_en_tabla(hash_t* hash, const char* clave){
    size_t posicion = hash_function(clave, hash->capacidad);
    size_t inicio = posicion;

    while (true) {
        tabla_t* actual = hash->tabla[posicion];
        if (actual!= NULL) {
            if (strcmp(actual->clave, clave) == 0)
                return actual;
            posicion = (posicion + 1) % hash->capacidad;
        } else {
            return NULL;
        }

        if (posicion == inicio) {
            // We've looped around the entire table and didn't find the key
            return NULL;
        }
    }

    return NULL;
}

void rehash(hash_t *hash) {
    size_t nueva_capacidad = hash->capacidad * 2;
    tabla_t **nueva_tabla = calloc(nueva_capacidad, sizeof(tabla_t*));

    for (size_t i = 0; i < hash->capacidad; i++) {
        tabla_t *tabla = hash->tabla[i];
        if (tabla) {
            size_t posicion = hash_function(tabla->clave, nueva_capacidad) % nueva_capacidad;
            while (nueva_tabla[posicion] != NULL) {
                posicion = (posicion + 1) % nueva_capacidad;
            }
            nueva_tabla[posicion] = tabla;
        }
    }

    free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->capacidad = nueva_capacidad;
}

// ================== FUNCIONES ORIGINALES ====================

hash_t *hash_crear(size_t capacidad)
{
	hash_t* hash = calloc(1, sizeof(hash_t));
	if (!hash) return NULL;

	size_t cap = capacidad;
	if (cap < 3)
		cap = CAPACIDAD_MINIMA;
	
	hash->capacidad = cap;
	hash->cantidad = 0;

	hash->tabla = calloc(hash->capacidad, sizeof(tabla_t*));
	if (!hash->tabla)
	{
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void **anterior) {
	if (hash == NULL || hash->tabla == NULL) return NULL;
	tabla_t* tabla = buscar_en_tabla(hash, clave);

	if (tabla){
		tabla->valor = elemento;
		return hash;
	}

	float factor_carga = (float)hash->cantidad / (float)hash->capacidad;
	
	if (factor_carga > FACTOR_CARGA_MAXIMA)
		rehash(hash);

	size_t posicion = hash_function(clave, hash->capacidad) % hash->capacidad;

	while (hash->tabla[posicion] != NULL) 
		posicion = (posicion + 1) % hash->capacidad;

	tabla_t* nueva_tabla = calloc(1, sizeof(tabla_t));
	nueva_tabla->clave = clave;
	nueva_tabla->valor = elemento;
	nueva_tabla->ocupado = true;
	hash->tabla[posicion] = nueva_tabla;
	hash->cantidad++;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
    if (hash == NULL || hash->tabla == NULL) return NULL;

    size_t posicion = hash_function(clave, hash->capacidad);
    size_t inicio = posicion;

    while (true) {
        tabla_t* actual = hash->tabla[posicion];
        if (actual != NULL) {
            if (strcmp(actual->clave, clave) == 0) {
                void* valor_anterior = actual->valor;
                free(actual); // Liberar la memoria de la tabla
                hash->tabla[posicion] = NULL; // Marcar la posición como vacía
                hash->cantidad--; // Decrementar la cantidad de entradas
                return valor_anterior;
            }
            posicion = (posicion + 1) % hash->capacidad;
        } else {
            return NULL;
        }

        if (posicion == inicio) {
            // We've looped around the entire table and didn't find the key
            return NULL;
        }
    }

    return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (hash == NULL || hash->tabla == NULL) return NULL;
	tabla_t* tabla = buscar_en_tabla(hash, clave);
	if (tabla) return tabla->valor;
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (hash == NULL || hash->tabla == NULL) return false;
	tabla_t* tabla = buscar_en_tabla(hash, clave);
	if (tabla) return true;
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash == NULL) return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
	if (hash == NULL) return;
	for (size_t i = 0; i < hash->capacidad; i++) {
		tabla_t* tabla = hash->tabla[i];
		if (tabla != NULL) free(tabla);
	}
	free(hash->tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *)) {
	if (hash == NULL) return;
	for (size_t i = 0; i < hash->capacidad; i++) {
		tabla_t* tabla = hash->tabla[i];
		if (tabla != NULL) {
			destructor(tabla->valor);
			free(tabla);
		}
	}
	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	return 0;
}
