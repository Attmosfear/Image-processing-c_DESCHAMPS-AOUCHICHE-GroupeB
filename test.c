/**
 * @file test.c
 * @author Projet TI202
 * @brief Programme de test automatique pour toutes les fonctionnalités
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "bmp8.h"
#include "bmp24.h"
#include "filters.h"

// Pour la création de dossiers
#ifdef _WIN32
    #include <direct.h>
    #define mkdir(dir, mode) _mkdir(dir)
#endif

/**
 * @brief Crée un dossier s'il n'existe pas
 * @param path Chemin du dossier
 */
void createDirectory(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);
        printf("Dossier créé : %s\n", path);
    }
}

/**
 * @brief Teste toutes les fonctionnalités pour les images 8 bits
 * @param inputFile Fichier d'entrée
 * @param outputDir Dossier de sortie
 */
void testBmp8Functions(const char* inputFile, const char* outputDir) {
    printf("\n=== TESTS IMAGES 8 BITS ===\n");
    printf("Image source : %s\n", inputFile);
    printf("Dossier de sortie : %s\n\n", outputDir);

    // Charger l'image originale
    t_bmp8* original = bmp8_loadImage(inputFile);
    if (!original) {
        printf("Erreur : Impossible de charger l'image 8 bits\n");
        return;
    }

    printf("Image chargée avec succès !\n");
    bmp8_printInfo(original);
    printf("\n");

    // Test 1 : Copie simple
    {
        printf("Test 1 : Copie simple... ");
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/01_copie.bmp", outputDir);
        bmp8_saveImage(outputPath, original);
        printf("OK\n");
    }

    // Test 2 : Négatif
    {
        printf("Test 2 : Négatif... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        bmp8_negative(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/02_negatif.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 3 : Luminosité +50
    {
        printf("Test 3 : Luminosité +50... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        bmp8_brightness(img, 50);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/03_luminosite_plus50.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 4 : Luminosité -50
    {
        printf("Test 4 : Luminosité -50... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        bmp8_brightness(img, -50);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/04_luminosite_moins50.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 5 : Binarisation (seuil 128)
    {
        printf("Test 5 : Binarisation (seuil 128)... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        bmp8_threshold(img, 128);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/05_binarisation_128.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 6 : Flou simple
    {
        printf("Test 6 : Flou simple... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        float** kernel = createBoxBlurKernel();
        bmp8_applyFilter(img, kernel, 3);
        freeFilterKernel(kernel, 3);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/06_flou_simple.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 7 : Flou gaussien
    {
        printf("Test 7 : Flou gaussien... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        float** kernel = createGaussianBlurKernel();
        bmp8_applyFilter(img, kernel, 3);
        freeFilterKernel(kernel, 3);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/07_flou_gaussien.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 8 : Détection de contours
    {
        printf("Test 8 : Détection de contours... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        float** kernel = createOutlineKernel();
        bmp8_applyFilter(img, kernel, 3);
        freeFilterKernel(kernel, 3);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/08_contours.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 9 : Relief
    {
        printf("Test 9 : Relief... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        float** kernel = createEmbossKernel();
        bmp8_applyFilter(img, kernel, 3);
        freeFilterKernel(kernel, 3);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/09_relief.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 10 : Netteté
    {
        printf("Test 10 : Netteté... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        float** kernel = createSharpenKernel();
        bmp8_applyFilter(img, kernel, 3);
        freeFilterKernel(kernel, 3);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/10_nettete.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    // Test 11 : Égalisation d'histogramme
    {
        printf("Test 11 : Égalisation d'histogramme... ");
        t_bmp8* img = bmp8_loadImage(inputFile);
        bmp8_equalize(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/11_egalisation.bmp", outputDir);
        bmp8_saveImage(outputPath, img);
        bmp8_free(img);
        printf("OK\n");
    }

    bmp8_free(original);
    printf("\nTous les tests 8 bits terminés avec succès !\n");
}

/**
 * @brief Teste toutes les fonctionnalités pour les images 24 bits
 * @param inputFile Fichier d'entrée
 * @param outputDir Dossier de sortie
 */
void testBmp24Functions(const char* inputFile, const char* outputDir) {
    printf("\n=== TESTS IMAGES 24 BITS ===\n");
    printf("Image source : %s\n", inputFile);
    printf("Dossier de sortie : %s\n\n", outputDir);

    // Charger l'image originale
    t_bmp24* original = bmp24_loadImage(inputFile);
    if (!original) {
        printf("Erreur : Impossible de charger l'image 24 bits\n");
        return;
    }

    printf("Image chargée avec succès !\n");
    bmp24_printInfo(original);
    printf("\n");

    // Test 1 : Copie simple
    {
        printf("Test 1 : Copie simple... ");
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/01_copie.bmp", outputDir);
        bmp24_saveImage(original, outputPath);
        printf("OK\n");
    }

    // Test 2 : Négatif
    {
        printf("Test 2 : Négatif... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_negative(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/02_negatif.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 3 : Niveaux de gris
    {
        printf("Test 3 : Niveaux de gris... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_grayscale(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/03_niveaux_gris.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 4 : Luminosité +50
    {
        printf("Test 4 : Luminosité +50... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_brightness(img, 50);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/04_luminosite_plus50.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 5 : Luminosité -50
    {
        printf("Test 5 : Luminosité -50... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_brightness(img, -50);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/05_luminosite_moins50.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 6 : Flou simple
    {
        printf("Test 6 : Flou simple... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_boxBlur(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/06_flou_simple.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 7 : Flou gaussien
    {
        printf("Test 7 : Flou gaussien... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_gaussianBlur(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/07_flou_gaussien.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 8 : Détection de contours
    {
        printf("Test 8 : Détection de contours... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_outline(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/08_contours.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 9 : Relief
    {
        printf("Test 9 : Relief... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_emboss(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/09_relief.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 10 : Netteté
    {
        printf("Test 10 : Netteté... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_sharpen(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/10_nettete.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    // Test 11 : Égalisation d'histogramme
    {
        printf("Test 11 : Égalisation d'histogramme... ");
        t_bmp24* img = bmp24_loadImage(inputFile);
        bmp24_equalize(img);
        char outputPath[256];
        snprintf(outputPath, sizeof(outputPath), "%s/11_egalisation.bmp", outputDir);
        bmp24_saveImage(img, outputPath);
        bmp24_free(img);
        printf("OK\n");
    }

    bmp24_free(original);
    printf("\nTous les tests 24 bits terminés avec succès !\n");
}

/**
 * @brief Fonction principale du programme de test
 */
int main(int argc, char* argv[]) {
    printf("=================================================\n");
    printf("   PROGRAMME DE TEST - TRAITEMENT D'IMAGES BMP\n");
    printf("   Projet TI202 - Tests automatiques\n");
    printf("=================================================\n");

    // Vérifier les arguments
    if (argc < 3) {
        printf("\nUtilisation : %s <image_8bits.bmp> <image_24bits.bmp>\n", argv[0]);
        printf("Exemple : %s barbara_gray.bmp flowers_color.bmp\n", argv[0]);
        return 1;
    }

    const char* image8bits = argv[1];
    const char* image24bits = argv[2];

    // Créer les dossiers de sortie
    createDirectory("tests_8bits");
    createDirectory("tests_24bits");

    // Lancer les tests 8 bits
    testBmp8Functions(image8bits, "tests_8bits");

    // Lancer les tests 24 bits
    testBmp24Functions(image24bits, "tests_24bits");

    printf("\n=================================================\n");
    printf("TOUS LES TESTS SONT TERMINÉS !\n");
    printf("Résultats dans les dossiers :\n");
    printf("- tests_8bits/\n");
    printf("- tests_24bits/\n");
    printf("=================================================\n");

    return 0;
}
