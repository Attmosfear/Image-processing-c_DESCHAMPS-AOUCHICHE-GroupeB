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