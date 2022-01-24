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
void applyShader(Scene *scn, int indexSphere, Ray *incindent_ray, Point3d *pos, int n);

#endif