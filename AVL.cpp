#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int content;
    struct Node *left;
    struct Node *right;
} node;

typedef struct {
    node *source;
} ref;

void start(ref *tree){
    tree->source = NULL;
}

node *insert(node *source, int value){
    if(source == NULL){
        node *New = (node *)malloc(sizeof(node));

        if(New == NULL){
            printf("Memory allocation error\n");
            exit(1);
        }

        New->content = value;
        New->left = NULL;
        New->right = NULL;

        return New;
    } else {
        if(value < source->content){
            source->left = insert(source->left, value);
        } else if(value > source->content){
            source->right = insert(source->right, value);
        } else {
            printf("Value already exists\n");
        }

        return source;
    }
}

int balance_factor(node *source){
    if(source == NULL){
        return 
        
    }
}