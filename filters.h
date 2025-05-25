//
// Created by desch on 25/05/2025.
//

#ifndef FILTERS_H
#define FILTERS_H

// Fonctions pour créer les différents noyaux de filtres
float** createBoxBlurKernel(void);
float** createGaussianBlurKernel(void);
float** createOutlineKernel(void);
float** createEmbossKernel(void);
float** createSharpenKernel(void);

// Fonction pour libérer un noyau
void freeFilterKernel(float** kernel, int size);

#endif // FILTERS_H