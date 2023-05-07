#include "draw_scene.h"
#include "3D_tools.h"
#include "game_objects.h"
#include <vector>

using namespace std;

Colors tempColor = Colors(); // Temporary color used for illumination function
Position tempPos = Position(0,0,0); // Temporary position used for illumination function

Position posBottom = Position(0,0,0);

// Function that draws a grid and an origin
void drawFrame() {
    // WIDTH GRID
    for (int i = - myGame.parameters.buildingWidth/2; i < myGame.parameters.buildingWidth/2+1; i++)
    {          
        // Y AXIS
        glColor4f(0,1,0,0.2);
        glBegin(GL_LINES);
                glVertex3f((GLfloat) i, 0, myGame.parameters.buildingHeight/2);
                glVertex3f((GLfloat) i, 0, -myGame.parameters.buildingHeight/2);
        glEnd();

        // Z AXIS
        glColor4f(0,0,1,0.2);
        glBegin(GL_LINES);
            glVertex2f((GLfloat) i, myGame.parameters.buildingDepth/2);
            glVertex2f((GLfloat) i, -myGame.parameters.buildingDepth/2);
        glEnd();
        
    }
    // HEIGHT GRID
    for (int i = - myGame.parameters.buildingHeight/2; i < myGame.parameters.buildingHeight/2+1; i++)
    {
        // Y AXIS
        glColor4f(0,1,0,0.2);
        glBegin(GL_LINES);
                glVertex3f(myGame.parameters.buildingWidth/2, 0, (GLfloat) i);
                glVertex3f(-myGame.parameters.buildingWidth/2, 0, (GLfloat) i);
        glEnd();

        // X AXIS
        glColor4f(1,0,0,0.2);
        glBegin(GL_LINES);
                glVertex3f(0.0, myGame.parameters.buildingDepth/2, (GLfloat) i);
                glVertex3f(0.0, -myGame.parameters.buildingDepth/2, (GLfloat) i);
        glEnd();
        
    }

    // DEPTH GRID
    for (int i = - myGame.parameters.buildingDepth/2; i < myGame.parameters.buildingDepth/2+1; i++)
    {
        // X AXIS
        glColor4f(1,0,0,0.2);
        glBegin(GL_LINES);
                glVertex3f(0.0, (GLfloat) i, myGame.parameters.buildingHeight/2);
                glVertex3f(0.0, (GLfloat) i, -myGame.parameters.buildingHeight/2);
        glEnd();

        // Z AXIS
        glColor4f(0,0,1,0.2);
        glBegin(GL_LINES);
            glVertex2f(-myGame.parameters.buildingWidth/2, (GLfloat) i);
            glVertex2f(myGame.parameters.buildingWidth/2, (GLfloat) i);
        glEnd();
    }

    
    // ORIGIN
    glLineWidth(6.0);
    glBegin(GL_LINES);
        // X
        glColor3f(1,0,0);
        glVertex3f(6.0,0.0,0.0);
        glVertex3f(-6.0,0.0,0.0);
        // Y
        glColor3f(0,1,0);
        glVertex3f(0.0,6.0,0.0);
        glVertex3f(0.0,-6.0,0.0);
        // Z
        glColor3f(0,0,1);
        glVertex3f(0.0,0.0,6.0);
        glVertex3f(0.0,0.0,-6.0);
    glEnd();
    glLineWidth(1.0);
}

void drawCorridor(Corridor myCorridor, Position posBall, Position posPlayer) {
	// Drawing of the bottom of the corridor
	// Definition of the y-position of the bottom
	posBottom.y=myGame.parameters.buildingDepth+myGame.parameters.buildingDepth*(myCorridor.numberOfSteps-1);
	glPushMatrix();
		glTranslatef(0,myGame.parameters.buildingDepth+myGame.parameters.buildingDepth*(myCorridor.numberOfSteps-1),0);
		glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
		glRotatef(90, 1, 0, 0);
		glBegin(GL_TRIANGLE_FAN);
			// Definition of the point to be illuminated
			tempPos = Position(-myGame.parameters.buildingWidth/2,posBottom.y,-myGame.parameters.buildingHeight/2);
			// Retrieval of the color of the point in relation to the position of the elements concerned and the base color of the object
			tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex3f(-0.5,-0.5,0.0);

			tempPos = Position(myGame.parameters.buildingWidth/2,posBottom.y,-myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex3f(0.5,-0.5,0.0);

			tempPos = Position(myGame.parameters.buildingWidth/2,posBottom.y,myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex3f(0.5,0.5,0.0);

			tempPos = Position(-myGame.parameters.buildingWidth/2,posBottom.y,myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex3f(-0.5,0.5,0.0);
		glEnd();
	glPopMatrix();

	// Drawing of the ring at the bottom of the corridor
	glPushMatrix();
		// The y-position is slightly shifted so that the ring is displayed despite the bottom of the corridor
		glTranslatef(0,myGame.parameters.buildingDepth+myGame.parameters.buildingDepth*(myCorridor.numberOfSteps-1)-(float) 1/100,0);
		glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
		glRotatef(90, 1, 0, 0);
		// Definition of ring thickness
		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP);
			tempPos = Position(-myGame.parameters.buildingWidth/2,posBottom.y,myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex2f(-0.5f, 0.5f);

			tempPos = Position(myGame.parameters.buildingWidth/2,posBottom.y,myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex2f(0.5f, 0.5f);

			tempPos = Position(myGame.parameters.buildingWidth/2,posBottom.y,-myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex2f(0.5f, -0.5f);

			tempPos = Position(-myGame.parameters.buildingWidth/2,posBottom.y,-myGame.parameters.buildingHeight/2);
			tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
			glVertex2f(-0.5f, -0.5f);
		glEnd();
		glLineWidth(1.0);
	glPopMatrix();
	
	// Call of the function to draw the wallSteps of the corridor
    drawWallStep(myCorridor.wallSteps, myCorridor, posBall, posPlayer);
}

void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, Position posBall, Position posPlayer) {
    for (int i = myWallSteps.size()-1; i > -1; i--)
    {		
		// Top wall
        glPushMatrix();
            glTranslatef(0,myWallSteps[i].pos.y-myGame.parameters.buildingDepth/2,myGame.parameters.buildingHeight/2);
            glScalef(myGame.parameters.buildingWidth, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			glBegin(GL_TRIANGLE_FAN);
				// Left outside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,-0.5,0.0);

				// Right outside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,-0.5,0.0);

				// Right inside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,0.5,0.0);

				// Left inside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,0.5,0.0);
			glEnd();
        glPopMatrix();

        // Bottom wall
        glPushMatrix();
            glTranslatef(0,myWallSteps[i].pos.y-myGame.parameters.buildingDepth/2,-myGame.parameters.buildingHeight/2);
            glScalef(myGame.parameters.buildingWidth, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			glBegin(GL_TRIANGLE_FAN);
				// Left outside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,-0.5,0.0);

				// Right outside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,-0.5,0.0);

				// Right inside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,0.5,0.0);

				// Left inside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,0.5,0.0);
			glEnd();
        glPopMatrix();

        // Left wall
        glPushMatrix();
            glTranslatef(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth/2,0);
            glRotatef(90, 0, 1, 0);
            glScalef(myGame.parameters.buildingHeight, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			glBegin(GL_TRIANGLE_FAN);
				// Top outside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,-0.5,0.0);

				// Bottom outside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,-0.5,0.0);
				
				// Bottom inside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,0.5,0.0);

				// Top inside corner
				tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,0.5,0.0);
			glEnd();
        glPopMatrix();

		// Right wall
        glPushMatrix();
            glTranslatef(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth/2,0);
            glRotatef(90, 0, 1, 0);
            glScalef(myGame.parameters.buildingHeight, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			glBegin(GL_TRIANGLE_FAN);
				// Top outside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,-0.5,0.0);

				// Bottom outside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y-myGame.parameters.buildingDepth,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,-0.5,0.0);
				
				// Bottom inside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,-myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(0.5,0.5,0.0);

				// Top inside corner
				tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,myGame.parameters.buildingHeight/2);
				tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
				glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
				glVertex3f(-0.5,0.5,0.0);
			glEnd();
        glPopMatrix();

		// The last ring of the corridor is covered above
		if (i != myWallSteps.size()-1)
		{
			// Ring
			glPushMatrix();
				glTranslatef(0,myWallSteps[i].pos.y,0);
				glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
				glRotatef(90, 1, 0, 0);
				glLineWidth(3.0);
				glBegin(GL_LINE_LOOP);
					// Top left corner
					tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,myGame.parameters.buildingHeight/2);
					tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex2f(-0.5f, 0.5f);

					// Top right corner
					tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,myGame.parameters.buildingHeight/2);
					tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex2f(0.5f, 0.5f);

					// Bottom right corner
					tempPos = Position(myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,-myGame.parameters.buildingHeight/2);
					tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex2f(0.5f, -0.5f);

					// Bottom left corner
					tempPos = Position(-myGame.parameters.buildingWidth/2,myWallSteps[i].pos.y,-myGame.parameters.buildingHeight/2);
					tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex2f(-0.5f, -0.5f);
				glEnd();
				glLineWidth(1.0);
			glPopMatrix();
		}

		// Call of the function to draw the Walls of the wallStep
		drawWall(myWallSteps[i].walls, posBall, posPlayer, myWallSteps[i].color);
    }
}

void drawWall(std::vector<Wall> myWalls, Position posBall, Position posPlayer, Colors wallColor) {
    for (int i = 0; i < (int) myWalls.size(); i++)
    {
        if (myWalls[i].pos.y-myGame.parameters.gameDepth >= 0)
		{
			// Wall
			glPushMatrix();
				// Placing the wall at the bottom of the wallStep
				glTranslatef(0,myWalls[i].pos.y,0);
				// Placement of the wall on the x and z axis
				glTranslatef(myWalls[i].pos.x,0,myWalls[i].pos.z);
				// Offset to place the origin of the wall at the top left
				glTranslatef((myWalls[i].width)/2,0,-(myWalls[i].height)/2);
				// Size of the wall
				glScalef(myWalls[i].width, 1, myWalls[i].height);
				glRotatef(90, 1, 0, 0);
				glBegin(GL_TRIANGLE_FAN);
					// Bottom left corner
					tempPos = Position(myWalls[i].pos.x,myWalls[i].pos.y,myWalls[i].pos.z-myWalls[i].height);
					tempColor = wallColor.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex3f(-0.5,-0.5,0.0);

					// Bottom right corner
					tempPos = Position(myWalls[i].pos.x+myWalls[i].width,myWalls[i].pos.y,myWalls[i].pos.z-myWalls[i].height);
					tempColor = wallColor.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex3f(0.5,-0.5,0.0);
					
					// Top right corner
					tempPos = Position(myWalls[i].pos.x+myWalls[i].width,myWalls[i].pos.y,myWalls[i].pos.z);
					tempColor = wallColor.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex3f(0.5,0.5,0.0);

					// Top left corner
					tempPos = Position(myWalls[i].pos.x,myWalls[i].pos.y,myWalls[i].pos.z);
					tempColor = wallColor.displayColor(posBall, posPlayer, tempPos, myGame.parameters.gameDepth);
					glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
					glVertex3f(-0.5,0.5,0.0);
				glEnd();
			glPopMatrix();
		}
    }
}

void drawBalls(std::vector<Ball> balls) {
	for(Ball ball : balls) {
		// printf("inside drawBalls, pos y of ball: %f\n", ball.pos.y);
		glPushMatrix();
			glTranslatef(ball.pos.x, ball.pos.y+myGame.parameters.gameDepth, ball.pos.z);
			glScalef(ball.radius, ball.radius, ball.radius);
			glColor4f((float) 69/255,(float) 69/255,(float) 142/255,1.);
			drawSphere();
		glPopMatrix();
	}
}

void drawPlayer(Player myPlayer) {
	glPushMatrix();
		glTranslatef(0,myPlayer.pos.y,0);
		glTranslatef(myPlayer.pos.x,0,myPlayer.pos.z);
		glScalef(myPlayer.width, 1, myPlayer.height);
		glRotatef(90, 1, 0, 0);
		glColor4f((float) 178/255,(float) 178/255,(float) 178/255,1.);
		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP);
				glVertex2f(-0.5f, 0.5f);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(0.5f, -0.5f);
				glVertex2f(-0.5f, -0.5f);
		glEnd();

		glLineWidth(1.0);
		glColor4f((float) 178/255,(float) 178/255,(float) 178/255,.25);
		drawSquare();
	glPopMatrix();
}