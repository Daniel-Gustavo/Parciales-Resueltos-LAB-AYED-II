#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

struct s_pstack {
    struct s_node **array;
    unsigned int size;
    priority_t max_priority; //mi arreglo no puede tener mas de este indice, al mismo tiempo tiene la mayor prioridad
};

struct s_node {
    pstack_elem elem;
    struct s_node *next;
};

static struct s_node * create_node(pstack_elem e) {
    struct s_node *new_node = malloc(sizeof(struct s_node));
    assert(new_node!=NULL);
    new_node->elem = e;
    new_node->next = NULL;
    return new_node;
}

static struct s_node * destroy_node(struct s_node *node) {
    free(node);
    node = NULL;
    return node;
}

    
static bool invrep(pstack s) {
    return s != NULL;
}


pstack pstack_empty(priority_t max_priority) { //pienso como inicializara
    pstack s = malloc(sizeof(struct s_pstack)); //le doy el stack su size of asociado
    s->array = calloc(max_priority+1,sizeof(struct s_pstack)); //le asignamos a nuestro arreglo el tamaño del nodo 
                                                               //(sumo uno por que la max puede ser 0 y quiero que se multi por 1)
    s->size = 0;                                                                                              
    s->max_priority = max_priority;
    assert(invrep(s) && pstack_is_empty(s));
    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {
    assert(invrep(s));
    bool brk = true;
    struct s_node *new_node = create_node(e);
    for (priority_t i = 0u ; s->max_priority >= i && brk ; i++){
        if (i == priority){
            if (s->array[i] == NULL){
                s->array[i] = new_node;
            } else {
                new_node->next = s->array[i]; //el siguiente va a ser el antiguo primero 
                s->array[i] = new_node; //el puntero que apunte a el primero ahora apunta a new_node
                brk = false;
            }
        }
    }
    s->size++;
    assert(invrep(s) && !pstack_is_empty(s));
    return s;
}

/*
pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {
    //assert(invrep(s));
    
    struct s_node *new_node = create_node(e);
    if (s->array[priority] == NULL){
        s->array[priority] = new_node;
    } else {
        new_node->next = s->array[priority]; //el siguiente va a ser el antiguo primero 
        s->array[priority] = new_node; //el puntero que apunte a el primero ahora apunta a new_node
    }
    s->size++;
    //assert(invrep(s) && !pstack_is_empty(s));
    return s;
}*/

bool pstack_is_empty(pstack s) {
    assert(invrep(s));
    return (s->size == 0); //podemos tener arreglo pero si no tenemos elementos es por que la pila esta vacia
}

pstack_elem pstack_top(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    bool brk = true;
    pstack_elem res;
    for (priority_t i = s->max_priority ; brk ; i--){
        if (s->array[i] != NULL){
            res = s->array[i]->elem;
            brk = false;
        }
    }
    return res; //como es una pila agrego y saco por izquierda, entonces en el nodo del ultimo indice del arreglo donde se encuentra una pila, agarro el primer elemento al que apunta
}

priority_t pstack_top_priority(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    bool brk = true;
    priority_t prio;
    for (priority_t i = s->max_priority ; brk ; i--){
        if (s->array[i] != NULL){
            prio = i;
            brk = false;
        }
    }
    return prio; 
}

size_t pstack_size(pstack s) {
    assert(invrep(s));
    return s->size;
}

pstack pstack_pop(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    bool brk = true;
    for (priority_t i = s->max_priority ; brk ; i--){
        if (s->array[i] != NULL){
            struct s_node *killme = s->array[i]; //apunta al stack con mayor prioridad 
            s->array[i] = s->array[i]->next;
            killme = destroy_node(killme);
            brk = false;
        }
    }
    s->size--;
    assert(invrep(s));
    return s;
}

pstack pstack_destroy(pstack s) {
    assert(invrep(s));
    for (priority_t i = 0 ; s->max_priority >= i ; i++){
        if (s->array[i] != NULL){
            while (s->array[i]->next != NULL){ //destruyo todos los nodos del stack
                struct s_node *it = s->array[i];
                s->array[i] = s->array[i]->next;
                it = destroy_node(it);
            }
            s->array[i] = destroy_node(s->array[i]); //destruyo el nodo que apunta al primero del stack
        }
    }
    free(s->array); //libero toda la memoria que calloquee en el arreglo
    free(s); //libero la memoria que malloquee en el stack
    return s;
}
//pensa en todas las variables a las que le asignas memorias y liberalas