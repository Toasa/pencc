#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

typedef struct string {
    char *str;
    int len;
} String;

typedef struct SC {
    String str;
    struct SC *next;
    int total;
} StringChain;

String newString(char *str, int len);

StringChain *initStringChain();

StringChain *newStringChain(String str);

int getStringOffset(StringChain *sc, String str);

bool isEqualStrings(String s1, String s2);

void error(char *err_msg, ...);

void assert(bool cond, char *err_msg, ...);