#include "traffic_light.h"

TrafficLight::TrafficLight(Vector2 pos, StateType startType) : position(pos) {
    currentState = nullptr;
    ChangeState(startType);
}
TrafficLight::~TrafficLight() {
    if (currentState) delete currentState;
}
void TrafficLight::ChangeState(StateType type) {
    if (currentState) delete currentState;
    if (type == LIGHT_RED) currentState = new RedState();
    else if (type == LIGHT_ORANGE) currentState = new OrangeState();
    else currentState = new GreenState();
}
void TrafficLight::Draw() {
    int boxWidth = 14;
    int boxHeight = 42; 
    int radius = 4;
    DrawRectangle(position.x - boxWidth/2, position.y - 20, boxWidth, boxHeight, BLACK);
    StateType currentType = currentState->GetType();
    Color rCol = (currentType == LIGHT_RED) ? RED : MAROON;
    Color oCol = (currentType == LIGHT_ORANGE) ? ORANGE : (Color){100, 50, 0, 255};
    Color gCol = (currentType == LIGHT_GREEN) ? GREEN : DARKGREEN;
    DrawCircle(position.x, position.y - 12, radius, rCol);
    DrawCircle(position.x, position.y + 1, radius, oCol);
    DrawCircle(position.x, position.y + 14, radius, gCol);
    DrawRectangleLines(position.x - boxWidth/2, position.y - 20, boxWidth, boxHeight, GRAY);
}