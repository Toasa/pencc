#include "util.h"
#include "token.h"

char *tokenTypes[17] = {
    [TK_INT]       = "TK_INT",
    [TK_ADD]       = "TK_ADD",
    [TK_SUB]       = "TK_SUB",
    [TK_MUL]       = "TK_MUL",
    [TK_DIV]       = "TK_DIV",
    [TK_LPARENT]   = "TK_LPARENT",
    [TK_RPARENT]   = "TK_RPARENT",
    [TK_EQ]        = "TK_EQ",
    [TK_NQ]        = "TK_NQ",
    [TK_LT]        = "TK_LT",
    [TK_LE]        = "TK_LE",
    [TK_GT]        = "TK_GT",
    [TK_GE]        = "TK_GE",
    [TK_IDENT]     = "TK_IDENT",
    [TK_ASSIGN]    = "TK_ASSIGN",
    [TK_SEMICOLON] = "TK_SEMICOLON",
    [TK_EOF]       = "TK_EOF",
};

char *input;

bool isDigit() {
    if (('0' <= *input) && (*input <= '9'))
        return true;
    else 
        return false;
}

bool isChar() {
    if (('a' <= *input) && (*input <= 'z') ||
        ('A' <= *input) && (*input <= 'Z'))
    {
        return true;
    }
    return false;
}

int num() {
    int n = *input++ - '0';
    while (isDigit())
        n = 10 * n + (*input++ - '0');
    return n;
}

void skip() {
    while (*input == ' ') {
        input++;
    }
}

void printTokens(Token *t) {
    while (t->type != TK_EOF) {
        printf("type: %s, val: %d\n", tokenTypes[t->type], t->val);
        t = t->next;
    }
    printf("type: %s, val: %d\n", tokenTypes[t->type], t->val);
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

    // 先頭の番兵
    Token *head_token = malloc(sizeof(Token));
    Token *cur_token = malloc(sizeof(Token));
    Token *new_token = malloc(sizeof(Token));
    Token *tail_token = malloc(sizeof(Token));

    head_token = cur_token;

    while (*input) {
        skip();
        if (isDigit()) {
            new_token = newToken(TK_INT, num());
        } else if (isChar()) {
            new_token = newToken(TK_IDENT, *input++);
        } else {
            if (*input == '+') {
                new_token = (Token *)newToken(TK_ADD, 0);
            } else if (*input == '-') {
                new_token = newToken(TK_SUB, 0);
            } else if (*input == '*') {
                new_token = newToken(TK_MUL, 0);
            } else if (*input == '/') {
                new_token = newToken(TK_DIV, 0);
            } else if (*input == '(') {
                new_token = newToken(TK_LPARENT, 0);
            } else if (*input == ')') {
                new_token = newToken(TK_RPARENT, 0);
            } else if (*input == '<') {
                if (*(input+1) == '=') {
                    new_token = newToken(TK_LE, 0);
                    input++;
                } else {
                    new_token = newToken(TK_LT, 0);
                }
            } else if (*input == '>') {
                if (*(input+1) == '=') {
                    new_token = newToken(TK_GE, 0);
                    input++;
                } else {
                    new_token = newToken(TK_GT, 0);
                }
            } else if (*input == '=') {
                if (*(input+1) == '=') {
                    new_token = newToken(TK_EQ, 0);
                    input++;
                } else {
                    new_token = newToken(TK_ASSIGN, 0);                    
                }
            } else if (*input == '!') {
                if (*(input+1) == '=') {
                    new_token = newToken(TK_NQ, 0);
                    input++;
                }
            } else if (*input == ';') {
                new_token = newToken(TK_SEMICOLON, 0);
            }
            input++;
        }
        cur_token->next = new_token;
        cur_token = new_token;
    }

    tail_token = newToken(TK_EOF, 0);
    cur_token->next = tail_token;

    return head_token->next;
}