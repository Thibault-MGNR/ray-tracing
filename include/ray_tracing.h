#ifndef RAY_TRACING_H
#define RAY_TRACING_H
#include <stdlib.h>
#include <stdio.h>
#include "maths.h"
#include "image.h"
#include "error_handler.h"
#include "main.h"

/* STRUCTURES */

typedef struct Shader Shader;

// structure d'une couleur
typedef struct Color{
    int r;
    int g;
    int b;
} Color;

// structures d'un rayon
typedef struct Ray{
    Vector3d dirVector;
    Point3d initPoint;
    Color color;
    float intensity;
} Ray;

// structures d'une sphere
typedef struct Sphere{
    Point3d position;
    Shader *shader;
    float radius;
} Sphere;

// structures d'une caméra
typedef struct Camera{
    Angle3d angleCamera;
    Point3d position;
    Ray ***tabOfRay;
    Image *img;
    double depthOfView;
    double width;
    double height;
    double depth;
} Camera;

// structures d'une lumière
typedef struct Light{
    Point3d position;
    double radius;
    double power;
} Light;

// structure d'une scene
typedef struct Scene{
    Camera camera;
    Sphere *tabOfSphere;
    Light *tabOfLight;
    int nbLights;
    int nbSphere;
} Scene;

/* FONCTIONS */

Scene *initScene(Image *img, Point3d positionCamera, Angle3d orientationCamera, int depthOfView);
Scene *closeScene(Scene *scn);
Ray *initRay(void);
void initCamera(Camera *camera, Point3d *pos, Angle3d *ang, int *dov, Image *img);
Ray *initRayCam(Camera *camera, int posX, int posY);
Ray *clearRay(Ray *ray);
void initTabOfRayCam(Camera *cmr);
void clearTabOfRay(Camera *cmr);
double calculateNearestIntersection(Sphere *sphere, Ray *ray);
void generateImage(Scene *scn);
void newSphere(Scene *scn, Point3d *pos, double radius, Color *color);
void clearTabOfSphere(Scene *scn);
int indexNearestIntersectionSphere(Scene *scn, Ray *ray, double *distance);
void newLight(Scene *scn, double power, Point3d *pos, double radius);
void clearTabOfLight(Scene *scn);
Point3d calculateCoordIntersection(Scene *scn, Ray  *ray, double dist);
Ray generateRayLightCoord(Point3d *point, Point3d *lightPos);
double calculateLightingCam(Scene *scn, int xCam, int yCam, double dist, int sphereIndex);
void calculateABC(Ray *ray, Sphere *sphere, double *a, double *b, double *c);
double calculateLighting(Scene *scn, int sphereIndex, Point3d *pos, Vector3d *lightVect);


#endif