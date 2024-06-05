#include "pa2m.h"
#include "src/hash_estructura_privada.h"

#include <string.h>
#include <stdlib.h>

void prueba_crear_hash_con_diferentes_capacidades()
{
	hash_t *hash = hash_crear(0);

	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Hash creado con capacidad 0 devuelve cantidad 0.");
	pa2m_afirmar(hash_capacidad(hash) == 3,
		     "Hash creado con capacidad 0 devuelve capacidad 3.");

	hash_destruir(hash);

	hash_t *hash2 = hash_crear(10000000);

	pa2m_afirmar(hash_cantidad(hash2) == 0,
		     "Hash creado con capacidad 10000000 devuelve cantidad 0.");
	pa2m_afirmar(
		hash_capacidad(hash2) == 10000000,
		"Hash creado con capacidad 10000000 devuelve capacidad 10000000.");

	hash_destruir(hash2);
}

void prueba_insertar_multiples_claves_y_valores()
{
	hash_t *hash = hash_crear(10);

	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Hash creado con capacidad 10 devuelve cantidad 0.");
	pa2m_afirmar(hash_capacidad(hash) == 10,
		     "Hash creado con capacidad 10 devuelve capacidad 10.");

	hash_insertar(hash, "clave1", "valor1", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Insertar clave1 y valor1 incrementa la cantidad a 1.");

	hash_insertar(hash, "clave2", "valor2", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 2,
		     "Insertar clave2 y valor2 incrementa la cantidad a 2.");

	hash_insertar(hash, "clave3", "valor3", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Insertar clave3 y valor3 incrementa la cantidad a 3.");

	hash_insertar(hash, "clave4", "valor4", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 4,
		     "Insertar clave4 y valor4 incrementa la cantidad a 4.");

	hash_insertar(hash, "clave5", "valor5", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 5,
		     "Insertar clave5 y valor5 incrementa la cantidad a 5.");

	hash_insertar(hash, "clave6", "valor6", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 6,
		     "Insertar clave6 y valor6 incrementa la cantidad a 6.");

	hash_insertar(hash, "clave7", "valor7", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 7,
		     "Insertar clave7 y valor7 incrementa la cantidad a 7.");

	hash_insertar(hash, "clave8", "valor8", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 8,
		     "Insertar clave8 y valor8 incrementa la cantidad a 8.");

	hash_insertar(hash, "clave9", "valor9", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 9,
		     "Insertar clave9 y valor9 incrementa la cantidad a 9.");

	hash_insertar(hash, "clave10", "valor10", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 10,
		     "Insertar clave10 y valor10 incrementa la cantidad a 10.");

	pa2m_afirmar(strcmp(hash_obtener(hash, "clave10"), "valor10") == 0,
		     "Buscar clave10 devuelve valor10.");

	hash_insertar(hash, "clave10", "valor10bis", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 10,
		     "Modifico clave10 a valor10bis y cantidad queda en 10.");

	pa2m_afirmar(strcmp(hash_obtener(hash, "clave10"), "valor10bis") == 0,
		     "Buscar clave10 devuelve valor10bis.");

	pa2m_afirmar(strcmp(hash_obtener(hash, "clave10"), "valor10") != 0,
		     "Buscar clave10 no devuelve valor10.");

	hash_destruir(hash);
}

void prueba_buscar_valores_y_verificar_existencia_de_claves()
{
	hash_t *hash = hash_crear(3);

	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);

	pa2m_afirmar(strcmp(hash_obtener(hash, "clave1"), "valor1") == 0,
		     "Buscar clave1 devuelve valor1.");
	pa2m_afirmar(strcmp(hash_obtener(hash, "clave2"), "valor2") == 0,
		     "Buscar clave2 devuelve valor2.");
	pa2m_afirmar(strcmp(hash_obtener(hash, "clave3"), "valor3") == 0,
		     "Buscar clave3 devuelve valor3.");

	pa2m_afirmar(hash_contiene(hash, "clave4") == false,
		     "Verificar si contiene clave4 devuelve false.");
	pa2m_afirmar(hash_contiene(hash, "clave3") == true,
		     "Verificar si contiene clave3 devuelve true.");

	hash_destruir(hash);
}

void prueba_quitar_claves_y_verificar_eliminacion()
{
	hash_t *hash = hash_crear(3);

	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);

	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Hash con 3 claves insertadas devuelve cantidad 3.");
	pa2m_afirmar(strcmp(hash_quitar(hash, "clave3"), "valor3") == 0,
		     "Quitar clave3 devuelve valor3.");
	pa2m_afirmar(hash_quitar(hash, "clave4") == NULL,
		     "No se puede quitar una clave que no está en hash.");
	pa2m_afirmar(hash_cantidad(hash) == 2,
		     "Quitar clave3 reduce la cantidad a 2.");

	pa2m_afirmar(strcmp(hash_quitar(hash, "clave2"), "valor2") == 0,
		     "Quitar clave2 devuelve valor2.");
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Quitar clave2 reduce la cantidad a 1.");

	pa2m_afirmar(strcmp(hash_quitar(hash, "clave1"), "valor1") == 0,
		     "Quitar clave1 devuelve valor1.");
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Quitar clave1 reduce la cantidad a 0.");

	hash_destruir(hash);
}

bool imprime_valor(const char *clave, void *valor, void *aux)
{
	if (!clave)
		return false;
	printf("El valor es: %s\n", (char *)valor);
	return true;
}

void prueba_iterador_interno_recorre_todas_las_claves()
{
	hash_t *hash = hash_crear(10);

	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);
	hash_insertar(hash, "clave4", "valor4", NULL);
	hash_insertar(hash, "clave5", "valor5", NULL);
	hash_insertar(hash, "clave6", "valor6", NULL);
	hash_insertar(hash, "clave7", "valor7", NULL);
	hash_insertar(hash, "clave8", "valor8", NULL);
	hash_insertar(hash, "clave9", "valor9", NULL);
	hash_insertar(hash, "clave10", "valor10", NULL);

	printf("\n");

	size_t cantidad_iteraciones =
		hash_con_cada_clave(hash, imprime_valor, NULL);

	pa2m_afirmar(
		cantidad_iteraciones == 10,
		"Iterador interno realiza 10 iteraciones en hash con 10 claves.");
	pa2m_afirmar(hash_cantidad(hash) == 10, "Hash con 10 claves.");

	hash_destruir(hash);
}

void destruir_string(void *elemento)
{
	if (!elemento)
		return;

	printf("(Destructor) Libero el vehiculo: %s\n", (char *)elemento);
	free(elemento);
}

void prueba_destruir_todo_con_funcion_de_destruccion()
{
	hash_t *hash = hash_crear(3);

	char** valor1 = malloc(sizeof(char*));
	char** valor2 = malloc(sizeof(char*));
	char** valor3 = malloc(sizeof(char*));

	*valor1 = "valor1";
	*valor2 = "valor2";
	*valor3 = "valor3";

	hash_insertar(hash, "clave1", *valor1, NULL);
	hash_insertar(hash, "clave2", *valor2, NULL);
	hash_insertar(hash, "clave3", *valor3, NULL);

	printf("\n");

	pa2m_afirmar(hash_cantidad(hash) == 3, "Hash de 3 valores en el heap.");

	hash_destruir_todo(hash, destruir_string);
}

void prueba_insertar_clave_NULL()
{
	hash_t *hash = hash_crear(10);

	hash_insertar(hash, NULL, "valor1", NULL);

	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "No se puede insertar una clave NULL");

	hash_destruir(hash);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n=========== Pruebas de creación de hash ===========");
	prueba_crear_hash_con_diferentes_capacidades();

	pa2m_nuevo_grupo(
		"\n=========== Pruebas de inserción de claves y valores ===========");
	prueba_insertar_multiples_claves_y_valores();

	pa2m_nuevo_grupo(
		"\n=========== Pruebas de búsqueda de valores y existencia de claves ===========");
	prueba_buscar_valores_y_verificar_existencia_de_claves();

	pa2m_nuevo_grupo(
		"\n=========== Pruebas de eliminación de claves ===========");
	prueba_quitar_claves_y_verificar_eliminacion();

	pa2m_nuevo_grupo(
		"\n=========== Pruebas del iterador interno ===========");
	prueba_iterador_interno_recorre_todas_las_claves();

	pa2m_nuevo_grupo(
		"\n=========== Pruebas de destrucción completa ===========");
	prueba_destruir_todo_con_funcion_de_destruccion();

	pa2m_nuevo_grupo("\n=========== Pruebas de NULLs ===========");
	prueba_insertar_clave_NULL();

	return pa2m_mostrar_reporte();
}
