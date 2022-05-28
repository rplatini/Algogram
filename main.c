#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "algogram.h"
#include "lista.h"
#include "errors.h"
#include "post.h"

#define CANT_PARAMETROS 2

#define LOGIN "login"
#define LOGOUT "logout"
#define PUBLICAR "publicar"
#define VER_FEED "ver_siguiente_feed"
#define LIKEAR_POST "likear_post"
#define MOSTRAR_LIKES "mostrar_likes"
#define ERROR_COMANDO "Error: Comando ingresado inexistente"


// post: devuelve un puntero a un string inicializado con lo que ingreso un usuario
char* input_usuario(){
    char* input = NULL;
    size_t tam = 0;

    if(getline(&input, &tam, stdin) == -1)
        return NULL;
    input[strlen(input)-1] = '\0';
    return input;
}


void ejecutar_comando(algogram_t* algogram, char* comando){
    if(strcmp(comando, LOGIN) == 0){
        char* input = input_usuario();

        if(!algogram_existe_usuario(algogram, input))
            fprintf(stdout, "%s\n", ERROR_US_NO_EXISTENTE);
            
        else if(!log_in(input, algogram))
            fprintf(stdout, "%s\n", ERROR_US_LOGGEADO);
            
        else fprintf(stdout, "Hola %s\n", input);
        free(input);


    } else if(strcmp(comando, LOGOUT) == 0){
        if(!log_out(algogram))
            fprintf(stdout, "%s\n", ERROR_US_NO_LOGGEADO);
        
        else fprintf(stdout, "Adios\n");


    } else if(strcmp(comando, PUBLICAR) == 0){
        char* input = input_usuario();

        if(!publicar_post(algogram, input))
            fprintf(stdout, "%s\n", ERROR_US_NO_LOGGEADO);
            
        else fprintf(stdout, "%s\n", POST_PUBLICADO);
        free(input);
            

    } else if(strcmp(comando, VER_FEED) == 0){
        post_t* post = ver_proximo_post(algogram);
        if(!post)
            fprintf(stdout, "%s\n", POST_FEED);
        else imprimir_post(post);


    } else if(strcmp(comando, LIKEAR_POST) == 0){
        char* input = input_usuario();

        if(!likear_post(algogram, input))
            fprintf(stdout, "%s\n", ERROR_POST);
        else fprintf(stdout, "%s\n", POST_LIKEADO);

        free(input);


    } else if(strcmp(comando, MOSTRAR_LIKES) == 0){
        char* input = input_usuario();
        lista_t* likes = mostrar_likes(algogram, input);

        if(!likes) fprintf(stdout, "%s\n", ERROR_LIKE);
        else {
            imprimir_likes(likes);
            lista_destruir(likes, NULL);
        }
        free(input);
    
    
    } else fprintf(stdout, "%s\n", ERROR_COMANDO);
}
  

int main(int argc, char* argv[]){
    if(argc != CANT_PARAMETROS){
        fprintf(stdout, "%s\n", ERROR_PARAMETROS);
        return -1;
    }

    FILE* usuarios = fopen(argv[1], "r");
    if(!usuarios) return -1;

    algogram_t* algogram = algogram_crear(usuarios);
    if(!algogram) return -1;
    
    char* comando = NULL; // variable donde se guarda lo que ingresa el usuario
    size_t tam = 0;

    while(getline(&comando, &tam, stdin) != EOF){
        comando[strlen(comando)-1] = '\0';    
        ejecutar_comando(algogram, comando);
    }

    free(comando);
    fclose(usuarios);
    algogram_destruir(algogram);
    return 0;
}