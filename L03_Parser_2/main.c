#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "reader.h"

/******************************************************************/

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("parser: no input file.\n");
        return -1;
    }
    if (compile(argv[1]) == IO_ERROR) {
        printf("Can\'t read input file!\n");
        return -1;
    }
    return 0;
}
