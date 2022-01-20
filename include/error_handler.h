#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <stdlib.h>
#include <stdio.h>

/* FONCTIONS */
void exitError(const char *msg);
void exitErrorAllocation(void);
void exitErrorOpenFile(void);
void exitErrorCreateFile(void);

#endif