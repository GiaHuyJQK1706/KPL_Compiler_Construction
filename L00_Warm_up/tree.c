#include "tree.h"

void insertNode(tree *root, data x) {
    if (*root == NULL) {
        *root = (node*)malloc(sizeof(node));
        (*root)->key = x;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else if (strcmp(x.word, (*root)->key.word) < 0) {
        insertNode(&(*root)->left, x);
    } else if (strcmp(x.word, (*root)->key.word) > 0) {
        insertNode(&(*root)->right, x);
    } else {
        (*root)->key.count++;
        if (!strstr((*root)->key.lines, x.lines)) {
            strcat((*root)->key.lines, x.lines);
        }
    }
}

tree search(tree root, char *x) {
    if (root == NULL) return NULL;
    if (strcmp(x, root->key.word) < 0) return search(root->left, x);
    else if (strcmp(x, root->key.word) > 0) return search(root->right, x);
    else return root;
}

void printTree(tree root) {
    if (root != NULL) {
        printTree(root->left);
        printf("%-15s %d %s\n", root->key.word, root->key.count, root->key.lines);
        printTree(root->right);
    }
}

void formatString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
    while (!isalpha(str[0])) memmove(str, str+1, strlen(str));
    while (!isalpha(str[strlen(str)-1])) str[strlen(str)-1] = '\0';
}

int checkValidString(char s[]) {
    for (int i = 0; i < strlen(s); i++) {
        if (isdigit(s[i])) return 0;
    }
    return 1;
}

void freeTree(tree *root) {
    if (*root != NULL) {
        freeTree(&(*root)->left);
        freeTree(&(*root)->right);
        free(*root);
        *root = NULL;
    }
}
