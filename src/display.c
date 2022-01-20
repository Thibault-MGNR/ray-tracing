#include "display.h"

/* ___________________________________________ */

void initLoading(const char *loadMsg, int min, int max, int step){
    printf("\n");
    printf(loadMsg);
    printf("\n");
    printf("|                                                  | 0%c", 37);
    loadingProgressAfter = 0;
    loadingProgressBefore = 0;
    loadingValueMax = max;
    loadingValueMin = min;
    loadingStep = step;
}

/* ___________________________________________ */

void closeLoading(void){
    printf("\n");
}

/* ___________________________________________ */

void showLoading(int value){
    loadingProgressAfter = (value - loadingValueMin) * 100 / (loadingValueMax - loadingValueMin);
    if(loadingProgressAfter - loadingProgressBefore >= loadingStep || loadingProgressAfter >= 100){
        loadingProgressBefore = loadingProgressAfter;
        printf("\r");
        for(int i = 0; i < 60; ++i){
            printf(" ");
        }
        printf("\r|");
        for(int i = 0; i < loadingProgressAfter + 1; ++i){
            if(i % 2 == 0)
                printf("&");
        }
        
        for(int i = loadingProgressAfter; i < 100; ++i){
            if(i % 2 == 0)
                printf(" ");
        }


        printf("| %d%c", loadingProgressAfter, 37);
    }
}

/* ___________________________________________ */