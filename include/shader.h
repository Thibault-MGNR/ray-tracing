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

#endif