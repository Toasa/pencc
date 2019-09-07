#include "util.h"
#include "token.h"

char *tokenTypes[21] = {
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
    [TK_IF]        = "TK_IF",
    [TK_ELSE]      = "TK_ELSE",
    [TK_WHILE]     = "TK_WHILE",
    [TK_RETURN]    = "TK_RETURN",
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

bool isChar(char c) {
    if (('a' <= c) && (c <= 'z') ||
        ('A' <= c) && (c <= 'Z'))
    {
        return true;
    }
    return false;
}

TokenType matchKeyword(char *name) {
    if (strcmp(name, "return") == 0) {
        return TK_RETURN;
    } else if (strcmp(name, "if") == 0) {
        return TK_IF;
    } else if (strcmp(name, "else") == 0) {
        return TK_ELSE;
    } else if (strcmp(name, "while") == 0) {
        return TK_WHILE;
    }
    return -1;
}

int num() {
    int n = *input++ - '0';
    while (isDigit())
        n = 10 * n + (*input++ - '0');
    return n;
}

int identLength() {
    int i = 1;
    while (isChar(*(input+i))) {
        i++;
    }
    return i;
}

void skip() {
    while (*input == ' ' || *input == '\t' || *input == '\n') {
        input++;
    }
}

void readIdentName(char *ident_buf, int len) {
    if (len >= 50) {
        error("Too long identifier name :(");
    }
    int i;
    for (i = 0; i < len; i++) {
        ident_buf[i] = input[i];
    }
    ident_buf[i] = '\0';
}

// debug用
void printTokens(Token *t) {
    while (t->type != TK_EOF) {
        if (t->type == TK_IDENT) {
            printf("type: %s, name: %s\n", tokenTypes[t->type], t->ident);
        } else if (t->type == TK_INT) {
            printf("type: %s, val: %d\n", tokenTypes[t->type], t->val);
        } else {
            printf("type: %s\n", tokenTypes[t->type]);
        }
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

Token *newIdentToken(char *name, int len) {
    Token *t = malloc(sizeof(Token));
    t->type = TK_IDENT;
    t->ident = name;
    t->identLen = len;
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
        } else if (isChar(*input)) {
            int len = identLength();
            
            // 変数名の長さは最大50文字（末尾にNULLを代入する）
            char *ident_buf = malloc(sizeof(char) * len + 1);
            readIdentName(ident_buf, len);

            // マッチすれば、keywordのTokenTypeを返す
            // そうでなければ、-1を返す
            TokenType tt = matchKeyword(ident_buf);
            if ((int)tt >= 0) {                
                new_token = newToken(tt, 0);
            } else {                
                new_token = newIdentToken(ident_buf, len);
            }
            input += len;
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