#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "abb.h"
#include "post.h"

// Estructura post
struct post {
    int id;
    char* user;
    char* posteo;
    abb_t* like_users; // abb de usuarios que le dieron like
};

// primitivas de post

post_t* post_crear(int id, char* user, char *posteo){
    post_t* post = malloc(sizeof(post_t));
    if (!post) return NULL;

    post->id = id;
    post->user = strdup(user);
    if(!user) return NULL;
    post->posteo = strdup(posteo);
    if(!post->posteo) return NULL;

    post->like_users = abb_crear(strcmp, NULL);
    if (!post->like_users) return NULL;

    return post;
}


int post_id(post_t*post){
    return post->id; 
}


char* post_posteo(post_t* post){
    return post->posteo; 
}


char* post_nombre_usuario(post_t* post){
    return post->user;
}

int post_cant_likes(post_t* post){
    return (int)abb_cantidad(post->like_users);
}

lista_t* post_obtener_likes(post_t* post){
    if(abb_cantidad(post->like_users) == 0){
        return NULL;
    }

    lista_t* lista = lista_crear();
    abb_iter_t* iter = abb_iter_in_crear(post->like_users);
    if(!iter) return NULL;
    
    while(!abb_iter_in_al_final(iter)){
        lista_insertar_ultimo(lista, (char*)abb_iter_in_ver_actual(iter));
        abb_iter_in_avanzar(iter);
    }
    abb_iter_in_destruir(iter);
    return lista;
}


bool post_esta_likeado(post_t* post, char* user){
    return abb_pertenece(post->like_users, user);
}


void post_actualizar_likes(post_t* post, char* user){
    abb_guardar(post->like_users, user, (void*)true);
}


void destruir_post(void* dato){
    post_t* post = (post_t*)dato;
    abb_destruir(post->like_users);
    free(post->user);
    free(post->posteo);
    free(post);
}