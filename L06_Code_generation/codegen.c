#include <stdio.h>
#include "reader.h"
#include "codegen.h"  

#define CODE_SIZE 10000

// Các biến toàn cục
extern SymTab* symtab;

extern Object* readiFunction;
extern Object* readcFunction;
extern Object* writeiProcedure;
extern Object* writecProcedure;
extern Object* writelnProcedure;

// Các biến toàn cục
CodeBlock* codeBlock;

int computeNestedLevel(Scope* scope) { // Hàm này tính toán mức lồng của một scope
  int level = 0;
  Scope* tmp = symtab->currentScope;
  while (tmp != scope) {
    tmp = tmp->outer;
    level ++;
  }
  return level;
}

void genVariableAddress(Object* var) { // Hàm này sinh mã để lấy địa chỉ của một biến
  int level = computeNestedLevel(VARIABLE_SCOPE(var));
  int offset = VARIABLE_OFFSET(var);
  genLA(level, offset);
}

void genVariableValue(Object* var) { // Hàm này sinh mã để lấy giá trị của một biến
  int level = computeNestedLevel(VARIABLE_SCOPE(var));
  int offset = VARIABLE_OFFSET(var);
  genLV(level, offset);
}

void genParameterAddress(Object* param) { // Hàm này sinh mã để lấy địa chỉ của một tham số
  int level = computeNestedLevel(PARAMETER_SCOPE(param));
  int offset = PARAMETER_OFFSET(param);
  genLA(level, offset);
}

void genParameterValue(Object* param) { // Hàm này sinh mã để lấy giá trị của một tham số
  int level = computeNestedLevel(PARAMETER_SCOPE(param));
  int offset = PARAMETER_OFFSET(param);
  genLV(level, offset);
}

void genReturnValueAddress(Object* func) { // Hàm này sinh mã để lấy địa chỉ của giá trị trả về của một hàm
  int level = computeNestedLevel(FUNCTION_SCOPE(func));
  int offset = RETURN_VALUE_OFFSET;
  genLA(level, offset);
}

void genReturnValueValue(Object* func) { // Hàm này sinh mã để lấy giá trị của giá trị trả về của một hàm
  int level = computeNestedLevel(FUNCTION_SCOPE(func));
  int offset = RETURN_VALUE_OFFSET;
  genLV(level, offset);
}

void genPredefinedProcedureCall(Object* proc) { // Hàm này sinh mã để gọi một thủ tục built-in
  if (proc == writeiProcedure)
    genWRI();
  else if (proc == writecProcedure)
    genWRC();
  else if (proc == writelnProcedure)
    genWLN();
}

void genProcedureCall(Object* proc) { // Hàm này sinh mã để gọi một thủ tục
  int level = computeNestedLevel(proc->procAttrs->scope->outer);
  genCALL(level, proc->procAttrs->codeAddress);
}

void genPredefinedFunctionCall(Object* func) { // Hàm này sinh mã để gọi một hàm built-in
  if (func == readiFunction)
    genRI();
  else if (func == readcFunction)
    genRC();
}

void genFunctionCall(Object* func) { // Hàm này sinh mã để gọi một hàm
  int level = computeNestedLevel(func->funcAttrs->scope->outer);
  genCALL(level, func->funcAttrs->codeAddress);
}

void genLA(int level, int offset) { // Hàm này sinh mã để lấy địa chỉ
  emitLA(codeBlock, level, offset);
}

void genLV(int level, int offset) { // Hàm này sinh mã để lấy giá trị
  emitLV(codeBlock, level, offset);
}

void genLC(WORD constant) { // Hàm này sinh mã để lấy một hằng
  emitLC(codeBlock, constant);
}

void genLI(void) { // Hàm này sinh mã để lấy giá trị từ địa chỉ
  emitLI(codeBlock);
}

void genINT(int delta) { // Hàm này sinh mã để tăng cấp frame
  emitINT(codeBlock,delta);
}

void genDCT(int delta) { // Hàm này sinh mã để giảm cấp frame
  emitDCT(codeBlock,delta);
}

Instruction* genJ(CodeAddress label) { // Hàm này sinh mã để nhảy
  Instruction* inst = codeBlock->code + codeBlock->codeSize;
  emitJ(codeBlock,label);
  return inst;
}

Instruction* genFJ(CodeAddress label) { // Hàm này sinh mã để nhảy nếu sai
  Instruction* inst = codeBlock->code + codeBlock->codeSize;
  emitFJ(codeBlock, label);
  return inst;
}

void genHL(void) { // Hàm này sinh mã để lưu giá trị từ đỉa chỉ
  emitHL(codeBlock);
}

void genST(void) { // Hàm này sinh mã để lưu giá trị
  emitST(codeBlock);
}

void genCALL(int level, CodeAddress label) { // Hàm này sinh mã để gọi một hàm
  emitCALL(codeBlock, level, label);
}

void genEP(void) { // Hàm này sinh mã để kết thúc chương trình
  emitEP(codeBlock);
}

void genEF(void) { // Hàm này sinh mã để kết thúc hàm
  emitEF(codeBlock);
}

void genRC(void) { // Hàm này sinh mã để đọc một ký tự
  emitRC(codeBlock);
}

void genRI(void) { // Hàm này sinh mã để đọc một số
  emitRI(codeBlock);
}

void genWRC(void) { // Hàm này sinh mã để in một ký tự
  emitWRC(codeBlock);
}

void genWRI(void) { // Hàm này sinh mã để in một số
  emitWRI(codeBlock);
}

void genWLN(void) { // Hàm này sinh mã để in xuống dòng
  emitWLN(codeBlock);
}

void genAD(void) { // Hàm này sinh mã để cộng
  emitAD(codeBlock);
}

void genSB(void) { // Hàm này sinh mã để trừ
  emitSB(codeBlock);
}

void genML(void) { // Hàm này sinh mã để nhân
  emitML(codeBlock);
}

void genDV(void) { // Hàm này sinh mã để chia
  emitDV(codeBlock);
}

void genNEG(void) { // Hàm này sinh mã để đảo dấu
  emitNEG(codeBlock);
}

void genCV(void) { // Hàm này sinh mã để copy giá trị
  emitCV(codeBlock);
}

void genEQ(void) { // Hàm này sinh mã để so sánh bằng
  emitEQ(codeBlock);
}

void genNE(void) { // Hàm này sinh mã để so sánh khác
  emitNE(codeBlock);
}

void genGT(void) { // Hàm này sinh mã để so sánh lớn hơn
  emitGT(codeBlock);
}

void genGE(void) { // Hàm này sinh mã để so sánh lớn hơn hoặc bằng
  emitGE(codeBlock);
}

void genLT(void) { // Hàm này sinh mã để so sánh nhỏ hơn
  emitLT(codeBlock);
}

void genLE(void) { // Hàm này sinh mã để so sánh nhỏ hơn hoặc bằng
  emitLE(codeBlock);
}

void updateJ(Instruction* jmp, CodeAddress label) { // Hàm này cập nhật mã nhảy
  jmp->q = label;
}

void updateFJ(Instruction* jmp, CodeAddress label) { // Hàm này cập nhật mã nhảy nếu sai
  jmp->q = label;
}

CodeAddress getCurrentCodeAddress(void) { // Hàm này trả về địa chỉ hiện tại của mã
  return codeBlock->codeSize;
}

int isPredefinedFunction(Object* func) { // Hàm này kiểm tra xem một function có phải là built-in không
  return ((func == readiFunction) || (func == readcFunction));
}

int isPredefinedProcedure(Object* proc) { // Hàm này kiểm tra xem một procedure có phải là built-in không
  return ((proc == writeiProcedure) || (proc == writecProcedure) || (proc == writelnProcedure));
}

void initCodeBuffer(void) { // Hàm này khởi tạo bộ đệm mã
  codeBlock = createCodeBlock(CODE_SIZE);
}

void printCodeBuffer(void) { // Hàm này in bộ đệm mã
  printCodeBlock(codeBlock);
}

void cleanCodeBuffer(void) { // Hàm này giải phóng bộ đệm mã
  freeCodeBlock(codeBlock);
}

int serialize(char* fileName) { // Hàm này lưu bộ đệm mã vào file
  FILE* f;

  f = fopen(fileName, "wb");
  if (f == NULL) return IO_ERROR;
  saveCode(codeBlock, f);
  fclose(f);
  return IO_SUCCESS;
}
