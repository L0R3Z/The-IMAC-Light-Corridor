#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct Game;
struct Ball;

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

    void updatePosition(GLfloat x, GLfloat y, GLfloat z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
} Position;

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

    void updateColors(float r, float g, float b) {
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

    Colors generateComplementaryColor() {
        Colors newColor = Colors();
        newColor.r = 1 - this->r;
        newColor.g = 1 - this->g;
        newColor.b = 1 - this->b;
        newColor.updateHSL();
        newColor.l = 75./100;
        newColor.updateRGB();
        return newColor;
    }

    // Calculation of the color to be displayed according to the points of light, the position of the object to be illuminated and its base color
    Colors displayColor(std::vector<Position> posBalls, Position posPlayer, Position posObject, GLfloat gameDepth)
    {
        std::vector<float> distanceBalls;
        
        // Position update based on game position
        // for (int i = 0; i < posBalls.size(); i++)
	    // {
        //     posBalls[i].y -= gameDepth;
        // }
        // posObject.y -= gameDepth;

        // Position update based on game position
        posBalls[0].y -= gameDepth;
        posObject.y -= gameDepth;

        Colors newColor = *this;
        this->updateHSL();

        // Calculation of the distance between the object and the points of light
        float distancePlayer = std::sqrt(pow((posObject.x - posPlayer.x), 2) + pow((posObject.y - posPlayer.y), 2) + pow((posObject.z - posPlayer.z), 2));
        for (int i = 0; i < posBalls.size(); i++)
	    {
            distanceBalls.push_back(std::sqrt(pow((posObject.x - posBalls[i].x), 2) + pow((posObject.y - posBalls[i].y), 2) + pow((posObject.z - posBalls[i].z), 2)));
        }

        // Set the light parameters
        float lightPropagationPlayer = 60.0f;
        float lightIntensityPlayer = 0.6f;
        float lightPropagationBall = 40.0f;
        float lightIntensityBall = 0.4f / (posBalls.size());

        // Calculate the brightness
        float brightnessPlayer = 1.0f - (distancePlayer / lightPropagationPlayer);
        float brightnessBall = 0;
        for (int i = 0; i < posBalls.size(); i++)
	    {
            brightnessBall += 1.0f - (distanceBalls[i] / lightPropagationBall);
        }

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

// Wall struct
typedef struct Wall
{
    GLfloat width;
    GLfloat height;
    Position pos;

    Wall() {}

    // Currently used constructor
    Wall(GLint width, GLint height, Position pos)
    {
        this->width = width;
        this->height = height;
        this->pos = pos;
        // this->depth = depth;
        // this->depthOfAStep = 12.0f;
    }

} Wall;

// Bonus struct
typedef struct Bonus
{
    GLfloat width;
    GLfloat height;
    GLfloat depth;
    Position pos;
    std::string type;
    bool alive = false;
    void (*effect)();

    Bonus() {
        this->alive = true;
        int typeId = rand() % 2;
        if (typeId == 1)
        {
            this->type = "life";
        }
        else if (typeId == 2)
        {
            this->type = "sticky";
        }
    }

    Bonus(float width, float depth, float height, float wallStepY) {
        this->alive = true;

        int typeId = rand() % 2;
        if (typeId == 1)
        {
            this->type = "life";
        }
        else if (typeId == 2)
        {
            this->type = "sticky";
        }

        int xLeftLimit = - width / 3; int xRightLimit = width / 3;
        int zBottomLimit = - height / 3; int zTopLimit = height / 3;

        this->pos.x =  xLeftLimit + rand() % (xRightLimit - xLeftLimit + 1);
        this->pos.y = wallStepY - depth / 2;
        this->pos.z = zBottomLimit + rand() % (zTopLimit - zBottomLimit + 1);
        this->width = 2;
    }

    // void lifeEffect()
    // {
    //     printf("%s", "you won a life!");
    //     // myGame.lives++;
    // }

    // void stickyEffect()
    // {
    //     printf("%s", "sticky effect activated!")
    //     // myGame.gameState = 1;
    //     // myGame.bonusState = 'sticky'
    // }
} Bonus;

// WallStep struct for obtacles groups, situated at precise steps inside the corridor
typedef struct WallStep
{
    GLfloat width;
    GLfloat height;
    Position pos;
    Colors color;
    std::vector<Wall> walls;
    Bonus bonus;

    WallStep() {}

    // Currently used constructor
    WallStep(GLfloat y)
    {
        // this->depth = depth;
        this->pos = Position(0, y, 0);
        this->color = Colors();
    }

} WallStep;

// Corridor struct
typedef struct Corridor
{
    GLint width;
    GLint height;
    GLfloat depthOfAStep;
    GLint numberOfSteps;
    std::vector<WallStep> wallSteps;
    Colors colorSideWalls;
    Colors colorCeillingWalls;
    Colors colorRings;
    GLuint leftWallTexture;
    GLuint rightWallTexture;
    GLuint groundTexture;
    GLuint ceilingTexture;
    GLuint wallsTexture;
    GLuint ballTexture;
    GLuint bottomTexture;
    GLuint bonusBoxTexture;

    Corridor() {}

    // Currently used constructor
    Corridor(GLfloat depthOfAStep, GLint numberOfSteps, GLfloat buildingWidth, GLfloat buildingHeight)
    {
        srand(time(NULL));

        this->depthOfAStep = depthOfAStep;
        this->numberOfSteps = numberOfSteps;

        this->width = buildingWidth;
        this->height = buildingHeight;

        // Generating corridor colors from a random color
        this->colorSideWalls = Colors();
        this->colorCeillingWalls = colorSideWalls.generateAlternateColor(20, -23, 14);
        this->colorRings = colorSideWalls.generateAlternateColor(8, 52, 15);
    }

    Corridor(std::vector<WallStep> wallSteps)
    {
        this->wallSteps = wallSteps;
    }


    void generateCorridor()
    {
        int randomTemp = 0;

        for (int i = 0; i < this->numberOfSteps; i++)
        {
            WallStep myWallStep = WallStep(this->depthOfAStep + this->depthOfAStep * i);

            // Random generation of obstacle walls
            randomTemp = rand() % 100 + 1;
            if (randomTemp < 80 && i != this->numberOfSteps - 1)
            {
                randomTemp = rand() % 5 + 1;
                switch (randomTemp)
                {
                // Moitié gauche
                case 1:
                {
                    Position myPosition = Position(-this->width / 2, myWallStep.pos.y, this->height / 2);
                    Wall myWall = Wall(this->width / 2, this->height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Moitie gauche (" << i << ")" << std::endl;
                    break;
                }

                // Moitié droite
                case 2:
                {
                    Position myPosition = Position(0, myWallStep.pos.y, this->height / 2);
                    Wall myWall = Wall(this->width / 2, this->height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Moitie droite (" << i << ")" << std::endl;
                    break;
                }

                // Petit gauche
                case 3:
                {
                    Position myPosition = Position(-this->width / 2, myWallStep.pos.y, this->height / 2);
                    Wall myWall = Wall(this->width / 4, this->height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Petit gauche (" << i << ")" << std::endl;
                    break;
                }

                // Petit droit
                case 4:
                {
                    Position myPosition = Position(this->width / 4, myWallStep.pos.y, this->height / 2);
                    Wall myWall = Wall(this->width / 4, this->height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    std::cout << "Petit droit (" << i << ")" << std::endl;
                    break;
                }

                // Porte grande ouverte
                case 5:
                {
                    Position myPosition = Position(this->width / 4, myWallStep.pos.y, this->height / 2);
                    Wall myWall = Wall(this->width / 4, this->height, myPosition);
                    myWallStep.walls.push_back(myWall);
                    Position mySecondPosition = Position(-this->width / 2, myWallStep.pos.y, this->height / 2);
                    Wall mySecondWall = Wall(this->width / 4, this->height, mySecondPosition);
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

            // Random generation of item boxes
            randomTemp = rand() % 100 + 1;
            if (randomTemp < 20 && i != this->numberOfSteps - 1)
            {
                myWallStep.bonus = Bonus(this->width, this->depthOfAStep, this->height, myWallStep.pos.y);
            }

            this->wallSteps.push_back(myWallStep);
        }
    }

} Corridor;

// Player struct for the racket that the player controls
typedef struct Player : Wall
{

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
    void updatePositionMouse(GLint positionX, GLint positionY, GLint WINDOW_WIDTH, GLint WINDOW_HEIGHT, GLfloat _viewSize, GLfloat aspectRatio, GLfloat xLimit, GLfloat zLimit)
    {
        this->pos.x = std::max((GLfloat)-xLimit/2, std::min((GLfloat)xLimit/2, (GLfloat)((_viewSize * aspectRatio) / WINDOW_WIDTH * positionX - (_viewSize * aspectRatio) / 2.0)));
        this->pos.z = std::max((GLfloat)-zLimit/2, std::min((GLfloat)zLimit/2, (GLfloat)(-_viewSize / WINDOW_HEIGHT * positionY + _viewSize / 2.0)));
    }

} Player;

// Ball struct
typedef struct Ball
{
    GLfloat radius;
    Position pos;
    Speed speed;
    GLfloat defaultSpeed;
    bool isLaunched = false;

    Ball() {}

    // Currently used constructor
    Ball(GLint rad, GLfloat speed)
    {
        this->radius = rad;
        this->pos = Position(0, rad + 1, 0);
        this->defaultSpeed = speed;
        this->speed = Speed(0, speed, 0);
        // this->pos = Position(0, 12 * 10., 0);
    }

    // Move the ball with precise x, y and z values
    void moveBall(float moveX, float moveY, float moveZ)
    {
        this->pos.x += moveX;
        this->pos.y += moveY;
        this->pos.z += moveZ;
    }

    // Updates the position of the ball relative to the user's mouse
    void updatePositionMouse(GLint positionX, GLint positionY, GLint WINDOW_WIDTH, GLint WINDOW_HEIGHT, GLfloat _viewSize, GLfloat aspectRatio, GLfloat xLimit, GLfloat zLimit)
    {
        this->pos.x = std::max((GLfloat)-xLimit/2, std::min((GLfloat)xLimit/2, (GLfloat)((_viewSize * aspectRatio) / WINDOW_WIDTH * positionX - (_viewSize * aspectRatio) / 2.0)));
        this->pos.z = std::max((GLfloat)-zLimit/2, std::min((GLfloat)zLimit/2, (GLfloat)(-_viewSize / WINDOW_HEIGHT * positionY + _viewSize / 2.0)));
    }

    // Move the ball every frame
    void gameMove()
    {
        this->moveBall(this->speed.x, this->speed.y, this->speed.z);
    }

    void resetPosition(Position playerPos, GLfloat gameDepth)
    {
        this->pos = Position(playerPos.x, this->radius + gameDepth + 1, playerPos.z);
    }

    void resetSpeed()
    {
        this->speed = Speed(0, this->defaultSpeed, 0);
    }

    void checkWAllCollisions(Corridor corridor, Player player, GLfloat gameDepth, Bonus& currentBonus, int& gameState)
    {
        // IF collide with a bonus
        // ADD the bonus to currentBonus
        // INCREASE score by 1000

        int collideItemWalls = this->checkWallsCollisions(corridor.wallSteps);
        if (collideItemWalls == 1)
        {
            this->speed.y = -this->speed.y;
        }
        if (collideItemWalls == 2)
        {
            this->speed.x = -this->speed.x;
        }

        if (this->checkPlayerCollisions(player, gameDepth))
        {
            // if (currentBonus && currentBonus.type == "sticky")
            // {
            //     gameState = 1;
            //     currentBonus = NULL;
            // }
            // else
            // {
                const GLfloat MAX_SPEED = std::abs(this->speed.y);

                // Calculer la distance du point d'impact par rapport au centre de la raquette
                float distanceFromCenterX = this->pos.x - player.pos.x;
                float distanceFromCenterZ = this->pos.z - player.pos.z;

                // Calculer la direction du rebondissement en fonction de la distance du point d'impact
                float reboundDirectionX = distanceFromCenterX / (player.width / 2);
                float reboundDirectionZ = distanceFromCenterZ / (player.height / 2);

                // Mettre à jour les composantes de vitesse en fonction de la direction du rebondissement
                this->speed.x = reboundDirectionX * MAX_SPEED; // Aucun mouvement horizontal
                this->speed.y = -this->speed.y; // Inverser la direction verticale
                this->speed.z = reboundDirectionZ * MAX_SPEED; // Utiliser la direction du rebondissement pour la vitesse en Z
            // }
        }

        int collideItemCorridor = this->checkCorridorCollisions(corridor);
        if (collideItemCorridor == 1)
        {
            this->pos.x = corridor.width / 2 - this->radius;
            this->speed.x = -this->speed.x;
        }
        if (collideItemCorridor == 2)
        {
            this->pos.x = -corridor.width / 2 + this->radius;
            this->speed.x = -this->speed.x;
        }
        if (collideItemCorridor == 3)
        {
            this->pos.z = corridor.height / 2 - this->radius;
            this->speed.z = -this->speed.z;
        }
        if (collideItemCorridor == 4)
        {
            this->pos.z = -corridor.height / 2 + this->radius;
            this->speed.z = -this->speed.z;
        }
    }

    int checkWallsCollisions(std::vector<WallStep> wallSteps)
    {
        for (WallStep wallStep : wallSteps)
        {
            for (Wall wall : wallStep.walls)
            {
                // Calculate the potential new position of the ball
                float nextX = this->pos.x + this->speed.x;
                float nextY = this->pos.y + this->speed.y;
                float nextZ = this->pos.z + this->speed.z;

                // Calculate the bounds of the ball with the radius taken into account
                float ballMinX = nextX - this->radius;
                float ballMaxX = nextX + this->radius;
                float ballMinY = nextY - this->radius;
                float ballMaxY = nextY + this->radius;
                float ballMinZ = nextZ - this->radius;
                float ballMaxZ = nextZ + this->radius;

                // Check for side collision
                if (this->pos.y + this->radius >= wall.pos.y && this->pos.y - this->radius <= wall.pos.y &&
                    ballMaxX >= wall.pos.x && ballMinX <= wall.pos.x + wall.width)
                {
                    return 2;
                }

                // Check for front/back collision
                if (ballMaxX >= wall.pos.x && ballMinX <= wall.pos.x + wall.width &&
                    ballMaxY >= wall.pos.y && ballMinY <= wall.pos.y &&
                    ballMinZ >= wall.pos.z - wall.height && ballMaxZ <= wall.pos.z)
                {
                    // Front/back collision detected
                    return 1;
                }
            }
        }
        return 0;
    }

    bool checkPlayerCollisions(Player player, GLfloat gameDepth)
    {
        // Calculate the potential new position of the ball
        float nextX = this->pos.x + this->speed.x;
        float nextY = this->pos.y + this->speed.y;
        float nextZ = this->pos.z + this->speed.z;

        // Calculate the bounds of the ball with the radius taken into account
        float ballMinX = nextX - this->radius;
        float ballMaxX = nextX + this->radius;
        float ballMinY = nextY - this->radius;
        float ballMaxY = nextY + this->radius;
        float ballMinZ = nextZ - this->radius;
        float ballMaxZ = nextZ + this->radius;

        // Check if the potential new position collides with the wall
        if (ballMaxX >= player.pos.x - player.width / 2 && ballMinX <= player.pos.x + player.width / 2 &&
            ballMaxY >= player.pos.y + gameDepth && ballMinY <= player.pos.y + gameDepth &&
            ballMaxZ >= player.pos.z - player.height / 2 && ballMinZ <= player.pos.z + player.height / 2)
        {
            // Collision detected
            return true;
        }
        return false;
    }

    int checkCorridorCollisions(Corridor corridor)
    {
        // Calculate the potential new position of the ball
        float nextX = this->pos.x + this->speed.x;
        float nextY = this->pos.y + this->speed.y;
        float nextZ = this->pos.z + this->speed.z;

        // Calculate the bounds of the ball with the radius taken into account
        float ballMinX = nextX - this->radius;
        float ballMaxX = nextX + this->radius;
        float ballMinZ = nextZ - this->radius;
        float ballMaxZ = nextZ + this->radius;

        // Check if the potential new position collides with the corridor
        if (ballMaxX >= corridor.width / 2)
        {
            return 1;
        }
        else if (ballMinX <= -corridor.width / 2)
        {
            return 2;
        }
        if (ballMaxZ >= corridor.height / 2)
        {
            return 3;
        }
        else if (ballMinZ <= -corridor.height / 2)
        {
            return 4;
        }
        return 0;
    }

} Ball;

typedef struct GameParameters
{
    GLfloat buildingWidth;
    GLfloat buildingHeight;
    GLfloat buildingDepth;
    GLfloat aperture;
    GLfloat gameDepth;

    GameParameters() {}

} GameParameters;

// General game struct containing all the elements needed
typedef struct Game
{
    Player player;
    GLint score;
    Corridor corridor;
    std::vector<Ball> balls;
    GLint nbOfBallsLaunched;
    GLint lives;
    GLint gameState;
    Bonus currentBonus;
    GLint renderSkinId;
    GameParameters parameters;

    // Currently used constructor
    Game()
    {
        // paramètres fixes globaux du jeu
        this->parameters.buildingWidth = 24.0f;
        this->parameters.buildingHeight = this->parameters.buildingWidth / 2;
        this->parameters.buildingDepth = 12.0f;
        this->parameters.aperture = 60.0f; // Ouverture de la caméra 60.0 de base
        this->parameters.gameDepth = 0;
        this->gameState = 10;
    }

    Game(Player player, Corridor corridor, std::vector<Ball> balls, GLint lives, GLint gameState, GLint renderSkinId)
    {
        this->player = player;
        this->corridor = corridor;
        this->balls = balls;
        this->lives = lives;
        this->gameState = gameState;
        this->renderSkinId = renderSkinId;
    }

    void loadMenu() {}

    void loadGame()
    {
        // Déclaration des multiples variables utiles au jeu
        this->corridor = Corridor(this->parameters.buildingDepth, rand() % 30 + 10, this->parameters.buildingWidth, this->parameters.buildingHeight); // Profondeur d'une étape (buildingDepth) / Nombre d'étapes
        this->player = Player(this->parameters.buildingWidth / 6);
        if (this->balls.empty())
        {
            Ball myBall;
            this->balls.push_back(myBall);
        }
        this->balls[0] = Ball(this->parameters.buildingWidth / 12, .1);
        this->lives = 5;
        this->gameState = 1;
        this->parameters.gameDepth = 0;
        this->nbOfBallsLaunched = 0;

        this->corridor.generateCorridor();
    }

    void loadSkin(std::vector<GLuint> myTextures) {
        this->renderSkinId = rand() % 3;
        switch (this->renderSkinId)
        {
            case 1:
                this->corridor.colorRings = Colors(140./255, 0, 116./255);
                this->corridor.ballTexture = myTextures[10];
                this->corridor.bonusBoxTexture = myTextures[26];
                this->corridor.wallsTexture = myTextures[24];
                this->corridor.leftWallTexture = myTextures[21];
                this->corridor.rightWallTexture = myTextures[22];
                this->corridor.bottomTexture = myTextures[25];
                this->corridor.groundTexture = myTextures[23];
                this->corridor.ceilingTexture = myTextures[20];
                printf("Rainbow Resort loaded\n");
                break;
            default:
                this->corridor.ballTexture = myTextures[27];
                this->renderSkinId = 0;
                break;
        }
    }

    void winGame()
    {
        printf("\nYou... %s", "won :)");
        this->gameState = 12;
        // this->loadGame();
    }

    void looseGame()
    {
        printf("\nYou... %s", "lost :(");
        this->gameState = 11;
        // this->loadGame();
    }

    void takeDamage()
    {
        this->lives--;
        printf("\nRemaining lives: %i", this->lives);
        if (this->lives == 0)
        {
            this->looseGame();
        }
        else
        {
            this->balls[0].resetPosition(this->player.pos, this->parameters.gameDepth);
            this->balls[0].resetSpeed();
            this->balls[0].isLaunched = false;
            this->gameState = 1;
            this->nbOfBallsLaunched--;
        }
    }

    void moveFront(GLint distance)
    {
        if ((this->balls[0].pos.y - this->parameters.gameDepth - distance - 2) > 0)
        {
            this->parameters.gameDepth += distance;
            if (!this->balls[0].isLaunched)
            {
                this->balls[0].moveBall(0, distance, 0);
            }
            this->score += distance * 50;
            printf("%i\n", this->score);
        }
    }

    void checkWinDamage()
    {
        for (Ball ball : this->balls)
        {
            if (ball.isLaunched)
            {
                // Check if ball is behind player
                if (ball.pos.y - ball.radius - this->parameters.gameDepth < this->player.pos.y)
                {
                    this->takeDamage();
                }
                // Else, check if ball reached the end of the corridor
                else if (ball.pos.y + ball.radius > this->corridor.depthOfAStep * this->corridor.wallSteps.size())
                {
                    this->winGame();
                }
            }
        }
    }
} Game;

#endif