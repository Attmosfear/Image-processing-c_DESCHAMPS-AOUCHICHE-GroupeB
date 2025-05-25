/**
 * @file bmp24.c
 * @author Projet TI202
 * @brief Implémentation des fonctions pour les images BMP 24 bits en couleur
 * @date 2025
 */

#include "bmp24.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Fonctions utilitaires pour la lecture/écriture
void file_rawRead(uint32_t position, void* buffer, uint32_t size, size_t n, FILE* file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite(uint32_t position, void* buffer, uint32_t size, size_t n, FILE* file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

/**
 * @brief Alloue une matrice de pixels
 * @param width Largeur de l'image
 * @param height Hauteur de l'image
 * @return Matrice de pixels allouée
 */
t_pixel** bmp24_allocateDataPixels(int width, int height) {
    t_pixel** pixels = (t_pixel**)malloc(height * sizeof(t_pixel*));
    if (!pixels) {
        printf("Erreur: Allocation mémoire échouée\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel*)malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            printf("Erreur: Allocation mémoire échouée\n");
            // Libérer la mémoire déjà allouée
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}

/**
 * @brief Libère une matrice de pixels
 * @param pixels Matrice de pixels
 * @param height Hauteur de la matrice
 */
void bmp24_freeDataPixels(t_pixel** pixels, int height) {
    if (pixels) {
        for (int i = 0; i < height; i++) {
            if (pixels[i]) {
                free(pixels[i]);
            }
        }
        free(pixels);
    }
}

/**
 * @brief Alloue une structure d'image BMP 24 bits
 * @param width Largeur de l'image
 * @param height Hauteur de l'image
 * @param colorDepth Profondeur de couleur (24 bits)
 * @return Structure d'image allouée
 */
t_bmp24* bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24* img = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (!img) {
        printf("Erreur: Allocation mémoire échouée\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        free(img);
        return NULL;
    }

    return img;
}

/**
 * @brief Libère une structure d'image BMP 24 bits
 * @param img Structure d'image à libérer
 */
void bmp24_free(t_bmp24* img) {
    if (img) {
        if (img->data) {
            bmp24_freeDataPixels(img->data, img->height);
        }
        free(img);
    }
}

/**
 * @brief Lit la valeur d'un pixel depuis un fichier
 * @param img Structure d'image
 * @param x Coordonnée x du pixel
 * @param y Coordonnée y du pixel
 * @param file Fichier BMP
 */
void bmp24_readPixelValue(t_bmp24* img, int x, int y, FILE* file) {
    // Position du pixel dans le fichier (en tenant compte de l'inversion des lignes)
    int realY = img->height - 1 - y;
    long position = img->header.offset + (realY * img->width + x) * 3;

    uint8_t bgr[3];
    fseek(file, position, SEEK_SET);
    fread(bgr, 1, 3, file);

    // Convertir BGR en RGB
    img->data[y][x].blue = bgr[0];
    img->data[y][x].green = bgr[1];
    img->data[y][x].red = bgr[2];
}

/**
 * @brief Lit toutes les données de l'image depuis un fichier
 * @param img Structure d'image
 * @param file Fichier BMP
 */
void bmp24_readPixelData(t_bmp24* img, FILE* file) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            bmp24_readPixelValue(img, x, y, file);
        }
    }
}

/**
 * @brief Écrit la valeur d'un pixel dans un fichier
 * @param img Structure d'image
 * @param x Coordonnée x du pixel
 * @param y Coordonnée y du pixel
 * @param file Fichier BMP
 */
void bmp24_writePixelValue(t_bmp24* img, int x, int y, FILE* file) {
    // Position du pixel dans le fichier (en tenant compte de l'inversion des lignes)
    int realY = img->height - 1 - y;
    long position = img->header.offset + (realY * img->width + x) * 3;

    uint8_t bgr[3];
    // Convertir RGB en BGR
    bgr[0] = img->data[y][x].blue;
    bgr[1] = img->data[y][x].green;
    bgr[2] = img->data[y][x].red;

    fseek(file, position, SEEK_SET);
    fwrite(bgr, 1, 3, file);
}

/**
 * @brief Écrit toutes les données de l'image dans un fichier
 * @param img Structure d'image
 * @param file Fichier BMP
 */
void bmp24_writePixelData(t_bmp24* img, FILE* file) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            bmp24_writePixelValue(img, x, y, file);
        }
    }
}

/**
 * @brief Charge une image BMP 24 bits depuis un fichier
 * @param filename Nom du fichier
 * @return Structure d'image chargée
 */
t_bmp24* bmp24_loadImage(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    // Lire l'en-tête complet du fichier
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    // Extraire les informations importantes
    uint16_t type = *(uint16_t*)&header[0];
    uint32_t offset = *(uint32_t*)&header[10];
    int32_t width = *(int32_t*)&header[18];
    int32_t height = *(int32_t*)&header[22];
    uint16_t colorDepth = *(uint16_t*)&header[28];

    // Vérifier le type de fichier
    if (type != 0x4D42) { // "BM" en little-endian
        printf("Erreur: Le fichier n'est pas un BMP valide\n");
        fclose(file);
        return NULL;
    }

    if (colorDepth != 24) {
        printf("Erreur: L'image n'est pas en 24 bits (profondeur: %d)\n", colorDepth);
        fclose(file);
        return NULL;
    }

    // Allouer l'image
    t_bmp24* img = bmp24_allocate(width, height, colorDepth);
    if (!img) {
        fclose(file);
        return NULL;
    }

    // Remplir les structures d'en-tête
    img->header.type = type;
    img->header.size = *(uint32_t*)&header[2];
    img->header.reserved1 = *(uint16_t*)&header[6];
    img->header.reserved2 = *(uint16_t*)&header[8];
    img->header.offset = offset;

    img->header_info.size = *(uint32_t*)&header[14];
    img->header_info.width = width;
    img->header_info.height = height;
    img->header_info.planes = *(uint16_t*)&header[26];
    img->header_info.bits = colorDepth;
    img->header_info.compression = *(uint32_t*)&header[30];
    img->header_info.imagesize = *(uint32_t*)&header[34];
    img->header_info.xresolution = *(int32_t*)&header[38];
    img->header_info.yresolution = *(int32_t*)&header[42];
    img->header_info.ncolors = *(uint32_t*)&header[46];
    img->header_info.importantcolors = *(uint32_t*)&header[50];

    // Se positionner au début des données
    fseek(file, offset, SEEK_SET);

    // Calculer le padding
    int padding = (4 - (width * 3) % 4) % 4;

    // Lire les données pixel par pixel
    for (int y = height - 1; y >= 0; y--) { // Les lignes sont inversées dans BMP
        for (int x = 0; x < width; x++) {
            unsigned char bgr[3];
            fread(bgr, 1, 3, file);
            img->data[y][x].blue = bgr[0];
            img->data[y][x].green = bgr[1];
            img->data[y][x].red = bgr[2];
        }
        // Ignorer le padding
        if (padding > 0) {
            fseek(file, padding, SEEK_CUR);
        }
    }

    fclose(file);
    return img;
}

/**
 * @brief Sauvegarde une image BMP 24 bits dans un fichier
 * @param img Structure d'image
 * @param filename Nom du fichier de sortie
 */
void bmp24_saveImage(t_bmp24* img, const char* filename) {
    if (!img) {
        printf("Erreur: Image NULL\n");
        return;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s en écriture\n", filename);
        return;
    }

    // Calculer le padding
    int padding = (4 - (img->width * 3) % 4) % 4;

    // Calculer la taille réelle des données
    int rowSize = img->width * 3 + padding;
    int dataSize = rowSize * img->height;

    // Mettre à jour les informations dans l'en-tête
    img->header_info.imagesize = dataSize;
    img->header.size = 54 + dataSize; // 54 = taille de l'en-tête

    // Créer l'en-tête complet
    unsigned char header[54] = {0};

    // En-tête de fichier
    *(uint16_t*)&header[0] = 0x4D42; // "BM"
    *(uint32_t*)&header[2] = img->header.size;
    *(uint16_t*)&header[6] = 0;
    *(uint16_t*)&header[8] = 0;
    *(uint32_t*)&header[10] = 54; // Offset des données

    // En-tête d'information
    *(uint32_t*)&header[14] = 40; // Taille de l'en-tête d'info
    *(int32_t*)&header[18] = img->width;
    *(int32_t*)&header[22] = img->height;
    *(uint16_t*)&header[26] = 1; // Planes
    *(uint16_t*)&header[28] = 24; // Bits par pixel
    *(uint32_t*)&header[30] = 0; // Pas de compression
    *(uint32_t*)&header[34] = dataSize;
    *(int32_t*)&header[38] = 2835; // 72 DPI
    *(int32_t*)&header[42] = 2835; // 72 DPI
    *(uint32_t*)&header[46] = 0;
    *(uint32_t*)&header[50] = 0;

    // Écrire l'en-tête
    fwrite(header, 1, 54, file);

    // Écrire les données pixel par pixel
    unsigned char paddingBytes[3] = {0, 0, 0};

    for (int y = img->height - 1; y >= 0; y--) { // Les lignes sont inversées dans BMP
        for (int x = 0; x < img->width; x++) {
            unsigned char bgr[3];
            bgr[0] = img->data[y][x].blue;
            bgr[1] = img->data[y][x].green;
            bgr[2] = img->data[y][x].red;
            fwrite(bgr, 1, 3, file);
        }
        // Écrire le padding si nécessaire
        if (padding > 0) {
            fwrite(paddingBytes, 1, padding, file);
        }
    }

    fclose(file);
    printf("Image sauvegardée avec succès dans %s\n", filename);
}

/**
 * @brief Affiche les informations d'une image
 * @param img Structure d'image
 */
void bmp24_printInfo(t_bmp24* img) {
    if (!img) {
        printf("Erreur: Image NULL\n");
        return;
    }

    printf("Image Info:\n");
    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);
    printf("Color Depth: %d\n", img->colorDepth);
    printf("Data Size: %u\n", img->header_info.imagesize);
}

/**
 * @brief Applique un effet négatif sur l'image
 * @param img Structure d'image
 */
void bmp24_negative(t_bmp24* img) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}

/**
 * @brief Convertit l'image en niveaux de gris
 * @param img Structure d'image
 */
void bmp24_grayscale(t_bmp24* img) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Calculer la moyenne des trois canaux
            uint8_t gray = (img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3;
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}

/**
 * @brief Ajuste la luminosité de l'image
 * @param img Structure d'image
 * @param value Valeur d'ajustement
 */
void bmp24_brightness(t_bmp24* img, int value) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Rouge
            int newRed = img->data[y][x].red + value;
            if (newRed > 255) newRed = 255;
            if (newRed < 0) newRed = 0;
            img->data[y][x].red = (uint8_t)newRed;

            // Vert
            int newGreen = img->data[y][x].green + value;
            if (newGreen > 255) newGreen = 255;
            if (newGreen < 0) newGreen = 0;
            img->data[y][x].green = (uint8_t)newGreen;

            // Bleu
            int newBlue = img->data[y][x].blue + value;
            if (newBlue > 255) newBlue = 255;
            if (newBlue < 0) newBlue = 0;
            img->data[y][x].blue = (uint8_t)newBlue;
        }
    }
}

/**
 * @brief Applique une convolution à un pixel
 * @param img Structure d'image
 * @param x Coordonnée x du pixel
 * @param y Coordonnée y du pixel
 * @param kernel Noyau de convolution
 * @param kernelSize Taille du noyau
 * @return Pixel résultant
 */
t_pixel bmp24_convolution(t_bmp24* img, int x, int y, float** kernel, int kernelSize) {
    t_pixel result = {0, 0, 0};
    int n = kernelSize / 2;
    float sumR = 0, sumG = 0, sumB = 0;

    // Vérifier les limites
    if (x < n || x >= img->width - n || y < n || y >= img->height - n) {
        return img->data[y][x];
    }

    // Appliquer le noyau
    for (int ky = -n; ky <= n; ky++) {
        for (int kx = -n; kx <= n; kx++) {
            int pixelY = y + ky;
            int pixelX = x + kx;

            sumR += img->data[pixelY][pixelX].red * kernel[ky + n][kx + n];
            sumG += img->data[pixelY][pixelX].green * kernel[ky + n][kx + n];
            sumB += img->data[pixelY][pixelX].blue * kernel[ky + n][kx + n];
        }
    }

    // Limiter les valeurs
    if (sumR < 0) sumR = 0;
    if (sumR > 255) sumR = 255;
    if (sumG < 0) sumG = 0;
    if (sumG > 255) sumG = 255;
    if (sumB < 0) sumB = 0;
    if (sumB > 255) sumB = 255;

    result.red = (uint8_t)sumR;
    result.green = (uint8_t)sumG;
    result.blue = (uint8_t)sumB;

    return result;
}

// Fonction utilitaire pour créer un noyau
float** createKernel(int size) {
    float** kernel = (float**)malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        kernel[i] = (float*)malloc(size * sizeof(float));
    }
    return kernel;
}

// Fonction utilitaire pour libérer un noyau
void freeKernel(float** kernel, int size) {
    for (int i = 0; i < size; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

/**
 * @brief Applique un flou simple (box blur)
 * @param img Structure d'image
 */
void bmp24_boxBlur(t_bmp24* img) {
    if (!img || !img->data) return;

    float** kernel = createKernel(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = 1.0f / 9.0f;
        }
    }

    // Créer une copie pour stocker le résultat
    t_bmp24* temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!temp) {
        freeKernel(kernel, 3);
        return;
    }

    // Appliquer le filtre
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    // Copier le résultat
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    bmp24_free(temp);
    freeKernel(kernel, 3);
}

/**
 * @brief Applique un flou gaussien
 * @param img Structure d'image
 */
void bmp24_gaussianBlur(t_bmp24* img) {
    if (!img || !img->data) return;

    float** kernel = createKernel(3);
    kernel[0][0] = 1.0f/16; kernel[0][1] = 2.0f/16; kernel[0][2] = 1.0f/16;
    kernel[1][0] = 2.0f/16; kernel[1][1] = 4.0f/16; kernel[1][2] = 2.0f/16;
    kernel[2][0] = 1.0f/16; kernel[2][1] = 2.0f/16; kernel[2][2] = 1.0f/16;

    t_bmp24* temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!temp) {
        freeKernel(kernel, 3);
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    bmp24_free(temp);
    freeKernel(kernel, 3);
}

/**
 * @brief Applique un filtre de détection de contours
 * @param img Structure d'image
 */
void bmp24_outline(t_bmp24* img) {
    if (!img || !img->data) return;

    float** kernel = createKernel(3);
    kernel[0][0] = -1; kernel[0][1] = -1; kernel[0][2] = -1;
    kernel[1][0] = -1; kernel[1][1] = 8;  kernel[1][2] = -1;
    kernel[2][0] = -1; kernel[2][1] = -1; kernel[2][2] = -1;

    t_bmp24* temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!temp) {
        freeKernel(kernel, 3);
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    bmp24_free(temp);
    freeKernel(kernel, 3);
}

/**
 * @brief Applique un filtre de relief
 * @param img Structure d'image
 */
void bmp24_emboss(t_bmp24* img) {
    if (!img || !img->data) return;

    float** kernel = createKernel(3);
    kernel[0][0] = -2; kernel[0][1] = -1; kernel[0][2] = 0;
    kernel[1][0] = -1; kernel[1][1] = 1;  kernel[1][2] = 1;
    kernel[2][0] = 0;  kernel[2][1] = 1;  kernel[2][2] = 2;

    t_bmp24* temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!temp) {
        freeKernel(kernel, 3);
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    bmp24_free(temp);
    freeKernel(kernel, 3);
}

/**
 * @brief Applique un filtre de netteté
 * @param img Structure d'image
 */
void bmp24_sharpen(t_bmp24* img) {
    if (!img || !img->data) return;

    float** kernel = createKernel(3);
    kernel[0][0] = 0;  kernel[0][1] = -1; kernel[0][2] = 0;
    kernel[1][0] = -1; kernel[1][1] = 5;  kernel[1][2] = -1;
    kernel[2][0] = 0;  kernel[2][1] = -1; kernel[2][2] = 0;

    t_bmp24* temp = bmp24_allocate(img->width, img->height, img->colorDepth);
    if (!temp) {
        freeKernel(kernel, 3);
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            temp->data[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = temp->data[y][x];
        }
    }

    bmp24_free(temp);
    freeKernel(kernel, 3);
}

/**
 * @brief Applique l'égalisation d'histogramme sur une image couleur
 * @param img Structure d'image
 */
void bmp24_equalize(t_bmp24* img) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    // Tableaux pour stocker les composantes YUV
    float** Y = (float**)malloc(img->height * sizeof(float*));
    float** U = (float**)malloc(img->height * sizeof(float*));
    float** V = (float**)malloc(img->height * sizeof(float*));

    for (int i = 0; i < img->height; i++) {
        Y[i] = (float*)malloc(img->width * sizeof(float));
        U[i] = (float*)malloc(img->width * sizeof(float));
        V[i] = (float*)malloc(img->width * sizeof(float));
    }

    // Convertir RGB vers YUV
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            float R = img->data[y][x].red;
            float G = img->data[y][x].green;
            float B = img->data[y][x].blue;

            Y[y][x] = 0.299f * R + 0.587f * G + 0.114f * B;
            U[y][x] = -0.14713f * R - 0.28886f * G + 0.436f * B;
            V[y][x] = 0.615f * R - 0.51499f * G - 0.10001f * B;
        }
    }

    // Calculer l'histogramme de la composante Y
    unsigned int hist[256] = {0};
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int yValue = (int)round(Y[y][x]);
            if (yValue < 0) yValue = 0;
            if (yValue > 255) yValue = 255;
            hist[yValue]++;
        }
    }

    // Calculer la CDF
    unsigned int cdf[256];
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + hist[i];
    }

    // Trouver la valeur minimale non nulle
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (hist[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    // Normaliser la CDF
    unsigned int N = img->width * img->height;
    unsigned int hist_eq[256];
    for (int i = 0; i < 256; i++) {
        if (N > cdf_min) {
            hist_eq[i] = round(((double)(cdf[i] - cdf_min) / (N - cdf_min)) * 255);
        } else {
            hist_eq[i] = 0;
        }
    }

    // Appliquer l'égalisation à la composante Y
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int yValue = (int)round(Y[y][x]);
            if (yValue < 0) yValue = 0;
            if (yValue > 255) yValue = 255;
            Y[y][x] = (float)hist_eq[yValue];
        }
    }

    // Convertir YUV vers RGB
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            float yVal = Y[y][x];
            float uVal = U[y][x];
            float vVal = V[y][x];

            float R = yVal + 1.13983f * vVal;
            float G = yVal - 0.39465f * uVal - 0.58060f * vVal;
            float B = yVal + 2.03211f * uVal;

            // Limiter les valeurs
            if (R < 0) R = 0;
            if (R > 255) R = 255;
            if (G < 0) G = 0;
            if (G > 255) G = 255;
            if (B < 0) B = 0;
            if (B > 255) B = 255;

            img->data[y][x].red = (uint8_t)round(R);
            img->data[y][x].green = (uint8_t)round(G);
            img->data[y][x].blue = (uint8_t)round(B);
        }
    }

    // Libérer la mémoire
    for (int i = 0; i < img->height; i++) {
        free(Y[i]);
        free(U[i]);
        free(V[i]);
    }
    free(Y);
    free(U);
    free(V);
}