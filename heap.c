#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL 30
#define CAPACIDAD_MAYOR 2
#define CAPACIDAD_MENOR 4
#define CUADRUPLE 4


// ESTRUCTURA DEL HEAP
struct heap {
    void** datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};


//FUNCION DE REDIMENSION
bool heap_redimensionar(heap_t *heap, size_t capacidad_nueva){
    void **datos_nuevo = realloc(heap->datos, capacidad_nueva * sizeof(void*));
    if (capacidad_nueva > 0 && datos_nuevo == NULL){
        return false;
    }
    heap->datos = datos_nuevo;
    heap->capacidad = capacidad_nueva;
    return true;
}

//FUNCION AUXILIAR PARA SWAP
void swap(void** x, void** y){
    void* aux = *x;
    *x = *y;
    *y = aux;
}

//FUNCIONES CALCULO POSICIONES
size_t obtener_padre(size_t hijo){
    return (hijo - 1) / 2;
}

size_t obtener_hijo_der(size_t padre){
    return (2*padre) + 2;
}

size_t obtener_hijo_izq(size_t padre){
    return (2*padre) + 1;
}

// pre: datos inicializado
// post: devuelve la posicion del maximo entre padre, hijo_izq e hijo_der
size_t obtener_maximo(void** datos, size_t padre, size_t hijo_izq, size_t hijo_der, size_t cantidad, cmp_func_t cmp){
    size_t maximo = padre;

    if(hijo_izq < cantidad && cmp(datos[hijo_izq], datos[maximo]) > 0)
        maximo = hijo_izq;

    if(hijo_der < cantidad && cmp(datos[hijo_der], datos[maximo]) > 0)
        maximo = hijo_der;
        
    return maximo;
}


//FUNCIONES DE UPHEAP Y DOWNHEAP PARA ENCOLAR Y DESENCOLAR
void upheap(cmp_func_t cmp, size_t hijo, void** datos, size_t cantidad){
    if(hijo == 0) return;

    size_t padre = obtener_padre(hijo);

    if(cmp(datos[padre], datos[hijo]) < 0){
        swap(&datos[padre], &datos[hijo]);
        upheap(cmp, padre, datos, cantidad);

    } else return;
}

void downheap(cmp_func_t cmp, size_t padre, void* datos[], size_t cantidad){
    if(padre >= cantidad) return;

    size_t hijo_izq = obtener_hijo_izq(padre);
    size_t hijo_der = obtener_hijo_der(padre);

    size_t maximo = obtener_maximo(datos, padre, hijo_izq, hijo_der, cantidad, cmp);

    if(maximo != padre) {
      swap(&datos[padre], &datos[maximo]);
      downheap(cmp, maximo, datos, cantidad);
    }
}


//FUNCION DE HEAPIFY 
void heapify(void** arreglo, cmp_func_t cmp, size_t cant){
    for(int i = ((int)(cant)/2)-1; i >= 0; i--){
        downheap(cmp, (size_t)i, arreglo, cant);
    }
}

// PRIMITIVAS DEL HEAP
heap_t *heap_crear(cmp_func_t cmp){
    heap_t *heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    void** datos = malloc(sizeof(void*)*CAPACIDAD_INICIAL);
    if (!datos){
        free(heap);
        return NULL;
    }

    heap->cantidad = 0;
    heap->capacidad = CAPACIDAD_INICIAL;
    heap->cmp = cmp;
    heap->datos = datos;
    return heap;
}


heap_t *heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp){
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    heap->datos = malloc(sizeof(void*)*n);
    if (!heap->datos){
        free(heap);
        return NULL;
    }
    for (size_t i = 0; i < n; i++){
        heap->datos[i] = arreglo[i];
    }
    heap->capacidad = n;
    heap->cantidad = n;
    heap->cmp = cmp;

    heapify(heap->datos, heap->cmp, heap->cantidad);
    return heap;
}


void heap_destruir(heap_t* heap, void destruir_elemento(void *e)){
    if(destruir_elemento){
        for(size_t i = 0; i < heap->cantidad; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}


size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}


bool heap_esta_vacio(const heap_t *heap){
    return heap->cantidad == 0;
}


bool heap_encolar(heap_t *heap, void *elem){
    if(heap->capacidad == heap->cantidad){
        if(!(heap_redimensionar(heap, (heap->capacidad) * CAPACIDAD_MAYOR))) return false;
    }

    heap->datos[heap->cantidad] = elem;
    upheap(heap->cmp, heap->cantidad, heap->datos, heap->cantidad);
    heap->cantidad++;
    return true;
}


void* heap_ver_max(const heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}


void* heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;
    
    if(heap->cantidad * CUADRUPLE <= heap->capacidad)
        if(!heap_redimensionar(heap, (heap->capacidad)/CAPACIDAD_MENOR)) return NULL;
    
    void* borrado = heap_ver_max(heap);

    swap(&heap->datos[0], &heap->datos[heap->cantidad-1]);
    heap->cantidad--;
    downheap(heap->cmp, 0, heap->datos, heap->cantidad);

    return borrado;
}


//ORDENAMIENTO HEAPSORT
void heap_sort(void* elementos[], size_t cant, cmp_func_t cmp){
    if(cant <= 1) return;

    heapify(elementos, cmp, cant);

    for(int i = ((int)cant)-1; i >= 0; i--){
        swap(&elementos[0], &elementos[i]);
        downheap(cmp, 0, elementos, (size_t)i);
    }
}