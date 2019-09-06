#include "util.h"
#include "token.h"
#include "parse.h"

Token *token;
StringChain *strings;

// parseProgram()で各statementをparseし、
// parse結果として得られる各statement nodeを
// stmtsの要素とし、格納する
Node *stmts[100];

// debug用
void printIdents() {
    StringChain *sc = strings->next;
    int i = 0;
    while (sc) {
        printf("IDENT: %s, OFFSET: %d\n", sc->str.str, (i + 1) * 8);

        sc = sc->next;
        i++;
    }
}

Node *parseEqual();

Node *newNode(NodeType type, int val, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->val = val;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *newIntNode(int val) {
    return newNode(ND_INT, val, NULL, NULL);
}

Node *newIdentNode(char *ident, int ident_len) {
    Node *n = newNode(ND_IDENT, 0, NULL, NULL);
    String str = newString(ident, ident_len);
    n->offset = getStringOffset(strings, str);
    return n;
}

bool curTokenTypeIs(TokenType type) {
    return token->type == type;
}

void expect(TokenType type) {
    if (!curTokenTypeIs(type)) {
        error("expect %s, but got %s", tokenTypes[type], tokenTypes[token->type]);
    }
}

void nextToken() {
    if (curTokenTypeIs(TK_EOF)) {
        error("Currrent token is TK_EOF, not exist next token");
    }
    token = token->next;
}

void eatToken(TokenType type) {
    expect(type);
    nextToken();
}

Node *parseNum() {
    if (!curTokenTypeIs(TK_INT) && !curTokenTypeIs(TK_LPARENT) && !curTokenTypeIs(TK_IDENT)) {
        error("expected TK_INT or TK_LPARENT or TK_IDENT, but got %s", tokenTypes[token->type]);
    }

    Node *n;
    if (curTokenTypeIs(TK_INT)) {
        int val = token->val;
        nextToken();
        n = newIntNode(val);
    } else if (curTokenTypeIs(TK_LPARENT)) {
        eatToken(TK_LPARENT);
        n = parseEqual();
        eatToken(TK_RPARENT);
    } else {
        char *ident = token->ident;
        int ident_len = token->identLen;
        nextToken();
        n = newIdentNode(ident, ident_len);
    }
    
    return n;
}

Node *parseUnary() {
    if (curTokenTypeIs(TK_SUB)) {
        eatToken(TK_SUB);
        Node *zero = newIntNode(0);
        Node *n = newNode(ND_SUB, 0, zero, parseNum());
        return n;
    }
 
    if (curTokenTypeIs(TK_ADD)) {
        nextToken();
    }
    return parseNum();
}

Node *parseMul() {
    Node *lhs = parseUnary();
    while (token->type == TK_MUL || token->type == TK_DIV) {
        if (token->type == TK_MUL) {
            nextToken();
            lhs = newNode(ND_MUL, 0, lhs, parseUnary());
        } else {
            nextToken();
            lhs = newNode(ND_DIV, 0, lhs, parseUnary());
        }
    }
    return lhs;
}

Node *parseAdd() {
    Node *lhs = parseMul();
    while (token->type == TK_ADD || token->type == TK_SUB) {
        if (token->type == TK_ADD) {
            nextToken();
            lhs = newNode(ND_ADD, 0, lhs, parseMul());
        } else {
            nextToken();        
            lhs = newNode(ND_SUB, 0, lhs, parseMul());
        }
    }
    return lhs;
}

Node *parseComp() {
    Node *lhs = parseAdd();
    while (curTokenTypeIs(TK_LT) || curTokenTypeIs(TK_LE) ||
           curTokenTypeIs(TK_GT) || curTokenTypeIs(TK_GE))
    {
        if (curTokenTypeIs(TK_LT)) {
            nextToken();
            lhs = newNode(ND_LT, 0, lhs, parseAdd());
        } else if (curTokenTypeIs(TK_LE)) {
            nextToken();
            lhs = newNode(ND_LE, 0, lhs, parseAdd());
        } else if (curTokenTypeIs(TK_GT)) {
            nextToken();
            lhs = newNode(ND_LT, 0, parseAdd(), lhs);
        } else if (curTokenTypeIs(TK_GE)) {
            nextToken();
            lhs = newNode(ND_LE, 0, parseAdd(), lhs);
        }
    }
    return lhs;
}

Node *parseEqual() {
    Node *lhs = parseComp();
    while (curTokenTypeIs(TK_EQ) || curTokenTypeIs(TK_NQ)) {
        if (curTokenTypeIs(TK_EQ)) {
            nextToken();
            lhs = newNode(ND_EQ, 0, lhs, parseComp());
        } else {
            nextToken();
            lhs = newNode(ND_NQ, 0, lhs, parseComp());
        }
    }
    return lhs;
}

Node *parseAssign() {
    Node *lhs = parseEqual();
    if (curTokenTypeIs(TK_ASSIGN)) {
        nextToken();
        lhs = newNode(ND_ASSIGN, 0, lhs, parseAssign());
    }
    return lhs;
}

Node *parseExpression() {
    return parseAssign();
}

Node *parseStatement() {
    Node *n = parseExpression();
    eatToken(TK_SEMICOLON);
    return n;
}

void parseProgram() {
    int i = 0;
    while (!curTokenTypeIs(TK_EOF)) {
        stmts[i++]  = parseStatement();
    }
    stmts[i] = NULL;
}
 
ParsedData parse(Token *token_) {
    token = token_;
    strings = initStringChain();
    parseProgram();

    ParsedData pd;
    pd.stmts = stmts;
    pd.identNum = strings->total;
    return pd;
}