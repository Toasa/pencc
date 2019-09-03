#include "util.h"
#include "token.h"

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

Token *tokenize(char *input_) {
    input = input_;
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