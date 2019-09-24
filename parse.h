typedef enum enm_nd {
    ND_INT,     // 46, 101, ...

    ND_ADD,     // +
    ND_SUB,     // -
    ND_MUL,     // *
    ND_DIV,     // /
    ND_REM,     // %
    ND_LSHIFT,  // >>
    ND_RSHIFT,  // <<

    ND_EQ,      // ==
    ND_NQ,      // !=
    ND_LT,      // <
    ND_LE,      // <=

    ND_PREINC,  // ++x
    ND_PREDEC,  // --x
    ND_POSTINC, // x++
    ND_POSTDEC, // x--

    ND_IDENT,   // a, foo, ...
    ND_IF,      // if
    ND_WHILE,   // while
    ND_FOR,     // for
    ND_RETURN,  // return
    ND_BLOCK,   // {...; ...; ...;}

    ND_FUNC,    // foo() { ...; } (function literal)
    ND_CALL,    // foo() (function call)

    ND_ASSIGN,  // =
} NodeType;

typedef struct Node {
    NodeType type;

    struct Node *lhs;
    struct Node *rhs;

    int val;    // typeがND_INTのときに使用
    int offset; // typeがND_ASSIGNのときに使用

    struct Node *cond; // typeがND_IF、ND_WHILE、ND_FORのときに使用
    struct Node *cons; // typeがND_IFのときに使用
    struct Node *alt;  // typeがND_IFのときに使用
    struct Node *expr; // typeがND_RETURN, ND_WHILE, ND_FOR, ND_FUNCのときに使用
                       // ND_FUNCでは関数のbodyを表す
    struct Node *init; // typeがND_FORのときに使用
    struct Node *post; // typeがND_FORのときに使用
    struct Node *next; // typeがND_BLOCK, ND_CALLのときに使用
                       // ND_CALLでは引数を表す

    char *func; // typeがND_CALL, ND_FUNCのときに使用。関数名を表す
    int argsNum; // typeがND_CALLのときに使用。引数の個数を表す
} Node;

typedef struct func {
    struct Node *topLevelFunc;
    int identNum;
} FuncData;

void printIdents();

FuncData **parse(Token *);