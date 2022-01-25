#include "ray_tracing.h"
#include "main.h"
#include "shader.h"

/* ___________________________________________ */

Scene *initScene(Image *img, Point3d positionCamera, Angle3d orientationCamera, int depthOfView){
    Scene *scene = malloc(sizeof(Scene));
    if(!scene)
        exitErrorAllocation();
    
    Camera *camera = &scene->camera;
    scene->nbSphere = 0;
    scene->nbLights = 0;
    initCamera(camera, &positionCamera, &orientationCamera, &depthOfView, img);
    initTabOfRayCam(camera);

    return scene;
}

/* ___________________________________________ */

Scene *closeScene(Scene *scn){
    clearTabOfRay(&scn->camera);
    clearTabOfLight(scn);
    clearTabOfSphere(scn);
    free(scn);
    return NULL;
}

/* ___________________________________________ */

void initCamera(Camera *camera, Point3d *pos, Angle3d *ang, int *dov, Image *img){
    camera->angleCamera = *ang;
    camera->position = *pos;
    camera->depthOfView = *dov;
    camera->width = pixelToMeter(IMAGE_WIDTH);
    camera->height = pixelToMeter(IMAGE_HEIGH);
    camera->depth = pixelToMeter((double)IMAGE_WIDTH / tan((double)*dov / 2.0));
    camera->img = img;
}

/* ___________________________________________ */

Ray *initRay(void){
    Ray *ray = malloc(sizeof(Ray));
    if(!ray)
        exitErrorAllocation();
    
    ray->intensity = 0;
    
    return ray;
}

/* ___________________________________________ */

Ray *initRayCam(Camera *camera, int posX, int posY){
    Ray *ray = initRay();
    ray->initPoint = camera->position;
    Vector3d orientation;
    orientation.x = pixelToMeter((double)posX - (IMAGE_WIDTH / 2.0));
    orientation.y = camera->depth;
    orientation.z = pixelToMeter((IMAGE_HEIGH / 2.0) - (double)posY);
    turnVectorAngle(&orientation, &camera->angleCamera);

    normalize(&orientation);
    
    ray->intensity = 0;
    ray->dirVector = orientation;

    return ray;
}

/* ___________________________________________ */

Ray *clearRay(Ray *ray){
    free(ray);
    return NULL;
}

/* ___________________________________________ */

void initTabOfRayCam(Camera *cmr){

    cmr->tabOfRay = malloc(sizeof(Ray) * IMAGE_HEIGH);
    if(!cmr->tabOfRay)
        exitErrorAllocation();

    initLoading("Generation de la matrice de vecteurs:", 0, IMAGE_HEIGH - 1, 10);
    for(int y = 0; y < IMAGE_HEIGH; ++y){
        cmr->tabOfRay[y] = malloc(sizeof(Ray) * IMAGE_WIDTH);
        if(!cmr->tabOfRay[y])
            exitErrorAllocation();
        for(int x = 0; x < IMAGE_WIDTH; ++x){
            cmr->tabOfRay[y][x] = initRayCam(cmr, x, y);
        }
        showLoading(y);
    }
    closeLoading();
}

/* ___________________________________________ */

void clearTabOfRay(Camera *cmr){
    for(int y = 0; y < IMAGE_HEIGH; ++y){
        for(int x = 0; x < IMAGE_WIDTH; ++x){
            cmr->tabOfRay[y][x] = clearRay(cmr->tabOfRay[y][x]);
        }
        free(cmr->tabOfRay[y]);
    }
    free(cmr->tabOfRay);
}

/* ___________________________________________ */

double calculateNearestIntersection(Sphere *sphere, Ray *ray){
    
    double a, b, c;
    calculateABC(ray, sphere, &a, &b, &c);
    double discriminant = calculateDiscriminant(a, b, c);

    if(discriminant < 0){
        return -1;
    } else if(discriminant < 0.0001) {
        return calculateFirstSolution(discriminant, a, b);
    } else {
        double first = calculateFirstSolution(discriminant, a, b);
        double second = calculateSecondSolution(discriminant, a, b);

        if(first > 0 && second > 0) {
            if(first < second){
                return first;
            } else {
                return second;
            }
        } else if(first > 0) {
            return first;
        } else if(second > 0) {
            return second;
        } else {
            return -1;
        }
    }
}

/* ___________________________________________ */

void generateImage(Scene *scn){
    initLoading("Generation de l'image:", 0, IMAGE_HEIGH - 1, 10);

    for(int y = 0; y < IMAGE_HEIGH; ++y){
        for(int x = 0; x < IMAGE_WIDTH; ++x){

            /*
            double distance = 0;
            int index = indexNearestIntersectionSphere(scn, scn->camera.tabOfRay[y][x], &distance);

            if(index >= 0){
                // double lightFactor = calculateLightingCam(scn, x, y, distance, index);
                // Color *sphereColor = &scn->tabOfSphere[index].shader->color;
                // changeColorPixelRGB(scn->camera.img, x, y, sphereColor->r * lightFactor, sphereColor->g * lightFactor, sphereColor->b * lightFactor);
            } else {
                changeColorPixelRGB(scn->camera.img, x, y, 10, 10, 10);
            }
            */
           
            Ray *ray = scn->camera.tabOfRay[y][x];
            Color *colorR = &ray->color;
            applyShader(scn, ray, 0);
            changeColorPixelRGB(scn->camera.img, x, y, colorR->r, colorR->g, colorR->b);

        }
        showLoading(y);
    }
    closeLoading();
}

/* ___________________________________________ */

int indexNearestIntersectionSphere(Scene *scn, Ray *ray, double *distance){
    double *dist = malloc(sizeof(double) * scn->nbSphere);
    if(!dist)
        exitErrorAllocation();
    
    for(int sphere = 0; sphere < scn->nbSphere; sphere++){
        double a = calculateNearestIntersection(&scn->tabOfSphere[sphere], ray);
        dist[sphere] = a;
    }

    int index = minPositiveTabIndex(dist, scn->nbSphere);
    *distance = dist[index];
    free(dist);

    return index;
}

/* ___________________________________________ */

void newSphere(Scene *scn, Point3d *pos, double radius, Color *color){
    scn->nbSphere++;
    if(scn->nbSphere == 1) {
        scn->tabOfSphere = malloc(sizeof(Sphere) * scn->nbSphere);
        if(!scn->tabOfSphere)
            exitErrorAllocation();
    } else {
        Sphere *temp = realloc(scn->tabOfSphere, sizeof(Sphere) * scn->nbSphere);
        if(!temp)
            exitErrorAllocation();
        
        scn->tabOfSphere = temp;
    }

    Sphere *sphere = &scn->tabOfSphere[scn->nbSphere - 1];
    sphere->position = *pos;
    sphere->radius = radius;
    sphere->shader = initShader();
    sphere->shader->color = *color;
}

/* ___________________________________________ */

void clearTabOfSphere(Scene *scn){
    for(int i = 0; i < scn->nbSphere; i++){
        scn->tabOfSphere[i].shader = closeShader(scn->tabOfSphere[i].shader);
    }
    free(scn->tabOfSphere);
}

/* ___________________________________________ */

void newLight(Scene *scn, double power, Point3d *pos, double radius){
    scn->nbLights++;
    if(scn->nbLights == 1) {
        scn->tabOfLight = malloc(sizeof(Light) * scn->nbLights);
        if(!scn->tabOfLight)
            exitErrorAllocation();
    } else {
        Light *temp = realloc(scn->tabOfLight, sizeof(Light) * scn->nbLights);
        if(!temp)
            exitErrorAllocation();
        
        scn->tabOfLight = temp;
    }

    Light *light = &scn->tabOfLight[scn->nbLights - 1];
    light->position = *pos;
    light->power = power;
    light->radius = radius;
}

/* ___________________________________________ */

void clearTabOfLight(Scene *scn){
    free(scn->tabOfLight);
}

/* ___________________________________________ */

Point3d calculateCoordIntersection(Scene *scn, Ray  *ray, double dist){
    Point3d pos;
    pos.x = ray->dirVector.x * dist + ray->initPoint.x;
    pos.y = ray->dirVector.y * dist + ray->initPoint.y;
    pos.z = ray->dirVector.z * dist + ray->initPoint.z;

    return pos;
}

/* ___________________________________________ */

Ray generateRayLightCoord(Point3d *point, Point3d *lightPos){
    Ray ray;

    ray.initPoint = *point;
    ray.dirVector.x = (lightPos->x - point->x) / 100;
    ray.dirVector.y = (lightPos->y - point->y) / 100;
    ray.dirVector.z = (lightPos->z - point->z) / 100;
    normalize(&ray.dirVector);

    return ray;
}

/* ___________________________________________ */

double calculateLightingCam(Scene *scn, int xCam, int yCam, double dist, int sphereIndex){
    Point3d pos = calculateCoordIntersection(scn, scn->camera.tabOfRay[yCam][xCam], dist);
    Vector3d lightVect;
    double factor = calculateLighting(scn, sphereIndex, &pos, &lightVect);
    return factor;
}

/* ___________________________________________ */

double calculateLighting(Scene *scn, int sphereIndex, Point3d *pos, Vector3d *lightVect){
    double illumination = 0;
    lightVect->x = 0; lightVect->y = 0; lightVect->z = 0;

    for(int i = 0; i < scn->nbLights; ++i){
        Point3d *initLightPos = &scn->tabOfLight[i].position;

        int isHide = 0;
        Ray ray = generateRayLightCoord(pos, initLightPos);

        for(int j = 0; j < scn->nbSphere; ++j){
            double factVect = calculateNearestIntersection(&scn->tabOfSphere[j], &ray);
            if((factVect != -1 && (sphereIndex != j))){
                isHide = 1;
            }
        }

        if(!isHide){
            Vector3d sphereVector;
            *lightVect = vectorSum(lightVect, &ray.dirVector);
            sphereVector.x = pos->x - scn->tabOfSphere[sphereIndex].position.x;
            sphereVector.y = pos->y - scn->tabOfSphere[sphereIndex].position.y;
            sphereVector.z = pos->z - scn->tabOfSphere[sphereIndex].position.z;
            normalize(&sphereVector);

            double angle = angleBetweenVectors(&sphereVector, &ray.dirVector);
            double distance = distBetweenPoints(pos, &scn->tabOfLight[i].position);

            if(angle <= PI / 2){
                illumination += (scn->tabOfLight[i].power * 10 * cos(angle)) / pow(distance, 2);
            }
        }
    }
    
    double factor = illuminationInfluence(illumination);
    return factor;
}

/* ___________________________________________ */

double illuminationInfluence(double illumination){
    double result = illumination / REF_WHITE_LUMINESCENCE;

    if(result > 1){
        result = 1;
    }

    return result;
}

/* ___________________________________________ */

void calculateABC(Ray *ray, Sphere *sphere, double *a, double *b, double *c){
    *a = pow(ray->dirVector.x, 2) + pow(ray->dirVector.y, 2) + pow(ray->dirVector.z, 2);

    *b = 2 * (ray->dirVector.x * (ray->initPoint.x - sphere->position.x) + ray->dirVector.y * (ray->initPoint.y - sphere->position.y) + ray->dirVector.z * (ray->initPoint.z - sphere->position.z));

    *c = pow((ray->initPoint.x - sphere->position.x), 2) + pow((ray->initPoint.y - sphere->position.y), 2) + pow((ray->initPoint.z - sphere->position.z), 2) - pow(sphere->radius, 2);
}

/* ___________________________________________ */
