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

void printTokens(Token *t) {
    char *tokenTypes[6] = {
        "TK_INT",
        "TK_ADD",
        "TK_SUB",
        "TK_MUL",
        "TK_DIV"
        "TK_EOF",
    };
    while (t->type != TK_EOF) {
        printf("type %s, val %d\n", tokenTypes[t->type], t->val);
        t = t->next;
    }
    printf("type %s, val %d\n", tokenTypes[t->type], t->val);
}

Token *newToken(TokenType type, int val) {
    Token *t = malloc(sizeof(Token));
    t->type = type;
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
            if (*input == '+') {
                new_token = (Token *)newToken(TK_ADD, 0);
            } else if (*input == '-') {
                new_token = newToken(TK_SUB, 0);
            } else if (*input == '*') {
                new_token = newToken(TK_MUL, 0);
            } else if (*input == '/') {
                new_token = newToken(TK_DIV, 0);
            }
            input++;
        }
        cur_token->next = new_token;
        cur_token = new_token;
    }

    tail_token = newToken(TK_EOF, 0);
    cur_token->next = tail_token;

    return head_token;
}