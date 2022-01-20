#include "error_handler.h"

/* ___________________________________________ */

void exitError(const char *msg){
    fprintf(stderr, "ERREUR: %s", msg);
    exit(EXIT_FAILURE);
}

/* ___________________________________________ */

void exitErrorAllocation(void){
    exitError("Allocation dynamique");
}

/* ___________________________________________ */

void exitErrorOpenFile(void){
    exitError("Impossible d'ouvrir le fichier");
}

/* ___________________________________________ */

void exitErrorCreateFile(void){
    exitError("Impossible de creer le fichier");
}

/* ___________________________________________ */