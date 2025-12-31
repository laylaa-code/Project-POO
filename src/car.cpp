#include "car.h"

// Initialisation d'une nouvelle instance de voiture
Car CreateCar(float x, float y, float speedX, float speedY, Color color, bool isVertical) {
    Car car;
    car.position = { x, y };
    car.speed = { speedX, speedY };
    car.color = color;
    car.isWaiting = false; // Initialisation de l'état d'attente

    // Définit la taille selon l'orientation (Verticale ou Horizontale)
    if (isVertical) {
        car.width = 25.0f;
        car.height = 40.0f;
    } else {
        car.width = 40.0f;
        car.height = 25.0f;
    }
    return car;
}

// Gère le déplacement et la téléportation aux bords de l'écran
void UpdateCar(Car* car, int screenWidth, int screenHeight) {
    car->position.x += car->speed.x;
    car->position.y += car->speed.y;

    // Téléportation (Boucle infinie du trafic)
    if (car->position.x > screenWidth + 50) car->position.x = -50;
    if (car->position.x < -50) car->position.x = screenWidth + 50;
    if (car->position.y > screenHeight + 50) car->position.y = -50;
    if (car->position.y < -50) car->position.y = screenHeight + 50;
}

// Rendu graphique de la voiture
void DrawCar(Car car) {
    // Dessin du corps principal
    DrawRectangleV(car.position, (Vector2){car.width, car.height}, car.color);
    
    // Ajout d'un petit rectangle jaune pour simuler les phares à l'avant
    if (car.speed.x > 0) { // Vers la droite
        DrawRectangle(car.position.x + car.width - 5, car.position.y + 2, 5, 5, YELLOW);
    } else if (car.speed.x < 0) { // Vers la gauche
        DrawRectangle(car.position.x, car.position.y + 2, 5, 5, YELLOW);
    } else if (car.speed.y > 0) { // Vers le bas
        DrawRectangle(car.position.x + 2, car.position.y + car.height - 5, 5, 5, YELLOW);
    } else if (car.speed.y < 0) { // Vers le haut
        DrawRectangle(car.position.x + 2, car.position.y, 5, 5, YELLOW);
    }
}