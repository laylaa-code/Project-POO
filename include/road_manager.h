#ifndef ROAD_MANAGER_H
#define ROAD_MANAGER_H

#include "raylib.h"

// Configuration
const float ROAD_WIDTH = 80.0f;

// Positions pour créer 4 intersections 
const float V_ROAD_1 = 200.0f;
const float V_ROAD_2 = 520.0f;
const float H_ROAD_1 = 150.0f;
const float H_ROAD_2 = 370.0f;

void DrawRoadSystem(int screenWidth, int screenHeight);

#endif