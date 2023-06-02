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

static bool invrep(pqueue q) {
    bool valid = true;
    //si pqueue tiene un solo elemento, es un pqueue valido, si es NULL, igualmente
    if (!pqueue_is_empty(q) && q->size>1){
        struct s_node *aux = NULL;
        aux = q->first;
        while (aux->next != NULL && valid){
            valid = aux->priority <= aux->next->priority; //chequeo que la prioridad del elem sig sea mayor, si es menor, retorno false;
            aux = aux->next;
        }
    }
    return valid;
}


pqueue pqueue_empty(void) {
    pqueue q=NULL;
    q = malloc(sizeof(pqueue));
    q->size = 0u;
    q->first = NULL;
    return q;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, unsigned int priority) {
    assert(invrep(q));
    struct s_node *new_node = create_node(e, priority);
    struct s_node *aux = NULL;
    bool queued = false;
    aux = q->first; //node iterativo
    if (pqueue_is_empty(q)){
        q->first = new_node;
    } else {
        while (!queued){ //chequeo priorities, agrego cuando sea menor.
            if(new_node->priority > aux->priority && aux->next != NULL){ 
                aux = aux->next;
            } else if (new_node->priority <= aux->priority && q->size == 1) {   
                q->first = new_node;   //enlazo el new_node al comienzo del pqueue
                new_node->next = aux;
                queued = true;
            } else if (aux->next == NULL && q->size != 1){
                aux->next = new_node;
                queued = true;
            } else if (q->size != 1 && aux->next != NULL) {
                new_node->next = aux->next; //enlazo el new_node al next del ultimo elemento al que fue mas grande.
                aux = new_node;
                queued = true;
            }
        }
    }
    ++q->size;
    
    assert(invrep(q) && !pqueue_is_empty(q));
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
