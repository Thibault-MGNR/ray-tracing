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
    shdr->shininess = 30;
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

    double diffuseIntensity = diffuseEffect(scn, shader, index, &pos);
    Color specularColor = specularEffect(scn, shader, index, &pos, &normalVect, incindent_ray);

    incindent_ray->color.r = shader->color.r * diffuseIntensity + specularColor.r;
    incindent_ray->color.g = shader->color.g * diffuseIntensity + specularColor.g;
    incindent_ray->color.b = shader->color.b * diffuseIntensity + specularColor.b;

    
    incindent_ray->color.r = (incindent_ray->color.r > 254) ? 254 : incindent_ray->color.r;
    incindent_ray->color.g = (incindent_ray->color.g > 254) ? 254 : incindent_ray->color.g;
    incindent_ray->color.b = (incindent_ray->color.b > 254) ? 254 : incindent_ray->color.b;


    // free(r);
}

/* ___________________________________________ */

double diffuseEffect(Scene *scn, Shader *shader, int sphereIndex, Point3d *pos){
    double lightIntensity = calculateLighting(scn, sphereIndex, pos);
    double diffuseIntensity = max(shader->diffuse * max(lightIntensity, 0), AMBIENT_LIGHT);
    return diffuseIntensity;
}

/* ___________________________________________ */

Color specularEffect(Scene *scn, Shader *shader, int sphereIndex, Point3d *pos, Vector3d *normalVect, Ray *incidentRay){
    double specularIntensity = 0;
    for(int i = 0; i < scn->nbLights; i++){
        Ray lightRay = generateRayLightCoord(pos, &scn->tabOfLight[i].position);
        int isHidden = 0;
        for(int i = 0; i < scn->nbSphere; i++){
            int index  = calculateNearestIntersection(&scn->tabOfSphere[i], &lightRay);
            if(index >= 0){
                isHidden = 1;
            }
        }
        if(1){
            Vector3d *L = &lightRay.dirVector;
            Vector3d vectorNull = {0, 0, 0};
            Vector3d *E = &incidentRay->dirVector;
            Vector3d oppE = vectorSub(&vectorNull, E);
            Vector3d H = vectorSub(&H, &oppE);
            normalize(&H);
            specularIntensity = specularIntensity + pow(max(scalarProduct(normalVect, &H), 0), shader->shininess);
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