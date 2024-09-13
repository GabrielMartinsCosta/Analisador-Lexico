#include "globals.h"
#include "scan.h"
#include "util.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Estados do DFA para análise léxica
typedef enum {
    START, INNUM1, INNUM, INREAL, INID, INAND, INOR, INLESS, INGREATER, INASSIGN, INNOTEQUAL, INDIV, INCOMMENT, INPAREN,
    DONE
} StateType;

// Lexema para Identificador ou Palavra reservada
char tokenString[MAXTOKENLENGTH + 1];

// BUFLEN = tamanho do buffer de entrada para linhas de código fonte
#define BUFLEN 256

static char lineBuf[BUFLEN]; // Linha atual
static int linePos = 0; // Posição no lineBuf
static int bufSize = 0; // Tamanho atual da string no Buffer
static int EOF_flag = 0; // Corrige o comportamento de ungetNextChar no EOF

// getNextChar pega o próximo caractere não branco no lineBuf, lendo uma nova linha se lineBuf estiver todo lido
static int getNextChar(void) {
    if (!(linePos < bufSize)) {
        line++;
        if (fgets(lineBuf, BUFLEN - 1, source)) {
            if (EchoSource) {
                fprintf(listing, "%4d: %s", line, lineBuf);
            }
            bufSize = strlen(lineBuf);
            linePos = 0;
            return lineBuf[linePos++];
        } else {
            EOF_flag = 1;
            return EOF;
        }
    } else {
        return lineBuf[linePos++];
    }
};

// unGetNextChar retrocede um caractere em lineeBuf
static void ungetNextChar(void) {
    if (!EOF_flag) {
        linePos--;
    }
}

// Tabela de busca de palavras reservadas
static struct {
    char *str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"inteiro", inteiro}, {"real", real}, {"se", se}, {"entao", entao}, {"senao", senao},
    {"enquanto", enquanto}, {"repita", repita}, {"ate", ate}, {"ler", ler}, {"mostrar", mostrar}
};

static struct {
    char symbol;
    TokenType tok;
} symbolTable[MAXSYMBOLS] = {
    {'+', PLUS}, {'-', MINUS}, {'*', TIMES}, {'/', OVER},
    {'<', LT}, {'>', GT},
    {'=', ASSIGN}, {';', SEMI}, {',', COMMA},
    {'(', LPAREN}, {')', RPAREN}, {'{', LBRACE}, {'}', RBRACE}
};

// Verifica se um identificador é uma palavra reservada
static TokenType reservedLookup(char *s) {
    for (int i = 0; i < MAXRESERVED; i++) {
        if (!strcmp(s, reservedWords[i].str)) {
            return reservedWords[i].tok;
        }
    }
    return ID;
}

static TokenType getTokenBySymbol(int symbol) {
    for (int i = 0; i < MAXSYMBOLS; i++) {
        if ((char) symbol == symbolTable[i].symbol) {
            return symbolTable[i].tok;
        }
    }
    return -1;
}

// getToken retorna o próximo token no arquivo fonte
TokenType getToken(void) {
    int tokenStringIndex = 0; // Índice para armazenamento em tokenString
    TokenType currentToken; // Armazena o TOKEN atual
    StateType state = START; // Estado autal

    int save; // Flag para indicar se o caractere corrente deve ser salvo em tokenString
    while (state != DONE) {
        int c = getNextChar();
        int previousChar = ' ';
        save = 1;
        switch (state) {
            case START:
                if (isdigit(c))
                    state = INNUM;
                else if (isalpha(c))
                    state = INID;
                else if (c == '&')
                    state = INAND;
                else if (c == '|')
                    state = INOR;
                else if (c == '<')
                    state = INLESS;
                else if (c == '>')
                    state = INGREATER;
                else if (c == '=')
                    state = INASSIGN;
                else if (c == '!')
                    state = INNOTEQUAL;
                else if (c == '/') {
                    save = 0;
                    state = INDIV;
                } else if (c == ' ' || c == '\t' || c == '\n')
                    save = 0;
                else {
                    state = DONE;
                    switch (c) {
                        case EOF:
                            save = 0;
                            currentToken = ENDFILE;
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            int nextChar = getNextChar();
                            if (!isdigit(previousChar) && isdigit(nextChar)) {
                                ungetNextChar();
                                state = INNUM;
                            }
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        case ',':
                            currentToken = COMMA;
                            break;
                        case '(':
                            currentToken = LPAREN;
                            break;
                        case ')':
                            currentToken = RPAREN;
                            break;
                        case '{':
                            currentToken = LBRACE;
                            break;
                        case '}':
                            currentToken = RBRACE;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;
            // case INNUM1:
            //     if (isdigit(c)) {
            //         state = INNUM;
            //         currentToken = NUM;
            //     } else {
            //         state = DONE;
            //         if (c == '+')
            //             currentToken = PLUS;
            //         else if (c == '-')
            //             currentToken = MINUS;
            //     }
            //     break;
            case INNUM:
                if (c == '.') {
                    state = INREAL;
                } else {
                    if (!isdigit(c)) {
                        ungetNextChar();
                        save = 0;
                        state = DONE;
                        currentToken = NUM;
                    }
                }
                break;
            case INREAL:
                if (!isdigit(c)) {
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = REAL;
                }
                break;
            case INID:
                if (!isalpha(c)) {
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = ID;
                }
                break;
            case INLESS:
                state = DONE;
                if (c == '=') {
                    currentToken = LE;
                } else {
                    ungetNextChar();
                    save = 0;
                    currentToken = LT;
                }
                break;
            case INGREATER:
                state = DONE;
                if (c == '=') {
                    currentToken = GE;
                } else {
                    ungetNextChar();
                    save = 0;
                    currentToken = GT;
                }
                break;
            case INASSIGN:
                state = DONE;
                currentToken = ASSIGN;
                if (c == '=') {
                    currentToken = EQ;
                }
                break;
            case INNOTEQUAL:
                if (c != '=') {
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = NE;
                }
                break;
            case INAND:
                if (c != '&') {
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = AND;
                }
                break;
            case INOR:
                if (c != '|') {
                    ungetNextChar();
                    save = 0;
                    state = DONE;
                    currentToken = OR;
                }
                break;
            case INDIV:
                if (c == '*') {
                    save = 0;
                    state = INCOMMENT;
                } else {
                    state = DONE;
                    currentToken = OVER;
                }
                break;
            case INCOMMENT:
                save = 0;
                if (c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                } else if (c == '/') {
                    state = START;
                }
                break;
            case DONE:
            default:
                fprintf(listing, "Scanner Bug: state= %d\n", state);
                state = DONE;
                currentToken = ERROR;
                break;
        }
        if ((save) && (tokenStringIndex <= MAXTOKENLENGTH))
            tokenString[tokenStringIndex++] = (char) c;
        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
        if (isdigit(c))
            previousChar = c;
    }
    if (TraceScan) {
        fprintf(listing, "\t%d: ", line);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}
