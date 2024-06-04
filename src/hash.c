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
	size_t tamaño;
	componente_tabla_t **componente_tabla;
};

// ================== FUNCIONES AUXILIARES ====================

size_t hash_function(const char *clave, size_t tamaño)
{
	size_t hash = 0;
	while (*clave) {
		hash = ((hash << 5) +
			(unsigned char)(*clave)); // Desplazamiento de bits a la izquierda y suma del valor ASCII del carácter
		clave++;
	}
	return hash %
	       tamaño; // Tomamos el módulo con el tamaño de la componente_tabla para obtener un índice válido
}

size_t funcion_de_hash(hash_t *hash, const char *clave){
	size_t valor = 0;
	size_t largo_clave = strlen(clave);
	for (size_t i = 0; i < largo_clave; i++){
		valor = (valor*100)+(valor/7)+(size_t)clave[i];
	}

	return valor % hash->capacidad;
}

size_t hash_capacidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;
	return hash->capacidad;
}

componente_tabla_t *buscar_en_tabla(hash_t *hash, const char *clave)
{
	size_t posicion = funcion_de_hash(hash, clave);
	size_t inicio = posicion;

	while (true) {
		componente_tabla_t *actual = hash->componente_tabla[posicion];
		if (actual != NULL) {
			if (strcmp(actual->clave, clave) == 0)
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
			size_t posicion = funcion_de_hash(hash, componente_tabla->clave) %
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


void rehash2(hash_t *hash){
	size_t nueva_capacidad = hash->capacidad * 2;
	componente_tabla_t **nueva_componente_tabla = calloc(nueva_capacidad, sizeof(componente_tabla_t*));
	for (size_t i = 0; i < nueva_capacidad; i++){
		nueva_componente_tabla[i] = NULL;
	}
	for (size_t i = 0; i < hash->capacidad; i++){
		componente_tabla_t *componente_tabla = hash->componente_tabla[i];
		if (componente_tabla){
			size_t posicion = funcion_de_hash(hash, componente_tabla->clave) % nueva_capacidad;
			while (nueva_componente_tabla[posicion] != NULL){
				posicion = (posicion+1)%nueva_capacidad;
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
	if (!hash)
		return NULL;

	size_t cap = capacidad;
	if (cap < 3)
		cap = CAPACIDAD_MINIMA;

	hash->capacidad = cap;
	hash->tamaño = 0;

	hash->componente_tabla =
		calloc(hash->capacidad, sizeof(componente_tabla_t *));
	if (!hash->componente_tabla) {
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (hash == NULL || hash->componente_tabla == NULL || clave == NULL)
		return NULL;

	componente_tabla_t *componente_tabla = buscar_en_tabla(hash, clave);

	if (componente_tabla) {
		if (anterior != NULL)
			*anterior = componente_tabla->valor;
		componente_tabla->valor = elemento;
		return hash;
	}

	float factor_carga = (float)hash->tamaño / (float)hash->capacidad;

	if (factor_carga > FACTOR_CARGA_MAXIMA)
		rehash2(hash);

	size_t posicion =
		funcion_de_hash(hash, clave) % hash->capacidad;

	while (hash->componente_tabla[posicion] != NULL)
		posicion = (posicion + 1) % hash->capacidad;

	componente_tabla_t *nueva_componente_tabla =
		calloc(1, sizeof(componente_tabla_t));
	nueva_componente_tabla->clave = clave;
	nueva_componente_tabla->valor = elemento;
	nueva_componente_tabla->ocupado = true;
	hash->componente_tabla[posicion] = nueva_componente_tabla;
	hash->tamaño++;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (hash == NULL || hash->componente_tabla == NULL || clave == NULL)
		return NULL;

	size_t posicion = funcion_de_hash(hash, clave);
	size_t inicio = posicion;

	while (true) {
		componente_tabla_t *actual = hash->componente_tabla[posicion];
		if (actual != NULL) {
			if (strcmp(actual->clave, clave) == 0) {
				void *valor_anterior = actual->valor;
				free(actual);
				hash->componente_tabla[posicion] = NULL;
				hash->tamaño--;
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
	size_t posicion = funcion_de_hash(hash, clave);
	while (hash->componente_tabla[posicion] != NULL){
		if (strcmp(hash->componente_tabla[posicion]->clave, clave) == 0)
			return hash->componente_tabla[posicion]->valor;
		posicion = (posicion+1)%hash->capacidad;
	}
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
	return hash->tamaño;
}

void hash_destruir(hash_t *hash)
{
	if (hash == NULL)
		return;
	for (size_t i = 0; i < hash->capacidad; i++) {
		componente_tabla_t *componente_tabla =
			hash->componente_tabla[i];
		if (componente_tabla != NULL)
			free(componente_tabla);
	}
	free(hash->componente_tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL) {
		return; // No hacer nada si hash es nulo
	}
	if (destructor == NULL) {
		return; // No hacer nada si destructor es nulo
	}
	for (size_t i = 0; i < hash->capacidad; i++) {
		componente_tabla_t *componente_tabla =
			hash->componente_tabla[i];
		if (componente_tabla != NULL) {
			destructor(
				componente_tabla
					->valor); // Llamar a destructor para cada elemento
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

	size_t tamaño = 0;
	bool detener = false;
	for (size_t i = 0; i < hash->capacidad && !detener; i++) {
		componente_tabla_t *componente_tabla =
			hash->componente_tabla[i];
		if (componente_tabla != NULL && componente_tabla->ocupado) {
			if (f(componente_tabla->clave, componente_tabla->valor,
			      aux)) {
				tamaño++;
			} else {
				detener = true;
			}
		}
	}

	return tamaño;
}