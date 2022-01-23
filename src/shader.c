#include "shader.h"
#include <stdlib.h>
#include <stdio.h>
#include "error_handler.h"

/* ___________________________________________ */

Shader *initShader(void){
    Shader *shdr = malloc(sizeof(Shader));
    if(!shdr)
        exitErrorAllocation();
    
    shdr->diffuse = 0;
    shdr->shininess = 0;
    shdr->specular = 0;

    return shdr;
}

/* ___________________________________________ */

Shader *closeShader(Shader *shader){
    free(shader);
    return NULL;
}

/* ___________________________________________ */

Ray *applyShader(Shader *shader, Ray *incindent_ray, Point3d *pos, int n){
    if(n > LIGHT_BOUNCE){
        Ray *r = initRay();
        r->intensity = 0;
        return r;
    }

    

}

/* ___________________________________________ */