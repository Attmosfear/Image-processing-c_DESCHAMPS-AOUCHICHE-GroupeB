//
// Created by desch on 21/03/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include "bmp24.h"
#include "kernel_loader.h"

// Variables globales pour les images
t_bmp8 *current_image8 = NULL;
t_bmp24 *current_image24 = NULL;
int image_type = 0; // 0 = aucune, 8 = image 8 bits, 24 = image 24 bits

void menu_principal() {
    printf("\n=== TRAITEMENT D'IMAGES BMP ===\n");
    printf("1. Ouvrir une image\n");
    printf("2. Sauvegarder une image\n");
    printf("3. Appliquer un traitement\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Quitter\n");
    printf(">>> Votre choix : ");
}

void menu_traitements_8bits() {
    printf("\n=== TRAITEMENTS IMAGE 8 BITS ===\n");
    printf("1. Négatif\n");
    printf("2. Luminosité\n");
    printf("3. Binarisation (threshold)\n");
    printf("4. Filtre Box Blur\n");
    printf("5. Filtre Gaussian Blur\n");
    printf("6. Filtre Contours (Outline)\n");
    printf("7. Filtre Relief (Emboss)\n");
    printf("8. Filtre Netteté (Sharpen)\n");
    printf("9. Retourner au menu principal\n");
    printf(">>> Votre choix : ");
}

void menu_traitements_24bits() {
    printf("\n=== TRAITEMENTS IMAGE 24 BITS ===\n");
    printf("1. Négatif\n");
    printf("2. Luminosité\n");
    printf("3. Conversion en niveaux de gris\n");
    printf("4. Filtre Box Blur\n");
    printf("5. Filtre Gaussian Blur\n");
    printf("6. Filtre Contours (Outline)\n");
    printf("7. Filtre Relief (Emboss)\n");
    printf("8. Filtre Netteté (Sharpen)\n");
    printf("9. Test convolution sur un pixel\n");
    printf("10. Retourner au menu principal\n");
    printf(">>> Votre choix : ");
}

int main() {
    int choix;
    char filename[256];

    printf("Bienvenue dans le programme de traitement d'images BMP!\n");

    while (1) {
        menu_principal();
        scanf("%d", &choix);

        switch (choix) {
            case 1: // Ouvrir une image
                printf("Chemin du fichier : ");
                scanf("%s", filename);

                // Libérer l'image précédente si elle existe
                if (current_image8 != NULL) {
                    bmp8_free(current_image8);
                    current_image8 = NULL;
                }
                if (current_image24 != NULL) {
                    bmp24_free(current_image24);
                    current_image24 = NULL;
                }

                // Essayer de charger comme image 8 bits d'abord
                current_image8 = bmp8_loadImage(filename);
                if (current_image8 != NULL) {
                    if (current_image8->colorDepth == 8) {
                        image_type = 8;
                        printf("Image 8 bits chargée avec succès!\n");
                    } else {
                        bmp8_free(current_image8);
                        current_image8 = NULL;
                        // Essayer de charger comme image 24 bits
                        current_image24 = bmp24_loadImage(filename);
                        if (current_image24 != NULL) {
                            image_type = 24;
                            printf("Image 24 bits chargée avec succès!\n");
                        } else {
                            printf("Erreur lors du chargement de l'image.\n");
                            image_type = 0;
                        }
                    }
                } else {
                    printf("Erreur lors du chargement de l'image.\n");
                    image_type = 0;
                }
                break;

            case 2: // Sauvegarder une image
                if (image_type == 0) {
                    printf("Aucune image chargée!\n");
                    break;
                }
                printf("Nom du fichier de sortie : ");
                scanf("%s", filename);

                if (image_type == 8) {
                    bmp8_saveImage(filename, current_image8);
                } else if (image_type == 24) {
                    bmp24_saveImage(current_image24, filename);
                }
                printf("Image sauvegardée avec succès!\n");
                break;

            case 3: // Appliquer un traitement
                if (image_type == 0) {
                    printf("Aucune image chargée!\n");
                    break;
                }

                if (image_type == 8) {
                    int choix_traitement;
                    menu_traitements_8bits();
                    scanf("%d", &choix_traitement);

                    switch (choix_traitement) {
                        case 1:
                            bmp8_negative(current_image8);
                            printf("Filtre négatif appliqué!\n");
                            break;
                        case 2: {
                            int value;
                            printf("Valeur de luminosité (-255 à 255) : ");
                            scanf("%d", &value);
                            bmp8_brightness(current_image8, value);
                            printf("Luminosité modifiée!\n");
                            break;
                        }
                        case 3: {
                            int threshold;
                            printf("Seuil de binarisation (0-255) : ");
                            scanf("%d", &threshold);
                            bmp8_threshold(current_image8, threshold);
                            printf("Binarisation appliquée!\n");
                            break;
                        }
                        case 4: {
                            float **kernel = get_box_blur_kernel();
                            bmp8_applyFilter(current_image8, kernel, 3);
                            free_kernel(kernel, 3);
                            printf("Filtre Box Blur appliqué!\n");
                            break;
                        }
                        case 5: {
                            float **kernel = get_gaussian_blur_kernel();
                            bmp8_applyFilter(current_image8, kernel, 3);
                            free_kernel(kernel, 3);
                            printf("Filtre Gaussian Blur appliqué!\n");
                            break;
                        }
                        case 6: {
                            float **kernel = get_outline_kernel();
                            bmp8_applyFilter(current_image8, kernel, 3);
                            free_kernel(kernel, 3);
                            printf("Filtre Contours appliqué!\n");
                            break;
                        }
                        case 7: {
                            float **kernel = get_emboss_kernel();
                            bmp8_applyFilter(current_image8, kernel, 3);
                            free_kernel(kernel, 3);
                            printf("Filtre Relief appliqué!\n");
                            break;
                        }
                        case 8: {
                            float **kernel = get_sharpen_kernel();
                            bmp8_applyFilter(current_image8, kernel, 3);
                            free_kernel(kernel, 3);
                            printf("Filtre Netteté appliqué!\n");
                            break;
                        }
                        case 9:
                            break;
                        default:
                            printf("Choix invalide!\n");
                    }
                } else if (image_type == 24) {
                    int choix_traitement;
                    menu_traitements_24bits();
                    scanf("%d", &choix_traitement);

                    switch (choix_traitement) {
                        case 1:
                            bmp24_negative(current_image24);
                            printf("Filtre négatif appliqué!\n");
                            break;
                        case 2: {
                            int value;
                            printf("Valeur de luminosité (-255 à 255) : ");
                            scanf("%d", &value);
                            // bmp24_brightness(current_image24, value); // À implémenter
                            printf("Fonction bmp24_brightness pas encore implémentée!\n");
                            break;
                        }
                        case 3:
                            // bmp24_grayscale(current_image24); // À implémenter
                            printf("Fonction bmp24_grayscale pas encore implémentée!\n");
                            break;
                        case 4: {
                            float **kernel = get_box_blur_kernel();
                            // bmp24_boxBlur(current_image24); // À implémenter avec bmp24_convolution
                            printf("Fonction bmp24_boxBlur pas encore implémentée!\n");
                            free_kernel(kernel, 3);
                            break;
                        }
                        case 5: {
                            float **kernel = get_gaussian_blur_kernel();
                            // bmp24_gaussianBlur(current_image24); // À implémenter avec bmp24_convolution
                            printf("Fonction bmp24_gaussianBlur pas encore implémentée!\n");
                            free_kernel(kernel, 3);
                            break;
                        }
                        case 6: {
                            float **kernel = get_outline_kernel();
                            // bmp24_outline(current_image24); // À implémenter avec bmp24_convolution
                            printf("Fonction bmp24_outline pas encore implémentée!\n");
                            free_kernel(kernel, 3);
                            break;
                        }
                        case 7: {
                            float **kernel = get_emboss_kernel();
                            // bmp24_emboss(current_image24); // À implémenter avec bmp24_convolution
                            printf("Fonction bmp24_emboss pas encore implémentée!\n");
                            free_kernel(kernel, 3);
                            break;
                        }
                        case 8: {
                            float **kernel = get_sharpen_kernel();
                            // bmp24_sharpen(current_image24); // À implémenter avec bmp24_convolution
                            printf("Fonction bmp24_sharpen pas encore implémentée!\n");
                            free_kernel(kernel, 3);
                            break;
                        }
                        case 9: {
                            // Test de la fonction de convolution sur un pixel
                            int test_x, test_y;
                            printf("Coordonnées du pixel à tester (x y) : ");
                            scanf("%d %d", &test_x, &test_y);

                            // Vérifier que les coordonnées sont valides
                            if (test_x >= 1 && test_x < current_image24->width - 1 &&
                                test_y >= 1 && test_y < current_image24->height - 1) {

                                printf("Pixel original à (%d,%d): R=%d G=%d B=%d\n",
                                       test_x, test_y,
                                       current_image24->data[test_y][test_x].red,
                                       current_image24->data[test_y][test_x].green,
                                       current_image24->data[test_y][test_x].blue);

                                float **kernel = get_box_blur_kernel();
                                t_pixel result = bmp24_convolution(current_image24, test_x, test_y, kernel, 3);

                                printf("Pixel après convolution Box Blur: R=%d G=%d B=%d\n",
                                       result.red, result.green, result.blue);

                                free_kernel(kernel, 3);
                            } else {
                                printf("Coordonnées invalides! Utilisez des valeurs entre (1,1) et (%d,%d)\n",
                                       current_image24->width-2, current_image24->height-2);
                            }
                            break;
                        }
                        case 10:
                            break;
                        default:
                            printf("Choix invalide!\n");
                    }
                }
                break;

            case 4: // Afficher les informations
                if (image_type == 8 && current_image8 != NULL) {
                    bmp8_printInfo(current_image8);
                } else if (image_type == 24 && current_image24 != NULL) {
                    printf("Image Info (24 bits):\n");
                    printf("\tWidth: %d\n", current_image24->width);
                    printf("\tHeight: %d\n", current_image24->height);
                    printf("\tColor Depth: %d\n", current_image24->colorDepth);
                } else {
                    printf("Aucune image chargée!\n");
                }
                break;

            case 5: // Quitter
                // Libérer la mémoire avant de quitter
                if (current_image8 != NULL) {
                    bmp8_free(current_image8);
                }
                if (current_image24 != NULL) {
                    bmp24_free(current_image24);
                }
                printf("Au revoir!\n");
                return 0;

            default:
                printf("Choix invalide!\n");
        }
    }
}