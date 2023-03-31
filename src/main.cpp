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

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 1000;
static const char WINDOW_TITLE[] = "The IMAC Light Corridor";
static float aspectRatio = 1.0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 90.;

/* IHM flag */
static int flag_animate_rot_scale = 0;
static int flag_animate_rot_arm = 0;

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
				if(dist_zoom<100.0f) dist_zoom*=1.1;
				std::cout << "Zoom is " << dist_zoom << std::endl;
				break;
			case GLFW_KEY_KP_3 :
				if(dist_zoom>1.0f) dist_zoom*=0.9;
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
				break;
			case GLFW_KEY_RIGHT :
				theta += 5;
				break;
			default:
				std::cout << "Touche non gérée (" << key << ")" << std::endl;
		}
	}
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

	// Rotation variables
	float alpha = 0;
	float alpha2 = 0;
	float alpha3 = 3.14/2;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Cleaning buffers and setting Matrix Mode */
		glClearColor(0.2,0.0,0.0,0.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		setCamera();

		// alpha+=0.1;
		alpha += 0.1;
		alpha2 += 1*flag_animate_rot_scale;
		alpha3 += 0.05*flag_animate_rot_arm;

			
		/* Initial scenery setup */
		// glTranslatef(cos(alpha2), sin(alpha2), 0);
		glRotatef(alpha2, 0, 0, 1);
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
		drawFrame();
		drawBase();

		glPushMatrix();
			glPushMatrix();
				glTranslatef(0,0,10);
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

		//DESSIN DE LA SPHERE
		glColor3f(1,0,0);
		//on fait tourner la sphere sur le cercle de rayon 4
		//methode 1 -> 1 seule translation
		glTranslatef(4*cos(alpha), 4*sin(alpha), 5); 
		//methode 2 -> 1 translation, 1 rotation, 1 translation
		drawSphere();

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

    glfwTerminate();
    return 0;
}