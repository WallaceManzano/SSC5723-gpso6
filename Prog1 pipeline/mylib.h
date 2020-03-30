#ifndef MYLIB_H
#define MYLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <string.h>

int pipeline (FILE *fpin, FILE *fpout, const char *cmd1, const char* cmd2, ...);

#endif
