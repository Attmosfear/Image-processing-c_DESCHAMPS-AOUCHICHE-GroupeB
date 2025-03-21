//
// Created by desch on 21/03/2025.
//




#include "bmp8.h"
#include <stdio.h>

t_bmp8 *bmp8_loadImage(const char *filename){
  t_bmp8 *image;
  // On ouvre le fichier
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    return NULL;
  }
  // Nous recuperons les informations du header
  fread(image->header, 1, 54, f);


  return NULL;
}