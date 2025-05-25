#ifndef BMP8_H
#define BMP8_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Structure pour représenter une image BMP 8 bits en niveaux de gris
typedef struct {
    unsigned char header[54];        // En-tête du fichier BMP
    unsigned char colorTable[1024];  // Table de couleurs
    unsigned char *data;             // Données de l'image
    unsigned int width;              // Largeur de l'image
    unsigned int height;             // Hauteur de l'image
    unsigned int colorDepth;         // Profondeur de couleur (8 bits)
    unsigned int dataSize;           // Taille des données
} t_bmp8;

// Fonctions de lecture et écriture
t_bmp8* bmp8_loadImage(const char* filename);
void bmp8_saveImage(const char* filename, t_bmp8* img);
void bmp8_free(t_bmp8* img);
void bmp8_printInfo(t_bmp8* img);

// Fonctions de traitement d'image
void bmp8_negative(t_bmp8* img);
void bmp8_brightness(t_bmp8* img, int value);
void bmp8_threshold(t_bmp8* img, int threshold);

// Fonctions de filtrage
void bmp8_applyFilter(t_bmp8* img, float** kernel, int kernelSize);

// Fonctions d'égalisation d'histogramme
unsigned int* bmp8_computeHistogram(t_bmp8* img);
unsigned int* bmp8_computeCDF(unsigned int* hist);
void bmp8_equalize(t_bmp8* img);

#endif // BMP8_H