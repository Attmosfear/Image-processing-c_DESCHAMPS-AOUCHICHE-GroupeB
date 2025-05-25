/**
 * @file filters.c
 * @author Malo DESCHAMPS
 * @brief Implémentation des fonctions pour créer les noyaux de filtres
 * @date 2025
 */

#include "filters.h"
#include <stdlib.h>

/**
 * @brief Crée le noyau pour le filtre box blur
 * @return Noyau 3x3 pour box blur
 */
float** createBoxBlurKernel(void) {
    float** kernel = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = (float*)malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = 1.0f / 9.0f;
        }
    }
    return kernel;
}

/**
 * @brief Crée le noyau pour le filtre gaussien
 * @return Noyau 3x3 pour flou gaussien
 */
float** createGaussianBlurKernel(void) {
    float** kernel = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = (float*)malloc(3 * sizeof(float));
    }

    kernel[0][0] = 1.0f/16; kernel[0][1] = 2.0f/16; kernel[0][2] = 1.0f/16;
    kernel[1][0] = 2.0f/16; kernel[1][1] = 4.0f/16; kernel[1][2] = 2.0f/16;
    kernel[2][0] = 1.0f/16; kernel[2][1] = 2.0f/16; kernel[2][2] = 1.0f/16;

    return kernel;
}

/**
 * @brief Crée le noyau pour le filtre de détection de contours
 * @return Noyau 3x3 pour détection de contours
 */
float** createOutlineKernel(void) {
    float** kernel = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = (float*)malloc(3 * sizeof(float));
    }

    kernel[0][0] = -1; kernel[0][1] = -1; kernel[0][2] = -1;
    kernel[1][0] = -1; kernel[1][1] = 8;  kernel[1][2] = -1;
    kernel[2][0] = -1; kernel[2][1] = -1; kernel[2][2] = -1;

    return kernel;
}

/**
 * @brief Crée le noyau pour le filtre de relief
 * @return Noyau 3x3 pour effet de relief
 */
float** createEmbossKernel(void) {
    float** kernel = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = (float*)malloc(3 * sizeof(float));
    }

    kernel[0][0] = -2; kernel[0][1] = -1; kernel[0][2] = 0;
    kernel[1][0] = -1; kernel[1][1] = 1;  kernel[1][2] = 1;
    kernel[2][0] = 0;  kernel[2][1] = 1;  kernel[2][2] = 2;

    return kernel;
}

/**
 * @brief Crée le noyau pour le filtre de netteté
 * @return Noyau 3x3 pour accentuation de la netteté
 */
float** createSharpenKernel(void) {
    float** kernel = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = (float*)malloc(3 * sizeof(float));
    }

    kernel[0][0] = 0;  kernel[0][1] = -1; kernel[0][2] = 0;
    kernel[1][0] = -1; kernel[1][1] = 5;  kernel[1][2] = -1;
    kernel[2][0] = 0;  kernel[2][1] = -1; kernel[2][2] = 0;

    return kernel;
}

/**
 * @brief Libère la mémoire allouée pour un noyau
 * @param kernel Noyau à libérer
 * @param size Taille du noyau
 */
void freeFilterKernel(float** kernel, int size) {
    if (kernel) {
        for (int i = 0; i < size; i++) {
            if (kernel[i]) {
                free(kernel[i]);
            }
        }
        free(kernel);
    }
}
