@echo off
REM Script de test automatique pour le projet de traitement d'images
REM Projet TI202 - 2025

echo ==========================================
echo    SCRIPT DE TEST AUTOMATIQUE
echo    Traitement d'images BMP
echo ==========================================
echo.

REM Vérifier que les images de test existent
if not exist "barbara_gray.bmp" (
    echo Erreur : barbara_gray.bmp introuvable
    echo Veuillez placer l'image de test 8 bits dans le repertoire courant
    pause
    exit /b 1
)

if not exist "flowers_color.bmp" (
    echo Erreur : flowers_color.bmp introuvable
    echo Veuillez placer l'image de test 24 bits dans le repertoire courant
    pause
    exit /b 1
)

REM Compiler le projet
echo Compilation du projet...
if exist *.o del *.o
if exist image_processing.exe del image_processing.exe
if exist test_images.exe del test_images.exe

gcc -o image_processing main.c bmp8.c bmp24.c filters.c -lm -Wall -Wextra -std=c99
if errorlevel 1 (
    echo Erreur : La compilation du programme principal a echoue
    pause
    exit /b 1
)

gcc -o test_images test.c bmp8.c bmp24.c filters.c -lm -Wall -Wextra -std=c99
if errorlevel 1 (
    echo Erreur : La compilation du programme de test a echoue
    pause
    exit /b 1
)

echo Compilation reussie !
echo.

REM Exécuter les tests
echo Execution des tests...
test_images.exe barbara_gray.bmp flowers_color.bmp

if errorlevel 1 (
    echo Erreur : Les tests ont echoue
    pause
    exit /b 1
)

echo.
echo ==========================================
echo Verification des resultats...
echo.

REM Compter les fichiers générés
set count_8bits=0
set count_24bits=0

if exist tests_8bits (
    for %%f in (tests_8bits\*.bmp) do set /a count_8bits+=1
)

if exist tests_24bits (
    for %%f in (tests_24bits\*.bmp) do set /a count_24bits+=1
)

echo Fichiers generes :
echo - Images 8 bits  : %count_8bits% fichiers
echo - Images 24 bits : %count_24bits% fichiers
echo.

REM Afficher les dossiers de résultats
if exist tests_8bits (
    echo Dossier tests_8bits cree avec succes
)

if exist tests_24bits (
    echo Dossier tests_24bits cree avec succes
)

echo.
echo ==========================================
echo Tests termines avec succes !
echo Les resultats sont disponibles dans :
echo - tests_8bits\
echo - tests_24bits\
echo ==========================================
echo.
pause