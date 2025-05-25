# Makefile pour le projet de traitement d'images BMP
# Projet TI202 - 2025

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
LDFLAGS = -lm

# Noms des exécutables
TARGET = image_processing_c
TEST_TARGET = test_images

# Fichiers sources communs
COMMON_SRCS = main.c bmp8.c bmp24.c filters.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Fichiers sources spécifiques
MAIN_SRC = main.c
TEST_SRC = test.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

# Headers
HEADERS = bmp8.h bmp24.h filters.h

# Règle par défaut
all: $(TARGET) $(TEST_TARGET)

# Règle pour créer l'exécutable principal
$(TARGET): $(MAIN_OBJ) $(COMMON_OBJS)
	$(CC) $(MAIN_OBJ) $(COMMON_OBJS) -o $(TARGET) $(LDFLAGS)

# Règle pour créer l'exécutable de test
$(TEST_TARGET): $(TEST_OBJ) $(COMMON_OBJS)
	$(CC) $(TEST_OBJ) $(COMMON_OBJS) -o $(TEST_TARGET) $(LDFLAGS)

# Règle pour compiler les fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers temporaires
clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)
	rm -rf tests_8bits tests_24bits

# Règle pour recompiler entièrement
rebuild: clean all

# Règle pour exécuter le programme principal
run: $(TARGET)
	./$(TARGET)

# Règle pour exécuter les tests
test: $(TEST_TARGET)
	./$(TEST_TARGET) barbara_gray.bmp flowers_color.bmp

# Règle pour compiler uniquement le programme principal
main: $(TARGET)

# Règle pour compiler uniquement le programme de test
test-only: $(TEST_TARGET)

# Règle pour vérifier la compilation
check: all
	@echo "=========================================="
	@echo "Compilation réussie !"
	@echo "=========================================="
	@echo "Deux programmes ont été générés :"
	@echo ""
	@echo "1. Programme principal (interactif) :"
	@echo "   ./$(TARGET)"
	@echo ""
	@echo "2. Programme de test (automatique) :"
	@echo "   ./$(TEST_TARGET) barbara_gray.bmp flowers_color.bmp"
	@echo "=========================================="

.PHONY: all clean rebuild run test main test-only check