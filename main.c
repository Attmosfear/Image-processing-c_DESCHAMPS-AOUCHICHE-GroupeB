/**
 * @file main.c
 * @author Projet TI202
 * @brief Programme principal pour le traitement d'images BMP
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"
#include "bmp24.h"
#include "filters.h"

// Variables globales pour stocker les images courantes
t_bmp8* currentImage8 = NULL;
t_bmp24* currentImage24 = NULL;
int imageType = 0; // 0: pas d'image, 8: image 8 bits, 24: image 24 bits


/**
 * @brief Affiche le menu principal
 */
void displayMainMenu(void) {
    printf("\n============ TRAITEMENT D'IMAGES BMP ============\n");
    printf("Veuillez choisir une option :\n");
    printf("1. Ouvrir une image\n");
    printf("2. Sauvegarder une image\n");
    printf("3. Appliquer un filtre\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Lancer les tests automatiques\n");
    printf("6. Quitter\n");
    printf(">>> Votre choix : ");
}


/**
 * @brief Affiche le menu des filtres pour images 8 bits
 */
void displayFilter8Menu(void) {
    printf("\nVeuillez choisir un filtre :\n");
    printf("1. Négatif\n");
    printf("2. Luminosité\n");
    printf("3. Binarisation\n");
    printf("4. Flou\n");
    printf("5. Flou gaussien\n");
    printf("6. Netteté\n");
    printf("7. Contours\n");
    printf("8. Relief\n");
    printf("9. Égalisation d'histogramme\n");
    printf("10. Retourner au menu précédent\n");
    printf(">>> Votre choix : ");
}

/**
 * @brief Affiche le menu des filtres pour images 24 bits
 */
void displayFilter24Menu(void) {
    printf("\nVeuillez choisir un filtre :\n");
    printf("1. Négatif\n");
    printf("2. Niveaux de gris\n");
    printf("3. Luminosité\n");
    printf("4. Flou\n");
    printf("5. Flou gaussien\n");
    printf("6. Netteté\n");
    printf("7. Contours\n");
    printf("8. Relief\n");
    printf("9. Égalisation d'histogramme\n");
    printf("10. Retourner au menu précédent\n");
    printf(">>> Votre choix : ");
}

/**
 * @brief Ouvre une image BMP
 */
void openImage(void) {
    char filename[256];
    printf("Chemin du fichier : ");
    scanf("%255s", filename);

    // Libérer l'image précédente si elle existe
    if (currentImage8) {
        bmp8_free(currentImage8);
        currentImage8 = NULL;
    }
    if (currentImage24) {
        bmp24_free(currentImage24);
        currentImage24 = NULL;
    }

    // Essayer d'abord comme image 8 bits
    currentImage8 = bmp8_loadImage(filename);
    if (currentImage8) {
        imageType = 8;
        printf("Image 8 bits chargée avec succès !\n");
        return;
    }

    // Si échec, essayer comme image 24 bits
    currentImage24 = bmp24_loadImage(filename);
    if (currentImage24) {
        imageType = 24;
        printf("Image 24 bits chargée avec succès !\n");
        return;
    }

    imageType = 0;
    printf("Erreur : Impossible de charger l'image\n");
}

/**
 * @brief Sauvegarde l'image courante
 */
void saveImage(void) {
    if (imageType == 0) {
        printf("Erreur : Aucune image chargée\n");
        return;
    }

    char filename[256];
    printf("Chemin du fichier : ");
    scanf("%255s", filename);

    if (imageType == 8 && currentImage8) {
        bmp8_saveImage(filename, currentImage8);
    } else if (imageType == 24 && currentImage24) {
        bmp24_saveImage(currentImage24, filename);
    }
}

/**
 * @brief Applique un filtre sur l'image 8 bits
 */
void applyFilter8(void) {
    int choice;
    float** kernel;

    displayFilter8Menu();
    scanf("%d", &choice);

    switch (choice) {
        case 1: // Négatif
            bmp8_negative(currentImage8);
            printf("Filtre négatif appliqué avec succès !\n");
            break;

        case 2: // Luminosité
            {
                int value;
                printf("Valeur de luminosité (-255 à 255) : ");
                scanf("%d", &value);
                bmp8_brightness(currentImage8, value);
                printf("Luminosité ajustée avec succès !\n");
            }
            break;

        case 3: // Binarisation
            {
                int threshold;
                printf("Valeur de seuil (0 à 255) : ");
                scanf("%d", &threshold);
                bmp8_threshold(currentImage8, threshold);
                printf("Binarisation appliquée avec succès !\n");
            }
            break;

        case 4: // Flou
            kernel = createBoxBlurKernel();
            bmp8_applyFilter(currentImage8, kernel, 3);
            freeFilterKernel(kernel, 3);
            printf("Filtre flou appliqué avec succès !\n");
            break;

        case 5: // Flou gaussien
            kernel = createGaussianBlurKernel();
            bmp8_applyFilter(currentImage8, kernel, 3);
            freeFilterKernel(kernel, 3);
            printf("Filtre flou gaussien appliqué avec succès !\n");
            break;

        case 6: // Netteté
            kernel = createSharpenKernel();
            bmp8_applyFilter(currentImage8, kernel, 3);
            freeFilterKernel(kernel, 3);
            printf("Filtre de netteté appliqué avec succès !\n");
            break;

        case 7: // Contours
            kernel = createOutlineKernel();
            bmp8_applyFilter(currentImage8, kernel, 3);
            freeFilterKernel(kernel, 3);
            printf("Filtre de contours appliqué avec succès !\n");
            break;

        case 8: // Relief
            kernel = createEmbossKernel();
            bmp8_applyFilter(currentImage8, kernel, 3);
            freeFilterKernel(kernel, 3);
            printf("Filtre de relief appliqué avec succès !\n");
            break;

        case 9: // Égalisation d'histogramme
            bmp8_equalize(currentImage8);
            printf("Égalisation d'histogramme appliquée avec succès !\n");
            break;

        case 10: // Retour
            return;

        default:
            printf("Choix invalide\n");
    }
}

/**
 * @brief Applique un filtre sur l'image 24 bits
 */
void applyFilter24(void) {
    int choice;

    displayFilter24Menu();
    scanf("%d", &choice);

    switch (choice) {
        case 1: // Négatif
            bmp24_negative(currentImage24);
            printf("Filtre négatif appliqué avec succès !\n");
            break;

        case 2: // Niveaux de gris
            bmp24_grayscale(currentImage24);
            printf("Conversion en niveaux de gris appliquée avec succès !\n");
            break;

        case 3: // Luminosité
            {
                int value;
                printf("Valeur de luminosité (-255 à 255) : ");
                scanf("%d", &value);
                bmp24_brightness(currentImage24, value);
                printf("Luminosité ajustée avec succès !\n");
            }
            break;

        case 4: // Flou
            bmp24_boxBlur(currentImage24);
            printf("Filtre flou appliqué avec succès !\n");
            break;

        case 5: // Flou gaussien
            bmp24_gaussianBlur(currentImage24);
            printf("Filtre flou gaussien appliqué avec succès !\n");
            break;

        case 6: // Netteté
            bmp24_sharpen(currentImage24);
            printf("Filtre de netteté appliqué avec succès !\n");
            break;

        case 7: // Contours
            bmp24_outline(currentImage24);
            printf("Filtre de contours appliqué avec succès !\n");
            break;

        case 8: // Relief
            bmp24_emboss(currentImage24);
            printf("Filtre de relief appliqué avec succès !\n");
            break;

        case 9: // Égalisation d'histogramme
            bmp24_equalize(currentImage24);
            printf("Égalisation d'histogramme appliquée avec succès !\n");
            break;

        case 10: // Retour
            return;

        default:
            printf("Choix invalide\n");
    }
}

/**
 * @brief Applique un filtre sur l'image courante
 */
void applyFilter(void) {
    if (imageType == 0) {
        printf("Erreur : Aucune image chargée\n");
        return;
    }

    if (imageType == 8) {
        applyFilter8();
    } else if (imageType == 24) {
        applyFilter24();
    }
}

/**
 * @brief Affiche les informations de l'image courante
 */
void displayImageInfo(void) {
    if (imageType == 0) {
        printf("Erreur : Aucune image chargée\n");
        return;
    }

    if (imageType == 8 && currentImage8) {
        bmp8_printInfo(currentImage8);
    } else if (imageType == 24 && currentImage24) {
        bmp24_printInfo(currentImage24);
    }
}

/**
 * @brief Lance les tests automatiques
 */
void launchAutomaticTests(void) {
    printf("\n=== LANCEMENT DES TESTS AUTOMATIQUES ===\n");

    // Vérifier l'existence des images de test
    FILE* test8 = fopen("barbara_gray.bmp", "rb");
    FILE* test24 = fopen("flowers_color.bmp", "rb");

    if (!test8 || !test24) {
        printf("Erreur : Images de test manquantes !\n");
        printf("Assurez-vous que les fichiers suivants sont présents :\n");
        printf("- barbara_gray.bmp (image 8 bits)\n");
        printf("- flowers_color.bmp (image 24 bits)\n");
        if (test8) fclose(test8);
        if (test24) fclose(test24);
        return;
    }
    fclose(test8);
    fclose(test24);

    // Créer les dossiers de sortie
    #ifdef _WIN32
        system("if not exist tests_8bits mkdir tests_8bits");
        system("if not exist tests_24bits mkdir tests_24bits");
    #else
        system("mkdir -p tests_8bits");
        system("mkdir -p tests_24bits");
    #endif

    // Tests pour les images 8 bits
    printf("\n--- Tests images 8 bits ---\n");
    t_bmp8* img8 = bmp8_loadImage("barbara_gray.bmp");
    if (img8) {
        // Test 1: Copie
        printf("1. Copie simple... ");
        bmp8_saveImage("tests_8bits/01_copie.bmp", img8);
        printf("OK\n");

        // Test 2: Négatif
        printf("2. Négatif... ");
        t_bmp8* temp8 = bmp8_loadImage("barbara_gray.bmp");
        bmp8_negative(temp8);
        bmp8_saveImage("tests_8bits/02_negatif.bmp", temp8);
        bmp8_free(temp8);
        printf("OK\n");

        // Test 3: Luminosité +50
        printf("3. Luminosité +50... ");
        temp8 = bmp8_loadImage("barbara_gray.bmp");
        bmp8_brightness(temp8, 50);
        bmp8_saveImage("tests_8bits/03_luminosite_plus50.bmp", temp8);
        bmp8_free(temp8);
        printf("OK\n");

        // Test 4: Binarisation
        printf("4. Binarisation (seuil 128)... ");
        temp8 = bmp8_loadImage("barbara_gray.bmp");
        bmp8_threshold(temp8, 128);
        bmp8_saveImage("tests_8bits/04_binarisation.bmp", temp8);
        bmp8_free(temp8);
        printf("OK\n");

        // Test 5: Flou
        printf("5. Flou simple... ");
        temp8 = bmp8_loadImage("barbara_gray.bmp");
        float** kernel = createBoxBlurKernel();
        bmp8_applyFilter(temp8, kernel, 3);
        freeFilterKernel(kernel, 3);
        bmp8_saveImage("tests_8bits/05_flou.bmp", temp8);
        bmp8_free(temp8);
        printf("OK\n");

        // Test 6: Contours
        printf("6. Détection de contours... ");
        temp8 = bmp8_loadImage("barbara_gray.bmp");
        kernel = createOutlineKernel();
        bmp8_applyFilter(temp8, kernel, 3);
        freeFilterKernel(kernel, 3);
        bmp8_saveImage("tests_8bits/06_contours.bmp", temp8);
        bmp8_free(temp8);
        printf("OK\n");

        // Test 7: Égalisation
        printf("7. Égalisation d'histogramme... ");
        temp8 = bmp8_loadImage("barbara_gray.bmp");
        bmp8_equalize(temp8);
        bmp8_saveImage("tests_8bits/07_egalisation.bmp", temp8);
        bmp8_free(temp8);
        printf("OK\n");

        bmp8_free(img8);
    }

    // Tests pour les images 24 bits
    printf("\n--- Tests images 24 bits ---\n");
    t_bmp24* img24 = bmp24_loadImage("flowers_color.bmp");
    if (img24) {
        // Test 1: Copie
        printf("1. Copie simple... ");
        bmp24_saveImage(img24, "tests_24bits/01_copie.bmp");
        printf("OK\n");

        // Test 2: Négatif
        printf("2. Négatif... ");
        t_bmp24* temp24 = bmp24_loadImage("flowers_color.bmp");
        bmp24_negative(temp24);
        bmp24_saveImage(temp24, "tests_24bits/02_negatif.bmp");
        bmp24_free(temp24);
        printf("OK\n");

        // Test 3: Niveaux de gris
        printf("3. Niveaux de gris... ");
        temp24 = bmp24_loadImage("flowers_color.bmp");
        bmp24_grayscale(temp24);
        bmp24_saveImage(temp24, "tests_24bits/03_niveaux_gris.bmp");
        bmp24_free(temp24);
        printf("OK\n");

        // Test 4: Luminosité
        printf("4. Luminosité +50... ");
        temp24 = bmp24_loadImage("flowers_color.bmp");
        bmp24_brightness(temp24, 50);
        bmp24_saveImage(temp24, "tests_24bits/04_luminosite.bmp");
        bmp24_free(temp24);
        printf("OK\n");

        // Test 5: Flou
        printf("5. Flou gaussien... ");
        temp24 = bmp24_loadImage("flowers_color.bmp");
        bmp24_gaussianBlur(temp24);
        bmp24_saveImage(temp24, "tests_24bits/05_flou_gaussien.bmp");
        bmp24_free(temp24);
        printf("OK\n");

        // Test 6: Contours
        printf("6. Détection de contours... ");
        temp24 = bmp24_loadImage("flowers_color.bmp");
        bmp24_outline(temp24);
        bmp24_saveImage(temp24, "tests_24bits/06_contours.bmp");
        bmp24_free(temp24);
        printf("OK\n");

        // Test 7: Égalisation
        printf("7. Égalisation d'histogramme... ");
        temp24 = bmp24_loadImage("flowers_color.bmp");
        bmp24_equalize(temp24);
        bmp24_saveImage(temp24, "tests_24bits/07_egalisation.bmp");
        bmp24_free(temp24);
        printf("OK\n");

        bmp24_free(img24);
    }

    printf("\n=== TESTS TERMINÉS ===\n");
    printf("Résultats disponibles dans :\n");
    printf("- tests_8bits/\n");
    printf("- tests_24bits/\n");
}

/**
 * @brief Fonction principale
 */
int main(void) {
    int choice;
    int running = 1;

    printf("=================================================\n");
    printf("   PROGRAMME DE TRAITEMENT D'IMAGES BMP\n");
    printf("   Projet TI202 - Algorithmique et Structures\n");
    printf("=================================================\n");

    while (running) {
        displayMainMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                openImage();
                break;

            case 2:
                saveImage();
                break;

            case 3:
                applyFilter();
                break;

            case 4:
                displayImageInfo();
                break;

            case 5:
                launchAutomaticTests();
                break;

            case 6:
                running = 0;
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }

    // Libérer la mémoire avant de quitter
    if (currentImage8) {
        bmp8_free(currentImage8);
    }
    if (currentImage24) {
        bmp24_free(currentImage24);
    }

    return 0;
}