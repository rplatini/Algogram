#ifndef USUARIO_H
#define USUARIO_H
#include <stdbool.h>
#include "post.h"

//Estructura
typedef struct usuario usuario_t;

// Primitivas de la estructura usuario

// Crea un usuario nuevo. Recibe un nombre de usuario y una posicion
usuario_t* usuario_crear(char* user, int pos_usuario);

// Pre: el usuario fue creado 
// Post: devuelve el nombre del usuario
char* usuario_nombre(usuario_t* usuario);


// pre: los parametros estan inicializados
// post: - guarda un post en el feed del usuario
//       - retorna true si se pudo guardar el post, false en caso contrario   
bool usuario_guardar_post(usuario_t* usuario, post_t* post, usuario_t* creador_post);

// pre: el usuario esta inicializado
// post: retorna true si el feed del usuario esta vacio, false en caso contrario
bool feed_esta_vacio(usuario_t* usuario);

// pre: usuario esta inicializado
// post: retorna el proximo post por ver del feed del usuario
post_t* usuario_ver_proximo_post(usuario_t* usuario);

// Pre: el usuario fue creado
// Post: Destruye el usuario. 
void usuario_destruir(void* dato);

#endif // USUARIO_H