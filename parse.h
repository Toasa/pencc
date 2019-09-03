typedef enum enm_nd {
    ND_INT, // 46, 101, ...
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
} NodeType;

typedef struct Node {
    int val;
    NodeType type;
    struct Node *lhs;
    struct Node *rhs;
} Node;

Node *parse(Token *);