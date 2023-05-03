#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Tool struct for 3D positions
typedef struct Position
{
    GLfloat x, y, z; // y is the depth axis

    Position() {}

    Position(GLfloat x, GLfloat y, GLfloat z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} Position;

// Tool struct for colors
typedef struct Colors
{
    float r, g, b, h, s, l;

    Colors()
    {
        this->r = (float)((rand() % 180) + 75) / 255;
        this->g = (float)((rand() % 180) + 75) / 255;
        this->b = (float)((rand() % 180) + 75) / 255;
    }

    Colors(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    // Function to update the HSL values of the color according to its RGB values
    void updateHSL()
    {
        float max_val = std::max(this->r, std::max(this->g, this->b));
        float min_val = std::min(r, std::min(this->g, this->b));
        float delta = max_val - min_val;

        // Calculate hue
        if (delta == 0)
        {
            this->h = 0;
        }
        else if (max_val == r)
        {
            this->h = 60 * ((g - b) / delta);
            if (this->h < 0)
                this->h += 360;
        }
        else if (max_val == g)
        {
            this->h = 60 * ((this->b - this->r) / delta + 2);
        }
        else
        {
            this->h = 60 * ((this->r - this->g) / delta + 4);
        }

        // Calculate lightness
        this->l = (max_val + min_val) / 2;

        // Calculate saturation
        if (delta == 0)
        {
            this->s = 0;
        }
        else
        {
            this->s = delta / (1 - std::abs(2 * this->l - 1));
        }
    }

    // Function to update the RGB values of the color according to its HSL values
    void updateRGB()
    {
        float max_val = (1 - std::abs(2 * this->l - 1)) * this->s;
        float min_val = this->l - max_val / 2.;

        float h_prime = this->h / 60.;
        float delta = max_val * (1 - std::abs(fmod(h_prime, 2) - 1));

        if (h_prime < 1)
        {
            this->r = max_val;
            this->g = delta;
            this->b = 0;
        }
        else if (h_prime < 2)
        {
            this->r = delta;
            this->g = max_val;
            this->b = 0;
        }
        else if (h_prime < 3)
        {
            this->r = 0;
            this->g = max_val;
            this->b = delta;
        }
        else if (h_prime < 4)
        {
            this->r = 0;
            this->g = delta;
            this->b = max_val;
        }
        else if (h_prime < 5)
        {
            this->r = delta;
            this->g = 0;
            this->b = max_val;
        }
        else
        {
            this->r = max_val;
            this->g = 0;
            this->b = delta;
        }

        this->r += min_val;
        this->g += min_val;
        this->b += min_val;
    }

    // Generates a new color from another by modifying its HSL values
    Colors generateAlternateColor(float h_delta, float s_delta, float l_delta)
    {
        Colors newColor = Colors(this->r, this->g, this->b);
        newColor.updateHSL();
        newColor.h += (float)h_delta / 360;
        newColor.s += (float)s_delta / 100;
        newColor.l += (float)l_delta / 100;
        if (newColor.h < 0)
        {
            newColor.h = 0;
        }
        if (newColor.s < 0)
        {
            newColor.s = 0;
        }
        if (newColor.l < 0)
        {
            newColor.l = 0;
        }
        newColor.updateRGB();
        return newColor;
    }

    // Calculation of the color to be displayed according to the points of light, the position of the object to be illuminated and its base color
    Colors displayColor(Position posBall, Position posPlayer, Position posObject, float game_depth)
    {
        // Position update based on game position
        posBall.y -= game_depth;
        posObject.y -= game_depth;

        // Print to console for debug
        // printf("\n\n\nposBall.x = %f | posPlayer.x = %f | posObject.x %f \n", posBall.x, posPlayer.x, posObject.x);
        // printf("posBall.y = %f | posPlayer.y = %f | posObject.y %f \n", posBall.y, posPlayer.y, posObject.y);
        // printf("posBall.z = %f | posPlayer.z = %f | posObject.z %f \n", posBall.z, posPlayer.z, posObject.z);

        Colors newColor = *this;
        this->updateHSL();

        // Calculation of the distance between the object and the points of light
        float distancePlayer = std::sqrt(pow((posObject.x - posPlayer.x), 2) + pow((posObject.y - posPlayer.y), 2) + pow((posObject.z - posPlayer.z), 2));
        float distanceBall = std::sqrt(pow((posObject.x - posBall.x), 2) + pow((posObject.y - posBall.y), 2) + pow((posObject.z - posBall.z), 2));

        // Set the light parameters
        float lightPropagationPlayer = 60.0f;
        float lightIntensityPlayer = 0.6f;
        float lightPropagationBall = 40.0f;
        float lightIntensityBall = 0.4f;

        // Calculate the brightness
        float brightnessPlayer = 1.0f - (distancePlayer / lightPropagationPlayer);
        float brightnessBall = 1.0f - (distanceBall / lightPropagationBall);

        // Clamp the brightness to [0, 1] range
        brightnessPlayer = std::max(std::min(brightnessPlayer, 1.0f), 0.0f);
        brightnessBall = std::max(std::min(brightnessBall, 1.0f), 0.0f);

        // Individual color calculation for each point of light
        Colors colorPlayer = this->generateAlternateColor(0, 0, -this->l * 100 + brightnessPlayer * lightIntensityPlayer * (this->l * 100));
        Colors colorBall = this->generateAlternateColor(0, 0, -this->l * 100 + brightnessBall * lightIntensityBall * (this->l * 100));

        // Fusion of the two calculated colors
        newColor = this->generateAlternateColor(0, 0, -this->l * 100 + ((colorPlayer.l + colorBall.l) / 1) * 100);
        return newColor;
    }

} Colors;

// Tool struct for 3D speed
typedef struct Speed
{
    GLfloat x, y, z;

    Speed() {}

    Speed(GLfloat x, GLfloat y, GLfloat z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} Speed;

// Ball struct
typedef struct Ball
{
    GLfloat radius;
    Position pos;
    Speed speed;
    GLuint *texture; // texture from the textures array
    // Light light;

    Ball() {}

    // Currently used constructor
    Ball(int rad)
    {
        this->radius = rad;
        this->pos = Position(0, 12 * 10., 0);
    }

    Ball(GLfloat rad, Position pos, Speed speed, GLuint *texture)
    {
        this->radius = rad;
        this->pos = pos;
        this->speed = speed;
        this->texture = texture;
    }

    void moveBall(float moveX, float moveY, float moveZ)
    {
        this->pos.x += moveX;
        this->pos.y += moveY;
        this->pos.z += moveZ;
    }

} Ball;

// Wall struct
typedef struct Wall
{
    GLfloat width;
    GLfloat height;
    Position pos;
    GLuint *texture;
    // Colors color;

    Wall() {}

    // Currently used constructor
    Wall(int width, int height, Position pos)
    {
        this->width = width;
        this->height = height;
        this->pos = pos;
        // this->depth = depth;
        // this->depthOfAStep = 12.0f;
    }

    Wall(GLfloat width, GLfloat height, Position pos, GLuint *texture)
    {
        this->width = width;
        this->height = height;
        this->pos = pos;
        this->texture = texture;
    }
} Wall;

// WallStep struct for obtacles groups, situated at precise steps inside the corridor
typedef struct WallStep
{
    float width;
    float height;
    // float depth; // Correspond au fond du wallStep sur l'axe Y
    Position pos;
    Colors color;
    // Colors displayColor;
    std::vector<Wall> walls;

    WallStep() {}

    // Currently used constructor
    WallStep(float y)
    {
        // this->depth = depth;
        this->pos = Position(0, y, 0);
        this->color = Colors();
    }

    WallStep(float width, float height, Position pos, std::vector<Wall> walls)
    {
        this->width = width;
        this->height = height;
        this->pos = pos;
        this->walls = walls;
    }

    // void lightImpact(Colors colorTunnel, float game_depth, float ballTempY)
    // {
    //     // this->displayColor = colorTunnel;
    //     Position posWall = Position(0, this->pos.y - game_depth, 0);
    //     colorTunnel.updateHSL();

    //     Position posPlayer = Position(0, 0, 0);
    //     float distancePlayer = std::sqrt(pow((posWall.x - posPlayer.x), 2) + pow((posWall.y - posPlayer.y), 2) + pow((posWall.z - posPlayer.z), 2));

    //     // Set the light parameters
    //     float lightPropagationPlayer = 60.0f;
    //     float lightIntensityPlayer = 0.6f;

    //     // Calculate the brightness
    //     float brightnessPlayer = 1.0f - (distancePlayer / lightPropagationPlayer);

    //     // Clamp the brightness to [0, 1] range
    //     brightnessPlayer = std::max(std::min(brightnessPlayer, 1.0f), 0.0f);

    //     Colors colorPlayer = colorTunnel.generateAlternateColor(0, 0, -colorTunnel.l * 100 + brightnessPlayer * lightIntensityPlayer * (colorTunnel.l * 100));

    //     Position posBall = Position(0, 12 * 10 - game_depth + ballTempY, 0);
    //     float distanceBall = std::sqrt(pow((posWall.x - posBall.x), 2) + pow((posWall.y - posBall.y), 2) + pow((posWall.z - posBall.z), 2));

    //     // Set the light parameters
    //     float lightPropagationBall = 40.0f;
    //     float lightIntensityBall = 0.4f;

    //     // Calculate the brightness
    //     float brightnessBall = 1.0f - (distanceBall / lightPropagationPlayer);

    //     // Clamp the brightness to [0, 1] range
    //     brightnessBall = std::max(std::min(brightnessBall, 1.0f), 0.0f);

    //     Colors colorBall = colorTunnel.generateAlternateColor(0, 0, -colorTunnel.l * 100 + brightnessBall * lightIntensityBall * (colorTunnel.l * 100));

    //     // this->displayColor = colorTunnel.generateAlternateColor(0, 0, -colorTunnel.l * 100 + ((colorPlayer.l + colorBall.l) / 1) * 100);
    //     // this->displayColor =  colorTunnel.generateAlternateColor(0, 0, -colorTunnel.l*100 + ((colorPlayer.l)/1)*100);
    // }

} WallStep;

// Corridor struct
typedef struct Corridor
{
    int width;
    int height;
    GLfloat depthOfAStep;
    int numberOfSteps;
    std::vector<WallStep> wallSteps;
    Colors colorSideWalls;
    Colors colorCeillingWalls;
    Colors colorRings;
    GLuint *sideWallsTexture;
    GLuint *groundTexture;
    GLuint *ceilingTexture;

    Corridor() {}

    // Currently used constructor
    Corridor(float depthOfAStep, int numberOfSteps)
    {
        srand(time(NULL));

        this->depthOfAStep = depthOfAStep;
        this->numberOfSteps = numberOfSteps;

        // Generating corridor colors from a random color
        this->colorSideWalls = Colors();
        this->colorCeillingWalls = colorSideWalls.generateAlternateColor(20, -23, 14);
        this->colorRings = colorSideWalls.generateAlternateColor(8, 52, 15);
    }

    // Corridor(int width, int height) {
    //     this->width = width;
    //     this->height = height;
    // }

    Corridor(std::vector<WallStep> wallSteps)
    {
        this->wallSteps = wallSteps;
    }

    Corridor(GLfloat width, GLfloat height, GLfloat stepDepth, std::vector<WallStep> wallSteps, GLuint *sideWallsTexture, GLuint *groundTexture, GLuint *ceilingTexture)
    {
        this->width = width;
        this->height = height;
        this->wallSteps = wallSteps;
        this->sideWallsTexture = sideWallsTexture;
        this->groundTexture = groundTexture;
        this->ceilingTexture = ceilingTexture;
    }

    void generateCorridor(float building_width, float building_height)
    {
        int randomTemp = 0;

        for (int i = 0; i < this->numberOfSteps; i++)
        {
            WallStep myWallStep = WallStep(this->depthOfAStep + this->depthOfAStep * i);

            randomTemp = rand() % 100 + 1;
            if (randomTemp < 80 && i != this->numberOfSteps - 1)
            {
                randomTemp = rand() % 5 + 1;
                switch (randomTemp)
                {
                // Moitié gauche
                case 1:
                {
                    Position myPosition = Position(-building_width / 2, myWallStep.pos.y, building_height / 2);
                    Wall myWall = Wall(building_width / 2, building_height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Moitie gauche (" << i << ")" << std::endl;
                    break;
                }

                // Moitié droite
                case 2:
                {
                    Position myPosition = Position(0, myWallStep.pos.y, building_height / 2);
                    Wall myWall = Wall(building_width / 2, building_height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Moitie droite (" << i << ")" << std::endl;
                    break;
                }

                // Petit gauche
                case 3:
                {
                    Position myPosition = Position(-building_width / 2, myWallStep.pos.y, building_height / 2);
                    Wall myWall = Wall(building_width / 4, building_height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Petit gauche (" << i << ")" << std::endl;
                    break;
                }

                // Petit droit
                case 4:
                {
                    Position myPosition = Position(building_width / 4, myWallStep.pos.y, building_height / 2);
                    Wall myWall = Wall(building_width / 4, building_height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Petit droit (" << i << ")" << std::endl;
                    break;
                }

                // Porte grande ouverte
                case 5:
                {
                    Position myPosition = Position(building_width / 4, myWallStep.pos.y, building_height / 2);
                    Wall myWall = Wall(building_width / 4, building_height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    Position mySecondPosition = Position(-building_width / 2, myWallStep.pos.y, building_height / 2);
                    Wall mySecondWall = Wall(building_width / 4, building_height, mySecondPosition);
                    myWallStep.walls.push_back(mySecondWall);
                    std::cout << "Porte grande ouverte (" << i << ")" << std::endl;
                    break;
                }

                default:
                {
                    std::cout << "No wall for the random value (" << randomTemp << ")" << std::endl;
                }
                }
            }

            this->wallSteps.push_back(myWallStep);
        }
    }

} Corridor;

// Player struct for the racket that the player controls
typedef struct Player : Wall
{
    // Light light;

    Player()
    {
        this->pos.y = 0;
    }

    // Currently used constructor
    Player(float width)
    {
        this->pos.x = 0;
        this->pos.y = 0;
        this->pos.z = 0;
        this->width = width;
        this->height = width;
    }

    // Updates the position of the puck relative to the user's mouse
    void updatePosition(int positionX, int positionY, int WINDOW_WIDTH, int WINDOW_HEIGHT, float _viewSize, float aspectRatio)
    {
        this->pos.x = (_viewSize * aspectRatio) / WINDOW_WIDTH * positionX - (_viewSize * aspectRatio) / 2.0;
        this->pos.z = -_viewSize / WINDOW_HEIGHT * positionY + _viewSize / 2.0;
    }

} Player;

typedef struct GameParameters
{
    float building_width;
    float building_height;
    float building_depth;
    float aperture; // Ouverture de la caméra 60.0 de base
    float game_depth;
    float ballTempY;

    GameParameters() {}
} GameParameters;

// General game struct containing all the elements needed
typedef struct Game
{
    GLfloat viewWidth;
    GLfloat viewHeight;
    Player player;
    Corridor corridor;
    std::vector<Ball> balls;
    int lives;
    int gameState;
    int renderSkinId;

    Game() {}

    Game(GLfloat viewWidth, GLfloat viewHeight, Player player, Corridor corridor, std::vector<Ball> balls, int lives, int gameState, int renderSkinId)
    {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->player = player;
        this->corridor = corridor;
        this->balls = balls;
        this->lives = lives;
        this->gameState = gameState;
        this->renderSkinId = renderSkinId;
    }
} Game;

#endif