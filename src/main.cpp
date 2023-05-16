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

// GLfloat aperture = 60.0f; // Ouverture de la caméra 60.0 de base
// float building_width = 24.0f;
// float building_height = building_width/2;
// float building_depth = 12.0f;
// float game_depth=0;

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
static const double FRAMERATE_IN_SECONDS = 1. / 90.;

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
		glfwGetCursorPos(window, &xpos, &ypos);
		printf("%f %f \n", xpos, ypos);
		pointsToDraw.push_back(Vertex(xpos, ypos));
	}
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	myGame.player.updatePosition(xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, _viewSize, aspectRatio);
	for (Ball &ball : myGame.balls)
	{
		if (!ball.isLaunched)
			ball.updatePosition(xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, _viewSize, aspectRatio);
	}
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_L:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case GLFW_KEY_P:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case GLFW_KEY_N:
			myGame.takeDamage();
			break;
		// Restart the game
		case GLFW_KEY_R:
			myGame.loadGame();
			break;
		case GLFW_KEY_C:
			myGame.balls[0].checkWAllCollisions(myGame.corridor, myGame.player, myGame.parameters.gameDepth);
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
		case GLFW_KEY_W:
			myGame.moveFront(5);
			break;
		case GLFW_KEY_S:
			myGame.moveFront(-5);
			break;
		case GLFW_KEY_SPACE:
		{
			// Launch the first unlaunched ball in the array
			for (Ball &ball : myGame.balls)
			{
				if (!ball.isLaunched)
				{
					printf("ffff %s", "zzz");
					ball.isLaunched = true;
					// If all the balls are launched, enable gameState 2
					if (++myGame.nbOfBallsLaunched == myGame.balls.size())
					{
						myGame.gameState = 2;
					}
					break;
				}
			}
			break;
		}
		// bool launched = false;
		// int nbOfLaunched = 0;
		// for(Ball& ball : myGame.balls) {
		// 	if(!ball.isLaunched && !launched) {
		// 		ball.isLaunched = !ball.isLaunched;
		// 		launched = !launched;
		// 	}
		// 	if(ball.isLaunched) nbOfLaunched++;
		// }
		// if(nbOfLaunched == myGame.balls.size()) {
		// 	if(myGame.gameState == 1) myGame.gameState = 2;
		// }
		// printf("nbOfLaunched: %i", nbOfLaunched);
		break;
			// if (formToDraw == GL_POLYGON)
			// {
			// 	formToDraw = GL_POINTS;
			// }
			// else
			// {
			// 	formToDraw = GL_POLYGON;
			// }
		default:
			std::cout << "Touche non gérée (" << key << ")" << std::endl;
		}
	}
	if (action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			myGame.moveFront(1);
			break;
		case GLFW_KEY_S:
			myGame.moveFront(-1);
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
	textures.push_back(loadImage("../res/kirby.png"));
	textures.push_back(loadImage("../res/lifeIcon.png"));
	textures.push_back(loadImage("../res/stoneWall.png"));
}

// Function that delete the textures used in game
void deleteTextures()
{
	// glDeleteTextures(1, &textures[0]);
	// glDeleteTextures(1, &textures[1]);
	// glDeleteTextures(1, &textures[2]);
	// glDeleteTextures(1, &textures[3]);
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
		drawBalls(myGame.balls, textures);
		drawCorridor(myGame.corridor, posBalls, myGame.player.pos, textures[12]);
	glPopMatrix();

	// drawFrame();
	drawPlayer(myGame.player);
	glDisable(GL_DEPTH_TEST);
	drawInterface(myGame, textures, posBalls, myGame.player.pos);
	glEnable(GL_DEPTH_TEST);

	// Test drawing function
	drawTestTextures();
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

	myGame.loadGame();

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);

	/* Callback to a function if the mouse is clicked */
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	/* Callback to a function if the mouse is moved */
	glfwSetCursorPosCallback(window, cursor_position_callback);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Activate depth so that elements can render in front of others
	glEnable(GL_DEPTH_TEST);

	// Activate transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	loadTextures();
	GLFWimage icon = loadIcon("../res/lifeIcon.png");
	glfwSetWindowIcon(window, 1, &icon);

	printf("corridor numberOfSteps: %i", myGame.corridor.numberOfSteps);

	// myGame.corridor.generateCorridor(building_width, building_height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		// glClearColor(1.0,1.0,1.,0.0);
		glClearColor(0.0, 0.0, 0., 0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		// GAMESTATES
		// 0: menu
		// 1: game when no ball moving
		// 2: game when all the balls are launched

		// MODEL
		for (Ball &ball : myGame.balls)
		{
			if (ball.isLaunched)
				ball.gameMove();
			
			ball.checkWAllCollisions(myGame.corridor, myGame.player, myGame.parameters.gameDepth);
		}

		// VIEW
		draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if (elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
		}
	}

	glDisable(GL_BLEND);
	deleteTextures();
	stbi_image_free(&icon);
	glfwTerminate();
	return 0;
}