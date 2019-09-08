#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "parse.h"
#include "gen_x86.h"

char *input;

void setInput(int len, char *src) {
    input = malloc(sizeof(char) * len + 10);
    strcpy(input, src);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Need to 2 arguments\n");
        return 1;
    }

    int input_len = strlen(argv[1]);
    setInput(input_len, argv[1]);
    
    Token *token_head = tokenize(input);
    ParsedData ps = parse(token_head);
    genAssembly(ps);

    return 0;
}