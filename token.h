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

Token *tokenize(char *input);