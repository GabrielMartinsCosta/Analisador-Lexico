#include "util.h"
#include "scan.h"

int line = 0;
FILE *source;
FILE *listing;
FILE *code;

int EchoSource = 0;
int TraceScan = 1;
int TraceParse = 0;
int TraceAnalyze = 0;
int TraceCode = 0;

void main() {

    TokenType token;

    if ((source = fopen("sample.pm", "r")) == NULL) {
        perror("Abertura de sample.pm: ");
        return;
    }
    if ((listing = fopen("listing.txt", "w")) == NULL) {
        perror("Abertura de listing.txt: ");
        return;
    }
    printf("Executando analise lexica de \"sample.pm\"...");
    token = getToken();
    while (token != ENDFILE) {
        token = getToken();
    }
    printf("\nConcluido.\nResultado em \"listing.txt\".\n\n");
    fclose(source);
    fclose(listing);
}
