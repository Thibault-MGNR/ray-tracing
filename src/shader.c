#include "shader.h"
#include <stdlib.h>
#include <stdio.h>
#include "error_handler.h"
#include "maths.h"
#include "ray_tracing.h"

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

void applyShader(Scene *scn, int indexSphere, Ray *incindent_ray, Point3d *pos, int n){/*
    if(n > LIGHT_BOUNCE){
        incindent_ray->intensity = 0;
        return;
    }
    Sphere *sphere = &scn->tabOfSphere[indexSphere];
    Shader *shader = sphere->shader;
    Vector3d normalVect;
    normalVect.x =  pos->x - sphere->position.x;
    normalVect.y =  pos->y - sphere->position.y;
    normalVect.z =  pos->z - sphere->position.z;
    normalize(&normalVect);
    Ray *r = initRay();
    r->dirVector = calcReflectVector(&incindent_ray->dirVector, &normalVect);
    r->initPoint = *pos;
    
    double lightIntensity = calculateLighting(scn, pos, indexSphere);
    incindent_ray->intensity = lightIntensity;
    int dist = 0;
    int indexSphereReflectContact = indexNearestIntersectionSphere(scn, r, &dist);
    if(indexSphereReflectContact >= 0){
        Point3d pos = calculateCoordIntersection(scn, r, dist);
        applyShader(scn, indexSphereReflectContact, r,  &pos, n + 1);
        float pointIntensity = r->intensity * REF_WHITE_LUMINESCENCE + lightIntensity;
        float lightFactor = illuminationInfluence(pointIntensity);
        double diffuseIntensity = max(shader->diffuse * max(scalarProduct(normalVect, ), 0), AMBIENT_LIGHT);
    } else {
        incindent_ray->intensity = 0;
    }
    free(r);*/
}

/* ___________________________________________ */