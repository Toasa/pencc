#include <stdio.h>
#include <stdlib.h>

char *input;

void add();

void gen_assembly() {
    int n = num();
    printf("        mov rax, %d\n", n);
    add();
}

// 100+99
void add() {
    while (*input == '+' || *input == '-') {
        int m;
        if (*input++ == '+') {
            m = num();
            printf("        add rax, %d\n", m);
        } else {
            m = num();
            printf("        sub rax, %d\n", m);
        }
    }
}

int num() {
    int n = *input++ - '0';
    while (isDigit())
        n = 10 * n + (*input++ - '0');
    return n;
}

int isDigit() {
    if ('0' <= *input && *input <= '9')
        return 1;
    else 
        return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Need to 2 arguments\n");
        return 1;
    }

    printf(".intel_syntax noprefix\n");
    printf(".global main\n\n");
    printf("main:\n");

    input = argv[1];
    gen_assembly();

    printf("        ret\n");
    return 0;
}
