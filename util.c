#include "util.h"

String newString(char *str, int len) {
    String s;
    s.str = malloc(sizeof(char) * len + 1);
    strcpy(s.str, str);
    s.len = len;
    return s;
}

StringChain *initStringChain() {
    StringChain *sc = malloc(sizeof(StringChain));
    sc->str = newString("HEAD", 4);
    sc->next = NULL;
    sc->total = 0;
    return sc;
}

StringChain *newStringChain(String str) {
    StringChain *sc = malloc(sizeof(StringChain));
    sc->str = str;
    sc->next = NULL;
    return sc;
}

int getStringOffset(StringChain *sc, String str) {
    StringChain *cur = malloc(sizeof(StringChain));
    cur = sc;
    int i = 0;

    while (cur->next) {
        if (isEqualStrings(cur->next->str, str)) {
            return (i + 1) * 8;
        }
        
        cur = cur->next;
        i++;
    }

    StringChain *newSC = newStringChain(str);
    cur->next = newSC;
    sc->total++;
    return (i + 1) * 8;
}

bool isEqualStrings(String s1, String s2) {
    if (strcmp(s1.str, s2.str) == 0) {
        return true;
    }
    return false;
}

void error(char *err_msg, ...) {
    va_list ap;
    va_start(ap, err_msg);
    vfprintf(stderr, err_msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}

void assert(bool cond, char *err_msg, ...) {
    if (!cond) {
        error(err_msg);
    }
}