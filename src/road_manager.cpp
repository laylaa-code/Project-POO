#include "road_manager.h"

void DrawRoadSystem(int screenWidth, int screenHeight) {
    Color asphalt = { 50, 50, 50, 255 };
    DrawRectangle(V_ROAD_1, 0, ROAD_WIDTH, screenHeight, asphalt);
    DrawRectangle(V_ROAD_2, 0, ROAD_WIDTH, screenHeight, asphalt);
    DrawRectangle(0, H_ROAD_1, screenWidth, ROAD_WIDTH, asphalt);
    DrawRectangle(0, H_ROAD_2, screenWidth, ROAD_WIDTH, asphalt);
    for (int i = 0; i < screenHeight; i += 40) {
        DrawRectangle(V_ROAD_1 + ROAD_WIDTH/2 - 2, i, 4, 20, WHITE);
        DrawRectangle(V_ROAD_2 + ROAD_WIDTH/2 - 2, i, 4, 20, WHITE);
    }
    for (int i = 0; i < screenWidth; i += 40) {
        DrawRectangle(i, H_ROAD_1 + ROAD_WIDTH/2 - 2, 20, 4, WHITE);
        DrawRectangle(i, H_ROAD_2 + ROAD_WIDTH/2 - 2, 20, 4, WHITE);
    }
}