# Projet de Traitement d'Images BMP

## Description du projet

Ce projet est une application de traitement d'images développée en langage C dans le cadre du module TI202 - Algorithmique et Structures de Données. Le programme permet de charger, manipuler et sauvegarder des images au format BMP, en supportant à la fois les images en niveaux de gris (8 bits) et les images en couleur (24 bits).

## Fonctionnalités implémentées

### Images 8 bits (niveaux de gris)
- ✅ Lecture et écriture d'images BMP 8 bits
- ✅ Affichage des informations de l'image
- ✅ Négatif
- ✅ Ajustement de la luminosité
- ✅ Binarisation (seuillage)
- ✅ Filtres de convolution :
    - Flou simple (box blur)
    - Flou gaussien
    - Détection de contours
    - Relief (emboss)
    - Netteté (sharpen)
- ✅ Égalisation d'histogramme

### Images 24 bits (couleur)
- ✅ Lecture et écriture d'images BMP 24 bits
- ✅ Affichage des informations de l'image
- ✅ Négatif
- ✅ Conversion en niveaux de gris
- ✅ Ajustement de la luminosité
- ✅ Filtres de convolution :
    - Flou simple (box blur)
    - Flou gaussien
    - Détection de contours
    - Relief (emboss)
    - Netteté (sharpen)
- ✅ Égalisation d'histogramme (avec conversion en espace YUV)

### Interface utilisateur
- ✅ Menu interactif en ligne de commande
- ✅ Navigation intuitive entre les différentes options
- ✅ Messages d'erreur clairs et explicites
- ✅ Gestion automatique du type d'image (8 ou 24 bits)

### Programme de test automatique
- ✅ Test de toutes les fonctionnalités pour les images 8 bits
- ✅ Test de toutes les fonctionnalités pour les images 24 bits
- ✅ Génération automatique des résultats dans des dossiers séparés

## Instructions de compilation

### Prérequis
- Compilateur GCC
- Bibliothèque mathématique standard (libm)

### Compilation
```bash
# Compilation simple
gcc -o image_processing main.c bmp8.c bmp24.c filters.c -lm -Wall -Wextra -std=c99

# Ou avec le Makefile (compile les deux programmes)
make

# Pour nettoyer les fichiers temporaires
make clean

# Pour recompiler entièrement
make rebuild
```

## Instructions d'exécution

### Programme principal
```bash
# Lancer le programme principal
./image_processing

# Ou avec le Makefile
make run
```

### Programme de test automatique
```bash
# Lancer les tests
./test_images barbara_gray.bmp flowers_color.bmp

# Ou avec le Makefile
make test
```

Le programme de test créera automatiquement deux dossiers :
- `tests_8bits/` : Contient tous les résultats des tests sur l'image 8 bits
- `tests_24bits/` : Contient tous les résultats des tests sur l'image 24 bits

### Utilisation du programme principal

1. **Ouvrir une image** : Choisir l'option 1 et entrer le chemin du fichier BMP
2. **Appliquer un filtre** : Choisir l'option 3 et sélectionner le filtre désiré
3. **Sauvegarder l'image** : Choisir l'option 2 et entrer le nom du fichier de sortie
4. **Afficher les informations** : Choisir l'option 4 pour voir les propriétés de l'image
5. **Quitter** : Choisir l'option 5

### Images de test
- `barbara_gray.bmp` : Image 8 bits en niveaux de gris pour tester les fonctions de la partie 1
- `flowers_color.bmp` : Image 24 bits en couleur pour tester les fonctions de la partie 2

## Structure du projet

```
.
├── main.c              # Programme principal avec l'interface utilisateur
├── test.c              # Programme de test automatique
├── bmp8.h              # En-tête pour les images 8 bits
├── bmp8.c              # Implémentation pour les images 8 bits
├── bmp24.h             # En-tête pour les images 24 bits
├── bmp24.c             # Implémentation pour les images 24 bits
├── filters.h           # En-tête pour les filtres
├── filters.c           # Implémentation des filtres
├── Makefile            # Fichier de compilation
├── README.md           # Ce fichier
├── .gitignore          # Fichier d'exclusion Git
├── barbara_gray.bmp    # Image de test 8 bits
├── flowers_color.bmp   # Image de test 24 bits
├── tests_8bits/        # Dossier de sortie pour les tests 8 bits (généré)
└── tests_24bits/       # Dossier de sortie pour les tests 24 bits (généré)
```

## Architecture du code

### Types de données principaux

- `t_bmp8` : Structure pour représenter une image 8 bits
- `t_bmp24` : Structure pour représenter une image 24 bits
- `t_pixel` : Structure pour représenter un pixel RGB
- `t_bmp_header` : Structure pour l'en-tête BMP
- `t_bmp_info` : Structure pour les informations BMP

### Modules

1. **Module BMP8** : Gestion complète des images 8 bits
2. **Module BMP24** : Gestion complète des images 24 bits
3. **Module Filters** : Création et gestion des noyaux de convolution
4. **Module Main** : Interface utilisateur et orchestration
5. **Module Test** : Tests automatiques de toutes les fonctionnalités

## Bugs connus et corrigés

- ✅ Gestion du padding pour les images 24 bits
- ✅ Inversion correcte des lignes dans les fichiers BMP
- ✅ Lecture et écriture correctes des en-têtes
- Les pixels en bordure ne sont pas modifiés par les filtres de convolution (comportement normal)

## Améliorations possibles

- Ajouter d'autres filtres (médian, Sobel, etc.)
- Ajouter le support d'autres profondeurs de couleur (1, 4, 16, 32 bits)
- Créer une interface graphique
- Optimiser les performances des filtres de convolution
- Ajouter la possibilité d'annuler les modifications
- Implémenter un système de logs pour les opérations

## Auteurs

Projet réalisé dans le cadre du module TI202 - Algorithmique et Structures de Données
Efrei Paris - P1/P1-INT - 2024/2025

## Licence

Ce projet est réalisé dans un cadre pédagogique.oisir l'option 3 et sélectionner le filtre désiré
3. **Sauvegarder l'image** : Choisir l'option 2 et entrer le nom du fichier de sortie
4. **Afficher les informations** : Choisir l'option 4 pour voir les propriétés de l'image
5. **Quitter** : Choisir l'option 5

### Images de test
- `barbara_gray.bmp` : Image 8 bits en niveaux de gris pour tester les fonctions de la partie 1
- `flowers_color.bmp` : Image 24 bits en couleur pour tester les fonctions de la partie 2

## Structure du projet

```
.
├── main.c          # Programme principal avec l'interface utilisateur
├── bmp8.h          # En-tête pour les images 8 bits
├── bmp8.c          # Implémentation pour les images 8 bits
├── bmp24.h         # En-tête pour les images 24 bits
├── bmp24.c         # Implémentation pour les images 24 bits
├── filters.h       # En-tête pour les filtres
├── filters.c       # Implémentation des filtres
├── Makefile        # Fichier de compilation
├── README.md       # Ce fichier
├── barbara_gray.bmp    # Image de test 8 bits
└── flowers_color.bmp   # Image de test 24 bits
```

## Architecture du code

### Types de données principaux

- `t_bmp8` : Structure pour représenter une image 8 bits
- `t_bmp24` : Structure pour représenter une image 24 bits
- `t_pixel` : Structure pour représenter un pixel RGB
- `t_bmp_header` : Structure pour l'en-tête BMP
- `t_bmp_info` : Structure pour les informations BMP

### Modules

1. **Module BMP8** : Gestion complète des images 8 bits
2. **Module BMP24** : Gestion complète des images 24 bits
3. **Module Filters** : Création et gestion des noyaux de convolution
4. **Module Main** : Interface utilisateur et orchestration

## Bugs connus

- Les images doivent avoir une largeur et une hauteur multiples de 4 (pas de gestion du padding)
- Les pixels en bordure ne sont pas modifiés par les filtres de convolution
- Le programme ne vérifie pas la validité complète du format BMP (signature, compression, etc.)

## Améliorations possibles

- Ajouter la gestion du padding pour les images de toute taille
- Implémenter d'autres filtres (médian, Sobel, etc.)
- Ajouter le support d'autres profondeurs de couleur (1, 4, 16, 32 bits)
- Créer une interface graphique
- Optimiser les performances des filtres de convolution
- Ajouter la possibilité d'annuler les modifications

## Auteurs

Projet réalisé dans le cadre du module TI202 - Algorithmique et Structures de Données
Efrei Paris - P1/P1-INT - 2024/2025

## Licence

Ce projet est réalisé dans un cadre pédagogique.