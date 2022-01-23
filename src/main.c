#include <stdio.h>
#include "image.h"
#include "ray_tracing.h"
#include "display.h"
#include "error_handler.h"
#include "maths.h"

int main(void){

    Image *img = newImage(1920, 1080, "TIBO");

    Scene *scene;
    Point3d posCamera = {0, 0, 0};
    Angle3d angleCamera = {0, 0, 0};

    scene = initScene(img, posCamera, angleCamera, degToRad(90));

    Point3d posSphere1 = {0, 10, 0};
    Color colorSphere1 = {250, 0, 0};
    newSphere(scene, &posSphere1, 1, &colorSphere1);

    Point3d posSphere2 = {0, 10, -1000};
    Color colorSphere2 = {0, 250, 0};
    newSphere(scene, &posSphere2, 998.5, &colorSphere2);

    Point3d posSphere3 = {2, 10, 0};
    Color colorSphere3 = {0, 0, 250};
    newSphere(scene, &posSphere3, 0.25, &colorSphere3);

    Point3d posLight1 = {3, 10, 3};
    newLight(scene, 4000, &posLight1, 0.1);

    Point3d posLight2 = {-3, 10, 3};
    newLight(scene, 1000, &posLight2, 0.1);

    generateImage(scene);

    scene = closeScene(scene);

    char adress[30];

    sprintf(adress, "out/rendu4.bmp");
    saveImage(img, adress);
    clearImage(img);

    return 0;
}

// gcc src/*.c -I include -o bin/prog
// bin\prog.exe