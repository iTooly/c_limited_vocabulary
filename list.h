//
//  list.h
//  List Library
//
//  Created by Gabriel Kapach on 15/09/2017.
//  Copyright © 2017 Gabriel Kapach. All rights reserved.
//

#ifndef list_h
#define list_h

#include <stdio.h>
#include <stdlib.h>

#endif /* list_h */

typedef struct node {
    int value;
    struct node *next;
    struct node *prev;
} node;

typedef struct list {
    node *head;
    node *tail;
    int count;
    int sum;
} list;

node* create_node(int value);
list* list_init_empty();
list* list_init(int value);
void list_destory(node* head);
float list_avg(list* self);
node* append_node(list* self, int value);
void remove_node(list* self, node* n);
node* first_node(list* self);
node* last_node(list* self);
node* next_node(node* n);
node* previous_node(node* n);
int node_value(node* n);
