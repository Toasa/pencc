#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

void error(char *err_msg, ...);

void assert(bool cond, char *err_msg);