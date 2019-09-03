#include "util.h"
#include "token.h"
#include "gen_x86.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Need to 2 arguments\n");
        return 1;
    }

    Token *token_head = tokenize(argv[1]);
    gen_assembly(token_head);

    return 0;
}
