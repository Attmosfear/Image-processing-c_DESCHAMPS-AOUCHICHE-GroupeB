//
// Created by desch on 22/05/2025.
//

#include "kernel_loader.h"

// Fonctions utilitaires pour créer les noyaux de convolution
float** create_kernel(int size) {
    float **kernel = malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        kernel[i] = malloc(size * sizeof(float));
    }
    return kernel;
}

void free_kernel(float **kernel, int size) {
    for (int i = 0; i < size; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

float** get_box_blur_kernel() {
    float **kernel = create_kernel(3);
    float factor = 1.0f / 9.0f;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = factor;
        }
    }
    return kernel;
}

float** get_gaussian_blur_kernel() {
    float **kernel = create_kernel(3);
    float factor = 1.0f / 16.0f;
    kernel[0][0] = 1 * factor; kernel[0][1] = 2 * factor; kernel[0][2] = 1 * factor;
    kernel[1][0] = 2 * factor; kernel[1][1] = 4 * factor; kernel[1][2] = 2 * factor;
    kernel[2][0] = 1 * factor; kernel[2][1] = 2 * factor; kernel[2][2] = 1 * factor;
    return kernel;
}

float** get_outline_kernel() {
    float **kernel = create_kernel(3);
    kernel[0][0] = -1; kernel[0][1] = -1; kernel[0][2] = -1;
    kernel[1][0] = -1; kernel[1][1] = 8; kernel[1][2] = -1;
    kernel[2][0] = -1; kernel[2][1] = -1; kernel[2][2] = -1;
    return kernel;
}

float** get_emboss_kernel() {
    float **kernel = create_kernel(3);
    kernel[0][0] = -2; kernel[0][1] = -1; kernel[0][2] = 0;
    kernel[1][0] = -1; kernel[1][1] = 1; kernel[1][2] = 1;
    kernel[2][0] = 0; kernel[2][1] = 1; kernel[2][2] = 2;
    return kernel;
}

float** get_sharpen_kernel() {
    float **kernel = create_kernel(3);
    kernel[0][0] = 0; kernel[0][1] = -1; kernel[0][2] = 0;
    kernel[1][0] = -1; kernel[1][1] = 5; kernel[1][2] = -1;
    kernel[2][0] = 0; kernel[2][1] = -1; kernel[2][2] = 0;
    return kernel;
}