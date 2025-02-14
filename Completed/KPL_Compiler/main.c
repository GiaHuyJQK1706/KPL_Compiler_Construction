/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "parser.h"
#include "codegen.h"


int dumpCode = 0;

/*
  printfUsage: in ra hướng dẫn sử dụng chương trình
  Cú pháp: kplc input output [-dump]
  Với: 
    input: file chứa chương trình kpl
    output: file chứa chương trình thực thi
    -dump: in ra mã chương trình
  -dump là tùy chọn
*/
void printUsage(void) {
  printf("Usage: kplc input output [-dump]\n");
  printf("   input: input kpl program\n");
  printf("   output: executable\n");
  printf("   -dump: code dump\n");
}

/*
  analyseParam: phân tích tham số dòng lệnh
  -dump: in ra mã chương trình
*/
int analyseParam(char* param) {
  if (strcmp(param, "-dump") == 0) {
    dumpCode = 1;
    return 1;
  } 
  return 0;
}


/******************************************************************/

int main(int argc, char *argv[]) { // Hàm main
  int i; 

  /*
    Kiểm tra số lượng tham số dòng lệnh
    Nếu không đủ tham số, in ra thông báo và kết thúc chương trình
  */

  if (argc <= 1) {
    printf("kplc: no input file.\n");
    printUsage();
    return -1;
  }

  if (argc <= 2) {
    printf("kplc: no output file.\n");
    printUsage();
    return -1;
  }

  // Kiểm tra các tham số dòng lệnh
  for ( i = 3; i < argc; i ++) 
    analyseParam(argv[i]);

  // Khởi tạo bộ đệm mã
  initCodeBuffer();

  // Biên dịch chương trình
  // Nếu không đọc được file đầu vào, in ra thông báo và kết thúc chương trình
  if (compile(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }

  // Lưu bộ đệm mã vào file
  // Nếu không ghi được file đầu ra, in ra thông báo và kết thúc chương trình
  if (serialize(argv[2]) == IO_ERROR) {
    printf("Can\'t write output file!\n");
    return -1;
  }

  // In ra bộ đệm mã nếu có tùy chọn -dump
  if (dumpCode) printCodeBuffer();
  
  // Giải phóng bộ đệm mã
  cleanCodeBuffer();

  return 0;
}
