#include "draw_scene.h"
#include "3D_tools.h"
#include "game_objects.h"
#include <vector>

using namespace std;

Colors tempColor = Colors();		  // Temporary color used for illumination function
Position tempPos = Position(0, 0, 0); // Temporary position used for illumination function
Position tempPos1 = Position(0, 0, 0);
Position tempPos2 = Position(0, 0, 0);
Position tempPos3 = Position(0, 0, 0);
Position tempPos4 = Position(0, 0, 0);

Position posBottom = Position(0, 0, 0);

void drawSquareLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, Colors baseColor, Position point1, Position point2, Position point3, Position point4) {
	glBegin(GL_TRIANGLE_FAN);
		// Bottom left corner
		tempColor = baseColor.displayColor(posBalls, posPlayer, point1, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glVertex3f(-0.5, -0.5, 0.0);

		// Bottom right corner
		tempColor = baseColor.displayColor(posBalls, posPlayer, point2, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glVertex3f(0.5, -0.5, 0.0);

		// Top right corner
		tempColor = baseColor.displayColor(posBalls, posPlayer, point3, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glVertex3f(0.5, 0.5, 0.0);

		// Top left corner
		tempColor = baseColor.displayColor(posBalls, posPlayer, point4, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
}

void drawTextureLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, GLuint texture, Position point1, Position point2, Position point3, Position point4) {
	Colors white = Colors(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_FAN);
		// Bottom left corner
		tempColor = white.displayColor(posBalls, posPlayer, point1, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(0., 1.);
		glVertex3f(-0.5, -0.5, 0.0);

		// Bottom right corner
		tempColor = white.displayColor(posBalls, posPlayer, point2, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(1., 1.);
		glVertex3f(0.5, -0.5, 0.0);

		// Top right corner
		tempColor = white.displayColor(posBalls, posPlayer, point3, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(1., 0.);
		glVertex3f(0.5, 0.5, 0.0);

		// Top left corner
		tempColor = white.displayColor(posBalls, posPlayer, point4, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		
		glTexCoord2f(0., 0.);
		glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawSimpleTexturedSquare(float x, float z, float width, float height, GLuint texture, Colors color) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);	
	glColor4f(color.r,color.g,color.b,1.);
	glPushMatrix();
		// Placement of the square on the x and z axis
		glTranslatef(x, 0, z);
		// Offset to place the origin of the square at the top left
		glTranslatef((width) / 2, 0, -(height) / 2);
		// Size of the square
		glScalef(width, 1, height);
		glRotatef(90, 1, 0, 0);
		glBegin(GL_POLYGON);
			glTexCoord2f(0., 0.); glVertex2f(-0.5f, 0.5f);
			glTexCoord2f(1., 0.); glVertex2f(0.5f, 0.5f);
			glTexCoord2f(1., 1.); glVertex2f(0.5f, -0.5f);
			glTexCoord2f(0., 1.); glVertex2f(-0.5f, -0.5f);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

// Function that draws a grid and an origin
void drawFrame()
{
	// WIDTH GRID
	for (int i = -myGame.parameters.buildingWidth / 2; i < myGame.parameters.buildingWidth / 2 + 1; i++)
	{
		// Y AXIS
		glColor4f(0, 1, 0, 0.2);
		glBegin(GL_LINES);
		glVertex3f((GLfloat)i, 0, myGame.parameters.buildingHeight / 2);
		glVertex3f((GLfloat)i, 0, -myGame.parameters.buildingHeight / 2);
		glEnd();

		// Z AXIS
		glColor4f(0, 0, 1, 0.2);
		glBegin(GL_LINES);
		glVertex2f((GLfloat)i, myGame.parameters.buildingDepth / 2);
		glVertex2f((GLfloat)i, -myGame.parameters.buildingDepth / 2);
		glEnd();
	}
	// HEIGHT GRID
	for (int i = -myGame.parameters.buildingHeight / 2; i < myGame.parameters.buildingHeight / 2 + 1; i++)
	{
		// Y AXIS
		glColor4f(0, 1, 0, 0.2);
		glBegin(GL_LINES);
		glVertex3f(myGame.parameters.buildingWidth / 2, 0, (GLfloat)i);
		glVertex3f(-myGame.parameters.buildingWidth / 2, 0, (GLfloat)i);
		glEnd();

		// X AXIS
		glColor4f(1, 0, 0, 0.2);
		glBegin(GL_LINES);
		glVertex3f(0.0, myGame.parameters.buildingDepth / 2, (GLfloat)i);
		glVertex3f(0.0, -myGame.parameters.buildingDepth / 2, (GLfloat)i);
		glEnd();
	}

	// DEPTH GRID
	for (int i = -myGame.parameters.buildingDepth / 2; i < myGame.parameters.buildingDepth / 2 + 1; i++)
	{
		// X AXIS
		glColor4f(1, 0, 0, 0.2);
		glBegin(GL_LINES);
		glVertex3f(0.0, (GLfloat)i, myGame.parameters.buildingHeight / 2);
		glVertex3f(0.0, (GLfloat)i, -myGame.parameters.buildingHeight / 2);
		glEnd();

		// Z AXIS
		glColor4f(0, 0, 1, 0.2);
		glBegin(GL_LINES);
		glVertex2f(-myGame.parameters.buildingWidth / 2, (GLfloat)i);
		glVertex2f(myGame.parameters.buildingWidth / 2, (GLfloat)i);
		glEnd();
	}

	// ORIGIN
	glLineWidth(6.0);
	glBegin(GL_LINES);
	// X
	glColor3f(1, 0, 0);
	glVertex3f(6.0, 0.0, 0.0);
	glVertex3f(-6.0, 0.0, 0.0);
	// Y
	glColor3f(0, 1, 0);
	glVertex3f(0.0, 6.0, 0.0);
	glVertex3f(0.0, -6.0, 0.0);
	// Z
	glColor3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 6.0);
	glVertex3f(0.0, 0.0, -6.0);
	glEnd();
	glLineWidth(1.0);
}

void drawCorridor(Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer, GLuint texture)
{
	// Drawing of the bottom of the corridor
	// Definition of the y-position of the bottom
	posBottom.y = myGame.parameters.buildingDepth + myGame.parameters.buildingDepth * (myCorridor.numberOfSteps - 1);
	glPushMatrix();
		glTranslatef(0, myGame.parameters.buildingDepth + myGame.parameters.buildingDepth * (myCorridor.numberOfSteps - 1), 0);
		glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
		glRotatef(90, 1, 0, 0);
		// Definition of the point to be illuminated
		tempPos1 = Position(-myGame.parameters.buildingWidth / 2, posBottom.y, -myGame.parameters.buildingHeight / 2);
		tempPos2 = Position(myGame.parameters.buildingWidth / 2, posBottom.y, -myGame.parameters.buildingHeight / 2);
		tempPos3 = Position(myGame.parameters.buildingWidth / 2, posBottom.y, myGame.parameters.buildingHeight / 2);
		tempPos4 = Position(-myGame.parameters.buildingWidth / 2, posBottom.y, myGame.parameters.buildingHeight / 2);
		// drawSquareLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.colorSideWalls, tempPos1, tempPos2, tempPos3, tempPos4);
		drawTextureLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, texture, tempPos1, tempPos2, tempPos3, tempPos4);
	glPopMatrix();

	// Drawing of the ring at the bottom of the corridor
	glPushMatrix();
	// The y-position is slightly shifted so that the ring is displayed despite the bottom of the corridor
	glTranslatef(0, myGame.parameters.buildingDepth + myGame.parameters.buildingDepth * (myCorridor.numberOfSteps - 1) - (float)1 / 100, 0);
	glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
	glRotatef(90, 1, 0, 0);
	// Definition of ring thickness
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	tempPos = Position(-myGame.parameters.buildingWidth / 2, posBottom.y, myGame.parameters.buildingHeight / 2);
	tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
	glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
	glVertex2f(-0.5f, 0.5f);

	tempPos = Position(myGame.parameters.buildingWidth / 2, posBottom.y, myGame.parameters.buildingHeight / 2);
	tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
	glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
	glVertex2f(0.5f, 0.5f);

	tempPos = Position(myGame.parameters.buildingWidth / 2, posBottom.y, -myGame.parameters.buildingHeight / 2);
	tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
	glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
	glVertex2f(0.5f, -0.5f);

	tempPos = Position(-myGame.parameters.buildingWidth / 2, posBottom.y, -myGame.parameters.buildingHeight / 2);
	tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
	glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
	glVertex2f(-0.5f, -0.5f);
	glEnd();
	glLineWidth(1.0);
	glPopMatrix();

	// Call of the function to draw the wallSteps of the corridor
	drawWallStep(myCorridor.wallSteps, myCorridor, posBalls, posPlayer);
}

void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer)
{
	for (int i = myWallSteps.size() - 1; i > -1; i--)
	{
		// Top wall
		glPushMatrix();
			glTranslatef(0, myWallSteps[i].pos.y - myGame.parameters.buildingDepth / 2, myGame.parameters.buildingHeight / 2);
			glScalef(myGame.parameters.buildingWidth, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			tempPos1 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, myGame.parameters.buildingHeight / 2); // Left outside corner
			tempPos2 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, myGame.parameters.buildingHeight / 2); // Right outside corner
			tempPos3 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, myGame.parameters.buildingHeight / 2); // Right inside corner
			tempPos4 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, myGame.parameters.buildingHeight / 2); // Left inside corner
			drawSquareLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.colorCeillingWalls, tempPos1, tempPos2, tempPos3, tempPos4);
		glPopMatrix();

		// Bottom wall
		glPushMatrix();
			glTranslatef(0, myWallSteps[i].pos.y - myGame.parameters.buildingDepth / 2, -myGame.parameters.buildingHeight / 2);
			glScalef(myGame.parameters.buildingWidth, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			tempPos1 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, -myGame.parameters.buildingHeight / 2); // Left outside corner
			tempPos2 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, -myGame.parameters.buildingHeight / 2); // Right outside corner
			tempPos3 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, -myGame.parameters.buildingHeight / 2); // Right inside corner
			tempPos4 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, -myGame.parameters.buildingHeight / 2); // Left inside corner
			drawSquareLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.colorCeillingWalls, tempPos1, tempPos2, tempPos3, tempPos4);
		glPopMatrix();

		// Left wall
		glPushMatrix();
			glTranslatef(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth / 2, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(myGame.parameters.buildingHeight, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			tempPos1 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, myGame.parameters.buildingHeight / 2); // Top outside corner
			tempPos2 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, -myGame.parameters.buildingHeight / 2); // Bottom outside corner
			tempPos3 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, -myGame.parameters.buildingHeight / 2); // Bottom inside corner
			tempPos4 = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, myGame.parameters.buildingHeight / 2); // Top inside corner
			drawSquareLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.colorSideWalls, tempPos1, tempPos2, tempPos3, tempPos4);
		glPopMatrix();

		// Right wall
		glPushMatrix();
			glTranslatef(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth / 2, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(myGame.parameters.buildingHeight, myGame.parameters.buildingDepth, myGame.parameters.buildingHeight);
			tempPos1 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, myGame.parameters.buildingHeight / 2); // Top outside corner
			tempPos2 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y - myGame.parameters.buildingDepth, -myGame.parameters.buildingHeight / 2); // Bottom outside corner
			tempPos3 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, -myGame.parameters.buildingHeight / 2); // Bottom inside corner
			tempPos4 = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, myGame.parameters.buildingHeight / 2); // Top inside corner
			drawSquareLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.colorSideWalls, tempPos1, tempPos2, tempPos3, tempPos4);
		glPopMatrix();

		// The last ring of the corridor is covered above
		if (i != myWallSteps.size() - 1)
		{
			// Ring
			glPushMatrix();
			glTranslatef(0, myWallSteps[i].pos.y, 0);
			glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
			glRotatef(90, 1, 0, 0);
			glLineWidth(3.0);
			glBegin(GL_LINE_LOOP);
			// Top left corner
			tempPos = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
			glVertex2f(-0.5f, 0.5f);

			// Top right corner
			tempPos = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
			glVertex2f(0.5f, 0.5f);

			// Bottom right corner
			tempPos = Position(myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, -myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
			glVertex2f(0.5f, -0.5f);

			// Bottom left corner
			tempPos = Position(-myGame.parameters.buildingWidth / 2, myWallSteps[i].pos.y, -myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 0.8);
			glVertex2f(-0.5f, -0.5f);
			glEnd();
			glLineWidth(1.0);
			glPopMatrix();
		}

		// Call of the function to draw the Walls of the wallStep
		drawWall(myWallSteps[i].walls, posBalls, posPlayer, myWallSteps[i].color);
	}
}

void drawWall(std::vector<Wall> myWalls, std::vector<Position> posBalls, Position posPlayer, Colors wallColor)
{
	for (int i = 0; i < (int)myWalls.size(); i++)
	{
		if (myWalls[i].pos.y - myGame.parameters.gameDepth >= 0)
		{
			// Wall
			glPushMatrix();
				// Placing the wall at the bottom of the wallStep
				glTranslatef(0, myWalls[i].pos.y, 0);
				// Placement of the wall on the x and z axis
				glTranslatef(myWalls[i].pos.x, 0, myWalls[i].pos.z);
				// Offset to place the origin of the wall at the top left
				glTranslatef((myWalls[i].width) / 2, 0, -(myWalls[i].height) / 2);
				// Size of the wall
				glScalef(myWalls[i].width, 1, myWalls[i].height);
				glRotatef(90, 1, 0, 0);
				// Bottom left corner
				tempPos1 = Position(myWalls[i].pos.x, myWalls[i].pos.y, myWalls[i].pos.z - myWalls[i].height);
				// Bottom right corner
				tempPos2 = Position(myWalls[i].pos.x + myWalls[i].width, myWalls[i].pos.y, myWalls[i].pos.z - myWalls[i].height);
				// Top right corner
				tempPos3 = Position(myWalls[i].pos.x + myWalls[i].width, myWalls[i].pos.y, myWalls[i].pos.z);
				// Top left corner
				tempPos4 = Position(myWalls[i].pos.x, myWalls[i].pos.y, myWalls[i].pos.z);
				drawSquareLight(myGame.parameters.gameDepth, 0.8, posBalls, posPlayer, wallColor, tempPos1, tempPos2, tempPos3, tempPos4);	
			glPopMatrix();
		}
	}
}

void drawBalls(std::vector<Ball> balls, std::vector<GLuint> myTextures)
{
	for (Ball ball : balls)
	{
		// printf("inside drawBalls, pos y of ball: %f\n", ball.pos.y);
		glPushMatrix();
			if(myGame.balls[0].isLaunched) glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);
			else glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);
			glScalef(ball.radius, ball.radius, ball.radius);
			// glColor4f((float)69 / 255, (float)69 / 255, (float)142 / 255, 1.);
			glColor3f(1., 1., 1.);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, myTextures[10]);
			GLUquadric* myQuadric = gluNewQuadric();
			gluQuadricTexture(myQuadric, GL_TRUE);
			gluSphere(myQuadric, 1.0, NB_SEG_CIRCLE, NB_SEG_CIRCLE);
			gluDeleteQuadric(myQuadric);
			delete myQuadric;
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

void drawPlayer(Player myPlayer)
{
	glPushMatrix();
		glTranslatef(0, myPlayer.pos.y, 0);
		glTranslatef(myPlayer.pos.x, 0, myPlayer.pos.z);
		glScalef(myPlayer.width, 1, myPlayer.height);
		glRotatef(90, 1, 0, 0);
		glColor4f((float)178 / 255, (float)178 / 255, (float)178 / 255, 1.);
		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(-0.5f, -0.5f);
		glEnd();
		glLineWidth(1.0);
		glColor4f((float)178 / 255, (float)178 / 255, (float)178 / 255, .25);
		drawSquare();
	glPopMatrix();
}

void drawInterface(Game myGame, std::vector<GLuint> myTextures, std::vector<Position> posBalls, Position posPlayer) {
	// Wall
	glPushMatrix();
		// Placing the wall at the bottom of the wallStep
		glTranslatef(0, 0, 0);
		// Placement of the wall on the x and z axis
		glTranslatef(-myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/2);
		// Offset to place the origin of the wall at the top left
		glTranslatef((myGame.parameters.buildingWidth / 2), 0, -((myGame.parameters.buildingHeight/6)/2));
		// Size of the wall
		glScalef(myGame.parameters.buildingWidth, 1, (myGame.parameters.buildingHeight/6));
		glRotatef(90, 1, 0, 0);
		tempColor = Colors(0.2,0.2,0.2);
		// Bottom left corner
		tempPos1 = Position(-myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/3);
		// Bottom right corner
		tempPos2 = Position(myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/3);
		// Top right corner
		tempPos3 = Position(myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/2);
		// Top left corner
		tempPos4 = Position(-myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/2);
		drawSquareLight(0, 0.5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);		
	glPopMatrix();

	// Lifes
	tempColor = myGame.corridor.colorSideWalls.generateComplementaryColor();
	glColor3f(tempColor.r,tempColor.g,tempColor.b);
	for (int i = 0; i < myGame.lives; i++)
	{
		drawSimpleTexturedSquare(-5*myGame.parameters.buildingWidth/12+(i*myGame.parameters.buildingWidth/48),11*myGame.parameters.buildingHeight/24,myGame.parameters.buildingHeight/12,myGame.parameters.buildingHeight/12,myTextures[11], tempColor);
	}
	glDisable(GL_TEXTURE_2D);

	// Score
	for (int i = 0; i < 6; i++)
	{
		drawSimpleTexturedSquare(5*myGame.parameters.buildingWidth/12-(i*myGame.parameters.buildingWidth/20),11*myGame.parameters.buildingHeight/24,myGame.parameters.buildingHeight/12,myGame.parameters.buildingHeight/12,myTextures[0], tempColor);
	}
	glDisable(GL_TEXTURE_2D);
}

void drawStartingMenu(Game myGame, std::vector<GLuint> myTextures) {
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tempColor = Colors(1,1,1);

	// Title
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, 5*myGame.parameters.buildingHeight/12, 2*myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/4, myTextures[13], tempColor);
	// Name
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/2, -myGame.parameters.buildingHeight/3, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[14], tempColor);
	
	// Start Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/6, myGame.parameters.buildingHeight/12, myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/6, myTextures[15], tempColor);
	// Exit Large Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/6, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/6, myTextures[16], tempColor);
}

void drawGameoverMenu(Game myGame, std::vector<GLuint> myTextures) {
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tempColor = Colors(1,1,1);

	// General things

	// Title
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, 5*myGame.parameters.buildingHeight/12, 2*myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/4, myTextures[13], tempColor);
	// Name
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/2, -myGame.parameters.buildingHeight/3, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[14], tempColor);

	// Game Over
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/12, myGame.parameters.buildingWidth/2, myGame.parameters.buildingHeight/6, myTextures[19], tempColor);

	// Restart Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[17], tempColor);
	// Exit Small Button
	drawSimpleTexturedSquare(myGame.parameters.buildingWidth/12, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[18], tempColor);
}

void drawVictoryMenu(Game myGame, std::vector<GLuint> myTextures) {
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tempColor = Colors(1,1,1);

	// General things

	// Title
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, 5*myGame.parameters.buildingHeight/12, 2*myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/4, myTextures[13], tempColor);
	// Name
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/2, -myGame.parameters.buildingHeight/3, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[14], tempColor);

	// Restart Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[17], tempColor);
	// Exit Small Button
	drawSimpleTexturedSquare(myGame.parameters.buildingWidth/12, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[18], tempColor);

	// Score
	tempColor = Colors(57./255,181./255,74./255);
	for (int i = 0; i < 6; i++)
	{
		drawSimpleTexturedSquare(myGame.parameters.buildingWidth/4-(i*myGame.parameters.buildingWidth/8.5),myGame.parameters.buildingHeight/12,myGame.parameters.buildingHeight/6,myGame.parameters.buildingHeight/6,myTextures[0], tempColor);
	}
}