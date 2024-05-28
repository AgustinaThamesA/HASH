#include "pa2m.h"
#include "src/hash_estructura_privada.h"

#include <string.h>

void crea_correctamente()
{
	hash_t *hash = hash_crear(0);

	pa2m_afirmar(
		hash_cantidad(hash) == 0,
		"Hash creado con capacidad 0 devuelve hash creado con cantidad 0.");
	pa2m_afirmar(
		hash_capacidad(hash) == 3,
		"Hash creado con capacidad 0 devuelve hash creado con capacidad 3.");

	hash_destruir(hash);

	hash_t *hash2 = hash_crear(10000000);

	pa2m_afirmar(
		hash_cantidad(hash2) == 0,
		"Hash creado con capacidad 10000000 devuelve hash creado con cantidad 0.");
	pa2m_afirmar(
		hash_capacidad(hash2) == 10000000,
		"Hash creado con capacidad 10000000 devuelve hash creado con capacidad 10000000.");

	hash_destruir(hash2);
}

void inserta_correctamente()
{
	hash_t *hash = hash_crear(10);

	pa2m_afirmar(
		hash_cantidad(hash) == 0,
		"Hash creado con capacidad 10 devuelve hash creado con cantidad 0.");
	pa2m_afirmar(
		hash_capacidad(hash) == 10,
		"Hash creado con capacidad 10 devuelve hash creado con capacidad 10.");

	hash_insertar(hash, "clave1", "valor1", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Inserto valor1 en hash, devuelve hash con cantidad 1.");

	hash_insertar(hash, "clave2", "valor2", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 2,
		     "Inserto valor2 en hash, devuelve hash con cantidad 2.");

	hash_insertar(hash, "clave3", "valor3", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Inserto valor3 en hash, devuelve hash con cantidad 3.");

	hash_insertar(hash, "clave4", "valor4", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 4,
		     "Inserto valor4 en hash, devuelve hash con cantidad 4.");

	hash_insertar(hash, "clave5", "valor5", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 5,
		     "Inserto valor5 en hash, devuelve hash con cantidad 5.");

	hash_insertar(hash, "clave6", "valor6", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 6,
		     "Inserto valor6 en hash, devuelve hash con cantidad 6.");

	hash_insertar(hash, "clave7", "valor7", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 7,
		     "Inserto valor7 en hash, devuelve hash con cantidad 7.");

	hash_insertar(hash, "clave8", "valor8", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 8,
		     "Inserto valor8 en hash, devuelve hash con cantidad 8.");

	hash_insertar(hash, "clave9", "valor9", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 9,
		     "Inserto valor9 en hash, devuelve hash con cantidad 9.");

	hash_insertar(hash, "clave10", "valor10", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 10,
		     "Inserto valor10 en hash, devuelve hash con cantidad 10.");

	hash_destruir(hash);
}

void busca_y_contiene_correctamente()
{
	hash_t *hash = hash_crear(3);

	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);

	pa2m_afirmar(strcmp(hash_obtener(hash, "clave1"), "valor1") == 0,
		     "Busca y encuentra valor1 en el hash.");
	pa2m_afirmar(strcmp(hash_obtener(hash, "clave2"), "valor2") == 0,
		     "Busca y encuentra valor2 en el hash.");
	pa2m_afirmar(strcmp(hash_obtener(hash, "clave3"), "valor3") == 0,
		     "Busca y encuentra valor3 en el hash.");

	pa2m_afirmar(hash_contiene(hash, "clave4") == false,
		     "Se fija si hash contiene clave4 y devuelve false.");
	pa2m_afirmar(hash_contiene(hash, "clave3") == true,
		     "Se fija si hash contiene clave3 y devuelve true.");

	hash_destruir(hash);
}

void quita_correctamente()
{
	hash_t *hash = hash_crear(3);

	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);

	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Devuelve hash con 3 claves insertadas.");
	pa2m_afirmar(strcmp(hash_quitar(hash, "clave3"), "valor3") == 0,
		     "Elimina correctamente clave3.");
	pa2m_afirmar(hash_quitar(hash, "clave4") == NULL,
		     "No puede eliminar una clave que no está en hash.");
	pa2m_afirmar(hash_cantidad(hash) == 2, "Devuelve hash con 2 claves.");

	hash_destruir(hash);
}

bool imprime_valor(const char *clave, void *valor, void *aux)
{
	if (!clave)
		return false;
	printf("El valor es: %s\n", (char *)valor);
	return true;
}

void hash_con_cada_clave_correctamente()
{
	hash_t *hash = hash_crear(3);

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

	pa2m_afirmar(cantidad_iteraciones == 10,
		     "Hay 10 iteraciones con un hash con 10 claves.");

	hash_destruir(hash);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== Pruebas creación ========================");
	crea_correctamente();

	pa2m_nuevo_grupo(
		"\n======================== Pruebas inserción ========================");
	inserta_correctamente();

	pa2m_nuevo_grupo(
		"\n======================== Pruebas búsqueda ========================");
	busca_y_contiene_correctamente();

	pa2m_nuevo_grupo(
		"\n======================== Pruebas eliminación ========================");
	quita_correctamente();

	pa2m_nuevo_grupo(
		"\n======================== Pruebas iterador interno ========================");
	hash_con_cada_clave_correctamente();

	return pa2m_mostrar_reporte();
}
