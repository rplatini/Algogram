#ifndef ABB_H
#define ABB_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

// PRIMITIVAS DEL ABB

// Pre: Se reciben dos funciones: una de comparacion de claves y otra para destruir datos. 
// Post: devuelve un abb vacio.
abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Pre: El abb fue creado.
// Post: devuelve true si pudo guardar, false si no
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si la clave no esta.
void *abb_borrar(abb_t *arbol, const char *clave);

// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si la clave no pertenece al ABB.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Pre: El abb fue creado.
// Post: Devuelve true o false si la clave esta o no en el abb. 
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Pre: El abb fue creado.
// Post: devuelve la cantidad de claves almacenadas en el abb.
size_t abb_cantidad(const abb_t *arbol);

// Pre: El abb fue creado.
// Post: Se destruye el abb y sus datos, y se libera la memoria.
void abb_destruir(abb_t *arbol);

// PRIMITIVAS DEL ITERADOR INTERNO

// Pre: el abb fue creada
// Post: - recorre el abb hasta el final de forma in order.
//       - si la funcion visitar no es NULL, se aplica a cada elemento de la lista
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

// PRIMITIVAS DEL ITERADOR EXTERNO

// Pre: El abb fue creado.
// Post: Devuelve una estructura que itera de forma in order sobre el abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Pre: El abb y el iterador fueron creados.
// Post: Devuelve true si se puede avanzar o false en caso contrario.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Pre: El abb y el iterador fueron creados.
// Post: Devuelve la clave actual en la que se encuentra el iterador. 
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Pre: El abb y el iterador fueron creados.
// Post: Devuele true si el iterador esta al final, caso contrario devuelve false.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Pre: El abb y el iterador fueron creados.
// Post: Destruye el iterador.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H

