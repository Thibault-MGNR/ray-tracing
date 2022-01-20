#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdio.h>

/* VARIABLES */

int loadingProgressBefore;
int loadingProgressAfter;
int loadingValueMin;
int loadingValueMax;
int loadingStep;

/* FONCTIONS */

void initLoading(const char *loadMsg, int min, int max, int step);
void showLoading(int value);
void closeLoading(void);

#endif