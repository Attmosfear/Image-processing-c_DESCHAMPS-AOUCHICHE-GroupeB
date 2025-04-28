//
// Created by desch on 25/04/2025.
//

#include "bmp24.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (pixels == NULL){
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            return NULL;
        }
    }
    return pixels;
}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {
    t_bmp24 * img = malloc(sizeof(t_bmp24));
    img->data = bmp24_allocateDataPixels(width, height);
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    if  (img->data == NULL) {
        return NULL;
    }
    return img;
}

void bmp24_free (t_bmp24 * img) {
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}


void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file) {
    t_pixel pixel;

    // calcule de la position du pixel dans le fichier
    uint32_t position = image->header.offset + (image->height - 1 - y) * (image->width * 3) + x * 3;

    file_rawRead(position, &pixel, sizeof(t_pixel), 1,  file);
    image->data[y][x] = pixel;
}

void bmp24_readPixelData (t_bmp24 * image, FILE * file) {

    for (int x = 0; x < image->height; x++){
        for (int y = 0; y < image->width; y++){
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file) {
    t_pixel pixel = image->data[y][x];

    // calcule de la position du pixel dans le fichier
    uint32_t position = image->header.offset + (image->height - 1 - y) * (image->width * 3) + x * 3;

    file_rawWrite(position, &pixel, sizeof(t_pixel), 1, file);
}

void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}

t_bmp24 * bmp24_loadImage (const char * filename) {
    FILE * f = fopen(filename, "rb");
    if (f ==  NULL) {
        printf("Erreur : impossoble de lire le fichier %s", filename);
        return NULL;
    }
    int height = 0;
    int width = 0;
    int colorDepth = 0;

    file_rawRead(BITMAP_WIDTH, &width, sizeof(int), 1, f);
    file_rawRead(BITMAP_HEIGHT, &height, sizeof(int), 1, f);
    file_rawRead(BITMAP_DEPTH, &colorDepth, sizeof(int), 1, f);
    t_bmp24 *image = bmp24_allocate(width, height, colorDepth);

    //Recuperation du Header et des Infos
    file_rawRead(HEADER_SIZE, &image->header, sizeof(t_bmp_header), 1, f);
    file_rawRead(INFO_SIZE, &image->header_info, sizeof(t_bmp_info), 1, f);

    //Recuperation de la data
    bmp24_readPixelData(image, f);

    fclose(f);
    return image;

}
