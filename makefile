# Nom de votre programme final
EXEC = build/simulation.exe

# Le compilateur utilisé (g++ pour le C++)
CXX = g++

# Dossiers où se trouvent vos fichiers
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Options de compilation
# -Iinclude : dit au compilateur de chercher vos fichiers .h dans le dossier include
CXXFLAGS = -I$(INC_DIR) -Wall -std=c++17

# Bibliothèques Raylib (indispensable pour que Raylib fonctionne)
# Note : Si vous êtes sur Windows, on ajoute opengl32, gdi32, winmm
LDFLAGS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm

# Liste automatique de tous vos fichiers .cpp dans le dossier src/
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# --- RÈGLES DE COMPILATION ---

# Règle principale : tapez "make" pour compiler
all: $(EXEC)

$(EXEC): $(SOURCES)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(SOURCES) -o $(EXEC) $(CXXFLAGS) $(LDFLAGS)
	@echo "--------------------------------------"
	@echo "Compilation terminee ! Executez : ./$(EXEC)"
	@echo "--------------------------------------"

# Règle pour nettoyer le projet : tapez "make clean"
clean:
	@if exist $(BUILD_DIR) del /q $(BUILD_DIR)\*.exe