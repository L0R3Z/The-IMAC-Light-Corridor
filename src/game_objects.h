#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include <stdlib.h>
#include <time.h>

// Tool struct for colors
typedef struct Colors {
    float r, g, b, h, s, l;
    
    Colors() {
        
        
        this->r = (float) ((rand() % 180) + 75) / 255;
        this->g = (float) ((rand() % 180) + 75) / 255;
        this->b = (float) ((rand() % 180) + 75) / 255;
    }

    Colors(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void updateHSL() {
        float max_val = std::max(this->r, std::max(this->g, this->b));
        float min_val = std::min(r, std::min(this->g, this->b));
        float delta = max_val - min_val;

        // Calculate hue
        if (delta == 0) {
            this->h = 0;
        } else if (max_val == r) {
            this->h = 60 * ((g - b) / delta);
            if (this->h < 0) this->h += 360;
        } else if (max_val == g) {
            this->h = 60 * ((this->b - this->r) / delta + 2);
        } else {
            this->h = 60 * ((this->r - this->g) / delta + 4);
        }

        // Calculate lightness
        this->l = (max_val + min_val) / 2;

        // Calculate saturation
        if (delta == 0) {
            this->s = 0;
        } else {
            this->s = delta / (1 - std::abs(2 * this->l - 1));
        }
    }

    void updateRGB() {
        float max_val = (1 - std::abs(2 * this->l - 1)) * this->s;
        float min_val = this->l - max_val / 2.;

        float h_prime = this->h / 60.;
        float delta = max_val * (1 - std::abs(fmod(h_prime, 2) - 1));

        if (h_prime < 1) {
            this->r = max_val;
            this->g = delta;
            this->b = 0;
        } else if (h_prime < 2) {
            this->r = delta;
            this->g = max_val;
            this->b = 0;
        } else if (h_prime < 3) {
            this->r = 0;
            this->g = max_val;
            this->b = delta;
        } else if (h_prime < 4) {
            this->r = 0;
            this->g = delta;
            this->b = max_val;
        } else if (h_prime < 5) {
            this->r = delta;
            this->g = 0;
            this->b = max_val;
        } else {
            this->r = max_val;
            this->g = 0;
            this->b = delta;
        }

        this->r += min_val;
        this->g += min_val;
        this->b += min_val;
    }

    Colors generateAlternateColor(float h_delta, float s_delta, float b_delta) {
        Colors newColor = Colors(this->r, this->g, this->b);
        newColor.updateHSL();
        newColor.h += (float) h_delta/360;
        newColor.s += (float) s_delta/100;
        newColor.b += (float) b_delta/100;
        newColor.updateRGB();
        return newColor;
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
    int radius;
    Position pos;
    Speed speed;
    GLuint* texture; // texture from the textures array
    // Light light;

    Ball() {}

    Ball(int rad) {
        this->radius = rad;
        this->pos = Position(0,24.,0);
    }

    Ball(int rad, Position pos, Speed speed, GLuint* texture) {
        this->radius = rad;
        this->pos = pos;
        this->speed = speed;
        this->texture = texture;
    }
} Ball;

// Wall struct
typedef struct Wall {
    int width;
    int height;
    Position pos;
    float depth;
    GLuint* texture;
    // Colors color;

    Wall() {}

    Wall(int width, int height, Position pos, float depth) {
        this->width = width;
        this->height = height;
        this->pos = pos;
        this->depth=depth;
    }

    Wall(int width, int height, Position pos, GLuint* texture) {
        this->width = width;
        this->height = height;
        this->pos = pos;
        this->texture = texture;
    }
} Wall;

// WallStep struct for obtacles groups, situated at precise steps inside the corridor
typedef struct WallStep {
    float width;
    float height;
    float depth; // Correspond au fond du wallStep sur l'axe Y
    Position pos;
    Colors color;
    std::vector<Wall> walls;

    WallStep() {

    }

    WallStep(float depth) {
        this->depth = depth;
        this->color = Colors();
    }

    WallStep(float width, float height, Position pos, std::vector<Wall> walls) {
        this->width = width;
        this->height = height;
        this->pos = pos;
        this->walls = walls;
    }
} WallStep;

// Corridor struct
typedef struct Corridor {
    int width;
    int height;
    float depthOfAStep;
    int numberOfSteps;
    std::vector<WallStep> wallSteps;
    Colors colorSideWalls;
    Colors colorCeillingWalls;
    Colors colorRings;
    GLuint* sideWallsTexture;
    GLuint* groundTexture;
    GLuint* ceilingTexture;

    Corridor() {}

    Corridor(float depthOfAStep, int numberOfSteps) {
        srand(time(NULL));

        this->depthOfAStep = depthOfAStep;
        this->numberOfSteps = numberOfSteps;
        this->colorSideWalls = Colors();
        this->colorCeillingWalls = colorSideWalls.generateAlternateColor(20, -23, 14);
        this->colorRings = colorSideWalls.generateAlternateColor(8, 52, 15);
    }

    // Corridor(int width, int height) {
    //     this->width = width;
    //     this->height = height;
    // }

    Corridor(std::vector<WallStep> wallSteps) {
        this->wallSteps = wallSteps;
    }

    Corridor(int width, int height, std::vector<WallStep> wallSteps, GLuint* sideWallsTexture, GLuint* groundTexture, GLuint* ceilingTexture) {
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

    Player() {
        this->pos.y=0;
    }

    Player(float width) {
        this->pos.x=0;
        this->pos.y=0;
        this->pos.y=0;
        this->width=width;
        this->height=width;
    }

    void updatePosition(int positionX, int positionY, int WINDOW_WIDTH, int WINDOW_HEIGHT, float _viewSize, float aspectRatio) {
        this->pos.x = (_viewSize * aspectRatio) / WINDOW_WIDTH * positionX - (_viewSize * aspectRatio) / 2.0;
		this->pos.z = -_viewSize / WINDOW_HEIGHT * positionY + _viewSize / 2.0;
    }

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