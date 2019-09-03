#include "util.h"
#include "token.h"
#include "gen_x86.h"

void gen_assembly_main(Token *t) {
    printf("        mov rax, %d\n", t->val);

    for (t = t->next; t->kind != TK_EOF; t = t->next) {
        if (t->kind == TK_ADD) {
            t = t->next;
            printf("        add rax, %d\n", t->val);
        } else {
            t = t->next;
            printf("        sub rax, %d\n", t->val);
        }        
    }
}

void gen_assembly(Token *t) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n\n");
    printf("main:\n");

    gen_assembly_main(t);

    printf("        ret\n");
}