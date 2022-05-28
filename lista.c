#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

//ESTRUCTURAS 
typedef struct nodo {
    void *dato;
    struct nodo* siguiente;
} nodo_t;

struct lista {
    struct nodo* primero;
    struct nodo* ultimo;
    size_t largo;
}; 

struct lista_iter {
	lista_t* lista;
	struct nodo* actual;
	struct nodo* anterior;
};


// PRIMITIVAS DEL NODO 
nodo_t* crear_nodo(void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;

    nodo->dato = dato;
    nodo->siguiente = NULL;
    return nodo;
 }

void destruir_nodo(nodo_t *nodo){
    free(nodo);
}


// PRIMITIVAS DE LA LISTA

lista_t* lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista) return NULL;

    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    
    return lista; 
}


bool lista_esta_vacia(const lista_t *lista){
    return(!lista->primero || !lista->ultimo || lista->largo == 0);
}


bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato);
    if(!nodo) return false;

    if(lista_esta_vacia(lista)){
        lista->ultimo = nodo;

    } else {
        nodo->siguiente = lista->primero;
    }
    lista->primero = nodo;
    lista->largo++;
    return true;
}


bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato);
    if(!nodo) return false;

    if(lista_esta_vacia(lista)){
        lista->primero = nodo;

    } else {
        lista->ultimo->siguiente = nodo;
    }
    lista->ultimo = nodo;
    //nodo->siguiente = NULL;
    lista->largo++;

    return true;
}

void *lista_ver_primero(const lista_t *lista){
    if(!lista_esta_vacia(lista)){
        return lista->primero->dato;
    }
    return NULL;
}

void *lista_borrar_primero(lista_t *lista){
    if(lista_esta_vacia(lista)) return NULL;
    
    void* auxiliar = lista->primero->dato;
    nodo_t *nodo = lista->primero;
    lista->primero = lista->primero->siguiente;
    destruir_nodo(nodo);
    
    if(lista_esta_vacia(lista)){
        lista->ultimo = NULL;
    }

    lista->largo--;
    return auxiliar;
}


void *lista_ver_ultimo(const lista_t* lista){
    if(!lista_esta_vacia(lista)){
        return lista->ultimo->dato;
    }
    return NULL;
}


size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t* lista, void(*destruir_dato)(void*)){
    while(!lista_esta_vacia(lista)){
        if(destruir_dato != NULL)
            destruir_dato(lista_borrar_primero(lista));

        else lista_borrar_primero(lista);
    }
    free(lista);
}


// PRIMITIVAS DEL ITERADOR INTERNO

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t *actual = lista->primero;
    while(actual && visitar(actual->dato, extra)){
        actual = actual->siguiente;
    }
}

// PRIMITIVAS DEL ITERADOR EXTERNO

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t *lista_iter = malloc(sizeof(lista_iter_t));
    if(!lista_iter) return NULL;

    lista_iter->lista = lista; 
    lista_iter->actual = lista->primero;
    lista_iter->anterior = NULL;
    
    return lista_iter;

}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->siguiente;
    return true;
}


bool lista_iter_al_final(const lista_iter_t *iter){
    return(iter->actual == NULL);
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if(lista_iter_al_final(iter)) return NULL;

    return iter->actual->dato;
}


void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}


bool lista_iter_insertar(lista_iter_t *iter, void *dato){    
    // caso iter al principio de la lista
    if(lista_esta_vacia(iter->lista) || iter->actual == iter->lista->primero){
        lista_insertar_primero(iter->lista, dato);
        iter->actual = iter->lista->primero;

    // caso iter al final de la lista
    } else if(lista_iter_al_final(iter)){
        iter->anterior = iter->lista->ultimo;
        lista_insertar_ultimo(iter->lista, dato);
        iter->actual = iter->lista->ultimo;

    // cualquier otro caso
    } else {
        nodo_t* nuevo = crear_nodo(dato);
        if(!nuevo) return false;

        iter->anterior->siguiente = nuevo;
        nuevo->siguiente = iter->actual;
        iter->actual = nuevo;
        iter->lista->largo++;
    }

    return true;
}


void *lista_iter_borrar(lista_iter_t *iter){
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)){
        return NULL;
    }

    nodo_t* borrado = iter->actual;
    void* dato = iter->actual->dato;

    // caso iter al principio de la lista
    if(iter->actual == iter->lista->primero){
        iter->lista->primero = iter->actual->siguiente;
        iter->actual = iter->actual->siguiente;

    // caso iter en el ultimo elemento valido de la lista
    } else if(iter->actual == iter->lista->ultimo){
        iter->lista->ultimo = iter->anterior;
        iter->lista->ultimo->siguiente = NULL;
        iter->actual = NULL;

    // cualquier otro caso     
    } else {
        iter->anterior->siguiente = iter->actual->siguiente;
        iter->actual = iter->actual->siguiente;
    }

    destruir_nodo(borrado);
    iter->lista->largo--;
    return dato;
}