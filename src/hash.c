#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "hash_estructura_privada.h"

#define FACTOR_CARGA_MAXIMA 0.7
#define CAPACIDAD_MINIMA 3

struct componente_tabla {
	const char *clave;
	void *valor;
	bool ocupado;
};

struct hash {
	size_t capacidad;
	size_t cantidad;
	componente_tabla_t **componente_tabla;
};

// ================== FUNCIONES AUXILIARES ====================

size_t hash_function(const char *clave)
{
	size_t hash = 0;
	while (*clave) {
		hash = ((hash << 5) + (unsigned char)(*clave));
		clave++;
	}
	return hash;
}

size_t hash_capacidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;
	return hash->capacidad;
}

componente_tabla_t *buscar_en_tabla(hash_t *hash, const char *clave)
{
	size_t posicion = hash_function(clave) % hash->capacidad;
	size_t inicio = posicion;

	while (true) {
		componente_tabla_t *actual = hash->componente_tabla[posicion];
		if (actual != NULL) {
			if (actual->ocupado &&
			    strcmp(actual->clave, clave) == 0)
				return actual;
			posicion = (posicion + 1) % hash->capacidad;
		} else {
			return NULL;
		}

		if (posicion == inicio)
			return NULL;
	}

	return NULL;
}

void rehash(hash_t *hash)
{
	size_t nueva_capacidad = hash->capacidad * 2;
	componente_tabla_t **nueva_componente_tabla =
		calloc(nueva_capacidad, sizeof(componente_tabla_t *));

	for (size_t i = 0; i < hash->capacidad; i++) {
		componente_tabla_t *componente_tabla =
			hash->componente_tabla[i];
		if (componente_tabla) {
			size_t posicion =
				hash_function(componente_tabla->clave) %
				nueva_capacidad;
			while (nueva_componente_tabla[posicion] != NULL) {
				posicion = (posicion + 1) % nueva_capacidad;
			}
			nueva_componente_tabla[posicion] = componente_tabla;
		}
	}

	free(hash->componente_tabla);
	hash->componente_tabla = nueva_componente_tabla;
	hash->capacidad = nueva_capacidad;
}

// ================== FUNCIONES ORIGINALES ====================

hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash) {
		return NULL;
	}

	size_t cap = capacidad;
	if (cap < 3) {
		cap = CAPACIDAD_MINIMA;
	}

	hash->capacidad = cap;
	hash->cantidad = 0;

	hash->componente_tabla =
		calloc(hash->capacidad, sizeof(componente_tabla_t *));
	if (!hash->componente_tabla) {
		free(hash);
		return NULL;
	}

	return hash;
}

char *strdup2(const char *s)
{
	char *s2 = malloc(strlen(s) + 1);
	strcpy(s2, s);
	return s2;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (hash == NULL || hash->componente_tabla == NULL || clave == NULL) {
		return NULL;
	}

	componente_tabla_t *componente_tabla = buscar_en_tabla(hash, clave);

	if (componente_tabla) {
		if (anterior != NULL) {
			*anterior = componente_tabla->valor;
		}
		componente_tabla->valor = elemento;
		return hash;
	}

	float factor_carga = (float)hash->cantidad / (float)hash->capacidad;

	if (factor_carga > FACTOR_CARGA_MAXIMA) {
		rehash(hash);
	}

	size_t posicion = hash_function(clave) % hash->capacidad;

	while (hash->componente_tabla[posicion] != NULL &&
	       hash->componente_tabla[posicion]->ocupado) {
		posicion = (posicion + 1) % hash->capacidad;
	}

	componente_tabla_t *nueva_componente_tabla =
		hash->componente_tabla[posicion];
	if (nueva_componente_tabla == NULL)
		nueva_componente_tabla = calloc(1, sizeof(componente_tabla_t));

	if (!nueva_componente_tabla) {
		return NULL;
	}

	nueva_componente_tabla->clave = strdup2(clave);
	nueva_componente_tabla->valor = elemento;
	nueva_componente_tabla->ocupado = true;
	hash->componente_tabla[posicion] = nueva_componente_tabla;
	hash->cantidad++;

	if (anterior != NULL) {
		*anterior = NULL;
	}
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (hash == NULL || hash->componente_tabla == NULL || clave == NULL)
		return NULL;

	size_t posicion = hash_function(clave) % hash->capacidad;
	size_t inicio = posicion;

	while (true) {
		componente_tabla_t *actual = hash->componente_tabla[posicion];
		if (actual != NULL) {
			if (actual->ocupado &&
			    strcmp(actual->clave, clave) == 0) {
				void *valor_anterior = actual->valor;
				free((char *)actual->clave);
				actual->clave = NULL;
				actual->ocupado = false;
				actual->valor = NULL;
				hash->cantidad--;
				return valor_anterior;
			}
			posicion = (posicion + 1) % hash->capacidad;
		} else {
			return NULL;
		}

		if (posicion == inicio)
			return NULL;
	}

	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (hash == NULL || hash->componente_tabla == NULL || clave == NULL)
		return NULL;
	componente_tabla_t *componente_tabla = buscar_en_tabla(hash, clave);
	if (componente_tabla)
		return componente_tabla->valor;
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (hash == NULL || hash->componente_tabla == NULL || clave == NULL)
		return false;
	componente_tabla_t *componente_tabla = buscar_en_tabla(hash, clave);
	if (componente_tabla)
		return true;
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (hash == NULL)
		return;
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		componente_tabla_t *componente_tabla =
			hash->componente_tabla[i];
		if (componente_tabla != NULL) {
			if (destructor != NULL) {
				destructor(componente_tabla->valor);
			}
			if (componente_tabla->clave)
				free((char *)componente_tabla->clave);
			free(componente_tabla);
		}
	}
	free(hash->componente_tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	if (hash == NULL || hash->componente_tabla == NULL || f == NULL)
		return 0;

	size_t cantidad = 0;
	for (size_t i = 0; i < hash->capacidad; i++) {
		componente_tabla_t *componente_tabla =
			hash->componente_tabla[i];
		if (componente_tabla != NULL && componente_tabla->ocupado) {
			cantidad++;
			if (!f(componente_tabla->clave, componente_tabla->valor,
			       aux)) {
				return cantidad;
			}
		}
	}

	return cantidad;
}
