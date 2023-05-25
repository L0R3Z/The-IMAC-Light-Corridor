#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "3D_tools.h"
#include "draw_scene.h"
#include "game_objects.h"

using namespace std;

/* Textures table */
vector<GLuint> textures;

static double xpos, ypos;

/* Window properties */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 400;
static const char WINDOW_TITLE[] = "The IMAC Light Corridor";
static float aspectRatio = 1.0;
static const int scalingFactor = 4;

Game myGame = Game();
vector<Position> posBalls;

static const float _viewSize = myGame.parameters.buildingHeight; // Correspond à building height à cause du cadrage sur le tunnel (peut changer)

struct Vertex
{
	GLfloat positionX;
	GLfloat positionY;
	GLfloat colorR;
	GLfloat colorG;
	GLfloat colorB;

	Vertex(int positionX, int positionY)
	{
		this->positionX = (_viewSize * aspectRatio) / WINDOW_WIDTH * positionX - (_viewSize * aspectRatio) / 2.0;
		this->positionY = -_viewSize / WINDOW_HEIGHT * positionY + _viewSize / 2.0;
		this->colorR = ((((GLfloat)(rand() % 180)) + 75) / 255);
		this->colorG = ((((GLfloat)(rand() % 180)) + 75) / 255);
		this->colorB = ((((GLfloat)(rand() % 180)) + 75) / 255);
		printf("%f \n", _viewSize);
		printf("%i %i \n", positionX, positionY);

		printf("%f %f \n", this->positionX, this->positionY);
	}
};

vector<Vertex> pointsToDraw;
int formToDraw = GL_POINTS;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1./60.;

/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error: " << description << std::endl;
}

void onWindowResized(GLFWwindow *window, int width, int height)
{
	aspectRatio = width / (float)height;
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
	// glViewport(0, 0, width/scalingFactor, height/scalingFactor);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(myGame.parameters.aperture, aspectRatio, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

/* User mouse button handling function */
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// bibux
		// glfwGetCursorPos(window, &xpos, &ypos);
		// printf("%f %f \n", xpos, ypos);
		// pointsToDraw.push_back(Vertex(xpos, ypos));
		if (myGame.gameState == 1)
		{
			// Launch the first unlaunched ball in the array
			for (Ball &ball : myGame.balls)
			{
				if (!ball.isLaunched)
				{
					ball.isLaunched = true;
					// If all the balls are launched, enable gameState 2
					if (++myGame.nbOfBallsLaunched == myGame.balls.size())
					{
						myGame.gameState = 2;
					}
				}
			}
		}
	}
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	// if()
	GLfloat convertedX = (_viewSize * aspectRatio) / WINDOW_WIDTH * xpos - (_viewSize * aspectRatio) / 2.0;
	GLfloat convertedY = -_viewSize / WINDOW_HEIGHT * ypos + _viewSize / 2.0;
	// printf("Converted X: %f, ", convertedX);
	// printf("Converted Y: %f\n", convertedY);

	myGame.player.updatePositionMouse(xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, _viewSize, aspectRatio, myGame.parameters.buildingWidth-myGame.player.width, myGame.parameters.buildingHeight-myGame.player.height);
	for (Ball &ball : myGame.balls)
	{
		if (!ball.isLaunched)
			ball.updatePositionMouse(xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, _viewSize, aspectRatio, myGame.parameters.buildingWidth-myGame.player.width, myGame.parameters.buildingHeight-myGame.player.height);
	}
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			// case GLFW_KEY_A:
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;

			// case GLFW_KEY_L:
			// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// 	break;

			// case GLFW_KEY_P:
			// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			// 	break;

			default:
				std::cout << "Touche non gérée (" << key << ")" << std::endl;
		}

		// If game is being played
		if (myGame.gameState == 1 || myGame.gameState == 2)
		{
			switch (key)
			{
				case GLFW_KEY_N:
					myGame.takeDamage();
					break;

				// Restart the game
				case GLFW_KEY_R:
					myGame.loadGame();
					myGame.loadSkin(textures);
					break;

				case GLFW_KEY_C:
					myGame.balls[0].checkWAllCollisions(myGame.corridor, myGame.player, myGame.parameters.gameDepth, myGame.currentBonus, myGame.gameState);
					break;

				case GLFW_KEY_KP_9:
					// if(dist_zoom<100.0f) dist_zoom*=1.1;
					if (dist_zoom < 100.0f)
						dist_zoom += 1;
					std::cout << "Zoom is " << dist_zoom << std::endl;
					break;

				case GLFW_KEY_KP_3:
					// if(dist_zoom>1.0f) dist_zoom*=0.9;
					if (dist_zoom > 1.0f)
						dist_zoom -= 1;
					std::cout << "Zoom is " << dist_zoom << std::endl;
					break;

				case GLFW_KEY_UP:
					if (phy > 2)
						phy -= 2;
					std::cout << "Phy is " << phy << std::endl;
					break;

				case GLFW_KEY_DOWN:
					if (phy <= 88.)
						phy += 2;
					std::cout << "Phy is " << phy << std::endl;
					break;

				case GLFW_KEY_LEFT:
					theta -= 5;
					std::cout << "Theta is " << theta << std::endl;
					break;

				case GLFW_KEY_RIGHT:
					theta += 5;
					std::cout << "Theta is " << theta << std::endl;
					break;

				case GLFW_KEY_SPACE:
					{
						if(myGame.gameState == 2)
						{
							myGame.moveFront(5);
						}
					}
					break;
			}
		}
		else
		{
			printf("%i", myGame.gameState);
			switch (key)
			{
				case GLFW_KEY_J:
					if (myGame.gameState == 10) {
						myGame.loadGame();
						myGame.loadSkin(textures);
					}
					break;
				
				case GLFW_KEY_A:
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
				
				case GLFW_KEY_R:
					if (myGame.gameState == 11 || myGame.gameState == 12) {
						myGame.loadGame();
						myGame.loadSkin(textures);
					}
					break;
			}
		}
	}

	// If holding key
	if (action == GLFW_REPEAT && (myGame.gameState == 1 || myGame.gameState == 2))
	{
		switch (key)
		{
			case GLFW_KEY_SPACE:
				if(myGame.gameState == 2)
				{
					myGame.moveFront(1);
				}
				break;

			case GLFW_KEY_KP_7:
				myGame.balls[0].moveBall(0, 1, 0);
				break;

			case GLFW_KEY_KP_1:
				myGame.balls[0].moveBall(0, -1, 0);
				break;

			case GLFW_KEY_KP_4:
				myGame.balls[0].moveBall(-1, 0, 0);
				break;

			case GLFW_KEY_KP_5:
				myGame.balls[0].moveBall(1, 0, 0);
				break;

			case GLFW_KEY_KP_8:
				myGame.balls[0].moveBall(0, 0, 1);
				break;

			case GLFW_KEY_KP_2:
				myGame.balls[0].moveBall(0, 0, -1);
				break;

			default:
				std::cout << "Touche non gérée (" << key << ")" << std::endl;
		}
	}
}

// Code a texture from the file path
// Return the GLuint representing the texture
GLuint loadImage(const char *filename)
{
	int x, y, n;
	unsigned char *loadedImage = stbi_load(filename, &x, &y, &n, 0);

	if (loadedImage != nullptr)
	{
		std::cout << "Image [" << filename << "] loaded" << std::endl;
	}

	GLuint tempTexture;
	glGenTextures(1, &tempTexture);
	glBindTexture(GL_TEXTURE_2D, tempTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage);
	stbi_image_free(loadedImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tempTexture;
}

// Function that load the textures used in game
void loadTextures()
{
	// 0 to 9
	textures.push_back(loadImage("../res/0.png"));
	textures.push_back(loadImage("../res/1.png"));
	textures.push_back(loadImage("../res/2.png"));
	textures.push_back(loadImage("../res/3.png"));
	textures.push_back(loadImage("../res/4.png"));
	textures.push_back(loadImage("../res/5.png"));
	textures.push_back(loadImage("../res/6.png"));
	textures.push_back(loadImage("../res/7.png"));
	textures.push_back(loadImage("../res/8.png"));
	textures.push_back(loadImage("../res/9.png"));
	
	// 10 to 18
	textures.push_back(loadImage("../res/lifeIcon.png"));
	textures.push_back(loadImage("../res/title.png"));
	textures.push_back(loadImage("../res/names.png"));
	textures.push_back(loadImage("../res/startButton.png"));
	textures.push_back(loadImage("../res/exitLargeButton.png"));
	textures.push_back(loadImage("../res/restartButton.png"));
	textures.push_back(loadImage("../res/exitSmallButton.png"));
	textures.push_back(loadImage("../res/gameOver.png"));
	textures.push_back(loadImage("../res/basic_ball.png"));

	// 19 to 26
	textures.push_back(loadImage("../res/kirby_ball.png"));
	textures.push_back(loadImage("../res/kirby_bonusBox.png"));
	textures.push_back(loadImage("../res/kirby_wallTile.png"));
	textures.push_back(loadImage("../res/kirby_leftWall.png"));
	textures.push_back(loadImage("../res/kirby_rightWall.png"));
	textures.push_back(loadImage("../res/kirby_bottomWall.png"));
	textures.push_back(loadImage("../res/kirby_floorTile.png"));
	textures.push_back(loadImage("../res/kirby_sky.png"));

	// 27 to 34
	textures.push_back(loadImage("../res/mario_ball.png"));
	textures.push_back(loadImage("../res/mario_bonusBox.png"));
	textures.push_back(loadImage("../res/mario_wallTile.png"));
	textures.push_back(loadImage("../res/mario_leftWall.png"));
	textures.push_back(loadImage("../res/mario_rightWall.png"));
	textures.push_back(loadImage("../res/mario_bottomWall.png"));
	textures.push_back(loadImage("../res/mario_floorTile.png"));
	textures.push_back(loadImage("../res/mario_sky.png"));
	
	
	
}

// Function that delete the textures used in game
void deleteTextures()
{
	for (int i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures[i]);
	}
}

GLFWimage loadIcon(const char *filename)
{
	int x, y, n;
	unsigned char *loadedImage = stbi_load(filename, &x, &y, &n, 0);

	if (loadedImage != nullptr)
	{
		std::cout << "Image [" << filename << "] loaded" << std::endl;
	}

	GLFWimage loadedIcon;
    loadedIcon.width = 64;
    loadedIcon.height = 64;
    loadedIcon.pixels = loadedImage;
	
	return loadedIcon;
}

void drawTestTextures()
{
	glColor3f(1., 1., 1.);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glPushMatrix();
	glScalef(5.f, 5.f, 0.f);
	glTranslatef(-1, -1, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0., 0.);
	glVertex2f(-0.5f, 0.5f);
	glTexCoord2f(1., 0.);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(1., 1.);
	glVertex2f(0.5f, -0.5f);
	glTexCoord2f(0., 1.);
	glVertex2f(-0.5f, -0.5f);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	glColor3f(1., 0., 0.);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glPushMatrix();
	glScalef(5.f, 5.f, 0.f);
	glTranslatef(1, 1, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0., 0.);
	glVertex2f(-0.5f, 0.5f);
	glTexCoord2f(1., 0.);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(1., 1.);
	glVertex2f(0.5f, -0.5f);
	glTexCoord2f(0., 1.);
	glVertex2f(-0.5f, -0.5f);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void draw()
{
	// drawFrame();
	switch (myGame.gameState)
	{
		// If the game is being played
		case 1:
		case 2:
			for (int i = 0; i < posBalls.size(); i++)
			{
				posBalls.erase(posBalls.begin());
			}

			for (int i = 0; i < myGame.balls.size(); i++)
			{
				posBalls.push_back(myGame.balls[i].pos);
			}

			glPushMatrix();
			glTranslatef(0, -myGame.parameters.gameDepth, 0);
				drawBalls(myGame.balls, myGame.corridor.ballTexture);
				drawCorridor(myGame.corridor, posBalls, myGame.player.pos);
				// drawFrame();
			glPopMatrix();

			drawPlayer(myGame.player);
			glDisable(GL_DEPTH_TEST);
			drawInterface(myGame, textures, posBalls, myGame.player.pos);
			glEnable(GL_DEPTH_TEST);
			break;
		
		// Starting menu
		case 10:
			drawStartingMenu(myGame, textures);
			break;
		
		// Game over menu
		case 11:
			drawGameoverMenu(myGame, textures);
			break;
		
		// Victory menu
		case 12:
			drawVictoryMenu(myGame, textures);
			break;
	}

	// Test drawing function
	// drawTestTextures();
	// glPointSize(25.0);
	// glBegin(formToDraw);
	// for (unsigned int i = 0; i < pointsToDraw.size(); i++)
	// {
	//     glColor4f(pointsToDraw.at(i).colorR, pointsToDraw.at(i).colorG, pointsToDraw.at(i).colorB, 1.0);
	// 	glVertex3f(pointsToDraw.at(i).positionX,0, pointsToDraw.at(i).positionY);
	// }
	// glEnd();
}

int main()
{
	// Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	// Create a windowed mode window and its OpenGL context
#ifdef __APPLE__
	// We need to explicitly ask for a 3.3 context on Mac
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// myGame.loadGame();

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);

	/* Callback to a function if the mouse is clicked */
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	/* Callback to a function if the mouse is moved */
	glfwSetCursorPosCallback(window, cursor_position_callback);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Enable vsync to cap the FPS
    glfwSwapInterval(1);

	// Activate depth so that elements can render in front of others
	glEnable(GL_DEPTH_TEST);

	// Activate transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	loadTextures();
	
	
	GLFWimage icon = loadIcon("../res/lifeIcon.png");
	glfwSetWindowIcon(window, 1, &icon);

	printf("corridor numberOfSteps: %i", myGame.corridor.numberOfSteps);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// printf("%i\n", myGame.score);
		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.0, 0.0, 0., 0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// glScalef(scalingFactor, scalingFactor, 1.0f);
		setCamera();

		// GAMESTATES
		// 1: game when no ball moving
		// 2: game when all the balls are launched
		// 10: start menu
		// 11: game ove 
		// 12: victory

		// MODEL
		if(myGame.gameState == 1 || myGame.gameState == 2) {
			for (Ball &ball : myGame.balls)
			{
				if (ball.isLaunched) {
					ball.gameMove();
					// myGame.score++;
				}

				ball.checkWAllCollisions(myGame.corridor, myGame.player, myGame.parameters.gameDepth, myGame.currentBonus, myGame.gameState);
				myGame.checkWinDamage();
			}
		}
		
		// VIEW
		draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDisable(GL_BLEND);
	deleteTextures();
	stbi_image_free(&icon);
	glfwTerminate();
	return 0;
}