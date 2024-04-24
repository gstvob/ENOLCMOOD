#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>
#include <math.h>


constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int PLAYER_SIZE = 60;
constexpr float PLAYER_FOV = M_PI / 3;
constexpr float HALF_FOV = PLAYER_FOV / 2;

constexpr int MAX_DEPTH = 20;
constexpr int HALF_WIDTH = (int) (WINDOW_WIDTH / 2);
constexpr int HALF_HEIGHT = (int) (WINDOW_HEIGHT / 2);
constexpr int NUM_RAYS = (int)(WINDOW_WIDTH / 2);
constexpr int HALF_NUM_RAYS = (int) (NUM_RAYS / 2);
const float DELTA_ANGLE = PLAYER_FOV / NUM_RAYS;
const float SCREEN_DIST = HALF_WIDTH / tan(HALF_FOV);
const float SCALE = (int) (WINDOW_WIDTH / NUM_RAYS);

constexpr int TEXTURE_SIZE = 1024;
constexpr int HALF_TEXTURE_SIZE = (int)TEXTURE_SIZE / 2;


#endif