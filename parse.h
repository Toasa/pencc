typedef enum enm_nd {
    ND_INT,    // 46, 101, ...

    ND_ADD,    // +
    ND_SUB,    // -
    ND_MUL,    // *
    ND_DIV,    // /

    ND_EQ,     // ==
    ND_NQ,     // !=
    ND_LT,     // <
    ND_LE,     // <=

    ND_IDENT,  // a, foo, ...
    ND_IF,     // if
    ND_WHILE,  // while
    ND_RETURN, // return

    ND_ASSIGN, // =
} NodeType;

typedef struct Node {
    NodeType type;

    struct Node *lhs;
    struct Node *rhs;

    int val;    // typeがND_INTのときに使用
    int offset; // typeがND_ASSIGNのときに使用

    struct Node *cond; // typeがND_IF、またはND_WHILEのときに使用
    struct Node *cons; // typeがND_IFのときに使用
    struct Node *alt;  // typeがND_IFのときに使用
    struct Node *expr; // typeがND_WHILEのときに使用
} Node;

typedef struct ps {
    Node **stmts;
    int identNum;
} ParsedData;

void printIdents();

ParsedData parse(Token *);