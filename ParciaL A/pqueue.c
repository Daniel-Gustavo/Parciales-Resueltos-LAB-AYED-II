#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

struct s_pqueue {
    unsigned int size;
    struct s_node *first;
};

struct s_node {
    pqueue_elem elem;
    unsigned int priority;
    struct s_node *next;
};

static struct s_node * create_node(pqueue_elem e, unsigned int priority) {
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));//si no anda, poner puntero en new node
    assert(new_node!=NULL);
    new_node->elem = e;
    new_node->next = NULL;
    new_node->priority = priority;
    return new_node;
}

static struct s_node * destroy_node(struct s_node *node) {
    assert(node != NULL);
    node->next = NULL;
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}
// 1 2 3 5 4
/*
static bool invrep(pqueue q) {
  struct s_node *aux = q->first; //creo un nodo auxiliar para simular el first.
    bool res = true; //creo una variable booleana para devolver.
    unsigned int prio = q->first->priority; //creo una variable para guardar la prioridad del primer nodo.
    while (aux->next != NULL && res) //recorro la cola hasta llegar al ultimo nodo.
    {
        if (prio > aux->next->priority){ //si la prioridad del nodo actual es mayor a la del siguiente, devuelvo false.
            res = false;
        }
        prio = aux->next->priority;
        aux = aux->next; //avanzo al siguiente nodo.
    }
    return res; //devuelvo el resultado.
}*/
// 1 2 3 5 4 

static bool invrep(pqueue q) {
    struct s_node *aux = q->first; //creo un nodo auxiliar para simular el first.
    bool res = true; //creo una variable booleana para devolver.
    if(q->first!=NULL){
        unsigned int prio = q->first->priority; //creo una variable para guardar la prioridad del primer nodo.
        while (aux->next != NULL && res) //recorro la cola hasta llegar al ultimo nodo.
        {
            if (prio > aux->next->priority){ //si la prioridad del nodo actual es mayor a la del siguiente, devuelvo false.
                res = false;
            }
        prio = aux->next->priority;
        aux = aux->next; //avanzo al siguiente nodo.
        }
    }
    return res; //devuelvo el resultado.
}


pqueue pqueue_empty(void) {
    pqueue q=NULL;
    q = malloc(sizeof(pqueue));
    q->size = 0;
    q->first = NULL;
   // q->first->priority = 0;
    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, unsigned int priority) {
    assert(invrep(q));
    struct s_node *new_node = create_node(e, priority);
    bool b = true;

    //primer if: cuando el primer nodo sea igual a NULL
    if(q->first == NULL){
        q->first = new_node;// le asigno el nuevo nodo
    }
    else{
        struct s_node *aux= NULL, *rec= NULL;
        rec = q->first;
        aux = rec;

        //segundo if: cuando la P del nuevo nodo sea menor que la del primer nodo
        if(new_node->priority < q->first->priority){//chequea prioridad
        q->first = new_node;//el primero sera el nuevo nodo
        q->first->next = aux;//el segundo sera el antiguo primero
        free(aux);
        }

        //while para recorrer la cola, esta parado en 
        while(rec->next!=NULL && b){//mientras que el siguiente no sea null
            
            //tercer if: chequea que la prioridad del que sigue de first, no sea menor que el primero;
            if(new_node->priority < rec->next->priority){
                b = false;//false para salir, entonces se quedara en la 2da pos
                //haremos el cambio mas adelante
            }

            //cuarto if: si el nn tiene mayor prioridad que el 2,
            if(new_node->priority > rec->next->priority){
                rec = rec->next;//pasamos al siguiente, para que vuelva a comparar
            }
        }
        //quinto if: esto es para meter el new node en la pos que va
        if(b==false){
        aux = rec->next;
        rec->next = new_node;
        new_node->next = aux;
        free(aux); free(rec);
        }
        else{
        rec->next = new_node;
        }
    }
    q->size++;
    return q;
}

bool pqueue_is_empty(pqueue q) {
    assert(invrep(q));
    return q->first == NULL;
}


pqueue_elem pqueue_peek(pqueue q) {
    pqueue_elem prioritario = 0;
    prioritario = q->first->elem;
    return prioritario;
}

unsigned int pqueue_peek_priority(pqueue q) {
    unsigned int priority_most_value;
    priority_most_value = q->first->priority;
    return priority_most_value;
}

unsigned int pqueue_size(pqueue q) {
    assert(invrep(q));
    unsigned int size=0;
    size = q->size;
    return size;
}

pqueue pqueue_dequeue(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
    struct s_node * killme=q->first;
    q->first = q->first->next;
    killme = destroy_node(killme);
    assert(invrep(q));
    return q;
}


pqueue pqueue_destroy(pqueue q) {
    assert(invrep(q));
    struct s_node *node=q->first;
    while (node != NULL) {
        struct s_node *killme=node;
        node = node->next;
        killme = destroy_node(killme);
    }
    free(q);
    q = NULL;
    assert(q == NULL);
    return q;
}
