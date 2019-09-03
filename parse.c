#include "util.h"
#include "token.h"
#include "parse.h"

Token *token;

Node *newNode(NodeType type, int val, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->val = val;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

void expect(TokenType type) {
    if (token->type != type) {
        error("invalid token type");
    }
}

Node *parseNum() {
    expect(TK_INT);
    int val = token->val;
    token = token->next;
    return newNode(ND_INT, val, NULL, NULL);
}

Node *parseMul() {
    Node *lhs = parseNum();
    while (token->type == TK_MUL || token->type == TK_DIV) {
        if (token->type == TK_MUL) {
            token = token->next;
            lhs = newNode(ND_MUL, 0, lhs, parseNum());
        } else {
            token = token->next;
            lhs = newNode(ND_DIV, 0, lhs, parseNum());
        }
    }
    return lhs;
}

Node *parseAdd() {
    Node *lhs = parseMul();
    while (token->type == TK_ADD || token->type == TK_SUB) {
        if (token->type == TK_ADD) {
            token = token->next;
            lhs = newNode(ND_ADD, 0, lhs, parseMul());
        } else {
            token = token->next;
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