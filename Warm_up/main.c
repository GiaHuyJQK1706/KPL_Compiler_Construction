#include "tree.h"

int main() {
    char buff[16384], temp[64], *p;
    int count = 0;
    data h;
    tree stopWords = NULL, wordTree = NULL;

    FILE *f_stop = fopen("stopw.txt", "r");
    FILE *f_passage = fopen("vanban.txt", "r");

    while (fscanf(f_stop, "%s", buff) != EOF) {
        formatString(buff);
        strcpy(h.word, buff);
        h.count = 0;
        insertNode(&stopWords, h);
    }

    while (fgets(buff, sizeof(buff), f_passage)) {
        count++;
        p = strtok(buff, " \t\n");
        while (p != NULL) {
            if (checkValidString(p)) {
                formatString(p);
                if (isupper(p[0]) && (strcmp(temp, ".") != 0)) {
                    strcpy(temp, p);
                    p = strtok(NULL, " \t\n");
                    continue;
                }
                strcpy(temp, p);
                strcpy(h.word, p);
                h.count = 1;
                sprintf(h.lines, ", %d", count);

                if (search(stopWords, h.word) == NULL) {
                    insertNode(&wordTree, h);
                }
            }
            p = strtok(NULL, " \t\n");
        }
    }
    
    fclose(f_passage); fclose(f_passage);
    printTree(wordTree);
    freeTree(&stopWords); freeTree(&wordTree);
    return 0;
}
