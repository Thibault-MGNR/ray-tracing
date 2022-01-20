#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "error_handler.h"

/*  STRUCT */

// Structure des données d'une image bitmap
typedef struct ImageData{
    char file[100];
    char signature[3];
    int tailleTotalFichier;
    signed char champReserve[5];
    int offset;
    int tailleEnteteImage;
    int largeurImage;
    int hauteurImage;
    int nbPlan;
    int profondeurCouleur;
    int compression;
    int tailleTotalImage;
    int resolutionHorizontale;
    int resolutionVerticale;
    int nbCouleurPalette;
    int nbCouleurImportante;
} ImageData;

// Structure d'un pixel d'image
typedef struct PixelData{
    int rouge;
    int vert;
    int bleu;
} PixelData;

// Structure d'une image
typedef struct Image{
    ImageData *metaDonee;
    PixelData ***matricePixel;
} Image;

/* FONCTIONS */

ImageData *newImagedata(void);
ImageData *clearImagedata(ImageData *bm);
Image *newImage(int sizex, int sizeY, const char *champReserve);
Image *clearImage(Image *img);
PixelData *newPixel(int r, int g, int b);
PixelData *clearPixel(PixelData *px);
void readBitmap(const char *adress, ImageData *bm);
void printMeta(Image *img);
void setImageData(Image *img, int sizeX, int sizeY, const char *champReserve);
void createMatricePixel(Image *img, int sizeX, int sizeY);
void clearMatricePixel(Image *img);
void printMatricePixel(Image *img);
void saveImage(Image *img, const char *name);
void changeColorPixelRGB(Image *img, int x, int y, int r, int g, int b);
void changeColorPixelR(Image *img, int x, int y, int r);
void changeColorPixelG(Image *img, int x, int y, int g);
void changeColorPixelB(Image *img, int x, int y, int b);
PixelData *getColorPixel(Image *img, int x, int y);

#endif