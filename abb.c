#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define SIN_HIJOS 0
#define UN_HIJO 1
#define DOS_HIJOS 2


// declaracion de estructuras

typedef struct abb_nodo {
    struct abb_nodo* izq;
    struct abb_nodo* der;
    void* dato;
    char* clave;
} abb_nodo_t;


struct abb {
    abb_nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};


struct abb_iter {
    abb_t* arbol;
    pila_t* pila;
};

// PRIMITIVAS NODO

abb_nodo_t* crear_abb_nodo(const char *clave, void *dato){
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if (!nodo) return NULL;

    char* copia_clave = strdup(clave);
    if(!copia_clave){
        free(nodo);
        return NULL;
    }  
    nodo->clave = copia_clave;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;

    return nodo;
}


void destruir_abb_nodo(abb_nodo_t* nodo){
    free(nodo->clave);
    free(nodo);
}

//FUNCIONES AUXILIARES

// PRIMITIVAS ABB

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) return NULL;

    arbol->raiz = NULL;
    arbol->cantidad = 0;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;

    return arbol;
}

/* pre: - los parametros de entrada deben estar inicializados
        - padre es el nodo que tiene como hijo al nodo buscado
   post: - devuelve el nodo cuya clave es la buscada, y actualiza el padre del nodo hallado
         - si el nodo no se encuentra, devuelve NULL
 */
abb_nodo_t* buscar_clave(abb_nodo_t* nodo, const char* clave, abb_comparar_clave_t cmp, abb_nodo_t** padre){
    if(!nodo) return NULL;

    if(cmp(clave, nodo->clave) == 0){
        return nodo;

    } else if(cmp(clave, nodo->clave) < 0){
        *padre = nodo;
        return buscar_clave(nodo->izq, clave, cmp, padre);

    } else {
        *padre = nodo;
        return buscar_clave(nodo->der, clave, cmp, padre);
    }
}


bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
    // caso arbol vacio
    if(arbol->cantidad == 0){
        arbol->raiz = crear_abb_nodo(clave, dato);
        if(!arbol->raiz) return false;
        arbol->cantidad++;
        return true;
    }

    abb_nodo_t* padre = arbol->raiz;
    abb_nodo_t* nodo_clave = buscar_clave(arbol->raiz, clave, arbol->cmp, &padre);

    // caso clave no existe y hay que crear nodo
    if(!nodo_clave){
        if(arbol->cmp(clave, padre->clave) > 0){
            padre->der = crear_abb_nodo(clave, dato);
            if(!padre->der) return false;

        } else {
            padre->izq = crear_abb_nodo(clave, dato);
            if(!padre->izq) return false;
        }
        arbol->cantidad++;

    // caso clave existe y hay que reemplazar dato
    } else {
        void* dato_viejo = nodo_clave->dato;
        nodo_clave->dato = dato;

        if(arbol->destruir_dato){
            arbol->destruir_dato(dato_viejo);
        }
    }
    return true;
}


void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t* padre = arbol->raiz;
    abb_nodo_t* nodo_clave = buscar_clave(arbol->raiz, clave, arbol->cmp, &padre);

    if(!nodo_clave) return NULL;
    return nodo_clave->dato;
}


bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_nodo_t* padre = arbol->raiz;
    abb_nodo_t* nodo_clave = buscar_clave(arbol->raiz, clave, arbol->cmp, &padre);

    return nodo_clave;
}

// funciones auxiliares de borrado

abb_nodo_t* buscar_reemplazante(abb_nodo_t* nodo, abb_nodo_t** padre){
    if(!nodo->izq) return nodo;
    *padre = nodo;
    return buscar_reemplazante(nodo->izq, padre);
}

int cantidad_hijos(abb_nodo_t* nodo){
    if(!nodo->izq && !nodo->der) return SIN_HIJOS;
    else if((!nodo->izq && nodo->der) || (nodo->izq && !nodo->der)) return UN_HIJO;
    else return DOS_HIJOS;
}

// intercambia las claves y los datos de 2 nodos
void swappear_datos(abb_nodo_t* nodo1, abb_nodo_t* nodo2, char* clave_aux, void* dato_aux){
    clave_aux = nodo1->clave;
    dato_aux = nodo1->dato;
    nodo1->clave = nodo2->clave;
    nodo1->dato = nodo2->dato;
    nodo2->clave = clave_aux;
    nodo2->dato = dato_aux;
}


void abb_borrar_(abb_nodo_t** raiz, abb_nodo_t* hijo, abb_nodo_t** padre){
    if(!*padre){ // si el elemento a borrar es la raiz del abb
        abb_nodo_t* raiz_vieja = *raiz;

        if((*raiz)->izq) *raiz = (*raiz)->izq;
        else if((*raiz)->der) *raiz = (*raiz)->der;
        else *raiz = NULL;

        destruir_abb_nodo(raiz_vieja);
        return;
    }

    if((*padre)->izq == hijo){
        if(cantidad_hijos(hijo) == SIN_HIJOS) (*padre)->izq = NULL;

        else {
            if(hijo->der) (*padre)->izq = hijo->der;
            else (*padre)->izq = hijo->izq;
        }
    } else {
        if(cantidad_hijos(hijo) == SIN_HIJOS) (*padre)->der = NULL;

        else {
            if(hijo->der) (*padre)->der = hijo->der;
            else (*padre)->der = hijo->izq;
        }
    } 
    destruir_abb_nodo(hijo);
}

// elimina un nodo del abb que tiene 2 hijos
void abb_borrar_2_hijos(abb_nodo_t** raiz, abb_nodo_t* hijo){
    abb_nodo_t* reemplazante;
    abb_nodo_t* padre_reemplazante = hijo;
    char* clave_aux = hijo->clave;
    void* dato_aux = hijo->dato;

    if(hijo == *raiz){ // si el elemento a borrar es la raiz
        reemplazante = buscar_reemplazante((*raiz)->der, &padre_reemplazante);
        swappear_datos(*raiz, reemplazante, clave_aux, dato_aux); // swap de los datos de la raiz con el reemplazante

    } else {
        reemplazante = buscar_reemplazante(hijo->der, &padre_reemplazante);
        swappear_datos(hijo, reemplazante, clave_aux, dato_aux); // swap de los datos del hijo y el reemplazante
    }
    abb_borrar_(raiz, reemplazante, &padre_reemplazante);
}


void *abb_borrar(abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_clave;
    abb_nodo_t* padre = NULL;

    nodo_clave = buscar_clave(arbol->raiz, clave, arbol->cmp, &padre);
    if(!nodo_clave) return NULL;

    void* dato_borrado = nodo_clave->dato;

    if(cantidad_hijos(nodo_clave) == DOS_HIJOS)
        abb_borrar_2_hijos(&arbol->raiz, nodo_clave);

    else
        abb_borrar_(&arbol->raiz, nodo_clave, &padre);

    arbol->cantidad--;
    return dato_borrado;
}


size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}


void _abb_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato){
    if(!nodo) return;

    _abb_destruir(nodo->izq, destruir_dato);
    _abb_destruir(nodo->der, destruir_dato);

    if(destruir_dato){
        destruir_dato(nodo->dato);
    }
    destruir_abb_nodo(nodo);
}


void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}


// primitivas iterador interno

bool _abb_in_order(abb_nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra){
    if(!nodo) return true;

    if(!_abb_in_order(nodo->izq, visitar, extra)) return false;
    if(!visitar(nodo->clave, nodo->dato, extra)) return false;
    return _abb_in_order(nodo->der, visitar, extra);
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    _abb_in_order(arbol->raiz, visitar, extra);
}


// PRIMITIVAS ITERADOR EXTERNO

void apilar_izquierdos(pila_t* pila, abb_nodo_t* nodo){
    if(!nodo) return;
    pila_apilar(pila, nodo);
    apilar_izquierdos(pila, nodo->izq);
}


abb_iter_t* abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;

    pila_t *pila = pila_crear();
    if (!pila){
        free(iter);
        return NULL;
    }
    iter->pila = pila;
    apilar_izquierdos(iter->pila, arbol->raiz);
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return false;

    abb_nodo_t* desapilado = pila_desapilar(iter->pila);
    if(desapilado->der){
        pila_apilar(iter->pila, desapilado->der);
        apilar_izquierdos(iter->pila, desapilado->der->izq);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter) || pila_esta_vacia(iter->pila)) return NULL;

    abb_nodo_t* actual = pila_ver_tope(iter->pila);
    return actual->clave;
}


bool abb_iter_in_al_final(const abb_iter_t *iter){
    if(pila_esta_vacia(iter->pila)) return true;
    return false;
}


void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}