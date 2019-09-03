#include "util.h"
#include "token.h"
#include "parse.h"
#include "gen_x86.h"

void walkAST(Node *n) {
    if (n->lhs) { 
        walkAST(n->lhs);
    }
    if (n->rhs) {
        walkAST(n->rhs);
    }
    if (n->type == ND_INT) {
        printf("        push %d\n", n->val);
    } else {
        printf("        pop rdx\n");
        printf("        pop rax\n");
        if (n->type == ND_ADD) {
            printf("        add rax, rdx\n");
        } else if (n->type == ND_SUB) {
            printf("        sub rax, rdx\n");
        } else if (n->type == ND_MUL) {
            printf("        mul rdx\n");
        }
        printf("        push rax\n");
    }
}

void gen_assembly(Node *n) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n\n");
    printf("main:\n");

    walkAST(n);

    printf("        pop rax\n");
    printf("        ret\n");
}