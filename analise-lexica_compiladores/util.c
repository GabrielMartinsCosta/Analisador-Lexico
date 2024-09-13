#include "globals.h"
#include "util.h"

// Imprime um TOKEN e seu lexema no arquivo "listing"
void printToken(TokenType token, const char* tokenString) {
    switch(token) {
        //inteiro, real, se, entao, senao, enquanto, repita, ate, ler, mostrar
        case inteiro:
        case real:
        case se:
        case entao:
        case senao:
        case enquanto:
        case repita:
        case ate:
        case ler:
        case mostrar:
            fprintf(listing, "Reserved word: %s\n", tokenString);
            break;
        case PLUS: fprintf(listing, "+\n"); break;
        case MINUS: fprintf(listing, "-\n"); break;
        case TIMES: fprintf(listing, "*\n"); break;
        case OVER: fprintf(listing, "/\n"); break;
        case AND: fprintf(listing, "&&\n"); break;
        case OR: fprintf(listing, "||\n"); break;
        case LT: fprintf(listing, "<\n"); break;
        case LE: fprintf(listing, "<=\n"); break;
        case GT: fprintf(listing, ">\n"); break;
        case GE: fprintf(listing, ">=\n"); break;
        case EQ: fprintf(listing, "==\n"); break;
        case NE: fprintf(listing, "!=\n"); break;
        case ASSIGN: fprintf(listing, "=\n"); break;
        case SEMI: fprintf(listing, ";\n"); break;
        case COMMA: fprintf(listing, ",\n"); break;
        case LPAREN: fprintf(listing, "(\n"); break;
        case RPAREN: fprintf(listing, ")\n"); break;
        case LBRACE: fprintf(listing, "{\n"); break;
        case RBRACE: fprintf(listing, "}\n"); break;
        case ENDFILE: fprintf(listing, "EOF\n"); break;
        case NUM: fprintf(listing, "NUM, value= %s\n", tokenString); break;
        case REAL: fprintf(listing, "REAL, value= %s\n", tokenString); break;
        case ID: fprintf(listing, "ID, name= %s\n", tokenString); break;
        case ERROR: fprintf(listing, "ERROR: %s\n", tokenString); break;
        default: fprintf(listing, "Unknown Token: %d\n", token); break; // Não deve acontecer nunca
    }
}
