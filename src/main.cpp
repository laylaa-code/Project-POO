#include "raylib.h"
#include "road_manager.h"
#include "car.h"
#include "traffic_light.h"
#include <vector>
#include <cmath>
#include <algorithm>

enum GameState { MENU, SIMULATION };

// --- STRUCTURE ET FONCTIONS POUR LES ARBRES ---
struct TreeDecoration {
    Vector2 position;
    float size;
    Color colorMain;
    Color colorHighlight;
};

void InitTrees(std::vector<TreeDecoration>& trees, int screenWidth, int screenHeight) {
    for (int i = 0; i < 300; i++) {
        float x = (float)GetRandomValue(0, screenWidth);
        float y = (float)GetRandomValue(0, screenHeight);
        float size = (float)GetRandomValue(15, 25);
        float margin = size + 5; 

        bool onVRoad1 = (x > V_ROAD_1 - margin && x < V_ROAD_1 + ROAD_WIDTH + margin);
        bool onVRoad2 = (x > V_ROAD_2 - margin && x < V_ROAD_2 + ROAD_WIDTH + margin);
        bool onHRoad1 = (y > H_ROAD_1 - margin && y < H_ROAD_1 + ROAD_WIDTH + margin);
        bool onHRoad2 = (y > H_ROAD_2 - margin && y < H_ROAD_2 + ROAD_WIDTH + margin);

        if (!onVRoad1 && !onVRoad2 && !onHRoad1 && !onHRoad2) {
            Color baseCol = (GetRandomValue(0, 2) == 0) ? DARKGREEN : LIME;
            Color highlightCol = ColorBrightness(baseCol, 0.2f);
            trees.push_back({{x, y}, size, baseCol, highlightCol});
        }
    }
}

// --- NOUVELLE DÉCORATION : TROTTOIRS ---
void DrawSidewalks(int screenWidth, int screenHeight) {
    float sw = 15.0f; 
    Color sidewalkCol = LIGHTGRAY;
    Color curbCol = GRAY;

    float v_roads[] = {V_ROAD_1, V_ROAD_2};
    float h_roads[] = {H_ROAD_1, H_ROAD_2};

    for (float rx : v_roads) {
        DrawRectangle((int)(rx - sw), 0, (int)sw, screenHeight, sidewalkCol);
        DrawRectangle((int)(rx - 2), 0, 2, screenHeight, curbCol); 
        DrawRectangle((int)(rx + ROAD_WIDTH), 0, (int)sw, screenHeight, sidewalkCol);
        DrawRectangle((int)(rx + ROAD_WIDTH), 0, 2, screenHeight, curbCol);
    }

    for (float ry : h_roads) {
        DrawRectangle(0, (int)(ry - sw), screenWidth, (int)sw, sidewalkCol);
        DrawRectangle(0, (int)(ry - 2), screenWidth, 2, curbCol);
        DrawRectangle(0, (int)(ry + ROAD_WIDTH), screenWidth, (int)sw, sidewalkCol);
        DrawRectangle(0, (int)(ry + ROAD_WIDTH), screenWidth, 2, curbCol);
    }
}

// --- NOUVELLE DÉCORATION : LAMPADAIRES ---
void DrawStreetLights(int screenWidth, int screenHeight) {
    float spacing = 120.0f;
    float v_roads[] = {V_ROAD_1, V_ROAD_2};
    float h_roads[] = {H_ROAD_1, H_ROAD_2};

    for (float rx : v_roads) {
        for (float y = 20; y < screenHeight; y += spacing) {
            // Lampadaire gauche
            DrawCircle((int)(rx - 7), (int)y, 3, BLACK);
            DrawCircle((int)(rx - 7), (int)y, 6, (Color){ 253, 249, 0, 120 });
            // Lampadaire droite
            DrawCircle((int)(rx + ROAD_WIDTH + 7), (int)y, 3, BLACK);
            DrawCircle((int)(rx + ROAD_WIDTH + 7), (int)y, 6, (Color){ 253, 249, 0, 120 });
        }
    }
    for (float ry : h_roads) {
        for (float x = 20; x < screenWidth; x += spacing) {
            // Lampadaire haut
            DrawCircle((int)x, (int)(ry - 7), 3, BLACK);
            DrawCircle((int)x, (int)(ry - 7), 6, (Color){ 253, 249, 0, 120 });
            // Lampadaire bas
            DrawCircle((int)x, (int)(ry + ROAD_WIDTH + 7), 3, BLACK);
            DrawCircle((int)x, (int)(ry + ROAD_WIDTH + 7), 6, (Color){ 253, 249, 0, 120 });
        }
    }
}

void DrawTreeDecorations(const std::vector<TreeDecoration>& trees) {
    for (const auto& tree : trees) {
        DrawCircleV(tree.position, tree.size, tree.colorMain);
        DrawCircle((int)tree.position.x, (int)(tree.position.y - 5), tree.size * 0.7f, tree.colorHighlight);
    }
}

void DrawCrosswalkDecoration() {
    float v_pts[] = {V_ROAD_1, V_ROAD_2};
    float h_pts[] = {H_ROAD_1, H_ROAD_2};
    for (float vp : v_pts) {
        for (float hp : h_pts) {
            for (int i = 5; i < ROAD_WIDTH; i += 15) {
                DrawRectangle((int)(vp + i), (int)(hp - 25), 8, 20, WHITE);
                DrawRectangle((int)(vp + i), (int)(hp + ROAD_WIDTH + 5), 8, 20, WHITE);
                DrawRectangle((int)(vp - 25), (int)(hp + i), 20, 8, WHITE);
                DrawRectangle((int)(vp + ROAD_WIDTH + 5), (int)(hp + i), 20, 8, WHITE);
            }
        }
    }
}

void DrawAmbulanceProcedural(Car car) {
    DrawRectangleRec({car.position.x, car.position.y, car.width, car.height}, WHITE);
    DrawRectangleLinesEx({car.position.x, car.position.y, car.width, car.height}, 2, BLACK);
    float centerX = car.position.x + car.width / 2;
    float centerY = car.position.y + car.height / 2;
    DrawRectangle((int)(centerX - 2), (int)(centerY - 8), 4, 16, RED);
    DrawRectangle((int)(centerX - 8), (int)(centerY - 2), 16, 4, RED);
    if ((int)(GetTime() * 10) % 2 == 0) {
        DrawCircle((int)(car.position.x + 5), (int)(car.position.y + 5), 3, BLUE);
        DrawCircle((int)(car.position.x + car.width - 5), (int)(car.position.y + 5), 3, RED);
    } else {
        DrawCircle((int)(car.position.x + 5), (int)(car.position.y + 5), 3, RED);
        DrawCircle((int)(car.position.x + car.width - 5), (int)(car.position.y + 5), 3, BLUE);
    }
}

void DrawCarProcedural(Car car) {
    if (car.width > car.height) { 
        DrawRectangle((int)(car.position.x + 5), (int)(car.position.y - 3), 10, 5, BLACK);
        DrawRectangle((int)(car.position.x + car.width - 15), (int)(car.position.y - 3), 10, 5, BLACK);
        DrawRectangle((int)(car.position.x + 5), (int)(car.position.y + car.height - 2), 10, 5, BLACK);
        DrawRectangle((int)(car.position.x + car.width - 15), (int)(car.position.y + car.height - 2), 10, 5, BLACK);
    } else { 
        DrawRectangle((int)(car.position.x - 3), (int)(car.position.y + 5), 5, 10, BLACK);
        DrawRectangle((int)(car.position.x + car.width - 2), (int)(car.position.y + 5), 5, 10, BLACK);
        DrawRectangle((int)(car.position.x - 3), (int)(car.position.y + car.height - 15), 5, 10, BLACK);
        DrawRectangle((int)(car.position.x + car.width - 2), (int)(car.position.y + car.height - 15), 5, 10, BLACK);
    }
    DrawRectangleRec({car.position.x, car.position.y, car.width, car.height}, car.color);
    DrawRectangleLinesEx({car.position.x, car.position.y, car.width, car.height}, 2, BLACK);
    Color glassColor = {255, 255, 255, 100};
    if (car.width > car.height) DrawRectangle((int)(car.position.x + 10), (int)(car.position.y + 4), (int)(car.width - 20), (int)(car.height - 8), glassColor);
    else DrawRectangle((int)(car.position.x + 4), (int)(car.position.y + 10), (int)(car.width - 8), (int)(car.height - 20), glassColor);
}

bool GuiButton(Rectangle bounds, const char* text, Color baseColor = LIGHTGRAY) {
    Vector2 mousePoint = GetMousePosition();
    bool clicked = false;
    Color btnColor = baseColor;
    if (CheckCollisionPointRec(mousePoint, bounds)) {
        btnColor = GRAY;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) clicked = true;
    }
    DrawRectangleRec(bounds, btnColor);
    DrawRectangleLinesEx(bounds, 2, BLACK);
    DrawText(text, (int)(bounds.x + (bounds.width / 2) - MeasureText(text, 20) / 2), (int)(bounds.y + (bounds.height / 2) - 10), 20, BLACK);
    return clicked;
}

bool IsInsideIntersection(Car car) {
    bool inVertical = (car.position.x + car.width > V_ROAD_1 && car.position.x < V_ROAD_1 + ROAD_WIDTH) ||
                      (car.position.x + car.width > V_ROAD_2 && car.position.x < V_ROAD_2 + ROAD_WIDTH);
    bool inHorizontal = (car.position.y + car.height > H_ROAD_1 && car.position.y < H_ROAD_1 + ROAD_WIDTH) ||
                        (car.position.y + car.height > H_ROAD_2 && car.position.y < H_ROAD_2 + ROAD_WIDTH);
    return inVertical && inHorizontal;
}

int main(int argc, char* argv[]) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Simulation - Smart City Priority");

    InitAudioDevice(); 
    Music bgMusic = LoadMusicStream("platformer.ogg");
    PlayMusicStream(bgMusic);

    SetTargetFPS(60);
    GameState currentState = MENU;

    std::vector<TreeDecoration> trees;
    InitTrees(trees, screenWidth, screenHeight);

    std::vector<Car> allCars;
    std::vector<Color> originalColors;
    Color carColors[] = { BLUE, RED, YELLOW, ORANGE, PURPLE, MAROON, SKYBLUE, GOLD, PINK, LIME, DARKBLUE, MAGENTA, VIOLET, BEIGE, BROWN, DARKGREEN, LIGHTGRAY, DARKGRAY, WHITE, BLACK };
    for (int i = 0; i < 20; i++) originalColors.push_back(carColors[i]);
    for (int i = 0; i < 20; i++) {
        int lane = i % 4;
        float offset = (i / 4) * 220.0f;
        Color col = (i == 0) ? WHITE : carColors[i];
        if (lane == 0)      allCars.push_back(CreateCar(0 - (i==0?0:offset), H_ROAD_1 + 20, 3.0f, 0.0f, col, false));
        else if (lane == 1) allCars.push_back(CreateCar(screenWidth + (i==0?0:offset), H_ROAD_2 + 45, -2.5f, 0.0f, col, false));
        else if (lane == 2) allCars.push_back(CreateCar(V_ROAD_1 + 20, 0 - (i==0?0:offset), 0.0f, 2.0f, col, true));
        else                allCars.push_back(CreateCar(V_ROAD_2 + 45, screenHeight + (i==0?0:offset), 0.0f, -3.5f, col, true));
    }

    std::vector<TrafficLight*> hLights, vLights;
    float v_pts[] = {V_ROAD_1, V_ROAD_2}, h_pts[] = {H_ROAD_1, H_ROAD_2};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            hLights.push_back(new TrafficLight({v_pts[i]-12, h_pts[j]-12}, LIGHT_GREEN));
            hLights.push_back(new TrafficLight({v_pts[i]+ROAD_WIDTH+12, h_pts[j]-12}, LIGHT_GREEN));
            vLights.push_back(new TrafficLight({v_pts[i]-12, h_pts[j]+ROAD_WIDTH+12}, LIGHT_RED));
            vLights.push_back(new TrafficLight({v_pts[i]+ROAD_WIDTH+12, h_pts[j]+ROAD_WIDTH+12}, LIGHT_RED));
        }
    }

    int currentCarCount = 20;
    float timer = 0.0f;
    bool isAdaptiveMode = true;
    bool accidentHappened = false;
    std::vector<int> crashedIndices; 
    float activeGreenDuration = 5.0f;

    while (!WindowShouldClose()) {
        UpdateMusicStream(bgMusic);

        if (currentState == MENU) {
            if (IsKeyPressed(KEY_ENTER)) currentState = SIMULATION;
        } 
        else {
            int waitingH = 0, waitingV = 0;
            for (int i = 0; i < currentCarCount; i++) if (allCars[i].isWaiting) (allCars[i].width > allCars[i].height) ? waitingH++ : waitingV++;
            if (isAdaptiveMode) {
                if (allCars[0].isWaiting) (allCars[0].width > allCars[0].height) ? waitingH += 50 : waitingV += 50;
                activeGreenDuration = (waitingH > waitingV + 2) ? 9.0f : (waitingV > waitingH + 2) ? 3.0f : 5.0f;
            } else { activeGreenDuration = 5.0f; }

            timer += GetFrameTime();
            float currentPos = fmod(timer, (activeGreenDuration + 2.0f) * 2);
            StateType hS, vS;
            if (currentPos < activeGreenDuration) { hS = LIGHT_GREEN; vS = LIGHT_RED; }
            else if (currentPos < activeGreenDuration + 2.0f) { hS = LIGHT_ORANGE; vS = LIGHT_RED; }
            else if (currentPos < (activeGreenDuration * 2) + 2.0f) { hS = LIGHT_RED; vS = LIGHT_GREEN; }
            else { hS = LIGHT_RED; vS = LIGHT_ORANGE; }

            for (auto l : hLights) l->ChangeState(hS);
            for (auto l : vLights) l->ChangeState(vS);

            for (int i = 0; i < currentCarCount; i++) {
                bool isCrashed = false;
                for (int idx : crashedIndices) if (idx == i) isCrashed = true;
                if (accidentHappened && isCrashed) { allCars[i].color = BLACK; allCars[i].isWaiting = true; continue; }
                bool stop = false;
                if (!IsInsideIntersection(allCars[i])) {
                    if (allCars[i].width > allCars[i].height) {
                        if (hS != LIGHT_GREEN) {
                            for (float vp : v_pts) {
                                if (allCars[i].speed.x > 0 && (allCars[i].position.x + allCars[i].width > vp - 15 && allCars[i].position.x < vp - 5)) stop = true;
                                if (allCars[i].speed.x < 0 && (allCars[i].position.x < vp + ROAD_WIDTH + 15 && allCars[i].position.x > vp + ROAD_WIDTH + 5)) stop = true;
                            }
                        }
                    } else {
                        if (vS != LIGHT_GREEN) {
                            for (float hp : h_pts) {
                                if (allCars[i].speed.y > 0 && (allCars[i].position.y + allCars[i].height > hp - 15 && allCars[i].position.y < hp - 5)) stop = true;
                                if (allCars[i].speed.y < 0 && (allCars[i].position.y < hp + ROAD_WIDTH + 15 && allCars[i].position.y > hp + ROAD_WIDTH + 5)) stop = true;
                            }
                        }
                    }
                }
                for (int j = 0; j < currentCarCount; j++) {
                    if (i == j) continue;
                    Rectangle sensor = { (allCars[i].speed.x > 0) ? allCars[i].position.x + allCars[i].width : (allCars[i].speed.x < 0) ? allCars[i].position.x - 40 : allCars[i].position.x, (allCars[i].speed.y > 0) ? allCars[i].position.y + allCars[i].height : (allCars[i].speed.y < 0) ? allCars[i].position.y - 40 : allCars[i].position.y, (allCars[i].speed.x != 0) ? 40 : allCars[i].width, (allCars[i].speed.y != 0) ? 40 : allCars[i].height };
                    if (CheckCollisionRecs(sensor, {allCars[j].position.x, allCars[j].position.y, allCars[j].width, allCars[j].height})) stop = true;
                }
                allCars[i].isWaiting = stop;
                if (!stop) UpdateCar(&allCars[i], screenWidth, screenHeight);
            }
        }

        BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawRoadSystem(screenWidth, screenHeight);
            DrawSidewalks(screenWidth, screenHeight);
            DrawStreetLights(screenWidth, screenHeight); // Ajout des lampadaires
            DrawTreeDecorations(trees);
            DrawCrosswalkDecoration();

            if (currentState == MENU) {
                DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});
                DrawText("GAME SMART CITY", screenWidth/2-150, 200, 40, GOLD);
                if (GuiButton({(float)screenWidth/2-100, 300, 200, 60}, "START", SKYBLUE)) currentState = SIMULATION;
            } else {
                for (int i = 0; i < currentCarCount; i++) (i == 0) ? DrawAmbulanceProcedural(allCars[i]) : DrawCarProcedural(allCars[i]);
                for (auto l : hLights) l->Draw();
                for (auto l : vLights) l->Draw();
                DrawRectangle(0, 0, screenWidth, 80, (Color){0, 0, 0, 120});
                if (GuiButton({20, 20, 40, 40}, "-")) if(currentCarCount > 1) currentCarCount--;
                DrawText(TextFormat("%i", currentCarCount), 65, 30, 20, WHITE);
                if (GuiButton({90, 20, 40, 40}, "+")) if(currentCarCount < 20) currentCarCount++;
                if (GuiButton({150, 20, 110, 40}, "Adaptatif", isAdaptiveMode ? SKYBLUE : LIGHTGRAY)) { isAdaptiveMode = true; timer = 0.0f; }
                if (GuiButton({270, 20, 80, 40}, "Fixe", !isAdaptiveMode ? SKYBLUE : LIGHTGRAY)) { isAdaptiveMode = false; timer = 0.0f; }
                if (GuiButton({365, 20, 115, 40}, accidentHappened ? "RESET" : "ACCIDENT", accidentHappened ? RED : LIGHTGRAY)) {
                    accidentHappened = !accidentHappened;
                    if (accidentHappened) {
                        crashedIndices.clear();
                        for (int i = 0; i < currentCarCount; i++) if (IsInsideIntersection(allCars[i])) crashedIndices.push_back(i);
                    } else {
                        for (int idx : crashedIndices) allCars[idx].color = originalColors[idx];
                        crashedIndices.clear();
                    }
                }
                DrawText(TextFormat("Mode: %s Vert: %.1fs", isAdaptiveMode ? "IA" : "FIXE", activeGreenDuration), 500, 30, 18, GOLD);
                if (isAdaptiveMode) {
                    int wH = 0, wV = 0;
                    for(int i=0; i<currentCarCount; i++) if(allCars[i].isWaiting) (allCars[i].width > allCars[i].height) ? wH++ : wV++;
                    DrawText(TextFormat("H:%i V:%i", wH, wV), 710, 30, 16, ORANGE);
                }
            }
        EndDrawing();
    }

    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    for (auto l : hLights) delete l;
    CloseWindow(); 
    return 0;
}