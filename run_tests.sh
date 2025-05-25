#!/bin/bash

# Script de test automatique pour le projet de traitement d'images
# Projet TI202 - 2025

echo "=========================================="
echo "   SCRIPT DE TEST AUTOMATIQUE"
echo "   Traitement d'images BMP"
echo "=========================================="
echo ""

# Vérifier que les images de test existent
if [ ! -f "barbara_gray.bmp" ]; then
    echo "Erreur : barbara_gray.bmp introuvable"
    echo "Veuillez placer l'image de test 8 bits dans le répertoire courant"
    exit 1
fi

if [ ! -f "flowers_color.bmp" ]; then
    echo "Erreur : flowers_color.bmp introuvable"
    echo "Veuillez placer l'image de test 24 bits dans le répertoire courant"
    exit 1
fi

# Compiler le projet
echo "Compilation du projet..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "Erreur : La compilation a échoué"
    exit 1
fi

echo "Compilation réussie !"
echo ""

# Exécuter les tests
echo "Exécution des tests..."
./test_images barbara_gray.bmp flowers_color.bmp

if [ $? -ne 0 ]; then
    echo "Erreur : Les tests ont échoué"
    exit 1
fi

echo ""
echo "=========================================="
echo "Vérification des résultats..."
echo ""

# Compter les fichiers générés
count_8bits=$(ls -1 tests_8bits/*.bmp 2>/dev/null | wc -l)
count_24bits=$(ls -1 tests_24bits/*.bmp 2>/dev/null | wc -l)

echo "Fichiers générés :"
echo "- Images 8 bits  : $count_8bits fichiers"
echo "- Images 24 bits : $count_24bits fichiers"
echo ""

# Afficher la taille des dossiers
if [ -d "tests_8bits" ]; then
    size_8bits=$(du -sh tests_8bits | cut -f1)
    echo "Taille du dossier tests_8bits  : $size_8bits"
fi

if [ -d "tests_24bits" ]; then
    size_24bits=$(du -sh tests_24bits | cut -f1)
    echo "Taille du dossier tests_24bits : $size_24bits"
fi

echo ""
echo "=========================================="
echo "Tests terminés avec succès !"
echo "Les résultats sont disponibles dans :"
echo "- tests_8bits/"
echo "- tests_24bits/"
echo "=========================================="