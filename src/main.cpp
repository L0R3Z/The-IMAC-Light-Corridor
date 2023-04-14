#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "3D_tools.h"
#include "draw_scene.h"

using namespace std;

/* Textures table */
vector<GLuint> textures;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 640;
static const unsigned int WINDOW_HEIGHT = 400;
static const char WINDOW_TITLE[] = "The IMAC Light Corridor";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 90.;

/* IHM flag */
static int flag_animate_rot_scale = 0;
static int flag_animate_rot_arm = 0;

float game_depth=0;

// Rotation variables
	float alpha = 0;
	float alpha2 = 0;
	float alpha3 = 3.14/2;

/* Error handling function */
void onError(int error, const char* description) {
    std::cout << "GLFW Error: " << description << std::endl;
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,aspectRatio,Z_NEAR,Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A :
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_L :
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				break;
			case GLFW_KEY_P :
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				break;
			case GLFW_KEY_R :
				flag_animate_rot_arm = 1-flag_animate_rot_arm;
				break;
			case GLFW_KEY_T :
				flag_animate_rot_scale = 1-flag_animate_rot_scale;
				break;
			case GLFW_KEY_KP_9 :
				// if(dist_zoom<100.0f) dist_zoom*=1.1;
				if(dist_zoom<100.0f) dist_zoom+=1;
				std::cout << "Zoom is " << dist_zoom << std::endl;
				break;
			case GLFW_KEY_KP_3 :
				// if(dist_zoom>1.0f) dist_zoom*=0.9;
				if(dist_zoom>1.0f) dist_zoom-=1;
				std::cout << "Zoom is " << dist_zoom << std::endl;
				break;
			case GLFW_KEY_UP :
				if (phy>2) phy -= 2;
				std::cout << "Phy is " << phy << std::endl;
				break;
			case GLFW_KEY_DOWN :
				if (phy<=88.) phy += 2;
				std::cout << "Phy is " << phy << std::endl;
				break;
			case GLFW_KEY_LEFT :
				theta -= 5;
				std::cout << "Theta is " << theta << std::endl;
				break;
			case GLFW_KEY_RIGHT :
				theta += 5;
				std::cout << "Theta is " << theta << std::endl;
				break;
			case GLFW_KEY_W :
				game_depth+=10;
				break;
			case GLFW_KEY_S :
				game_depth-=10;
				break;
			default:
				std::cout << "Touche non gérée (" << key << ")" << std::endl;
		}
	}
}

// Code a texture from the file path
// Return the GLuint representing the texture
GLuint loadImage(const char* filename) {
	int x, y, n;
	unsigned char* loadedImage = stbi_load(filename, &x, &y, &n, 0);

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
void loadTextures() {
	textures.push_back(loadImage("../res/0.png"));
	textures.push_back(loadImage("../res/1.png"));
}

// Function that delete the textures used in game
void deleteTextures() {
	glDeleteTextures(1, &textures[0]);
	glDeleteTextures(1, &textures[1]);
}

void drawTestTextures() {
	// glColor4f(1., 1., 1.,1);
	glColor3f(1., 1., 1.);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, textures[0]);
		glPushMatrix();
			glScalef(5.f, 5.f, 0.f);
			glTranslatef(-1,-1,0);
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

	glBindTexture(GL_TEXTURE_2D, textures[1]);
		glPushMatrix();
			glScalef(5.f, 5.f, 0.f);
			glTranslatef(1,1,0);
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

void drawTunnelPart(int tunnel_depth) {
	// Top wall
	glColor4f(0./255,0./255,69./255,0.8);
	glPushMatrix();
		glTranslatef(0,building_width*tunnel_depth,building_width/2);
		glScalef(building_width, building_width, building_width);
		drawSquare();
	glPopMatrix();

	// Right wall
	glColor4f(69./255,69./255,142./255,0.8);
	glPushMatrix();
		glTranslatef(building_width/2,building_width*tunnel_depth,0);
		glRotatef(90, 0, 1, 0);
		glScalef(building_width, building_width, building_width);
		drawSquare();
	glPopMatrix();

	// Bottom wall
	glColor4f(69./255,69./255,105./255,0.8);
	glPushMatrix();
		glTranslatef(0,building_width*tunnel_depth,-building_width/2);
		glScalef(building_width, building_width, building_width);
		drawSquare();
	glPopMatrix();

	// Left wall
	glColor4f(69./255,105./255,178./255,0.8);
	glPushMatrix();
		glTranslatef(-building_width/2,building_width*tunnel_depth,0);
		glRotatef(90, 0, 1, 0);
		glScalef(building_width, building_width, building_width);
		drawSquare();
	glPopMatrix();

	// Bottom
	glColor4f(0.,0.,0.,0.8);
	glPushMatrix();
		glTranslatef(0,building_width*tunnel_depth+building_width/2,0);
		glRotatef(90, 1, 0, 0);
		glScalef(building_width, building_width, building_width);
		if (tunnel_depth==9)
		{
			drawSquare();
		} else {
			glLineWidth(6.0);
			glBegin(GL_LINE_LOOP);
					glVertex2f(-0.5f, 0.5f);
					glVertex2f(0.5f, 0.5f);
					glVertex2f(0.5f, -0.5f);
					glVertex2f(-0.5f, -0.5f);
			glEnd();
			glLineWidth(1.0);
		}
	glPopMatrix();
}

void drawBalance() {
	// alpha+=0.1;
		alpha += 0.1;
		alpha2 += 1*flag_animate_rot_scale;
		alpha3 += 0.05*flag_animate_rot_arm;

			
	/* Initial scenery setup */
	// glTranslatef(cos(alpha2), sin(alpha2), 0);
	glPushMatrix();
		glTranslatef(0.0,0.0,-0.01);
		glScalef(10.0,10.0,1.0);
		glColor3f(0.0,0.0,0.1);
		drawSquare();
		glBegin(GL_POINTS);
			glColor3f(1.0,1.0,0.0);
			glVertex3f(0.0,0.0,0.0);
		glEnd();
	glPopMatrix();
	
	/* Scene rendering */
	drawBase();

	glPushMatrix();
		glPushMatrix();
			glTranslatef(0,0,10);
			glRotatef(alpha2, 0, 0, 1);
			glRotatef(cos(alpha3)*20, 1, 1, 0);
			drawArm();

			glPushMatrix();
				glTranslatef(-7,7,0);
				glRotatef(-cos(alpha3)*20, 1, 1, 0);
				drawPan();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(7,-7,0);
				glRotatef(-cos(alpha3)*20, 1, 1, 0);
				drawPan();
			glPopMatrix();
		glPopMatrix();		
	glPopMatrix();
}

void draw() {
	// drawBalance();
	drawFrame();
	
	glPushMatrix();
		glTranslatef(0,-game_depth,0);
		for (int i = 0; i < 10; i++)
		{
			drawTunnelPart(i);
		}
	glPopMatrix();
	drawTestTextures();

	//DESSIN DE LA SPHERE
		glColor4f(1,0,0,0.5);
		//on fait tourner la sphere sur le cercle de rayon 4
		//methode 1 -> 1 seule translation
		glTranslatef(4*cos(alpha), 4*sin(alpha), 5); 
		//methode 2 -> 1 translation, 1 rotation, 1 translation
		drawSphere();
}


int main() {
    // Initialize the library
    if (!glfwInit()) {
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
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);

    onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	// Activate depth so that elements can render in front of others
	glEnable(GL_DEPTH_TEST);

	// Activate transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	loadTextures();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(1.0,1.0,1.,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS) 
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}
	}

	glDisable(GL_BLEND);
	deleteTextures();
    glfwTerminate();
    return 0;
}