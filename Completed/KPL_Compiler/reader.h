/* 
 * @copyright (c) 2008, 2025 Hedspi, SoICT - HUST
 * @author Huu-Duc Nguyen, Gia-Huy Do
 * @version 1.1
 */

#ifndef __READER_H__
#define __READER_H__

#define IO_ERROR 0
#define IO_SUCCESS 1

int readChar(void);
int openInputStream(char *fileName);
void closeInputStream(void);

#endif
