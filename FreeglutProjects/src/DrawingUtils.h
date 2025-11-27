#ifndef DRAWING_UTILS_H
#define DRAWING_UTILS_H

#include "Common.h"

void drawCylinder(float base, float top, float height, int slices);
void drawSphereSolid(float radius);
void drawStickman(float phase, float helmetHue = 0.9f); // helmetHue: 0.0-1.0 for different colors
void drawBike(int colorIndex = 0);  // colorIndex 0-4 for 5 different vibrant colors
void drawStickmanOnBike(float pedalPhase, float helmetHue, int colorIndex = 0); // Proper seated position with bike color
void drawGarage();
void drawGarageDoor(float openProgress); // 0.0 = closed, 1.0 = fully open
void drawOutdoorEnvironment(); // Road, sky, sun, trees
void drawParkEnvironment(); // Park: grass, trees, path
void drawRoad();
void drawSky();
void drawSun();
void drawTree(float scale = 1.0f);

// ===== KICK BUTTOWSKI STYLE ACTION EFFECTS =====
void drawSpeedLines(float x, float y, float z, float intensity, float direction = 1.0f);
void drawImpactBurst(float x, float y, float z, float size, float phase);
void drawZoomLines(float centerX, float centerY, float intensity);
void drawDustCloud(float x, float y, float z, float size, float age);
void drawStickmanWheelie(float pedalPhase, float helmetHue, float wheelieAngle, int colorIndex = 0);
void drawStickmanVictory(float phase, float helmetHue, int colorIndex = 0);
void drawActionText(const char* text, float x, float y, float size, float r, float g, float b);

#endif // DRAWING_UTILS_H
