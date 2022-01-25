#include "maths.h"

/* ___________________________________________ */

void turnVectorAngle(Vector3d *vct, Angle3d *angle){
    if(angle->x == 0 && angle->y == 0 && angle->z == 0)
        return;
    double c1 = cos(angle->z / 2);
    double c2 = cos(angle->y / 2);
    double c3 = cos(angle->z / 2);
    double s1 = sin(angle->z / 2);
    double s2 = sin(angle->y / 2);
    double s3 = sin(angle->z / 2);

    double a = c1 * c2 * c3 - s1 * s2 * s3;
    double b = s1 * s2 * c3 + c1 * c2 * s3;
    double c = s1 * c2 * c3 + c1 * s2 * s3;
    double d = c1 * s2 * c3 - s1 * c2 * s3;

    double t2 = a*b; double t3 = a*c; double t4 = a*d; double t5 = -b*b;
    double t6 = b*c; double t7 = b*d; double t8 = -c*c; double t9 = c*d;
    double t10 = -d*d;
    double v1 = 2 * ((t8 + t10) * vct->x + (t6 - t4) * vct->y + (t3 + t7) * vct->z) + vct->x;
    double v2 = 2 * ((t4 + t6) * vct->x + (t5 + t10) * vct->y + (t9 - t2) * vct->z) + vct->y;
    double v3 = 2 * ((t7 - t3) * vct->x + (t2 + t9) * vct->y + (t5 + t8) * vct->z) + vct->z;
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

    double scalar = scalarProduct(v1, v2);
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

double vect3dNorm(Vector3d *vct){
    return sqrt(pow(vct->x,2) + pow(vct->y,2) + pow(vct->z,2));
}

/* ___________________________________________ */

Vector3d calcReflectVector(Vector3d *incidentVect, Vector3d *normalVect){
    double theta = angleBetweenVectors(incidentVect, normalVect);
    Vector3d rVct;
    rVct.x = incidentVect->x + (2 * cos(theta)) * normalVect->x;
    rVct.y = incidentVect->y + (2 * cos(theta)) * normalVect->y;
    rVct.z = incidentVect->z + (2 * cos(theta)) * normalVect->z;
    normalize(&rVct);
    return rVct;
}

/* ___________________________________________ */

double max(double a, double b){
    if(a >= b)
        return a;
    return b;
}

/* ___________________________________________ */

double scalarProduct(Vector3d *a, Vector3d *b){
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

/* ___________________________________________ */

void normalize(Vector3d *vect){
    double norme = vect3dNorm(vect);
    vect->x = vect->x / norme;
    vect->y = vect->y / norme;
    vect->z = vect->z / norme;
}

/* ___________________________________________ */

Vector3d vectorSum(Vector3d *v1, Vector3d *v2){
    Vector3d sum;
    sum.x = v1->x + v2->x;
    sum.y = v1->y + v2->y;
    sum.z = v1->z + v2->z;

    return sum;
}

/* ___________________________________________ */

Vector3d vectorSub(Vector3d *v1, Vector3d *v2){
    Vector3d sub;
    sub.x = v1->x - v2->x;
    sub.y = v1->y - v2->y;
    sub.z = v1->z - v2->z;

    return sub;
}

/* ___________________________________________ */