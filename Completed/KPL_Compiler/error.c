/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

#define NUM_OF_ERRORS 29

/*
  Cấu trúc dữ liệu lưu trữ thông tin lỗi
  Mỗi lỗi bao gồm một mã lỗi và một thông điệp
  Cần cập nhật số lượng lỗi khi thêm một lỗi mới
  Cần cập nhật thông điệp khi thêm một lỗi mới
*/
struct ErrorMessage {
  ErrorCode errorCode;
  char *message;
};

// Mảng lưu trữ thông tin lỗi
struct ErrorMessage errors[29] = {
  {ERR_END_OF_COMMENT, "End of comment expected."},
  {ERR_IDENT_TOO_LONG, "Identifier too long."},
  {ERR_INVALID_CONSTANT_CHAR, "Invalid char constant."},
  {ERR_INVALID_SYMBOL, "Invalid symbol."},
  {ERR_INVALID_IDENT, "An identifier expected."},
  {ERR_INVALID_CONSTANT, "A constant expected."},
  {ERR_INVALID_TYPE, "A type expected."},
  {ERR_INVALID_BASICTYPE, "A basic type expected."},
  {ERR_INVALID_VARIABLE, "A variable expected."},
  {ERR_INVALID_FUNCTION, "A function identifier expected."},
  {ERR_INVALID_PROCEDURE, "A procedure identifier expected."},
  {ERR_INVALID_PARAMETER, "A parameter expected."},
  {ERR_INVALID_STATEMENT, "Invalid statement."},
  {ERR_INVALID_COMPARATOR, "A comparator expected."},
  {ERR_INVALID_EXPRESSION, "Invalid expression."},
  {ERR_INVALID_TERM, "Invalid term."},
  {ERR_INVALID_FACTOR, "Invalid factor."},
  {ERR_INVALID_LVALUE, "Invalid lvalue in assignment."},
  {ERR_INVALID_ARGUMENTS, "Wrong arguments."},
  {ERR_UNDECLARED_IDENT, "Undeclared identifier."},
  {ERR_UNDECLARED_CONSTANT, "Undeclared constant."},
  {ERR_UNDECLARED_INT_CONSTANT, "Undeclared integer constant."},
  {ERR_UNDECLARED_TYPE, "Undeclared type."},
  {ERR_UNDECLARED_VARIABLE, "Undeclared variable."},
  {ERR_UNDECLARED_FUNCTION, "Undeclared function."},
  {ERR_UNDECLARED_PROCEDURE, "Undeclared procedure."},
  {ERR_DUPLICATE_IDENT, "Duplicate identifier."},
  {ERR_TYPE_INCONSISTENCY, "Type inconsistency"},
  {ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY, "The number of arguments and the number of parameters are inconsistent."}
};

void error(ErrorCode err, int lineNo, int colNo) { // Hàm này in ra thông báo lỗi ở dòng lineNo, cột colNo
  int i;
  for (i = 0 ; i < NUM_OF_ERRORS; i ++) 
    if (errors[i].errorCode == err) {
      printf("%d-%d:%s\n", lineNo, colNo, errors[i].message);
      exit(0);
    }
}

void missingToken(TokenType tokenType, int lineNo, int colNo) { // Hàm này in ra thông báo thiếu token trong code
  printf("%d-%d:Missing %s\n", lineNo, colNo, tokenToString(tokenType));
  exit(0);
}

void assert(char *msg) { // Hàm này in ra thông báo lỗi
  printf("%s\n", msg);
}
