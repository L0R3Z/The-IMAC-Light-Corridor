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

void drawSquareLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, Colors baseColor, Position point1, Position point2, Position point3, Position point4);

void drawTextureLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, GLuint texture, Position point1, Position point2, Position point3, Position point4);

void drawFrame();

void drawCorridor(Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer, GLuint texture);

void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer);

void drawWall(std::vector<Wall> myWalls, std::vector<Position> posBalls, Position posPlayer, Colors wallColor);

void drawBalls(std::vector<Ball> balls, std::vector<GLuint> myTextures);

void drawPlayer(Player myPlayer);

void drawInterface(Game myGame, std::vector<GLuint> myTextures, std::vector<Position> posBalls, Position posPlayer);
