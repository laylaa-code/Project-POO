#ifndef CAR_H
#define CAR_H

#include "raylib.h"

struct Car {
    Vector2 position; // Position actuelle de la voiture
    Vector2 speed;    // Vitesse de déplacement (directionnelle)
    Color color;      // Couleur d'affichage
    float width;      // Largeur de la voiture
    float height;     // Hauteur de la voiture
    bool isWaiting;   // Indique si la voiture est actuellement arrêtée
};

// Signature pour créer une voiture avec ses paramètres initiaux
Car CreateCar(float x, float y, float speedX, float speedY, Color color, bool isVertical);

// Met à jour la position de la voiture sur l'écran
void UpdateCar(Car* car, int screenWidth, int screenHeight);

// Dessine la voiture et ses détails (ex: phares)
void DrawCar(Car car);

#endif