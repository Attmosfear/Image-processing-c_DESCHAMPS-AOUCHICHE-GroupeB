//
// Created by desch on 21/03/2025.
//




#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>


t_bmp8 *bmp8_loadImage(const char *filename){
  // Allocation de la memoir pour l'image
  t_bmp8 * image = malloc(sizeof(t_bmp8));
  // On ouvre le fichier
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    return NULL;
  }
  // Nous recuperons les informations du header
  fread(image->header, sizeof(unsigned char), 54, f);
  image->width = *(unsigned int*)&image->header[18];
  image->height = *(unsigned int*)&image->header[22];
  image->colorDepth = *(unsigned int*)&image->header[28];
  image->dataSize = *(unsigned int*)&image->header[34];
  // Nous recuperons la table de couleur
  fread(image->colorTable, sizeof(unsigned char), 1024, f);
  // Allocation de la memoire pour les donnees
  image->data = malloc(image->dataSize);
  // Nous recuperons les donnees
  fread(image->data, sizeof(unsigned char), image->dataSize, f);
  fclose(f);
  return image;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
  FILE *file = fopen(filename, "w");
  fwrite(img->header, sizeof(unsigned char), 54, file);
  fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
  fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
  fclose(file);
}

void bmp8_free(t_bmp8 *img) {
  free(img->data);
  free(img);
}

void bmp8_printInfo(t_bmp8 *img) {
  printf("Image Info :\n");
  printf("\tWidth : %d\n", img->width);
  printf("\tHeight : %d\n", img->height);
  printf("\tColor Depth : %d\n", img->colorDepth);
  printf("\tData Size : %d\n", img->dataSize);
}

void bmp8_negative(t_bmp8 *img) {
  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] = 255 - img->data[i];
  }
}

void bmp8_brightness(t_bmp8 *img, int value) {
  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] += value;
    if (img->data[i] > 255) {
      img->data[i] = 255;
    }
    if (img  ->data[i] < 0) {
      img->data[i] = 0;
    }
  }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
  for (int i = 0; i < img->dataSize; i++) {
    if (img->data[i] >= threshold) {
      img->data[i] = 255;
    } else {
      img->data[i] = 0;
    }
  }
}

void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {

  int n = kernelSize /2;

  // Creation d'une data temporaire
  unsigned char *tempData = malloc(img->dataSize * sizeof(unsigned char));


  for (int i = 1; i < (img->width-n); i++) {
    for (int j = 1; j < (img->height-n); j++) {
      int index = j * img->width + i;
      float somme = 0;
      for (int k = -n; k < n; k++) {
        for (int l = -n; l < n; l++) {
          somme += img->data[(j + k) * img->width + (i + l)] * kernel[k + n][l + n];
        }
      }
      tempData[index] = (unsigned char)somme;
    }
  }

  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] = tempData[i];
  }

  free(tempData);
}