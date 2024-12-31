#include <stdio.h>
#include "reader.h"

FILE *inputStream;
int lineNo, colNo;
int currentChar;

int readChar(void) { // Hàm này đọc một ký tự từ file đầu vào và trả về ký tự đó
  currentChar = getc(inputStream);
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  return currentChar;
}

int openInputStream(char *fileName) { // Hàm này mở file đầu vào và trả về IO_ERROR nếu không mở được file, IO_SUCCESS nếu mở file thành công
  inputStream = fopen(fileName, "rt");
  if (inputStream == NULL)
    return IO_ERROR;
  lineNo = 1;
  colNo = 0;
  readChar();
  return IO_SUCCESS;
}

void closeInputStream() { // Hàm này đóng file đầu vào
  fclose(inputStream);
}

