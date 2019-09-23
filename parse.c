#include "util.h"
#include "token.h"
#include "parse.h"

Token *token;
StringChain *strings;

// parseProgram()で各function literalparseし、
// parse結果として得られる各function nodeを
// funcsの要素とし、格納する
FuncData *funcs[100];

Node *parseEqual();
Node *parseStatement();
Node *parseExpression();

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

Node *newFuncNode(NodeType type, char *ident, int ident_len) {
    Node *n = newNode(type, 0, NULL, NULL);
    n->func = malloc(sizeof(char) * ident_len + 1);
    strcpy(n->func, ident);
    return n;
}

bool curTokenTypeIs(TokenType type) {
    return token->type == type;
}

bool nextTokenTypeIs(TokenType type) {
    if (curTokenTypeIs(TK_EOF)) {
        error("Currrent token is TK_EOF, not exist next token");
    }
    return token->next->type == type;
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

Node *parseIdent() {
    char *ident = token->ident;
    int ident_len = token->identLen;
    nextToken();
    Node *n = newIdentNode(ident, ident_len);
    return n;
}

void parseFunctionArgs(Node *n) {
    eatToken(TK_LPARENT);

    Node *cur = malloc(sizeof(Node));
    n->next = cur;

    int args_num = 0;
    while (!curTokenTypeIs(TK_RPARENT)) {
        Node *next;
        if (n->type == ND_CALL) {
            next = parseExpression();
        } else if (n->type == ND_FUNC) {
            next = parseIdent();
        } else {
            error("ND_CALL or ND_FUNC Only");
        }
        
        cur->next = next;
        cur = next;
        if (curTokenTypeIs(TK_COMMA)) {
            nextToken();
        }
        args_num++;
    }
    n->next = n->next->next;
    n->argsNum = args_num;

    eatToken(TK_RPARENT);
}

Node *parseFunctionName(Node *n, NodeType type) {
    char *ident = token->ident;
    int ident_len = token->identLen;
    n = newFuncNode(type, ident, ident_len);
    eatToken(TK_IDENT);
    return n;
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
    } else if (curTokenTypeIs(TK_IDENT)) {
        if (nextTokenTypeIs(TK_LPARENT)) {
            // function call
            n = parseFunctionName(n, ND_CALL);
            parseFunctionArgs(n);
        } else {
            // identifier
            n = parseIdent();
        }
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
    while (token->type == TK_MUL || token->type == TK_DIV
        || token->type == TK_REM) {
        if (token->type == TK_MUL) {
            nextToken();
            lhs = newNode(ND_MUL, 0, lhs, parseUnary());
        } else if (token->type == TK_DIV) {
            nextToken();
            lhs = newNode(ND_DIV, 0, lhs, parseUnary());
        } else if (token->type == TK_REM) {
            nextToken();
            lhs = newNode(ND_REM, 0, lhs, parseUnary());
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
    } else if (curTokenTypeIs(TK_ADD_ASSIGN)) {
        nextToken();
        Node *rhs = parseEqual();
        rhs = newNode(ND_ADD, 0, lhs, rhs);
        lhs = newNode(ND_ASSIGN, 0, lhs, rhs);
    } else if (curTokenTypeIs(TK_SUB_ASSIGN)) {
        nextToken();
        Node *rhs = parseEqual();
        rhs = newNode(ND_SUB, 0, lhs, rhs);
        lhs = newNode(ND_ASSIGN, 0, lhs, rhs);
    } else if (curTokenTypeIs(TK_MUL_ASSIGN)) {
        nextToken();
        Node *rhs = parseEqual();
        rhs = newNode(ND_MUL, 0, lhs, rhs);
        lhs = newNode(ND_ASSIGN, 0, lhs, rhs);
    } else if (curTokenTypeIs(TK_DIV_ASSIGN)) {
        nextToken();
        Node *rhs = parseEqual();
        rhs = newNode(ND_DIV, 0, lhs, rhs);
        lhs = newNode(ND_ASSIGN, 0, lhs, rhs);
    } else if (curTokenTypeIs(TK_REM_ASSIGN)) {
        nextToken();
        Node *rhs = parseEqual();
        rhs = newNode(ND_REM, 0, lhs, rhs);
        lhs = newNode(ND_ASSIGN, 0, lhs, rhs);
    }
    return lhs;
}

Node *parseExpression() {
    return parseAssign();
}

Node *parseIfStatement() {
    Node *n = malloc(sizeof(Node));
    n->type = ND_IF;
    nextToken();
    eatToken(TK_LPARENT);
    n->cond = parseExpression();
    eatToken(TK_RPARENT);
    n->cons = parseStatement();
    if (curTokenTypeIs(TK_ELSE)) {
        nextToken();
        n->alt = parseStatement();
    }
    return n;
}

Node *parseReturnStatement() {
    Node *n = malloc(sizeof(Node));
    nextToken();
    n->expr = parseExpression();
    n->type = ND_RETURN;
    eatToken(TK_SEMICOLON);
    return n;
}

Node *parseWhileStatement() {
    nextToken();
    Node *n = malloc(sizeof(Node));
    n->type = ND_WHILE;
    eatToken(TK_LPARENT);
    n->cond = parseExpression();
    eatToken(TK_RPARENT);
    n->expr = parseStatement();
    return n;
}

Node *parseForStatement() {
    nextToken();
    Node *n = malloc(sizeof(Node));
    n->type = ND_FOR;
    eatToken(TK_LPARENT);
    n->init = parseExpression();
    eatToken(TK_SEMICOLON);
    n->cond = parseExpression();
    eatToken(TK_SEMICOLON);
    n->post = parseExpression();
    eatToken(TK_RPARENT);
    n->expr = parseStatement();
    return n;
}

Node *parseBlockStatement() {
    nextToken();
    Node *n = malloc(sizeof(Node));
    n->type = ND_BLOCK;

    Node *cur = malloc(sizeof(Node));
    n->next = cur;

    while (!curTokenTypeIs(TK_RBRACE)) {
        Node *next = parseStatement();
        cur->next = next;
        cur = next;
    }

    eatToken(TK_RBRACE);

    n->next = n->next->next;
    return n;
}

Node *parseStatement() {
    Node *n;
    if (curTokenTypeIs(TK_RETURN)) {
        n = parseReturnStatement();
    } else if (curTokenTypeIs(TK_IF)) {
        n = parseIfStatement();
    } else if (curTokenTypeIs(TK_WHILE)) {
        n = parseWhileStatement();
    } else if (curTokenTypeIs(TK_FOR)) {
        n = parseForStatement();
    } else if (curTokenTypeIs(TK_LBRACE)) {
        n = parseBlockStatement();
    } else {
        n = parseExpression();
        eatToken(TK_SEMICOLON);
    }
    return n;
}

Node *parseFunctionLiteral() {
    Node *n;

    // ident
    n = parseFunctionName(n, ND_FUNC);

    // args
    // 関数の引数もその関数内のローカル変数として、処理する
    parseFunctionArgs(n);

    // block
    n->expr = parseBlockStatement();

    return n;
}

void parseProgram() {
    int i = 0;
    while (!curTokenTypeIs(TK_EOF)) {
        strings = initStringChain();
        funcs[i] = malloc(sizeof(FuncData));
        funcs[i]->topLevelFunc  = parseFunctionLiteral();
        funcs[i]->identNum = strings->total;
        i++;
    }
    funcs[i] = NULL;
}
 
FuncData **parse(Token *token_) {
    token = token_;
    parseProgram();
    return funcs;
}