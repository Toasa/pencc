#include <stdio.h>
#include <stdlib.h>

typedef enum enm {
    TK_INT,
    TK_ADD,
    TK_SUB,
    TK_EOF,
} TokenKind;

typedef struct Token {
    TokenKind kind;
    int val;
    struct Token *next;
} Token;

char *input;

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

void skip() {
    while (*input == ' ') {
        input++;
    }
}

Token *newToken(TokenKind kind, int val) {
    Token *t = malloc(sizeof(Token));
    t->kind = kind;
    t->val = val;
    return t;
}

Token *tokenize() {
    skip();

    Token *head_token = malloc(sizeof(Token));
    Token *cur_token = malloc(sizeof(Token));
    Token *new_token = malloc(sizeof(Token));
    Token *tail_token = malloc(sizeof(Token));

    head_token = newToken(TK_INT, num());
    cur_token = head_token;

    while (*input) {
        skip();
        if (isDigit()) {
            new_token = newToken(TK_INT, num());
        } else {
            if (*input++ == '+') {
                new_token = (Token *)newToken(TK_ADD, 0);
            } else {
                new_token = newToken(TK_SUB, 0);
            }
        }
        cur_token->next = new_token;
        cur_token = new_token;
    }

    tail_token = newToken(TK_EOF, 0);
    cur_token->next = tail_token;

    return head_token;
}

void gen_assembly(Token *t) {
    printf("        mov rax, %d\n", t->val);

    for (t = t->next; t->kind != TK_EOF; t = t->next) {
        if (t->kind == TK_ADD) {
            t = t->next;
            printf("        add rax, %d\n", t->val);
        } else {
            t = t->next;
            printf("        sub rax, %d\n", t->val);
        }        
    }
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
    Token *token_head = tokenize();

    gen_assembly(token_head);

    printf("        ret\n");
    return 0;
}
