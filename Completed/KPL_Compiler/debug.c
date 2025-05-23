/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#include <stdio.h>
#include "debug.h"

void pad(int n) { // Hàm này in ra n ký tự trắng
  int i;
  for (i = 0; i < n ; i++) printf(" ");
}

void printType(Type* type) { // Hàm này in ra một kiểu
  switch (type->typeClass) {
  case TP_INT:
    printf("Int");
    break;
  case TP_CHAR:
    printf("Char");
    break;
  case TP_ARRAY:
    printf("Arr(%d,",type->arraySize);
    printType(type->elementType);
    printf(")");
    break;
  }
}

void printConstantValue(ConstantValue* value) { // Hàm này in ra một hằng số
  switch (value->type) {
  case TP_INT:
    printf("%d",value->intValue);
    break;
  case TP_CHAR:
    printf("\'%c\'",value->charValue);
    break;
  default:
    break;
  }
}

void printObject(Object* obj, int indent) { // Hàm này in ra một object
  switch (obj->kind) {
  case OBJ_CONSTANT:
    pad(indent);
    printf("Const %s = ", obj->name);
    printConstantValue(obj->constAttrs->value);
    break;
  case OBJ_TYPE:
    pad(indent);
    printf("Type %s = ", obj->name);
    printType(obj->typeAttrs->actualType);
    break;
  case OBJ_VARIABLE:
    pad(indent);
    printf("Var %s : ", obj->name);
    printType(obj->varAttrs->type);
    printf(" at offset %d", obj->varAttrs->localOffset);
    break;
  case OBJ_PARAMETER:
    pad(indent);
    if (obj->paramAttrs->kind == PARAM_VALUE) 
      printf("Param %s : ", obj->name);
    else
      printf("Param VAR %s : ", obj->name);
    printType(obj->paramAttrs->type);
    printf(" at offset %d", obj->paramAttrs->localOffset);
    break;
  case OBJ_FUNCTION:
    pad(indent);
    printf("Function %s : ",obj->name);
    printType(obj->funcAttrs->returnType);
    printf(" at address %d\n", obj->funcAttrs->codeAddress);
    printScope(obj->funcAttrs->scope, indent + 4);
    break;
  case OBJ_PROCEDURE:
    pad(indent);
    printf("Procedure %s at address %d\n",obj->name, obj->procAttrs->codeAddress);
    printScope(obj->procAttrs->scope, indent + 4);
    break;
  case OBJ_PROGRAM:
    pad(indent);
    printf("Program %s at address %d\n",obj->name, obj->progAttrs->codeAddress);
    printScope(obj->progAttrs->scope, indent + 4);
    break;
  }
}

void printObjectList(ObjectNode* objList, int indent) { // Hàm này in ra một danh sách object
  ObjectNode *node = objList;
  while (node != NULL) {
    printObject(node->object, indent);
    printf("\n");
    node = node->next;
  }
}

void printScope(Scope* scope, int indent) { // Hàm này in ra một scope
  printObjectList(scope->objList, indent);
}

