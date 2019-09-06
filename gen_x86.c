#include "util.h"
#include "token.h"
#include "parse.h"
#include "gen_x86.h"

void genExpr(Node *n);

int calcOffset(char c) {
    return (c - 'a' + 1) * 8;
}

void genIdent(Node *n) {
    char ident = n->ident;
    int offset = calcOffset(ident);
    printf("        mov rax, [rbp - %d]\n", offset);
    printf("        push rax\n");
}

void genAssign(Node *n) {
    genExpr(n->rhs);
    char ident = n->lhs->ident;
    int offset = calcOffset(ident);
    printf("        pop rax\n");
    printf("        mov [rbp - %d], rax\n", offset);
    printf("        push rax\n");
}

void genExpr(Node *n) {
    if (n->lhs) { 
        genExpr(n->lhs);
    }
    if (n->rhs) {
        genExpr(n->rhs);
    }

    if (n->type == ND_INT) {
        printf("        push %d\n", n->val);
    } else if (n->type == ND_IDENT) {
        genIdent(n);
    } else if (n->type == ND_ASSIGN) {
        genAssign(n);
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
        } else if (n->type == ND_EQ) {
            printf("        cmp rax, rdi\n");
            printf("        sete al\n");
            printf("        movzb rax, al\n");
        } else if (n->type == ND_NQ) {
            printf("        cmp rax, rdi\n");
            printf("        setne al\n");
            printf("        movzb rax, al\n");
        } else if (n->type == ND_LT) {
            printf("        cmp rax, rdi\n");
            printf("        setl al\n");
            printf("        movzb rax, al\n");
        } else if (n->type == ND_LE) {
            printf("        cmp rax, rdi\n");
            printf("        setle al\n");
            printf("        movzb rax, al\n");
        }
        printf("        push rax\n");
    }
}

void gen(Node **stmts) {
    printf("        push rbp\n");
    printf("        mov rbp, rsp\n");
    printf("        sub rsp, %d\n", 208);

    int i;
    for (i = 0; stmts[i]; i++) {
        Node *n = stmts[i];        
        genExpr(n);

        printf("        pop rax\n");
    }

    printf("        mov rsp, rbp\n");
    printf("        pop rbp\n");
    printf("        ret\n");
}

void genAssembly(Node **stmts) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n\n");
    printf("main:\n");

    gen(stmts);

    printf("        ret\n");
}