#pragma warning(disable: 6001)
#pragma warning(disable: 6011)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];
int state;
int ln, cn;
char str[MAX_IDENT_LEN];
char c;
/***************************************************************/


Token* getToken(void) {
	Token* token = (Token*)malloc(sizeof(Token));
	switch (state) {
	case 0:
		if (currentChar == EOF) state = 1;
		else
			switch (charCodes[currentChar])
			{
			case CHAR_SPACE:
				state = 2; break;
			case CHAR_LETTER:
				ln = lineNo;
				cn = colNo;
				state = 3;
				break;
			case CHAR_DIGIT:
				state = 7;
				break;
			case CHAR_PLUS:
				state = 9;
				break;
			case CHAR_MINUS:
				state = 10;
				break;
			case CHAR_TIMES:
				state = 11;
				break;
			case CHAR_SLASH:
				state = 12;
				break;
			case CHAR_LT:
				state = 13;
				break;
			case CHAR_GT:
				state = 16;
				break;
			case CHAR_EQ:
				state = 19;
				break;
			case CHAR_EXCLAIMATION:
				state = 20;
				break;
			case CHAR_COMMA:
				state = 23;
				break;
			case CHAR_PERIOD:
				state = 24;
				break;
			case CHAR_SEMICOLON:
				state = 27;
				break;
			case CHAR_COLON:
				state = 28;
				break;
			case CHAR_SINGLEQUOTE:
				state = 31;
				break;
			case CHAR_LPAR:
				state = 35;
				break;
			case CHAR_RPAR:
				state = 42;
				break;
			default:
				state = 43;
			}
		return getToken();
	case 1:
		return makeToken(TK_EOF, lineNo, colNo);
	case 2:
		readChar();
		state = 0;
		return getToken();
	case 3:
	{
		ln = lineNo;
		cn = colNo;
		int count = 1;
		str[0] = (char)currentChar;
		readChar();
		while ((currentChar != EOF) &&
			((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT)))
		{
			if (count <= MAX_IDENT_LEN)
			{
				str[count++] = (char)currentChar;
			}
			readChar();
		}
		if (count > MAX_IDENT_LEN)
		{
			error(ERR_IDENT_TOO_LONG, ln, cn);
			return makeToken(TK_NONE, ln, cn);
		}
		str[count] = '\0';
		state = 4;
		return getToken();
	}
	case 4:
		token->tokenType = checkKeyword(str);
		if (token->tokenType == TK_NONE) state = 5; else state = 6;
		return getToken();
	case 5:
		token = makeToken(TK_IDENT, ln, cn);
		strcpy(token->string, str);
		state = 0;
		return token;
	case 6:
		token = makeToken(checkKeyword(str), ln, cn);
		state = 0;
		return token;
	case 7:
		token = makeToken(TK_NONE, lineNo, colNo);
		int index = 0;
		char string[11];
		ln = lineNo;
		cn = colNo;
		while (currentChar == '0')
			readChar();
		while (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT)
		{
			if (index >= 10)
			{
				while (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT)
				{
					index++;
					readChar();
				}
				error(ERR_NUMBER_TOO_LONG, ln, cn);
				return token;
				break;
			}
			string[index] = currentChar;
			index++;
			readChar();
		}
		if (index == 0)
		{
			token->tokenType = TK_NUMBER;
			token->value = 0;
			token->string[0] = '0';
			token->string[1] = '\0';
			state = 0;

			return token;
		}

		string[index] = '\0';
		char strNumber[11];
		sprintf(strNumber, "%d", INT_MAX);
		if (strlen(string) == strlen(strNumber) && strcmp(string, strNumber) > 0)
		{
			error(ERR_NUMBER_TOO_LONG, ln, cn);
			return token;
		}
		token->tokenType = TK_NUMBER;
		strcpy(token->string, string);
		token->value = atoi(string);
		state = 0;
		return token;
	case 9:
		readChar();
		state = 0;
		return makeToken(SB_PLUS, lineNo, colNo - 1);
	case 10:
		readChar();
		state = 0;
		return makeToken(SB_MINUS, lineNo, colNo - 1);
	case 11:
		readChar();
		state = 0;
		return makeToken(SB_TIMES, lineNo, colNo - 1);
	case 12:
		readChar();
		state = 0;
		return makeToken(SB_SLASH, lineNo, colNo - 1);
	case 13:
		readChar();
		if (charCodes[currentChar] == CHAR_EQ) state = 14; else state = 15;
		return getToken();
	case 14:
		readChar();
		state = 0;
		return makeToken(SB_LE, lineNo, colNo - 1);
	case 15:
		state = 0;
		return makeToken(SB_LT, lineNo, colNo - 1);
	case 16:
		readChar();
		if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
			state = 17;
		else
			state = 18;
		return getToken();
	case 17:
		readChar();
		state = 0;
		return makeToken(SB_GE, lineNo, colNo - 1);
	case 18:
		state = 0;
		return makeToken(SB_GT, lineNo, colNo - 1);
	case 19:
		readChar();
		state = 0;
		return makeToken(SB_EQ, lineNo, colNo - 1);
	case 20:
		readChar();
		if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
			state = 21;
		else
			state = 22;
		return getToken();
	case 21:
		readChar();
		state = 0;
		return makeToken(SB_NEQ, lineNo, colNo - 1);
	case 22:
		token = makeToken(TK_NONE, lineNo, colNo - 1);
		error(ERR_INVALID_SYMBOL, token->lineNo, token->colNo);
		return token;
	case 23:
		readChar();
		state = 0;
		return makeToken(SB_COMMA, lineNo, colNo - 1);
	case 24:
		readChar();
		if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_RPAR))
			state = 25;
		else
			state = 26;
		return getToken();
	case 25:
		readChar();
		state = 0;
		return makeToken(SB_RSEL, lineNo, colNo - 1);
	case 26:
		readChar();
		state = 0;
		return makeToken(SB_PERIOD, lineNo, colNo - 1);
	case 27:
		ln = lineNo;
		cn = colNo;
		readChar();
		if (ln != lineNo)
		{
			state = 0;
			return makeToken(SB_SEMICOLON, ln, cn);
		}

		state = 0;
		return makeToken(SB_SEMICOLON, lineNo, colNo - 1);
	case 28:
		readChar();
		if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
			state = 29;
		else
			state = 30;
		return getToken();
	case 29:
		readChar();
		state = 0;
		return makeToken(SB_ASSIGN, lineNo, colNo - 1);
	case 30:
		state = 0;
		return makeToken(SB_COLON, lineNo, colNo - 1);
	case 31:
		readChar();
		if (currentChar == EOF)
			state = 34;
		else
			if (isprint(currentChar))
				state = 32;
			else state = 34;
		return getToken();
	case 32:
		c = currentChar;
		readChar();
		if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
			state = 33;
		else
			state = 34;
		return getToken();
	case 33:
		token = makeToken(TK_CHAR, lineNo, colNo - 1);
		token->string[0] = c;
		token->string[1] = '\0';
		readChar();
		state = 0;
		return token;
	case 34:
		error(ERR_INVALID_CONSTANT_CHAR, lineNo, colNo - 2);
	case 35:
		ln = lineNo;
		cn = colNo;
		readChar();
		if (currentChar == EOF)
			state = 41;
		else
			switch (charCodes[currentChar])
			{
			case CHAR_PERIOD:
				state = 36;
				break;
			case CHAR_TIMES:
				state = 38;
				break;
			default:state = 41;
			}
		return getToken();
	case 36:
		readChar();
		state = 0;
		return makeToken(SB_LSEL, ln, cn);
	case 37:
		while ((currentChar != EOF) && (charCodes[currentChar] != CHAR_TIMES))
		{
			state = 37;
			readChar();
		}
		if (currentChar == EOF)
			state = 40;
		else state = 38;
		return getToken();
	case 38:
		while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_TIMES))
		{
			state = 38;
			readChar();
		}
		if (currentChar == EOF)
			state = 40;
		else
			if (charCodes[currentChar] == CHAR_RPAR) state = 39;
			else state = 37;
		return getToken();
	case 39:
		state = 0;
		readChar();
		return getToken();
	case 40:
		error(ERR_END_OF_COMMENT, lineNo, colNo);
	case 41:
		state = 0;
		return makeToken(SB_LPAR, ln, cn);
	case 42:
		ln = lineNo;
		cn = colNo;
		readChar();
		if (ln != lineNo)
		{
			state = 0;
			return makeToken(SB_RPAR, ln, cn);

		}
		state = 0;
		return makeToken(SB_RPAR, lineNo, colNo - 1);
	case 43:
		token = makeToken(TK_NONE, lineNo, colNo);
		error(ERR_INVALID_SYMBOL, lineNo, colNo);
		readChar();
		state = 0;
		return token;
	}
}

Token* getValidToken(void) {
	state=0;
	Token *token = getToken();
	while (token->tokenType == TK_NONE) {
		free(token);
		token = getToken();
	} 
	return token;
}

/******************************************************************/

void printToken(Token* token) {

	printf("%d-%d:", token->lineNo, token->colNo);

	switch (token->tokenType) {
	case TK_NONE: printf("TK_NONE\n"); break;
	case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
	case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
	case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
	case TK_EOF: printf("TK_EOF\n"); break;
	case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
	case KW_CONST: printf("KW_CONST\n"); break;
	case KW_TYPE: printf("KW_TYPE\n"); break;
	case KW_VAR: printf("KW_VAR\n"); break;
	case KW_INTEGER: printf("KW_INTEGER\n"); break;
	case KW_CHAR: printf("KW_CHAR\n"); break;
	case KW_ARRAY: printf("KW_ARRAY\n"); break;
	case KW_OF: printf("KW_OF\n"); break;
	case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
	case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
	case KW_BEGIN: printf("KW_BEGIN\n"); break;
	case KW_END: printf("KW_END\n"); break;
	case KW_CALL: printf("KW_CALL\n"); break;
	case KW_IF: printf("KW_IF\n"); break;
	case KW_THEN: printf("KW_THEN\n"); break;
	case KW_ELSE: printf("KW_ELSE\n"); break;
	case KW_WHILE: printf("KW_WHILE\n"); break;
	case KW_DO: printf("KW_DO\n"); break;
	case KW_FOR: printf("KW_FOR\n"); break;
	case KW_TO: printf("KW_TO\n"); break;
	case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
	case SB_COLON: printf("SB_COLON\n"); break;
	case SB_PERIOD: printf("SB_PERIOD\n"); break;
	case SB_COMMA: printf("SB_COMMA\n"); break;
	case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
	case SB_EQ: printf("SB_EQ\n"); break;
	case SB_NEQ: printf("SB_NEQ\n"); break;
	case SB_LT: printf("SB_LT\n"); break;
	case SB_LE: printf("SB_LE\n"); break;
	case SB_GT: printf("SB_GT\n"); break;
	case SB_GE: printf("SB_GE\n"); break;
	case SB_PLUS: printf("SB_PLUS\n"); break;
	case SB_MINUS: printf("SB_MINUS\n"); break;
	case SB_TIMES: printf("SB_TIMES\n"); break;
	case SB_SLASH: printf("SB_SLASH\n"); break;
	case SB_LPAR: printf("SB_LPAR\n"); break;
	case SB_RPAR: printf("SB_RPAR\n"); break;
	case SB_LSEL: printf("SB_LSEL\n"); break;
	case SB_RSEL: printf("SB_RSEL\n"); break;
	}
}
