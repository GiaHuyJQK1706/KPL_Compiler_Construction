/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __VM_H__
#define __VM_H__

#include "instructions.h"

#define PS_ACTIVE         0
#define PS_INACTIVE       1
#define PS_NORMAL_EXIT    2
#define PS_IO_ERROR       3
#define PS_DIVIDE_BY_ZERO 4
#define PS_STACK_OVERFLOW 5

typedef WORD* Memory;

void printMemory(void);
void printCodeBuffer(void);

void resetVM(void);
void initVM(void);
void cleanVM(void);

int loadExecutable(FILE* f);
int saveExecutable(FILE* f);

int run(void);

#endif
