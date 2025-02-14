/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include "symtab.h"

void checkFreshIdent(char *name);
Object* checkDeclaredIdent(char *name);
Object* checkDeclaredConstant(char *name);
Object* checkDeclaredType(char *name);
Object* checkDeclaredVariable(char *name);
Object* checkDeclaredFunction(char *name);
Object* checkDeclaredProcedure(char *name);
Object* checkDeclaredLValueIdent(char *name);

void checkIntType(Type* type);
void checkCharType(Type* type);
void checkArrayType(Type* type);
void checkBasicType(Type* type);
void checkTypeEquality(Type* type1, Type* type2);

#endif
