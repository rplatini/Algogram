#include "pila.h"
#include <stdlib.h>

#define CAPACIDAD_INICIAL 10
#define MITAD_CAPACIDAD 2
#define DOBLE_CAPACIDAD 2
#define CUADRUPLE_CANTIDAD 4



/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

bool redimensionar_pila(pila_t* pila, size_t capacidad_nueva){
    void** datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));

    if(capacidad_nueva > 0 && datos_nuevo == NULL){
        return false;
    }
    pila->datos = datos_nuevo;
    pila->capacidad = capacidad_nueva;
    return true;
    
}

pila_t *pila_crear(void){
    pila_t* pila = malloc(sizeof(pila_t));

    if(pila == NULL) return NULL;

    pila->datos = malloc(sizeof(void*)*CAPACIDAD_INICIAL);

    if(pila->datos == NULL) free(pila);

    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad == 0);
}

bool pila_apilar(pila_t *pila, void *valor){
    if(pila->cantidad == pila->capacidad){

        bool exito = redimensionar_pila(pila, (pila->capacidad)*DOBLE_CAPACIDAD);
        if(!exito) return false;
    } 
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;

    return true;
}

void *pila_ver_tope(const pila_t *pila){
    if(pila_esta_vacia(pila)) return NULL;
    return pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){
    if(pila_esta_vacia(pila)) return NULL;

    pila->cantidad--;

    if(((pila->cantidad)*CUADRUPLE_CANTIDAD <= pila->capacidad) && (pila->cantidad != 0)){
        if(pila->capacidad >= CAPACIDAD_INICIAL){
            redimensionar_pila(pila, (pila->capacidad)/MITAD_CAPACIDAD);
        }
    }
    return pila->datos[pila->cantidad];
}
