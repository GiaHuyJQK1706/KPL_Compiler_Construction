/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "parser.h"
#include "reader.h"
#include "scanner.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
    Token *tmp = currentToken;
    currentToken = lookAhead;
    lookAhead = getValidToken();
    free(tmp);
}

void eat(TokenType tokenType) {
    if (lookAhead->tokenType == tokenType) {
        printToken(lookAhead);
        scan();
    } else
        missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
    assert("Parsing a Program ....");
    eat(KW_PROGRAM);
    eat(TK_IDENT);
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_PERIOD);
    assert("Program parsed!");
}

void compileBlock(void) {
    assert("Parsing a Block ....");
    if (lookAhead->tokenType == KW_CONST) {
        eat(KW_CONST);
        compileConstDecl();
        compileConstDecls();
        compileBlock2();
    } else
        compileBlock2();
    assert("Block parsed!");
}

void compileBlock2(void) {
    if (lookAhead->tokenType == KW_TYPE) {
        eat(KW_TYPE);
        compileTypeDecl();
        compileTypeDecls();
        compileBlock3();
    } else
        compileBlock3();
}

void compileBlock3(void) {
    if (lookAhead->tokenType == KW_VAR) {
        eat(KW_VAR);
        compileVarDecl();
        compileVarDecls();
        compileBlock4();
    } else
        compileBlock4();
}

void compileBlock4(void) {
    compileSubDecls();
    compileBlock5();
}

void compileBlock5(void) {
    eat(KW_BEGIN);
    compileStatements();
    eat(KW_END);
}

void compileConstDecls(void) {
    while (lookAhead->tokenType == TK_IDENT) compileConstDecl();
}

void compileConstDecl(void) {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileConstant();
    eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
    while (lookAhead->tokenType == TK_IDENT)
        compileTypeDecl();
}

void compileTypeDecl(void) {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileType();
    eat(SB_SEMICOLON);
}

void compileVarDecls(void) {
    while (lookAhead->tokenType == TK_IDENT)
        compileVarDecl();
}

void compileVarDecl(void) {
    eat(TK_IDENT);
    eat(SB_COLON);
    compileType();
    eat(SB_SEMICOLON);
}

void compileSubDecls(void) {
    assert("Parsing subroutines ....");
    switch (lookAhead->tokenType) {
        case KW_FUNCTION:
            compileFuncDecl();
            compileSubDecls();
            break;
        case KW_PROCEDURE:
            compileProcDecl();
            compileSubDecls();
            break;
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
            break;
    }
    assert("Subroutines parsed ....");
}

void compileFuncDecl(void) {
    assert("Parsing a function ....");
    eat(KW_FUNCTION);
    eat(TK_IDENT);
    compileParams();
    eat(SB_COLON);
    compileBasicType();
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_SEMICOLON);
    assert("Function parsed ....");
}

void compileProcDecl(void) {
    assert("Parsing a procedure ....");
    eat(KW_PROCEDURE);
    eat(TK_IDENT);
    compileParams();
    eat(SB_SEMICOLON);
    compileBlock();
    eat(SB_SEMICOLON);
    assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
    // TODO
}

void compileConstant(void) {
    // TODO
}

void compileConstant2(void) {
    // TODO
}

void compileType(void) {
    switch (lookAhead->tokenType) {
        case KW_INTEGER:
            eat(KW_INTEGER);
            break;
        case KW_CHAR:
            eat(KW_CHAR);
            break;
        case KW_ARRAY:
            eat(KW_ARRAY);
            eat(SB_LSEL);
            eat(TK_NUMBER);
            eat(SB_RSEL);
            eat(KW_OF);
            compileType();
            break;
        case TK_IDENT:
            eat(TK_IDENT);
            break;
        default:
            error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileBasicType(void) {
    // TODO
}

void compileParams(void) {
    // TODO
}

void compileParams2(void) {
    // TODO
}

void compileParam(void) {
    // TODO
}

void compileStatements(void) {
    compileStatement();
    compileStatements2();
}

void compileStatements2(void) {
    if (lookAhead->tokenType == SB_SEMICOLON) {
        eat(SB_SEMICOLON);
        compileStatement();
        compileStatements2();
    }
}

void compileStatement(void) {
    switch (lookAhead->tokenType) {
        case TK_IDENT:
            compileAssignSt();
            break;
        case KW_CALL:
            compileCallSt();
            break;
        case KW_BEGIN:
            compileGroupSt();
            break;
        case KW_IF:
            compileIfSt();
            break;
        case KW_WHILE:
            compileWhileSt();
            break;
        case KW_FOR:
            compileForSt();
            break;
            // EmptySt needs to check FOLLOW tokens
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
            break;
            // Error occurs
        default:
            error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileAssignSt(void) {
    assert("Parsing an assign statement ....");
    eat(TK_IDENT);
    if (lookAhead->tokenType == SB_LSEL) {
        compileIndexes();
    }
    eat(SB_ASSIGN);
    compileExpression();
    assert("Assign statement parsed ....");
}

void compileCallSt(void) {
    assert("Parsing a call statement ....");
    eat(KW_CALL);
    eat(TK_IDENT);
    compileArguments();
    assert("Call statement parsed ....");
}

void compileGroupSt(void) {
    assert("Parsing a group statement ....");
    eat(KW_BEGIN);
    compileStatements();
    eat(KW_END);
    assert("Group statement parsed ....");
}

void compileIfSt(void) {
    assert("Parsing an if statement ....");
    eat(KW_IF);
    compileCondition();
    eat(KW_THEN);
    compileStatement();
    if (lookAhead->tokenType == KW_ELSE) compileElseSt();
    assert("If statement parsed ....");
}

void compileElseSt(void) {
    eat(KW_ELSE);
    compileStatement();
}

void compileWhileSt(void) {
    assert("Parsing a while statement ....");
    eat(KW_WHILE);
    compileCondition();
    eat(KW_DO);
    compileStatement();
    assert("While statement parsed ....");
}

void compileForSt(void) {
    assert("Parsing a for statement ....");
    eat(KW_FOR);
    eat(TK_IDENT);
    eat(SB_ASSIGN);
    compileExpression();
    eat(KW_TO);
    compileExpression();
    eat(KW_DO);
    compileStatement();
    assert("For statement parsed ....");
}

void compileArguments(void) {
    switch (lookAhead->tokenType) {
        case SB_LPAR:
            eat(SB_LPAR);
            compileExpression();
            compileArguments2();
            eat(SB_RPAR);
            break;
        // Follow - same as call statement as statement:
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
        // Follow - term2
        case SB_TIMES:
        case SB_SLASH:
        // Follow - expression3
        // Follow (For statement)
        case KW_TO:
        case KW_DO:
        // Follow (arguments2)
        case SB_COMMA:
        // Follow (condition2)
        case SB_EQ:
        case SB_NEQ:
        case SB_LE:
        case SB_LT:
        case SB_GE:
        case SB_GT:
        // Follow (factor)
        case SB_RPAR:
        // Follow (indexes)
        case SB_RSEL:
        // Follow (if statement)
        case KW_THEN:
            break;
        // Error
        default:
            error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileArguments2(void) {
    switch (lookAhead->tokenType) {
        case SB_COMMA:
            eat(SB_COMMA);
            compileExpression();
            compileArguments2();
            break;
        // Follow
        case SB_RPAR:
            break;
        // Error:
        default:
            error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
            break;
    }
}

void compileCondition(void) {
    // TODO
}

void compileCondition2(void) {
    // TODO
}

void compileExpression(void) {
    assert("Parsing an expression");
    switch (lookAhead->tokenType) {
        case SB_PLUS:
            eat(SB_PLUS);
            compileExpression2();
            break;
        case SB_MINUS:
            eat(SB_MINUS);
            compileExpression2();
            break;
        default:
            compileExpression2();
            break;
    }
    assert("Expression parsed");
}

void compileExpression2(void) {
    compileTerm();
    compileExpression3();
}

void compileExpression3(void) {
    switch (lookAhead->tokenType) {
        case SB_PLUS:
            eat(SB_PLUS);
            compileTerm();
            compileExpression3();
            break;
        case SB_MINUS:
            eat(SB_MINUS);
            compileTerm();
            compileExpression3();
            break;
            // check the FOLLOW set
        case KW_TO:
        case KW_DO:
        case SB_RPAR:
        case SB_COMMA:
        case SB_EQ:
        case SB_NEQ:
        case SB_LE:
        case SB_LT:
        case SB_GE:
        case SB_GT:
        case SB_RSEL:
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
        case KW_THEN:
            break;
        default:
            error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
    }
}

void compileTerm(void) {
    compileFactor();
    compileTerm2();
}

void compileTerm2(void) {
    switch (lookAhead->tokenType) {
        case SB_TIMES:
            eat(SB_TIMES);
            compileFactor();
            compileTerm2();
            break;
        case SB_SLASH:
            eat(SB_SLASH);
            compileFactor();
            compileTerm2();
            break;
            // check the FOLLOW set
        case SB_PLUS:
        case SB_MINUS:
        case KW_TO:
        case KW_DO:
        case SB_RPAR:
        case SB_COMMA:
        case SB_EQ:
        case SB_NEQ:
        case SB_LE:
        case SB_LT:
        case SB_GE:
        case SB_GT:
        case SB_RSEL:
        case SB_SEMICOLON:
        case KW_END:
        case KW_ELSE:
        case KW_THEN:
            break;
        default:
            error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
    }
}

void compileFactor(void) {
    // TODO
}

void compileIndexes(void) {
    while (lookAhead->tokenType == SB_LSEL) {
        eat(SB_LSEL);
        compileExpression();
        eat(SB_RSEL);
    }
}

int compile(char *fileName) {
    if (openInputStream(fileName) == IO_ERROR) return IO_ERROR;

    currentToken = NULL;
    lookAhead = getValidToken();
    compileProgram();
    free(currentToken);
    free(lookAhead);
    closeInputStream();
    return IO_SUCCESS;
}