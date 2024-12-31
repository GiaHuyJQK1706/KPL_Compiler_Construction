#ifndef __READER_H__
#define __READER_H__

#define IO_ERROR 0
#define IO_SUCCESS 1

int readChar(void);
int openInputStream(char *fileName);
void closeInputStream(void);

#endif
