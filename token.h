typedef enum enm_tk {
    TK_INT,       // 46, 101, ...

    TK_ADD,       // +
    TK_SUB,       // -
    TK_MUL,       // *

    TK_DIV,       // /
    TK_LPARENT,   // (
    TK_RPARENT,   // )

    TK_EQ,        // ==
    TK_NQ,        // !=
    TK_LT,        // <
    TK_LE,        // <=
    TK_GT,        // >
    TK_GE,        // >=

    TK_IDENT,     // 識別子

    TK_ASSIGN,    // =

    TK_SEMICOLON, // ;

    TK_EOF,
} TokenType;

char *tokenTypes[17];

typedef struct Token {
    TokenType type;
    int val;            // TokenTypeがTK_INTの場合、その値を表す、その他のTokenTypeでは0に固定
    struct Token *next;
} Token;

Token *tokenize(char *);

void printTokens(Token *);