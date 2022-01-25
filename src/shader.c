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
    shdr->shininess = 250;
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
    if(n >= LIGHT_BOUNCE){
        incindent_ray->intensity = 0;
        return;
    }

    double dist;
    int index = indexNearestIntersectionSphere(scn, incindent_ray, &dist);
    if(index < 0){
        incindent_ray->intensity = 0;
        incindent_ray->color.r = 10;
        incindent_ray->color.g = 10;
        incindent_ray->color.b = 10;
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

    /*
    Ray *r = initRay();
    r->dirVector = calcReflectVector(&incindent_ray->dirVector, &normalVect);
    r->initPoint = pos;
    r->intensity = 0;
    */

    Color diffuseColor = diffuseEffect(scn, shader, index, &pos);
    Color specularColor = specularEffect(scn, shader, index, &pos, &normalVect, incindent_ray);

    incindent_ray->color = sumColors(&diffuseColor, &specularColor);
    normalizeColor(&incindent_ray->color);

    // free(r);
}

/* ___________________________________________ */

Color diffuseEffect(Scene *scn, Shader *shader, int sphereIndex, Point3d *pos){
    Color diffuseColor;
    double lightIntensity = calculateLighting(scn, sphereIndex, pos);
    double diffuseIntensity = max(shader->diffuse * max(lightIntensity, 0), AMBIENT_LIGHT);
    diffuseColor.r = shader->color.r * diffuseIntensity;
    diffuseColor.g = shader->color.g * diffuseIntensity;
    diffuseColor.b = shader->color.b * diffuseIntensity;

    return diffuseColor;
}

/* ___________________________________________ */

Color specularEffect(Scene *scn, Shader *shader, int sphereIndex, Point3d *pos, Vector3d *normalVect, Ray *incidentRay){
    double specularIntensity = 0;
    for(int i = 0; i < scn->nbLights; i++){
        Ray lightRay = generateRayLightCoord(pos, &scn->tabOfLight[i].position);
        int isHidden = 0;
        for(int i = 0; i < scn->nbSphere; i++){
            int index  = calculateNearestIntersection(&scn->tabOfSphere[i], &lightRay);
            if(index >= 0 && index != sphereIndex){
                isHidden = 1;
            }
        }
        if(!isHidden){
            Vector3d *L = &lightRay.dirVector;
            Vector3d vectorNull = {0, 0, 0};
            Vector3d *E = &incidentRay->dirVector;
            Vector3d oppE = vectorSub(&vectorNull, E);
            Vector3d H = vectorSum(L, &oppE);
            normalize(&H);
            specularIntensity += pow(max(scalarProduct(normalVect, &H), 0), shader->shininess);
        }
    }

    if(specularIntensity > 1)
        specularIntensity = 1;
    
    Color specularColor;
    specularColor.r = 254 * specularIntensity;
    specularColor.g = 254 * specularIntensity;
    specularColor.b = 254 * specularIntensity;

    return specularColor;
}

/* ___________________________________________ */

Color sumColors(Color *c1, Color *c2){
    Color sum;
    sum.r = c1->r + c2->r;
    sum.g = c1->g + c2->g;
    sum.b = c1->b + c2->b;
    normalizeColor(&sum);

    return sum;
}

/* ___________________________________________ */

void normalizeColor(Color *c1){
    c1->r = max(min(c1->r, 254), 0);
    c1->g = max(min(c1->g, 254), 0);
    c1->b = max(min(c1->b, 254), 0);
}

/* ___________________________________________ */