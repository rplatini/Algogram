#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "abb.h"
#include "lista.h"
#include "pila.h"
#include "algogram.h"
#include "usuario.h"
#include "post.h"


struct algogram {
    hash_t* usuarios;
    usuario_t* usuario_loggeado;
    hash_t* posts;
    int id_post;
};


// pre: recibe un archivo de usuarios abierto
// post: guarda los usuarios del archivo en un hash
hash_t* cargar_usuarios(FILE* usuarios){
    hash_t* hash_usuarios = hash_crear(usuario_destruir);
    if(!hash_usuarios) return NULL;

    char* linea = NULL;
    size_t tam = 0;
    int pos_usuario = 0;

    while(getline(&linea, &tam, usuarios) != EOF){
    
        linea[strlen(linea)-1] = '\0';
        usuario_t* usuario = usuario_crear(linea, pos_usuario);
        hash_guardar(hash_usuarios, usuario_nombre(usuario), usuario);
        pos_usuario++;
    }
    free(linea);
    return hash_usuarios;
}


// funciones de creacion y destruccion de estructuras

algogram_t* algogram_crear(FILE* usuarios){
    algogram_t* algogram = malloc(sizeof(algogram_t));
    if(!algogram) return NULL;

    algogram->usuarios = cargar_usuarios(usuarios); // se cargan los usuarios a un hash
    if(!algogram->usuarios) return NULL;

    algogram->posts = hash_crear(destruir_post);
    if(!algogram->posts) return NULL;
    
    algogram->usuario_loggeado = NULL;
    algogram->id_post = 0;
    return algogram;
}


void algogram_destruir(algogram_t* algogram){
    hash_destruir(algogram->posts);
    hash_destruir(algogram->usuarios); 
    free(algogram);
}


// primitivas de algogram

bool algogram_existe_usuario(algogram_t* algogram, char* user){
    return hash_pertenece(algogram->usuarios, user);
}


bool log_in(char* user, algogram_t* algogram){
    if(algogram->usuario_loggeado)
        return false;

    usuario_t* usuario_a_loggear = (usuario_t*)hash_obtener(algogram->usuarios, user);
    algogram->usuario_loggeado = usuario_a_loggear;
    return true;
}


bool log_out(algogram_t* algogram){
    if(!algogram->usuario_loggeado)
        return false;
    
    algogram->usuario_loggeado = NULL;
    return true;
}

/*
pre: el post fue creado
post: agrega el post publicado en el feed de cada usuario
 */
bool actualizar_feed(algogram_t* algogram, post_t* post){
    hash_iter_t* iter = hash_iter_crear(algogram->usuarios);
    if(!iter) return false; 

    while(!hash_iter_al_final(iter)){
        const char* user = hash_iter_ver_actual(iter);

        if(strcmp(user, usuario_nombre(algogram->usuario_loggeado)) != 0){
            usuario_t* usuario = (usuario_t*)hash_obtener(algogram->usuarios, user);

            if(!usuario_guardar_post(usuario, post, algogram->usuario_loggeado)) return false;
        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return true;
}


bool publicar_post(algogram_t* algogram, char* posteo){
    if(!algogram->usuario_loggeado)
        return false;  
    
    char id[5];
    sprintf(id, "%d", algogram->id_post);
    post_t* post = post_crear(algogram->id_post, usuario_nombre(algogram->usuario_loggeado), posteo);
    if(!post) return false;

    hash_guardar(algogram->posts, id, post);
    if(!actualizar_feed(algogram, post)) return false;
    algogram->id_post++;

    return true;
}


void imprimir_post(post_t* post){
    fprintf(stdout, "Post ID %i\n", post_id(post));
    fprintf(stdout, "%s dijo: %s\n", post_nombre_usuario(post), post_posteo(post));
    fprintf(stdout, "Likes: %i\n", post_cant_likes(post));
}

post_t* ver_proximo_post(algogram_t* algogram){
    if(!algogram->usuario_loggeado || feed_esta_vacio(algogram->usuario_loggeado)){
        return NULL;
    }
    post_t* post = usuario_ver_proximo_post(algogram->usuario_loggeado);
    return post;
}


bool likear_post(algogram_t* algogram, char* id){
    post_t* post = hash_obtener(algogram->posts, id);

    if(!post || !algogram->usuario_loggeado)
        return false;

    if(!post_esta_likeado(post, usuario_nombre(algogram->usuario_loggeado)))
        post_actualizar_likes(post, usuario_nombre(algogram->usuario_loggeado));
        
    return true;
}


lista_t* mostrar_likes(algogram_t* algogram, char* id){
    if(!hash_pertenece(algogram->posts, id)){
        return NULL;
    }
    post_t* post = hash_obtener(algogram->posts, id);
    lista_t* likes = post_obtener_likes(post);
    return likes;
}


void imprimir_likes(lista_t* likes){
    lista_iter_t* iter = lista_iter_crear(likes);
    if(!iter) return;

    fprintf(stdout, "El post tiene %li likes:\n", lista_largo(likes));

    while(!lista_iter_al_final(iter)){
        fprintf(stdout, "\t%s\n", (char*)lista_iter_ver_actual(iter));
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
}

