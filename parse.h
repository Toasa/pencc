typedef enum enm_nd {
    ND_INT, // 46, 101, ...

    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /

    ND_EQ,  // ==
    ND_NQ,  // !=
    ND_LT,  // <
    ND_LE,  // <=
} NodeType;

typedef struct Node {
    int val;
    NodeType type;
    struct Node *lhs;
    struct Node *rhs;
} Node;

Node *parse(Token *);