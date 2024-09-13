#ifndef SCAN_H
#define SCAN_H

#include "globals.h"

#define MAXTOKENLENGTH 50 // Tamanho máximo de um TOKEN
extern char tokenString[MAXTOKENLENGTH + 1]; // Armazena o lexema de cada TOKEN

TokenType getToken(void); // Retorna o próximo TOKEN do arquivo fonte

#endif
