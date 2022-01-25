#ifndef SHADER_H
#define SHADER_H
#include "ray_tracing.h"

/* STRUCTURES */
// structure d'un shader
typedef struct Shader{
    float specular;
    float shininess;
    float diffuse;
    Color color;
} Shader;

/* FONCTIONS */

Shader *initShader(void);
Shader *closeShader(Shader *shader);
void applyShader(Scene *scn, Ray *incindent_ray, int n);
Color diffuseEffect(Scene *scn, Shader *shader, int sphereIndex, Point3d *pos);
Color specularEffect(Scene *scn, Shader *shader, int sphereIndex, Point3d *pos, Vector3d *normalVect, Ray *incidentRay);
Color sumColors(Color *c1, Color *c2);
void normalizeColor(Color *c1);

#endif