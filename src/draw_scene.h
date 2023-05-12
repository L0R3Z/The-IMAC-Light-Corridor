#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "game_objects.h"
#include <vector>

extern Game myGame;
// extern float building_width;
// extern float building_height;
// extern float building_depth;
// extern float game_depth;
// extern float aperture;
// extern GameParameters myGameparameters;

void drawFrame();

// void drawTunnelPart(int tunnel_depth);

void drawCorridor(Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer);

void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer);

void drawWall(std::vector<Wall> myWalls, std::vector<Position> posBalls, Position posPlayer, Colors wallColor);

void drawBalls(std::vector<Ball> balls, std::vector<GLuint> myTextures);

void drawPlayer(Player myPlayer);

void drawInterface(Game myGame, std::vector<Position> posBalls, Position posPlayer);
