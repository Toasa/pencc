#include "util.h"
#include "token.h"
#include "parse.h"
#include "gen_x86.h"

void genExpr(Node *n);

int calcOffset(char c) {
    return (c - 'a' + 1) * 8;
}

void genIdent(Node *n) {
    printf("        mov rax, [rbp - %d]\n", n->offset);
    printf("        push rax\n");
}

void genAssign(Node *n) {
    genExpr(n->rhs);
    int offset = n->lhs->offset;
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

void genStmt(Node *n) {
    if (n->type == ND_RETURN) {
        genExpr(n->expr);
        printf("        pop rax\n");
        printf("        mov rsp, rbp\n");
        printf("        pop rbp\n");
        printf("        ret\n");
    } else if (n->type == ND_IF) {
        genExpr(n->cond);
        printf("        pop rax\n");
        printf("        cmp rax, 0\n");
        printf("        je .L_ALT\n");
        genStmt(n->cons);
        printf("        jmp .L_END\n");
        printf(".L_ALT:\n");
        if (n->alt) {
            genStmt(n->alt);
        }
        printf(".L_END:\n");
        printf("        pop rax\n");
    } else if (n->type == ND_WHILE) {
        printf(".L_WHILESTART:\n");
        genExpr(n->cond);
        printf("        pop rax\n");
        printf("        cmp rax, 0\n");
        printf("        je .L_ALT\n");
        genStmt(n->expr);
        printf("        jmp .L_WHILESTART\n");
        printf(".L_ALT:\n");
        printf("        pop rax\n");
    } else if (n->type == ND_FOR) {
        genExpr(n->init);
        printf(".L_start:\n");
        genExpr(n->cond);
        printf("        pop rax\n");
        printf("        cmp rax, 0\n");
        printf("        je .L_end\n");
        genStmt(n->expr);
        genExpr(n->post);
        printf("        jmp .L_start\n");
        printf(".L_end:\n");
        printf("        pop rax\n");
    } else if (n->type == ND_BLOCK) {
        Node *cur = n->next;
        while (cur) {
            genStmt(cur);
            cur = cur->next;
        }
    } else {
        genExpr(n);
        printf("        pop rax\n");
    }
}

void genStmts(Node **stmts) {
    int i;
    for (i = 0; stmts[i]; i++) {
        genStmt(stmts[i]);
    }
}

void gen(ParsedData pd) {
    printf("        push rbp\n");
    printf("        mov rbp, rsp\n");
    printf("        sub rsp, %d\n", pd.identNum * 8);

    genStmts(pd.stmts);

    printf("        mov rsp, rbp\n");
    printf("        pop rbp\n");
    printf("        ret\n");
}

void genAssembly(ParsedData pd) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n\n");
    printf("main:\n");

    gen(pd);
}