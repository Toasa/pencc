#include "util.h"
#include "token.h"
#include "parse.h"

Token *token;

Node *parseAdd();

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
    if (token->type != TK_INT && token->type != TK_LPARENT) {
        error("expected TK_INT or TK_LPARENT, but got %s", tokenTypes[token->type]);
    }

    Node *n;
    if (curTokenTypeIs(TK_INT)) {
        int val = token->val;
        nextToken();
        n = newIntNode(val);
    } else {
        eatToken(TK_LPARENT);
        n = parseAdd();
        eatToken(TK_RPARENT);
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

Node *parse(Token *token_) {
    token = token_;
    Node *node = parseAdd();
    return node;
}