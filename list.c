//
//  list.c
//  List Library
//
//  Created by Gabriel Kapach on 15/09/2017.
//  Copyright © 2017 Gabriel Kapach. All rights reserved.
//

#include "list.h"

/*
 * Function: create_node
 * ---------------------
 * Returns a pointer for a new list node
 *
 * value: the value of the new node
 *
 */

node* create_node(int value) {
    node *tmp = (node*) malloc(sizeof(node));
    
    if (tmp == NULL) {
        printf("\n Node creation failed \n");
        return NULL;
    }
    
    tmp->value = value;
    
    return tmp;
}

/*
 * Function: list_init_empty
 * -------------------
 * Returns a new list with zero nodes
 *
 */

list* list_init_empty() {
    list* self = (list*) malloc(sizeof(list));
    
    if (self == NULL) {
        printf("\n List creation failed \n");
        exit(0);
    }
    
    // Set the new list data
    self->count = 0;
    self->sum = 0;
    
    return self;
}

/*
 * Function: list_init
 * -------------------
 * Returns a new list with one node
 *
 * value: the value of the first node
 *
 */

list* list_init(int value) {
    list* self = (list*) malloc(sizeof(list));
    
    if (self == NULL) {
        printf("\n List creation failed \n");
        exit(0);
    }
    
    // Create head node
    self->head = create_node(value);
    
    // Set the new list data
    if (self->head) {
        self->count = 1;
        self->sum = value;
        self->tail = self->head;
    }
    
    return self;
}

/*
 * Function: list_destory
 * ----------------------
 * Setting free all the memory which was allocated for the list
 *
 * head: the head node of a list
 *
 */

void list_destory(node* head) {
    node* tmp = head->next;
    
    if (tmp) {
        list_destory(tmp);
    }
    
    free(head);
}

/*
 * Function: list_avg
 * ------------------
 * Returns the average of all the list node's values
 *
 * list: reference to the list you're working on
 *
 */

float list_avg(list* self) {
    return (float)self->sum / self->count;
}

/*
 * Function: append_node
 * ---------------------
 * Append a new node to the list
 *
 * list: reference to the list you're working on
 * value: value for the new node
 *
 * returns: the new node pointer
 */

node* append_node(list* self, int value) {
    node *tmp = create_node(value);
    
    if (tmp) {
        tmp->prev = self->tail;
        self->tail->next = tmp;
        self->tail = tmp;
        
        self->count++;
        self->sum += value;
    }
    
    return tmp;
}

/*
 * Function: remove_node
 * ---------------------
 * Remove a node from the list
 *
 * list: reference to the list you're working on
 * n: the node we want to remove
 *
 */

void remove_node(list* self, node* n) {
    node *tmp = n->prev;
    
    // Reconnecting the nodes of the list
    if (tmp) {
        tmp->next = n->next;
        
        if (n->next) {
            n->next->prev = tmp;
        }
    } else {
        self->head = n->next;
    }
    
    self->count--;
    self->sum -= n->value;
    
    free(n);
}

/*
 * Function: first_node
 * --------------------
 * Retruns the head of the list
 *
 * list: reference to the list you're working on
 *
 */

node* first_node(list* self) {
    return self->head;
}

/*
 * Function: last_node
 * -------------------
 * Retruns the tail of the list
 *
 * list: reference to the list you're working on
 *
 */

node* last_node(list* self) {
    return self->tail;
}

/*
 * Function: next_node
 * -------------------
 * Retruns the next node in the list
 *
 * n: reference to the node you're working on
 *
 */

node* next_node(node* n) {
    return n->next;
}

/*
 * Function: prev_node
 * -------------------
 * Retruns the previous node in the list
 *
 * n: reference to the node you're working on
 *
 */

node* prev_node(node* n) {
    return n->next;
}

/*
 * Function: node_value
 * --------------------
 * Retruns the value of a node
 *
 * n: reference to the node you're working on
 *
 */

int node_value(node* n) {
    return n->value;
}
