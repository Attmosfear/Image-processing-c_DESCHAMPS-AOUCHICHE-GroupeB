/**
 * @file bmp8.c
 * @author Malo DESCHAMPS et Samy AOUCHICHE
 * @brief Implémentation des fonctions pour les images BMP 8 bits en niveaux de gris
 * @date 2025
 */

#include "bmp8.h"

/**
 * @brief Charge une image BMP 8 bits depuis un fichier
 * @param filename Nom du fichier à charger
 * @return Pointeur vers l'image chargée, NULL en cas d'erreur
 */
t_bmp8* bmp8_loadImage(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    // Allouer la mémoire pour l'image
    t_bmp8* img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Erreur: Allocation mémoire échouée\n");
        fclose(file);
        return NULL;
    }

    // Lire l'en-tête
    fread(img->header, sizeof(unsigned char), 54, file);

    // Extraire les informations de l'en-tête
    img->width = *(unsigned int*)&img->header[18];
    img->height = *(unsigned int*)&img->header[22];
    img->colorDepth = *(unsigned short*)&img->header[28];
    img->dataSize = *(unsigned int*)&img->header[34];

    // Vérifier que c'est bien une image 8 bits
    if (img->colorDepth != 8) {
        printf("Erreur: L'image n'est pas en 8 bits (profondeur: %d)\n", img->colorDepth);
        free(img);
        fclose(file);
        return NULL;
    }

    // Si dataSize est 0, calculer la taille
    if (img->dataSize == 0) {
        img->dataSize = img->width * img->height;
    }

    // Lire la table de couleurs
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    // Allouer la mémoire pour les données
    img->data = (unsigned char*)malloc(img->dataSize);
    if (!img->data) {
        printf("Erreur: Allocation mémoire pour les données échouée\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Lire les données de l'image
    fread(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    return img;
}

/**
 * @brief Sauvegarde une image BMP 8 bits dans un fichier
 * @param filename Nom du fichier de sortie
 * @param img Pointeur vers l'image à sauvegarder
 */
void bmp8_saveImage(const char* filename, t_bmp8* img) {
    if (!img) {
        printf("Erreur: Image NULL\n");
        return;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s en écriture\n", filename);
        return;
    }

    // Écrire l'en-tête
    fwrite(img->header, sizeof(unsigned char), 54, file);

    // Écrire la table de couleurs
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);

    // Écrire les données
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    printf("Image sauvegardée avec succès dans %s\n", filename);
}

/**
 * @brief Libère la mémoire allouée pour une image
 * @param img Pointeur vers l'image à libérer
 */
void bmp8_free(t_bmp8* img) {
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

/**
 * @brief Affiche les informations d'une image
 * @param img Pointeur vers l'image
 */
void bmp8_printInfo(t_bmp8* img) {
    if (!img) {
        printf("Erreur: Image NULL\n");
        return;
    }

    printf("Image Info:\n");
    printf("Width: %u\n", img->width);
    printf("Height: %u\n", img->height);
    printf("Color Depth: %u\n", img->colorDepth);
    printf("Data Size: %u\n", img->dataSize);
}

/**
 * @brief Applique un effet négatif sur l'image
 * @param img Pointeur vers l'image
 */
void bmp8_negative(t_bmp8* img) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

/**
 * @brief Ajuste la luminosité de l'image
 * @param img Pointeur vers l'image
 * @param value Valeur d'ajustement (peut être négative)
 */
void bmp8_brightness(t_bmp8* img, int value) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        int newValue = img->data[i] + value;
        if (newValue > 255) newValue = 255;
        if (newValue < 0) newValue = 0;
        img->data[i] = (unsigned char)newValue;
    }
}

/**
 * @brief Applique un seuillage binaire sur l'image
 * @param img Pointeur vers l'image
 * @param threshold Valeur de seuil
 */
void bmp8_threshold(t_bmp8* img, int threshold) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

/**
 * @brief Applique un filtre de convolution sur l'image
 * @param img Pointeur vers l'image
 * @param kernel Noyau de convolution
 * @param kernelSize Taille du noyau (doit être impaire)
 */
void bmp8_applyFilter(t_bmp8* img, float** kernel, int kernelSize) {
    if (!img || !img->data || !kernel) {
        printf("Erreur: Paramètres invalides\n");
        return;
    }

    // Créer une copie des données pour stocker le résultat
    unsigned char* newData = (unsigned char*)malloc(img->dataSize);
    if (!newData) {
        printf("Erreur: Allocation mémoire échouée\n");
        return;
    }
    memcpy(newData, img->data, img->dataSize);

    int n = kernelSize / 2;

    // Appliquer le filtre (sans traiter les bords)
    for (int y = n; y < (int)img->height - n; y++) {
        for (int x = n; x < (int)img->width - n; x++) {
            float sum = 0.0;

            // Appliquer le noyau
            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    int pixelY = y + ky;
                    int pixelX = x + kx;
                    int pixelIndex = pixelY * img->width + pixelX;
                    sum += img->data[pixelIndex] * kernel[ky + n][kx + n];
                }
            }

            // Limiter la valeur entre 0 et 255
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            newData[y * img->width + x] = (unsigned char)sum;
        }
    }

    // Copier le résultat dans l'image originale
    memcpy(img->data, newData, img->dataSize);
    free(newData);
}

/**
 * @brief Calcule l'histogramme d'une image
 * @param img Pointeur vers l'image
 * @return Tableau de 256 entiers contenant l'histogramme
 */
unsigned int* bmp8_computeHistogram(t_bmp8* img) {
    if (!img || !img->data) {
        return NULL;
    }

    unsigned int* hist = (unsigned int*)calloc(256, sizeof(unsigned int));
    if (!hist) {
        printf("Erreur: Allocation mémoire échouée\n");
        return NULL;
    }

    // Compter les pixels pour chaque niveau de gris
    for (unsigned int i = 0; i < img->dataSize; i++) {
        hist[img->data[i]]++;
    }

    return hist;
}

/**
 * @brief Calcule la CDF et normalise l'histogramme
 * @param hist Histogramme d'entrée
 * @return Histogramme cumulé normalisé
 */
unsigned int* bmp8_computeCDF(unsigned int* hist) {
    if (!hist) {
        return NULL;
    }

    unsigned int* cdf = (unsigned int*)malloc(256 * sizeof(unsigned int));
    if (!cdf) {
        printf("Erreur: Allocation mémoire échouée\n");
        return NULL;
    }

    // Calculer la CDF
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // Trouver la valeur minimale non nulle
    unsigned int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (hist[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    // Nombre total de pixels
    unsigned int N = cdf[255];

    // Normaliser la CDF
    unsigned int* hist_eq = (unsigned int*)malloc(256 * sizeof(unsigned int));
    if (!hist_eq) {
        printf("Erreur: Allocation mémoire échouée\n");
        free(cdf);
        return NULL;
    }

    for (int i = 0; i < 256; i++) {
        if (N > cdf_min) {
            hist_eq[i] = round(((double)(cdf[i] - cdf_min) / (N - cdf_min)) * 255);
        } else {
            hist_eq[i] = 0;
        }
    }

    free(cdf);
    return hist_eq;
}

/**
 * @brief Applique l'égalisation d'histogramme sur l'image
 * @param img Pointeur vers l'image
 */
void bmp8_equalize(t_bmp8* img) {
    if (!img || !img->data) {
        printf("Erreur: Image invalide\n");
        return;
    }

    // Calculer l'histogramme
    unsigned int* hist = bmp8_computeHistogram(img);
    if (!hist) return;

    // Calculer l'histogramme égalisé
    unsigned int* hist_eq = bmp8_computeCDF(hist);
    if (!hist_eq) {
        free(hist);
        return;
    }

    // Appliquer la transformation
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (unsigned char)hist_eq[img->data[i]];
    }

    free(hist);
    free(hist_eq);
}