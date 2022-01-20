#include "maths.h"

/* ___________________________________________ */

void turnVectorAngle(Vector3d *vct, Angle3d *angle){
    if(angle->x != 0) {
        Vector3d temp = *vct;
        vct->y = ((temp.y * cos(angle->x)) - (temp.z * sin(angle->x)));
        vct->z = ((temp.y * sin(angle->x)) + (temp.z * cos(angle->x)));
    } if(angle->y != 0) {
        Vector3d temp = *vct;
        vct->x = ((temp.x * cos(angle->y)) + (temp.z * sin(angle->y)));
        vct->z = ((temp.x * -sin(angle->y)) + (temp.z * cos(angle->y)));
    } if(angle->z != 0) {
        Vector3d temp = *vct;
        vct->x = ((temp.x * cos(angle->z)) - (temp.y * sin(angle->z)));
        vct->y = ((temp.x * sin(angle->z)) + (temp.y * cos(angle->z)));
    }
}

/* ___________________________________________ */

double degToRad(double a){
    return a * PI / 180;
}

/* ___________________________________________ */

double calculateDiscriminant(double a, double b, double c){
    return pow(b, 2) - 4 * a * c;
}

/* ___________________________________________ */

double calculateFirstSolution(double disc, double a, double b){
    return (-b + sqrt(disc)) / (2 * a);
}

/* ___________________________________________ */

double calculateSecondSolution(double disc, double a, double b){
    return (-b - sqrt(disc)) / (2 * a);
}

/* ___________________________________________ */

int minPositiveTabIndex(double *tab, int sizeTab){
    if(sizeTab <= 0)
        return -1;
    int index = -1;

    for(int i = 0; i < sizeTab; ++i){
        if(tab[i] > 0.0){
            index = i;
        }
    }

    if(index == -1)
        return index;
    
    for(int i = 0; i < sizeTab; ++i){
        if(tab[i] < tab[index] && tab[i] > 0.0){
            index = i;
        }
    }

    return index;
}

/* ___________________________________________ */

double pixelToMeter(double px){
    return px / 6100;
}

/* ___________________________________________ */

double angleBetweenVectors(Vector3d *v1, Vector3d *v2){

    double scalar = (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
    double v1Standard = sqrt(pow(v1->x, 2) + pow(v1->y, 2) + pow(v1->z, 2));
    double v2Standard = sqrt(pow(v2->x, 2) + pow(v2->y, 2) + pow(v2->z, 2));
    double angle = acos(scalar / (v1Standard * v2Standard));

    return angle;
}

/* ___________________________________________ */

double distBetweenPoints(Point3d *p1, Point3d *p2){
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2) + pow(p1->z - p2->z, 2));
}

/* ___________________________________________ */