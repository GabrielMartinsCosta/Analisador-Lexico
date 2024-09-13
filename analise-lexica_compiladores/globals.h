#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>

#define MAXRESERVED 10
#define MAXSYMBOLS 19

typedef enum {
    ENDFILE, ERROR, // TOKENS de Controle
    inteiro, real, se, entao, senao, enquanto, repita, ate, ler, mostrar, // Palavras reservadas
    ID, NUM, REAL, // TOKENS Multicaracteres
    PLUS, MINUS, TIMES, OVER, AND, OR, LT, LE, GT, GE, EQ, NE, ASSIGN, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE // Símbolos Especiais
} TokenType;

extern FILE *source; // arquivo de código fonte
extern FILE *listing; // arquivo com texto de saída
extern FILE *code; // arquivo de código para a máquina alvo

extern int line; // número da linha do código fonte
extern int EchoSource; // Se for TRUE faz o programa fonte ser impresso no arquivo listing com o número de linha durante a análise sintática
extern int TraceScan; // Se for TRUE faz a informação do TOKEN ser impressa quando cada TOKEN é reconhecido pela varredura
extern int TraceParse; // Se for TRUE faz a árvore sintática ser impressa de forma linearizada (identada por filho)
extern int TraceAnalyse; // Se for TRUE faz a inserção e a busca na tabela de símbolos serem mostradas no arquivo listing
extern int TraceCode; // Se for TRUE faz os comentários serem gravados no arquivo de código da máquina alvo quando o código é gerado
extern int Error; // Se for TRUE previne passadas futuras se ocorrer um erro

#endif //GLOBALS_H
