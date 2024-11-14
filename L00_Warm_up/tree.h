/*
 * Luu vao CTDL cay nhi phan se toi uu thoi gia tim kiem, chen hon la danh sach lien ket 
*/

#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[32]; 
    int count;     
    char lines[256];
} data;

typedef struct nodeType {
    data key;
    struct nodeType *left, *right;
} node;

typedef struct nodeType *tree;

void insertNode(tree *root, data x);
tree search(tree root, char *x);
void printTree(tree root);
void formatString(char *str);
int checkValidString(char s[]);
void freeTree(tree *root);

#endif
