/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
int main(int argc, char *argv[]) {
  char op[20];
  FILE* f;

  if (argc <= 1) {
    printf("Usage: smc input output\n");
    printf("   input: input stack machine assembly program\n");
    printf("   output: output binary\n");}


    f = fopen(argv[1],"r");
    if (f == NULL) {
        printf("smc: Can\'t read input file!\n");
        return -1;
    }

    CodeBlock* cb = createCodeBlock(1000);
    while(fscanf(f,"%s",op)!=EOF){
        readInstructions(cb,op,f);
    }

    f = fopen(argv[2], "wb");
    if (f == NULL) printf("smc: Can\'t write output file!\n");

    saveCode(cb, f);
    fclose(f);

}

