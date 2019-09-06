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

    ND_IDENT,  // a, b, ...

    ND_ASSIGN, // =
} NodeType;

typedef struct Node {
    NodeType type;

    int val; // typeがND_INTのときに使用
    char ident; // typeがND_ASSIGNのときに使用
    
    struct Node *lhs;
    struct Node *rhs;
} Node;

Node **parse(Token *);