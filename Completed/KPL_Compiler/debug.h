/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#ifndef __DEBUG_H__
#define __DEBUG_H_

#include "symtab.h"

void printType(Type* type);
void printConstantValue(ConstantValue* value);
void printObject(Object* obj, int indent);
void printObjectList(ObjectNode* objList, int indent);
void printScope(Scope* scope, int indent);

#endif
