typedef enum enm_tk {
    TK_INT,        // 46, 101, ...

    TK_ADD,        // +
    TK_SUB,        // -
    TK_MUL,        // *
    TK_DIV,        // /
    TK_REM,        // %
    TK_LSHIFT,     // <<
    TK_RSHIFT,     // >>

    TK_LPARENT,    // (
    TK_RPARENT,    // )
    TK_LBRACE,     // {
    TK_RBRACE,     // }

    TK_EQ,         // ==
    TK_NQ,         // !=
    TK_LT,         // <
    TK_LE,         // <=
    TK_GT,         // >
    TK_GE,         // >=

    TK_IDENT,      // 識別子
    TK_ASSIGN,     // =

    TK_ADD_ASSIGN, // +=
    TK_SUB_ASSIGN, // -=
    TK_MUL_ASSIGN, // *=
    TK_DIV_ASSIGN, // /=
    TK_REM_ASSIGN, // %=

    TK_INC,        // ++
    TK_DEC,        // --

    TK_IF,         // if
    TK_ELSE,       // else
    TK_WHILE,      // while
    TK_FOR,        // for
    TK_RETURN,     // return

    TK_SEMICOLON,  // ;
    TK_COMMA,      // ,

    TK_EOF,
} TokenType;

char *tokenTypes[35];

typedef struct Token {
    TokenType type;
    int val;            // TokenTypeがTK_INTの場合、その値を表す、その他のTokenTypeでは0に固定
    char *ident;        // TokenTypeがTK_IDENTの場合、その識別子を表す
    int identLen;       // TokenTypeがTK_IDENTの場合、その識別子の長さを表す
    struct Token *next;
} Token;

Token *tokenize(char *);

void printTokens(Token *);