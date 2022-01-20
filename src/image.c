#include "image.h"

/* ___________________________________________ */

ImageData *newImagedata(void){
    ImageData *img = malloc(sizeof(ImageData));
    if(!img)
        exitErrorAllocation();

    return img;
}

/* ___________________________________________ */

ImageData *clearImagedata(ImageData *bm){
    free(bm);
    return NULL;
}

/* ___________________________________________ */

void readBitmap(const char *adress, ImageData *bm){
    strcpy(bm->file, adress);
    FILE *file = fopen(adress, "rb");

    if(!file)
        exitErrorOpenFile();

    fread(bm->signature, 1, 2, file);
    fread(&bm->tailleTotalFichier, 1, 4, file);
    fread(bm->champReserve, 1, 4, file);
    fread(&bm->offset, 1, 4, file);
    fread(&bm->tailleEnteteImage, 1, 4, file);
    fread(&bm->largeurImage, 1, 4, file);
    fread(&bm->hauteurImage, 1, 4, file);
    fread(&bm->nbPlan, 1, 2, file);
    fread(&bm->profondeurCouleur, 1, 2, file);
    fread(&bm->compression, 1, 4, file);
    fread(&bm->tailleTotalImage, 1, 4, file);
    fread(&bm->resolutionHorizontale, 1, 4, file);
    fread(&bm->resolutionVerticale, 1, 4, file);
    fread(&bm->nbCouleurPalette, 1, 4, file);
    fread(&bm->nbCouleurImportante, 1, 4, file);


    fclose(file);
}

/* ___________________________________________ */

void printMeta(Image *img){
    ImageData *bm = img->metaDonee;
    printf("\nLien fichier: %s\n\n", bm->file);
    printf("Signature: %s\n", bm->signature);
    printf("Taille total du fichier: %d octets\n", bm->tailleTotalFichier);
    printf("Champ reserve: %s\n", bm->champReserve);
    printf("Offset: %d\n", bm->offset);
    printf("Taille entete de l'image: %d octets\n", bm->tailleEnteteImage);
    printf("Largeur de l'image: %d\n", bm->largeurImage);
    printf("Hauteur de l'image: %d\n", bm->hauteurImage);
    printf("Nombre de plans: %#00.2x\n", bm->nbPlan);
    printf("Profondeur de couleur: %d\n", bm->profondeurCouleur);
    printf("Compression: %d\n", bm->compression);
    printf("Taille total de l'image: %d octets\n", bm->tailleTotalImage);
    printf("Resolution horizontale: %d\n", bm->resolutionHorizontale);
    printf("Resolution verticale: %d\n", bm->resolutionVerticale);
    printf("Nombre de couleurs: %d\n", bm->nbCouleurPalette);
    printf("Nombre de couleurs importante: %d\n\n", bm->nbCouleurImportante);
}

/* ___________________________________________ */

Image *newImage(int sizeX, int sizeY, const char *champReserve){
    Image *img = malloc(sizeof(Image));
    if(!img)
        exitErrorAllocation();

    setImageData(img, sizeX, sizeY, champReserve);
    createMatricePixel(img, sizeX, sizeY);

    return img;
}

/* ___________________________________________ */

Image *clearImage(Image *img){
    initLoading("Liberation de la memoire:", 0, 3, 30);
    clearMatricePixel(img);
    showLoading(1);
    clearImagedata(img->metaDonee);
    showLoading(2);
    free(img);
    showLoading(3);
    closeLoading();

    return NULL;
}

/* ___________________________________________ */

PixelData *newPixel(int r, int g, int b){
    PixelData *px = malloc(sizeof(PixelData));
    if(!px)
        exitErrorAllocation();
    
    px->rouge = r;
    px->vert = g;
    px->bleu = b;

    return px;
}

/* ___________________________________________ */

PixelData *clearPixel(PixelData *px){
    free(px);
    return NULL;
}

/* ___________________________________________ */

void setImageData(Image *img, int sizeX, int sizeY, const char *champReserve){
    img->metaDonee = newImagedata();
    strcpy(img->metaDonee->signature, "BM");
    strcpy(img->metaDonee->champReserve, champReserve);
    img->metaDonee->tailleEnteteImage = 40;
    img->metaDonee->largeurImage = sizeX;    
    img->metaDonee->hauteurImage = sizeY;
    img->metaDonee->nbPlan = 0x01;
    img->metaDonee->profondeurCouleur = 24;
    img->metaDonee->compression = 0;
    img->metaDonee->resolutionHorizontale = 2834;
    img->metaDonee->resolutionVerticale = 2834;
    img->metaDonee->nbCouleurPalette = 0;
    img->metaDonee->nbCouleurImportante = 0;
    img->metaDonee->tailleTotalImage = sizeY * (sizeX + (sizeX % 4)) * 3;
    img->metaDonee->offset = 54;
    img->metaDonee->tailleTotalFichier = img->metaDonee->offset + img->metaDonee->tailleTotalImage;
}

/* ___________________________________________ */

void createMatricePixel(Image *img, int sizeX, int sizeY){
    img->matricePixel = malloc(sizeof(PixelData) * sizeY);
    if(!img->matricePixel)
        exitErrorAllocation();

    for(int i = 0; i < sizeY; ++i){
        img->matricePixel[i] = malloc(sizeof(PixelData) * sizeX);
        if(!img->matricePixel[i])
            exitErrorAllocation();
    }

    initLoading("Creation de la matrice de l'image:", 0, sizeY - 1, 10);
    for(int i = 0; i < sizeY; ++i){
        for(int j = 0; j < sizeX; ++j){
            int r = 0;
            int b = 0;
            int g = 0;
            img->matricePixel[i][j] = newPixel(r, g, b);
        }
        showLoading(i);
    }

    closeLoading();
}

/* ___________________________________________ */

void clearMatricePixel(Image *img){
    for(int i = 0; i < img->metaDonee->hauteurImage; ++i){
        for(int j = 0; j < img->metaDonee->largeurImage; ++j){
            clearPixel(img->matricePixel[i][j]);
        }
        free(img->matricePixel[i]);
    }
    free(img->matricePixel);
}

/* ___________________________________________ */

void printMatricePixel(Image *img){
    for(int i = 0; i < img->metaDonee->hauteurImage; ++i){
        for(int j = 0; j < img->metaDonee->largeurImage; ++j){
            printf("(%d, %d, %d), ", img->matricePixel[i][j]->rouge, img->matricePixel[i][j]->vert, img->matricePixel[i][j]->bleu);
        }
        printf("\n");
    }
    printf("\n\n");
}

/* ___________________________________________ */

void saveImage(Image *img, const char *name){
    FILE *file;
    file = fopen(name, "wb+");
    if(!file)
        exitErrorCreateFile();


    strcpy(img->metaDonee->file, name);
    ImageData *meta = img->metaDonee;
    PixelData ***matricePixel = img->matricePixel;

    initLoading("Sauvegarde de l'image:", 0, meta->hauteurImage - 1, 10);

    fwrite(meta->signature, 1, 2, file);
    fwrite(&meta->tailleTotalFichier, 1, 4, file);
    fwrite(meta->champReserve, 1, 4, file);
    fwrite(&meta->offset, 1, 4, file);
    fwrite(&meta->tailleEnteteImage, 1, 4, file);
    fwrite(&meta->largeurImage, 1, 4, file);
    fwrite(&meta->hauteurImage, 1, 4, file);
    fwrite(&meta->nbPlan, 1, 2, file);
    fwrite(&meta->profondeurCouleur, 1, 2, file);
    fwrite(&meta->compression, 1, 4, file);
    fwrite(&meta->tailleTotalImage, 1, 4, file);
    fwrite(&meta->resolutionHorizontale, 1, 4, file);
    fwrite(&meta->resolutionVerticale, 1, 4, file);
    fwrite(&meta->nbCouleurPalette, 1, 4, file);
    fwrite(&meta->nbCouleurImportante, 1, 4, file);

    for(int y = meta->hauteurImage - 1; y >= 0; --y){
        for(int x = 0; x < meta->largeurImage; ++x){
            fwrite(&matricePixel[y][x]->bleu, 1, 1, file);
            fwrite(&matricePixel[y][x]->vert, 1, 1, file);
            fwrite(&matricePixel[y][x]->rouge, 1, 1, file);
        }

        int full = 0xFF;
        int empty = 0x00;

        switch((meta->largeurImage * 3) % 4){
            case 0:
                break;
            case 1:
                fwrite(&full, 1, 1, file);
                break;
            case 2:
                fwrite(&empty, 1, 1, file);
                fwrite(&full, 1, 1, file);
                break;
            case 3:
                fwrite(&empty, 1, 1, file);
                fwrite(&empty, 1, 1, file);
                fwrite(&full, 1, 1, file);
                break;
            default:
                printf("Erreur nombre de pixel par ligne incomprehensible: %d\n", (meta->largeurImage * 3) % 4);
                exit(EXIT_FAILURE);
                break;
        }
        showLoading(meta->hauteurImage - y);
    }
    
    closeLoading();
    fclose(file);
}

/* ___________________________________________ */

void changeColorPixelRGB(Image *img, int x, int y, int r, int g, int b){
    img->matricePixel[y][x]->bleu = b;
    img->matricePixel[y][x]->vert = g;
    img->matricePixel[y][x]->rouge = r;
}

/* ___________________________________________ */

void changeColorPixelR(Image *img, int x, int y, int r){
    img->matricePixel[y][x]->rouge = r;
}

/* ___________________________________________ */

void changeColorPixelG(Image *img, int x, int y, int g){
    img->matricePixel[y][x]->vert = g;
}

/* ___________________________________________ */

void changeColorPixelB(Image *img, int x, int y, int b){
    img->matricePixel[y][x]->bleu = b;
}

/* ___________________________________________ */

PixelData *getColorPixel(Image *img, int x, int y){
    return img->matricePixel[y][x];
}

/* ___________________________________________ */