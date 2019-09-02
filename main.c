#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  printf(".intel_syntax noprefix\n");
  printf(".global main\n\n");
  printf("main:\n");
  printf("    mov rax, %d\n", atoi(argv[1]));
  printf("    ret\n");
  return 0;
}
