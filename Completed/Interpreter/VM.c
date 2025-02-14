/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <curses.h> // for getch(), include <ncurses.h> for unix/linux

#include "vm.h"

CodeBlock *codeBlock;
WORD* stack;
WORD* global;
int t;
int b;
int pc;
int ps;
int stackSize;
int codeSize;
int debugMode;

void resetVM(void) {
  pc = 0;
  t = -1;
  b = 0;
  ps = PS_INACTIVE;
}

void initVM(void) {
  codeBlock = createCodeBlock(codeSize);
  stack = (Memory) malloc(stackSize * sizeof(WORD));
  resetVM();
}

void cleanVM(void) {
  freeCodeBlock(codeBlock);
  free(stack);
}

int loadExecutable(FILE* f) {
  loadCode(codeBlock,f);
  resetVM();
  return 1;
}

int saveExecutable(FILE* f) {
  saveCode(codeBlock,f);
  return 1;
}

int checkStack(void) {
  return ((t >= 0) && (t <stackSize));
}

int base(int p) {
  int currentBase = b;
  while (p > 0) {
    currentBase = stack[currentBase + 3];
    p --;
  }
  return currentBase;
}

void printMemory(void) {
  int i;
  printf("Start dumping...\n");
  for (i = 0; i <= t; i++) 
    printf("  %4d: %d\n",i,stack[i]);
  printf("Finish dumping!\n");
}

void printCodeBuffer(void) {
  printCodeBlock(codeBlock);
}

int run(void) {
  Instruction* code = codeBlock->code;
  int count = 0;
  int number;
  char s[100];

  WINDOW* win = initscr();
  nonl();
  cbreak();
  noecho();
  scrollok(win,TRUE);
  
  ps = PS_ACTIVE;
  while (ps == PS_ACTIVE) {
    if (debugMode) {
      sprintInstruction(s,&(code[pc]));
      printf( "%6d-%-4d:  %s\n",count++,pc,s);
    }

    switch (code[pc].op) {
    case OP_LA: 
      t ++;
      if (checkStack())
	stack[t] = base(code[pc].p) + code[pc].q;
      break;
    case OP_LV: 
      t ++;
      if (checkStack())
	stack[t] = stack[base(code[pc].p) + code[pc].q];
      break;
    case OP_LC: 
      t ++;
      if (checkStack())
	stack[t] = code[pc].q;
      break;
    case OP_LI: 
      stack[t] = stack[stack[t]];
      break;
    case OP_INT:
      t += code[pc].q;
      checkStack();
      break;
    case OP_DCT: 
      t -= code[pc].q;
      checkStack();
      break;
    case OP_J: 
      pc = code[pc].q - 1;
      break;
    case OP_FJ: 
      if (stack[t] == FALSE) 
	pc = code[pc].q - 1;
      t --;
      checkStack();
      break;
    case OP_HL: 
      ps = PS_NORMAL_EXIT;
      break;
    case OP_ST: 
      stack[stack[t-1]] = stack[t];
      t -= 2;
      checkStack();
      break;
    case OP_CALL: 
      stack[t+2] = b;                 // Dynamic Link
      stack[t+3] = pc;                // Return Address
      stack[t+4] = base(code[pc].p);  // Static Link
      b = t + 1;                      // Base & Result
      pc = code[pc].q - 1;              
      break;
    case OP_EP: 
      t = b - 1;                      // Previous top
      pc = stack[b+2];                // Saved return address
      b = stack[b+1];                 // Saved base
      break;
    case OP_EF:
      t = b;                          // return value is on the top of the stack
      pc = stack[b+2];                // Saved return address
      b = stack[b+1];                 // saved base
      break;
    case OP_RC: 
      t ++;
      echo();
      scanf("%c",&number);
      noecho();
      stack[t] = number;
      checkStack();
      break;
    case OP_RI:
      t ++;
      echo();
      scanf("%d",&number);
      noecho();
      stack[t] = number;
      checkStack();
      break;
    case OP_WRC: 
      printf("%c",stack[t]);
      t --;
      checkStack();
      break;     
    case OP_WRI: 
      printf("%d",stack[t]);
      t --;
      checkStack();
      break;
    case OP_WLN:
      printf("\n");
      break;
    case OP_AD:
      t --;
      if (checkStack()) 
	stack[t] += stack[t+1];
      break;
    case OP_SB:
      t --;
      if (checkStack()) 
	stack[t] -= stack[t+1];
      break;
    case OP_ML:
      t --;
      if (checkStack()) 
	stack[t] *= stack[t+1];
      break;

    case OP_DV: 
      t --;
      if (checkStack()) {
	if (stack[t+1] == 0)
	  ps = PS_DIVIDE_BY_ZERO;
	else stack[t] /= stack[t+1];
      }
      break;
    case OP_NEG:
      stack[t] = - stack[t];
      break;
    case OP_CV: 
      stack[t+1] = stack[t];
      t ++;
      checkStack();
      break;
    case OP_EQ:
      t --;
      if (stack[t] == stack[t+1]) 
         stack[t] = TRUE;
      else 
           stack[t] = FALSE;
      checkStack();
      break;
    case OP_NE:
      t --;
      if (stack[t] != stack[t+1]) 
	     stack[t] = TRUE;
      else stack[t] = FALSE;
      checkStack();
      break;
    case OP_GT:
      t --;
      if (stack[t] > stack[t+1]) 
	     stack[t] = TRUE;
      else 
           stack[t] = FALSE;
      checkStack();
      break;
    case OP_LT:
      t --;
      if (stack[t] < stack[t+1]) 
	     stack[t] = TRUE;
      else 
           stack[t] = FALSE;
      checkStack();
      break;
    case OP_GE:
      t --;
      if (stack[t] >= stack[t+1]) 
	stack[t] = TRUE;
      else stack[t] = FALSE;
      checkStack();
      break;
    case OP_LE:
      t --;
      if (stack[t] <= stack[t+1]) 
	stack[t] = TRUE;
      else stack[t] = FALSE;
      checkStack();
      break;
    case OP_BP:
      // Just for debugging
      debugMode = 1;
      break;
    default: break;
    }

    if (debugMode) {
      int command;
      int level, offset;
      int interactive = 1;
      
      do {
	interactive = 0;

	command = getch();
	switch (command) {
	case 'a':
	case 'A':
	  printf("\nEnter memory location (level, offset):");
	  scanf("%d %d", &level, &offset);
	  printf("Absolute address = %d\n", base(level) + offset);
	  interactive = 1;
	  break;
	case 'm':
	case 'M':
	  printf("\nEnter memory location (level, offset):");
	  scanf("%d %d", &level, &offset);
	  printf("Value = %d\n", stack[base(level) + offset]);
	  interactive = 1;
	  break;
	case 't':
	case 'T':
	  printf("Top (%d) = %d\n", t, stack[t]);
	  interactive = 1;
	  break;
	case 'c':
	case 'C':
	  debugMode = 0;
	  break;
	case 'h':
	case 'H':
	  ps = PS_NORMAL_EXIT;
	  break;
	default: break;
	}
      } while (interactive);
    }
    pc ++;
  }
  printf("\nPress any key to exit...");getch();
  endwin();
  return ps;
}
