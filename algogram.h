#ifndef ALGOGRAM_H
#define ALGOGRAM_H
#include <stdbool.h>
#include "hash.h"
#include "heap.h"
#include "lista.h"
#include "post.h"

//ESTRUCTURA
struct algogram;
typedef struct algogram algogram_t;

// pre: el archivo de usuarios esta abierto
// post: crea y devuelve una estructura algogram
algogram_t* algogram_crear(FILE* usuarios);

// post: destruye una estructura algogram
void algogram_destruir(algogram_t* algogram);


// PRIMITIVAS DE ALGOGRAM

bool algogram_existe_usuario(algogram_t* algogram, char* user);


// pre: user esta inicializado, algogram creado
// post: loggea un usuario. En caso de que no haya usuario loggeado, imprime un error
bool log_in(char* user, algogram_t* algogram);

// pre: hay un usuario loggeado
// post: desloggea un usuario. En caso de que no haya usuario loggeado, imprime un error
bool log_out(algogram_t* algogram);

// pre: hay un usuario loggeado
// post: - se imprime el post que publico el usuario loggeado
bool publicar_post(algogram_t* algogram, char* posteo);

// pre: hay un usuario loggeado
// post: se imprime el siguiente post del feed del usuario actualmente loggeado
post_t* ver_proximo_post(algogram_t* algogram);

// pre: el post esta inicializado
// post: imprime los datos de un post
void imprimir_post(post_t* post);

// pre: hay un usuario loggeado, id esta inicializado
// post: se likea el post cuyo id es el pasado por parametro
bool likear_post(algogram_t* algogram, char* id);

// pre: el id esta inicializado
// post: retorna un abb que guarda el nombre de los usuarios que le dieron like al post cuyo id es el pasado por parametro
lista_t* mostrar_likes(algogram_t* algogram, char* id);

// pre: -
// post: imprime el nombre de los usuarios que likearon un post
void imprimir_likes(lista_t* likes);


#endif // ALGOGRAM_H