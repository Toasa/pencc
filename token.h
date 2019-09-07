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
    TK_RETURN,    // return

    TK_SEMICOLON, // ;

    TK_EOF,
} TokenType;

char *tokenTypes[18];

typedef struct Token {
    TokenType type;
    int val;            // TokenTypeがTK_INTの場合、その値を表す、その他のTokenTypeでは0に固定
    char *ident;        // TokenTypeがTK_IDENTの場合、その識別子を表す
    int identLen;       // TokenTypeがTK_IDENTの場合、その識別子の長さを表す
    struct Token *next;
} Token;

Token *tokenize(char *);

void printTokens(Token *);