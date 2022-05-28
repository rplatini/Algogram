#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "usuario.h"
#include "post.h"

//Estructuras
struct usuario {
    char* user;
    heap_t* feed;
    int pos_usuario;
};


typedef struct prioridad {
    post_t* post;
    int prioridad;
} prioridad_t;


// funciones internas del TDA usuario
prioridad_t* prioridad_crear(post_t* post, int afinidad){
    prioridad_t* prioridad = malloc(sizeof(prioridad_t));
    if(!prioridad) return NULL;

    prioridad->post = post;
    prioridad->prioridad = afinidad;
    return prioridad;
}

void prioridad_destruir(void* dato){
    prioridad_t* prioridad = (prioridad_t*)dato;
    free(prioridad);
}


// funcion de comparacion del feed de usuarios
int comparar_distancias(const void* a, const void* b){
    prioridad_t* prioridad_1 = (prioridad_t*)a;
    prioridad_t* prioridad_2 = (prioridad_t*)b;
   
    int afinidad = prioridad_2->prioridad - prioridad_1->prioridad;

    if (afinidad == 0) // el que tenga menor id ira primero en el heap
        return (post_id(prioridad_1->post) < post_id(prioridad_2->post)) ? 1 : -1; 

    return afinidad;
}


// primitivas del TDA usuario
usuario_t* usuario_crear(char* user, int pos_usuario){
    usuario_t *usuario = malloc(sizeof(usuario_t));
    if(!usuario) return NULL;

    usuario->user = strdup(user);
    if(!usuario->user) return NULL;

    usuario->feed = heap_crear(comparar_distancias);
    if(!usuario->feed) return NULL;

    usuario->pos_usuario = pos_usuario;
    return usuario;
}


char* usuario_nombre(usuario_t* usuario){
    return usuario->user;
}

// pre: usuario y creador_post estan inicializados
// post: retorna la afinidad entre los 2 usuarios pasados por parametro
int calcular_afinidad(usuario_t* usuario, usuario_t* creador_post){
    int afinidad = usuario->pos_usuario - creador_post->pos_usuario;
    if(afinidad < 0) afinidad = (-1)*afinidad;
    return afinidad;
}


bool usuario_guardar_post(usuario_t* usuario, post_t* post, usuario_t* creador_post){
    int afinidad = calcular_afinidad(usuario, creador_post);

    prioridad_t* prioridad = prioridad_crear(post, afinidad);
    if(!prioridad) return false;
    heap_encolar(usuario->feed, prioridad);
    return true;
}


bool feed_esta_vacio(usuario_t* usuario){
    return heap_esta_vacio(usuario->feed);
}


post_t* usuario_ver_proximo_post(usuario_t* usuario){
    prioridad_t* prioridad = heap_desencolar(usuario->feed);
    post_t* post = prioridad->post;
    prioridad_destruir(prioridad);
    return post;
}


void usuario_destruir(void* dato){
    usuario_t* usuario = (usuario_t*)dato;
    heap_destruir(usuario->feed, prioridad_destruir);
    free(usuario->user);
    free(usuario);
}

