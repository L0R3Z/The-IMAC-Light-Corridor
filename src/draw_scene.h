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

void drawCorridor(Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer);

void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer);

void drawWall(std::vector<Wall> myWalls, std::vector<Position> posBalls, Position posPlayer, Colors wallColor, GLuint wallsTexture);

void drawBalls(std::vector<Ball> balls, GLuint ballsTexture);

void drawBonusBoxes(float x, float y, float z, float width, Game myGame, std::vector<Position> posBalls, Position posPlayer);

void drawPlayer(Player myPlayer);

void drawInterface(Game myGame, std::vector<GLuint> myTextures, std::vector<Position> posBalls, Position posPlayer);

void drawStartingMenu(Game myGame, std::vector<GLuint> myTextures);
void drawGameoverMenu(Game myGame, std::vector<GLuint> myTextures);
void drawVictoryMenu(Game myGame, std::vector<GLuint> myTextures);
