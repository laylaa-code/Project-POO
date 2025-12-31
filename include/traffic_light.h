#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "raylib.h"

// États pour l'identification
enum StateType { LIGHT_RED, LIGHT_ORANGE, LIGHT_GREEN };

class TrafficLight; // Forward declaration

// Interface de base pour le State Pattern
class ILightState {
public:
    virtual ~ILightState() {}
    virtual StateType GetType() = 0;
};

// Classes concrètes pour chaque état
class RedState : public ILightState { public: StateType GetType() override { return LIGHT_RED; } };
class OrangeState : public ILightState { public: StateType GetType() override { return LIGHT_ORANGE; } };
class GreenState : public ILightState { public: StateType GetType() override { return LIGHT_GREEN; } };

class TrafficLight {
private:
    ILightState* currentState;

public:
    Vector2 position; // Récupéré de ta struct originale

    TrafficLight(Vector2 pos, StateType startType);
    ~TrafficLight();

    void ChangeState(StateType type);
    void Draw(); // Utilise ta logique de dessin existante
    StateType GetState() { return currentState->GetType(); }
};

#endif