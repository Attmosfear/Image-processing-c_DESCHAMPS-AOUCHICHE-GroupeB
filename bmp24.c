//
// Created by desch on 25/04/2025.
//

#include "bmp24.h"

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
    file_rawRead(0, &image->header, HEADER_SIZE, 1, f);
    file_rawRead(HEADER_SIZE, &image->header_info, INFO_SIZE, 1, f);

    //Recuperation de la data
    bmp24_readPixelData(image, f);

    fclose(f);
    return image;

}

void bmp24_saveImage (t_bmp24 * img, const char * filename) {
    FILE * f = fopen(filename, "wb");
    if (f ==  NULL) {
        printf("Erreur : impossoble de lire le fichier %s", filename);
        return;
    }
    file_rawWrite(0, &img->header, HEADER_SIZE, 1, f);
    file_rawWrite(HEADER_SIZE, &img->header_info, INFO_SIZE, 1, f);
    bmp24_writePixelData(img, f);
    fclose(f);
}

void bmp24_negative (t_bmp24 * img) {
    for (int i = 0; i<img->height; i++) {
        for (int j = 0; j< img->width; j++) {
            t_pixel pixel = img->data[i][j];
            pixel.blue = 255 - pixel.blue;
            pixel.red = 255 - pixel.red;
            pixel.green = 255 - pixel.green;
        }
    }
}

void bmp24_brightness (t_bmp24 * img, int value) {
    for (int i = 0; i<img->height; i++) {
        for (int j = 0; j< img->width; j++) {
            t_pixel pixel = img->data[i][j];
            pixel.blue = pixel.blue+value;
            if (pixel.blue > 255) {
                pixel.blue = 255;
            }
            if (pixel.blue < 0) {
                pixel.blue = 0;
            }
            pixel.red = pixel.red + value;
            if (pixel.red > 255) {
                pixel.red = 255;
            }
            if (pixel.red < 0) {
                pixel.red = 0;
            }
            pixel.green = pixel.green + value;
            if (pixel.green > 55) {
                pixel.green = 255;
            }
            if (pixel.green < 0) {
                pixel.green = 0;
            }
        }
    }
}

void bmp24_grayscale(t_bmp24 * img) {
    for (int i = 0; i<img->height; i++) {
        for (int j = 0; j< img->width; j++) {
            t_pixel pixel = img->data[i][j];
            pixel.blue = pixel.red = pixel.green = ((pixel.blue + pixel.green + pixel.red)/3);
        }
    }
}

t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    int n = kernelSize / 2;  // Rayon du noyau

    float sommeR = 0, sommeG = 0, sommeB = 0;

    for (int k = -n; k <= n; k++) {
        for (int l = -n; l <= n; l++) {
            // Coordonnées du pixel dans l'image
            int img_y = y + k;
            int img_x = x + l;

            // Coordonnées dans le kernel
            int kernel_i = k + n;
            int kernel_j = l + n;

            sommeR += img->data[img_y][img_x].red * kernel[kernel_i][kernel_j];
            sommeG += img->data[img_y][img_x].green * kernel[kernel_i][kernel_j];
            sommeB += img->data[img_y][img_x].blue * kernel[kernel_i][kernel_j];
        }
    }

    t_pixel result;
    result.red = (uint8_t)((sommeR < 0) ? 0 : (sommeR > 255) ? 255 : sommeR);
    result.green = (uint8_t)((sommeG < 0) ? 0 : (sommeG > 255) ? 255 : sommeG);
    result.blue = (uint8_t)((sommeB < 0) ? 0 : (sommeB > 255) ? 255 : sommeB);

    return result;
}

void bmp24_boxBlur(t_bmp24 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur: image invalide\n");
        return;
    }

    // Créer le noyau box blur
    float **kernel = get_box_blur_kernel();

    // Créer une copie temporaire de l'image pour éviter de modifier les pixels pendant le calcul
    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) {
        printf("Erreur: allocation mémoire échouée\n");
        free_kernel(kernel, 3);
        return;
    }

    // Copier les données originales
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp_data[y][x] = img->data[y][x];
        }
    }

    // Appliquer le filtre (éviter les bords)
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    // Libérer la mémoire
    bmp24_freeDataPixels(temp_data, img->height);
    free_kernel(kernel, 3);
}

void bmp24_gaussianBlur(t_bmp24 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur: image invalide\n");
        return;
    }

    // Créer le noyau gaussian blur
    float **kernel = get_gaussian_blur_kernel();

    // Créer une copie temporaire de l'image
    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) {
        printf("Erreur: allocation mémoire échouée\n");
        free_kernel(kernel, 3);
        return;
    }

    // Copier les données originales
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp_data[y][x] = img->data[y][x];
        }
    }

    // Appliquer le filtre (éviter les bords)
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    // Libérer la mémoire
    bmp24_freeDataPixels(temp_data, img->height);
    free_kernel(kernel, 3);
}

void bmp24_outline(t_bmp24 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur: image invalide\n");
        return;
    }

    // Créer le noyau outline
    float **kernel = get_outline_kernel();

    // Créer une copie temporaire de l'image
    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) {
        printf("Erreur: allocation mémoire échouée\n");
        free_kernel(kernel, 3);
        return;
    }

    // Copier les données originales
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp_data[y][x] = img->data[y][x];
        }
    }

    // Appliquer le filtre (éviter les bords)
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    // Libérer la mémoire
    bmp24_freeDataPixels(temp_data, img->height);
    free_kernel(kernel, 3);
}

void bmp24_emboss(t_bmp24 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur: image invalide\n");
        return;
    }

    // Créer le noyau emboss
    float **kernel = get_emboss_kernel();

    // Créer une copie temporaire de l'image
    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) {
        printf("Erreur: allocation mémoire échouée\n");
        free_kernel(kernel, 3);
        return;
    }

    // Copier les données originales
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp_data[y][x] = img->data[y][x];
        }
    }

    // Appliquer le filtre (éviter les bords)
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    // Libérer la mémoire
    bmp24_freeDataPixels(temp_data, img->height);
    free_kernel(kernel, 3);
}

void bmp24_sharpen(t_bmp24 * img) {
    if (img == NULL || img->data == NULL) {
        printf("Erreur: image invalide\n");
        return;
    }

    // Créer le noyau sharpen
    float **kernel = get_sharpen_kernel();

    // Créer une copie temporaire de l'image
    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) {
        printf("Erreur: allocation mémoire échouée\n");
        free_kernel(kernel, 3);
        return;
    }

    // Copier les données originales
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp_data[y][x] = img->data[y][x];
        }
    }

    // Appliquer le filtre (éviter les bords)
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            img->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    // Libérer la mémoire
    bmp24_freeDataPixels(temp_data, img->height);
    free_kernel(kernel, 3);
}