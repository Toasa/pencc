#include "util.h"
#include "token.h"

void gen_assembly(Token *t) {
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

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Need to 2 arguments\n");
        return 1;
    }

    printf(".intel_syntax noprefix\n");
    printf(".global main\n\n");
    printf("main:\n");

    Token *token_head = tokenize(argv[1]);

    gen_assembly(token_head);

    printf("        ret\n");
    return 0;
}
