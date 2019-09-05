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
        printf("        pop rdi\n");
        printf("        pop rax\n");
        if (n->type == ND_ADD) {
            printf("        add rax, rdi\n");
        } else if (n->type == ND_SUB) {
            printf("        sub rax, rdi\n");
        } else if (n->type == ND_MUL) {
            printf("        mul rdi\n");
        } else if (n->type == ND_DIV) {
            printf("        cqo\n");
            // idivはdivの符号ありバージョン
            printf("        idiv rdi\n");
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