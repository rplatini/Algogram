#ifndef POST_H
#define POST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Estructura post
typedef struct post post_t;

// primitivas de la estructura post

// pre: los parametros estan inicializados
// post: crea un post y lo devuelve
post_t* post_crear(int id, char* user, char* posteo);

// pre: el post fue creado. 
// post: devuelve el id del post
int post_id(post_t* post);

// pre: el post fue creado.
// post: devuelve el usuario que creo el post. 
char* post_nombre_usuario(post_t* post);

// Pre: el post fue creado. 
// Post: devuelve la publicacion hecha por el usuario. 
char* post_posteo(post_t* post); 

// pre: el post fue creado
// post: retorna la cantidad de likes que tiene el post
int post_cant_likes(post_t* post);

// pre: post y user estan inicializados
// post: devuelve true si el usuario pasado por parametro ya likeo el post, false en caso contrario
bool post_esta_likeado(post_t* post, char* user);

// pre: los parametros estan inicializados
// post: agrega a los likes del post el like del usuario pasado por parametro
void post_actualizar_likes(post_t* post, char* user);

// pre: el post fue creado
// post: devuelve una lista con los nombres de los usuarios que le dieron like al post
lista_t* post_obtener_likes(post_t* post);

// Pre: el post fue creado. 
// Post: destruye el post. 
void destruir_post(void* dato);

#endif // POST_H