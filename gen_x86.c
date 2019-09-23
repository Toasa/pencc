#include "util.h"
#include "token.h"
#include "parse.h"
#include "gen_x86.h"

char *regs[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

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
    } else if (n->type == ND_CALL) {
        int i = 0;
        Node *cur = n->next;
        while (cur) {
            genExpr(cur);
            printf("        pop rax\n");
            printf("        mov %s, rax\n", regs[i]);
            cur = cur->next;
            i++;
        }

        printf("        mov rax, 0\n");
        printf("        call %s\n", n->func);
        printf("        push rax\n");
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

void genFunc(Node *func, int ident_num) {
    printf("        push rbp\n");
    printf("        mov rbp, rsp\n");
    printf("        sub rsp, %d\n", ident_num * 8);

    int args_i;
    for (args_i = 0; args_i < func->argsNum; args_i++) {
        printf("        mov [rbp - %d], %s\n", (args_i + 1) * 8, regs[args_i]);
    }

    genStmt(func->expr);

    printf("        mov rsp, rbp\n");
    printf("        pop rbp\n");
    printf("        ret\n");
}

void gen(FuncData **funcs) {
    int i;
    for (i = 0; funcs[i]; i++) {
        printf("%s:\n", funcs[i]->topLevelFunc->func);
        genFunc(funcs[i]->topLevelFunc, funcs[i]->identNum);
    }
}

void genGlobalFuncs(FuncData **funcs) {
    int i;
    for (i = 0; funcs[i]; i++) {
        printf(".global %s\n", funcs[i]->topLevelFunc->func);
    }
    printf("\n\n");
}

void genAssembly(FuncData **funcs) {
    printf(".intel_syntax noprefix\n");

    genGlobalFuncs(funcs);

    gen(funcs);
}