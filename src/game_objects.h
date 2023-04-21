#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

// Tool struct for colors
typedef struct Colors {
    GLfloat r, g, b, h, s, l;
    
    Colors() {}

    Colors(GLfloat r, GLfloat g, GLfloat b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

} Colors;

// Tool struct for 3D positions
typedef struct Position {
    GLfloat x, y, z; // y -> profondeur

    Position() {}

    Position(GLfloat x, GLfloat y, GLfloat z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} Position;

// Tool struct for 3D speed
typedef struct Speed {
    GLfloat x, y, z;

    Speed() {}

    Speed(GLfloat x, GLfloat y, GLfloat z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} Speed;

// Ball struct
typedef struct Ball {
    GLfloat radius;
    Position pos;
    Speed speed;
    GLuint* texture; // texture from the textures array
    // Light light;

    Ball() {}

    Ball(GLfloat rad, Position pos, Speed speed, GLuint* texture) {
        this->radius = rad;
        this->pos = pos;
        this->speed = speed;
        this->texture = texture;
    }
} Ball;

// Wall struct
typedef struct Wall {
    GLfloat width;
    GLfloat height;
    Position pos;
    GLuint* texture;

    Wall() {}

    Wall(GLfloat width, GLfloat height, Position pos, GLuint* texture) {
        this->width = width;
        this->height = height;
        this->pos = pos;
        this->texture = texture;
    }
} Wall;

// WallStep struct for obtacles groups, situated at precise steps inside the corridor
typedef struct WallStep {
    GLfloat width;
    GLfloat height;
    GLfloat depth;
    Position pos;
    std::vector<Wall> walls;

    WallStep() {}

    WallStep(GLfloat width, GLfloat height, GLfloat depth, Position pos, std::vector<Wall> walls) {
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->pos = pos;
        this->walls = walls;
    }
} WallStep;

// Corridor struct
typedef struct Corridor {
    GLfloat width;
    GLfloat height;
    std::vector<WallStep> wallSteps;
    GLuint* sideWallsTexture;
    GLuint* groundTexture;
    GLuint* ceilingTexture;

    Corridor() {}

    Corridor(GLfloat width, GLfloat height) {
        this->width = width;
        this->height = height;
    }

    Corridor(GLfloat width, GLfloat height, std::vector<WallStep> wallSteps, GLuint* sideWallsTexture, GLuint* groundTexture, GLuint* ceilingTexture) {
        this->width = width;
        this->height = height;
        this->wallSteps = wallSteps;
        this->sideWallsTexture = sideWallsTexture;
        this->groundTexture = groundTexture;
        this->ceilingTexture = ceilingTexture;
    }
} Corridor;

// Player struct for the racket that the player controls
typedef struct Player : Wall {
    // Light light;

    Player() {}
} Player;

// General game struct containing all the elements needed
typedef struct Game {
    Player player;
    Corridor corridor;
    std::vector<Ball> balls;
    int lives;
    int gameState;
    int renderSkinId;

    Game() {}

    Game(Player player, Corridor corridor, std::vector<Ball> balls, int lives, int gameState, int renderSkinId) {
        this->player = player;
        this->corridor = corridor;
        this->balls = balls;
        this->lives = lives;
        this->gameState = gameState;
        this->renderSkinId = renderSkinId;
    }
} Game;

#endif