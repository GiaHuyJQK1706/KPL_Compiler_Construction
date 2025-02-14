/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "token.h"

Token* getToken(void);
Token* getValidToken(void);
void printToken(Token *token);

#endif
