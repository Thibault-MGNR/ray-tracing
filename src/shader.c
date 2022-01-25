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
    
    shdr->diffuse = 1;
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

void applyShader(Scene *scn, Ray *incindent_ray, int n){
    /**
     * @brief à moi même
     * 
     * calculer la lumière diffuse pour chaque source de lumière et non le vecteur somme des lumières
     * 
     * diviser la puissance lumineuse par le carré de la distance
     * 
     */
    if(n >= LIGHT_BOUNCE){
        incindent_ray->intensity = 0;
        return;
    }

    double dist;
    int index = indexNearestIntersectionSphere(scn, incindent_ray, &dist);
    if(index < 0){
        incindent_ray->intensity = 0;
        incindent_ray->color.r = 0;
        incindent_ray->color.g = 0;
        incindent_ray->color.b = 0;
        return;
    }

    Point3d pos = calculateCoordIntersection(scn, incindent_ray, dist);
    Sphere *sphere = &scn->tabOfSphere[index];
    Shader *shader = sphere->shader;
    Vector3d normalVect;
    normalVect.x =  pos.x - sphere->position.x;
    normalVect.y =  pos.y - sphere->position.y;
    normalVect.z =  pos.z - sphere->position.z;
    normalize(&normalVect);

    Ray *r = initRay();
    r->dirVector = calcReflectVector(&incindent_ray->dirVector, &normalVect);
    r->initPoint = pos;
    r->intensity = 0;
    
    Vector3d lightVect;
    double lightIntensity = calculateLighting(scn, index, &pos, &lightVect);
    incindent_ray->intensity = lightIntensity;
    normalize(&lightVect);

    // Point3d pos = calculateCoordIntersection(scn, r, dist);
    // applyShader(scn, r, n + 1);
    double pointIntensity = r->intensity * REF_WHITE_LUMINESCENCE + lightIntensity;
    double lightFactor = illuminationInfluence(pointIntensity);
    double diffuseIntensity = max(shader->diffuse * max(scalarProduct(&normalVect, &lightVect), 0), AMBIENT_LIGHT);

    incindent_ray->color.r = shader->color.r * diffuseIntensity;
    incindent_ray->color.g = shader->color.g * diffuseIntensity;
    incindent_ray->color.b = shader->color.b * diffuseIntensity;

    free(r);
}

/* ___________________________________________ */