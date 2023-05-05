#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "game_objects.h"
#include <vector>

extern float building_width;
extern float building_height;
extern float building_depth;
extern float aperture;
extern float game_depth;

void drawFrame();

void drawTunnelPart(int tunnel_depth);

void drawCorridor(Game myGame, Corridor myCorridor, Position posBall, Position posPlayer);

void drawWallStep(Game myGame, std::vector<WallStep> myWallSteps, Corridor myCorridor, Position posBall, Position posPlayer); 

void drawWall(Game myGame, std::vector<Wall> myWalls, Position posBall, Position posPlayer, Colors wallColor);

void drawPlayer(Player myPlayer);

void drawBalls(std::vector<Ball> balls);