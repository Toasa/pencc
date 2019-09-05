typedef enum enm_tk {
    TK_INT,     // 46, 101, ...
    TK_ADD,     // +
    TK_SUB,     // -
    TK_MUL,     // *
    TK_DIV,     // /
    TK_LPARENT, // (
    TK_RPARENT, // )       
    TK_EOF,
} TokenType;

char *tokenTypes[8];

typedef struct Token {
    TokenType type;
    int val;
    struct Token *next;
} Token;

Token *tokenize(char *);

void printTokens(Token *);