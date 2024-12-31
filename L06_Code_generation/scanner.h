#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "token.h"

Token* getToken(void);
Token* getValidToken(void);
void printToken(Token *token);

#endif
