#ifndef _LISTA_H
#define _LISTA_H
#include <stdbool.h>
#include <stdio.h>

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

// PRIMITIVAS DE LA PILA


// PRE: -
// POST: devuelve una lista vacia
lista_t *lista_crear(void);

/* PRE: la lista fue creada
   POST: devuelve true si la lista esta vacia, false en caso contrario
 */
bool lista_esta_vacia(const lista_t *lista);

/* PRE: la lista fue creada
   POST: - inserta un elemento al principio de la lista 
         - devuelve true si el elemento se pudo insertar, false en caso contrario
 */
bool lista_insertar_primero(lista_t *lista, void *dato);

/* PRE: la lista fue creada
   POST: - inserta un elemento al final de la lista 
         - devuelve true si el elemento se pudo insertar, false en caso contrario
 */
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/* PRE: la lista fue creada
   POST: - elimina el primer elemento de la lista 
         - devuelve el valor del elemento borrado o NULL si no pudo borrarse
 */
void *lista_borrar_primero(lista_t *lista);

/* PRE: la lista fue creada
   POST: - devuelve el valor del primer elemento de la lista, o NULL si la lista esta vacia
 */
void *lista_ver_primero(const lista_t *lista);

/* PRE: la lista fue creada
   POST: - devuelve el valor del ultimo elemento de la lista, o NULL si la lista esta vacia
 */
void *lista_ver_ultimo(const lista_t* lista);

/* PRE: la lista fue creada
   POST: - devuelve el largo de la lista
 */
size_t lista_largo(const lista_t *lista);

/* POST: destruye la lista
 */
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));


// PRIMITIVA DEL ITERADOR INTERNO


/* PRE: la lista fue creada
   POST: - recorre la lista hasta el final
         - si la funcion visitar no es NULL, se aplica a cada elemento de la lista
 */
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


// PRIMITIVAS DEL ITERADOR EXTERNO

/* PRE: la lista fue creada
   POST: crea un iterador de la lista
 */
lista_iter_t *lista_iter_crear(lista_t *lista);

/* PRE: la lista y el iterador fueron creados
   POST: - el iterador avanza una posicion en la lista. 
         - devuelve true si el iterador pudo avanzar, false en caso contrario
 */
bool lista_iter_avanzar(lista_iter_t *iter);

/* PRE: la lista y el iterador fueron creados
   POST: - devuelve el valor del elemento en la posicion actual del iterador 
 */
void *lista_iter_ver_actual(const lista_iter_t *iter);

/* PRE: la lista y el iterador fueron creados
   POST: devuelve true si el iterador esta al final de la lista 
 */
bool lista_iter_al_final(const lista_iter_t *iter);

/* PRE: 
   POST: destruye el iterador 
 */
void lista_iter_destruir(lista_iter_t *iter);

/* PRE: la lista y el iterador fueron creados
   POST: - inserta un elemento en la posicion actual del iterador 
         - devuelve true si el elemento se inserto, false en caso contrario
 */
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/* PRE: la lista y el iterador fueron creados
   POST: - elimina un elemento en la posicion actual del iterador 
         - devuelve true si el elemento se borro, false en caso contrario
 */
void *lista_iter_borrar(lista_iter_t *iter);

// pruebas estudiantes
void pruebas_lista_estudiante();




















#endif // _LISTA_H