#include "draw_scene.h"
#include "3D_tools.h"
#include "game_objects.h"
#include <vector>

using namespace std;

Colors tempColor = Colors(); // Temporary color used for illumination function
Position tempPos1 = Position(0, 0, 0); // Temporary position used for illumination function
Position tempPos2 = Position(0, 0, 0);
Position tempPos3 = Position(0, 0, 0);
Position tempPos4 = Position(0, 0, 0);

/*
   DRAWING PRIMITIVES
*/

// Draws lines (Used for frame drawing)
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float alpha)
{
    glColor4f(r, g, b, alpha);
    glBegin(GL_LINES);
    	glVertex3f(x1, y1, z1);
    	glVertex3f(x2, y2, z2);
    glEnd();
}

// Draws an untextured colored rectangle (Light taken into account)
void drawSquareLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, Colors baseColor, Position point1, Position point2, Position point3, Position point4) {
	glBegin(GL_TRIANGLE_FAN);
		// Bottom left corner
		tempColor = baseColor.displayColor(posBalls, posPlayer, point1, gameDepth); // Display color generation
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

// Draws a rectangle with a stretched texture (Light taken into account)
void drawTextureLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, GLuint texture, Position point1, Position point2, Position point3, Position point4) {
	// Current color initialization for the lighting
	Colors white = Colors(1, 1, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Settings for texture enlargement suitable for pixel art
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_TRIANGLE_FAN);
		// Bottom left corner
		tempColor = white.displayColor(posBalls, posPlayer, point1, gameDepth); // Display color generation
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(0., 1.); // Attaching the texture to the whole shape
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

// Draws a rectangle with a repeating texture as a tileset (Light taken into account)
void drawTilesetLight(float gameDepth, float opacity, std::vector<Position> posBalls, Position posPlayer, GLuint texture, float scaleX, float scaleY, Position point1, Position point2, Position point3, Position point4) {
    // Current color initialization for the lighting
	Colors white = Colors(1, 1, 1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

	// Parameters for texture pattern
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Settings for texture enlargement suitable for pixel art
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBegin(GL_TRIANGLE_FAN);
		// Bottom left corner
		tempColor = white.displayColor(posBalls, posPlayer, point1, gameDepth); // Display color generation
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity); 
		glTexCoord2f(0., scaleY); // Texture placement according to its size passed as a parameter
		glVertex3f(-0.5, -0.5, 0.0);

		// Bottom right corner
		tempColor = white.displayColor(posBalls, posPlayer, point2, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(scaleX, scaleY);
		glVertex3f(0.5, -0.5, 0.0);

		// Top right corner
		tempColor = white.displayColor(posBalls, posPlayer, point3, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(scaleX, 0.);
		glVertex3f(0.5, 0.5, 0.0);

		// Top left corner
		tempColor = white.displayColor(posBalls, posPlayer, point4, gameDepth);
		glColor4f(tempColor.r, tempColor.g, tempColor.b, opacity);
		glTexCoord2f(0., 0.);
		glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Draws a corridor wall based on the parameters
void drawWall(float offsetX, float offsetY, float offsetZ, float rotationX, float rotationY, float scaleX, float scaleY, float scaleZ, Position tempPos1, Position tempPos2, Position tempPos3, Position tempPos4, Game game, Colors color, GLuint texture, std::vector<Position> posBalls)
{
    glPushMatrix();
		glTranslatef(offsetX, offsetY, offsetZ);
		glScalef(scaleX, scaleY, scaleZ);
		glRotatef(rotationX, 1, 0, 0);
		glRotatef(rotationY, 0, 1, 0);
		
		// Depending on the visual skin, the function called is different
		if (game.renderSkinId == 0 || game.renderSkinId == 3) {
			drawSquareLight(game.parameters.gameDepth, 1, posBalls, game.player.pos, color, tempPos1, tempPos2, tempPos3, tempPos4);
		} else {
			drawTextureLight(game.parameters.gameDepth, 1, posBalls, game.player.pos, texture, tempPos1, tempPos2, tempPos3, tempPos4);
		}
    glPopMatrix();
}

// Draws textured rectangles for menus and interface (Light is not taken into account)
void drawSimpleTexturedSquare(float x, float z, float width, float height, GLuint texture, Colors color) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Settings for texture enlargement suitable for pixel art
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

/*
   DEBUG TOOLS
*/

// Check that a texture is loaded correctly
void checkTexture(GLuint texture)
{
    // Check if texture ID is valid
    if (texture != 0)
    {
        GLint width, height;
        glBindTexture(GL_TEXTURE_2D, texture);

        // Get texture width and height
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

        glBindTexture(GL_TEXTURE_2D, 0);

        // Check if the texture is empty
        if (width > 0 && height > 0)
        {
            printf("Texture is loaded successfully.\n");
            printf("Texture width: %d, height: %d\n", width, height);
        }
        else
        {
            printf("Texture is empty or not loaded.\n");
        }
    }
    else
    {
        printf("Invalid texture.\n");
    }
}

/*
   DRAWING FUNCTIONS
*/

// Draws the frame
void drawFrame()
{
    // WIDTH GRID
    for (int i = -myGame.parameters.buildingWidth / 2; i < myGame.parameters.buildingWidth / 2 + 1; i++)
    {
        // Y AXIS
        drawLine((GLfloat)i, 0, myGame.parameters.buildingHeight / 2, (GLfloat)i, 0, -myGame.parameters.buildingHeight / 2, 0, 1, 0, 0.2);
        // Z AXIS
        drawLine((GLfloat)i, myGame.parameters.buildingDepth / 2, 0, (GLfloat)i, -myGame.parameters.buildingDepth / 2, 0, 0, 0, 1, 0.2);
    }
    
    // HEIGHT GRID
    for (int i = -myGame.parameters.buildingHeight / 2; i < myGame.parameters.buildingHeight / 2 + 1; i++)
    {
        // Y AXIS
        drawLine(myGame.parameters.buildingWidth / 2, 0, (GLfloat)i, -myGame.parameters.buildingWidth / 2, 0, (GLfloat)i, 0, 1, 0, 0.2);
        // X AXIS
        drawLine(0, myGame.parameters.buildingDepth / 2, (GLfloat)i, 0, -myGame.parameters.buildingDepth / 2, (GLfloat)i, 1, 0, 0, 0.2);
    }
    
    // DEPTH GRID
    for (int i = -myGame.parameters.buildingDepth / 2; i < myGame.parameters.buildingDepth / 2 + 1; i++)
    {
        // X AXIS
        drawLine(0, (GLfloat)i, myGame.parameters.buildingHeight / 2, 0, (GLfloat)i, -myGame.parameters.buildingHeight / 2, 1, 0, 0, 0.2);
        // Z AXIS
        drawLine(-myGame.parameters.buildingWidth / 2, (GLfloat)i, 0, myGame.parameters.buildingWidth / 2, (GLfloat)i, 0, 0, 0, 1, 0.2);
    }
    
    // ORIGIN
	glLineWidth(6.0);
    drawLine(6.0, 0.0, 0.0, -6.0, 0.0, 0.0, 1, 0, 0, 1.0);  // X
    drawLine(0.0, 6.0, 0.0, 0.0, -6.0, 0.0, 0, 1, 0, 1.0);  // Y
    drawLine(0.0, 0.0, 6.0, 0.0, 0.0, -6.0, 0, 0, 1, 1.0);  // Z
	glLineWidth(1.0);
}

// Draws the corridor, call the functions to draw its elements
void drawCorridor(Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer)
{
	// Draws the bottom of the corridor
	float posY = myGame.parameters.buildingDepth + myGame.parameters.buildingDepth * (myCorridor.numberOfSteps - 1); // Y of the bottom
	tempPos1.updatePosition(-myGame.parameters.buildingWidth / 2, posY, -myGame.parameters.buildingHeight / 2); // Position of the corners of the wall
	tempPos2.updatePosition(myGame.parameters.buildingWidth / 2, posY, -myGame.parameters.buildingHeight / 2);
	tempPos3.updatePosition(myGame.parameters.buildingWidth / 2, posY, myGame.parameters.buildingHeight / 2);
	tempPos4.updatePosition(-myGame.parameters.buildingWidth / 2, posY, myGame.parameters.buildingHeight / 2);
	drawWall(0, posY, 0, 90, 0, myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight, tempPos1, tempPos2, tempPos3, tempPos4, myGame, myCorridor.colorSideWalls, myCorridor.bottomTexture, posBalls);

	// Draws the ring at the bottom of the corridor
	glPushMatrix();
		// The y-position is slightly shifted so that the ring is displayed despite the bottom of the corridor
		glTranslatef(0, myGame.parameters.buildingDepth + myGame.parameters.buildingDepth * (myCorridor.numberOfSteps - 1) - (float)1 / 100, 0);
		glScalef(myGame.parameters.buildingWidth, 1, myGame.parameters.buildingHeight);
		glRotatef(90, 1, 0, 0);

		// Definition of ring thickness
		glLineWidth(3.0);

		glBegin(GL_LINE_LOOP);
			tempPos1.updatePosition(-myGame.parameters.buildingWidth / 2, posY, myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 1);
			glVertex2f(-0.5f, 0.5f);

			tempPos1.updatePosition(myGame.parameters.buildingWidth / 2, posY, myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 1);
			glVertex2f(0.5f, 0.5f);

			tempPos1.updatePosition(myGame.parameters.buildingWidth / 2, posY, -myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 1);
			glVertex2f(0.5f, -0.5f);

			tempPos1.updatePosition(-myGame.parameters.buildingWidth / 2, posY, -myGame.parameters.buildingHeight / 2);
			tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
			glColor4f(tempColor.r, tempColor.g, tempColor.b, 1);
			glVertex2f(-0.5f, -0.5f);
		glEnd();

		glLineWidth(1.0);
	glPopMatrix();

	// Call of the function to draw the wallSteps of the corridor
	drawWallStep(myCorridor.wallSteps, myCorridor, posBalls, posPlayer);
}

// Draws a corrider step: side walls, ceiling and floor
void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, std::vector<Position> posBalls, Position posPlayer)
{
	// Inverted loop to draw the steps starting from the end
	for (int i = myWallSteps.size() - 1; i > -1; i--)
	{
		float buildingDepth = myGame.parameters.buildingDepth;
		float buildingHeight = myGame.parameters.buildingHeight;
		float buildingWidth = myGame.parameters.buildingWidth;

		// Ceiling
		float posX = 0; // Corridor x center
		float posY1 = myWallSteps[i].pos.y - buildingDepth / 2; // WallStep y center
		float posY2 = myWallSteps[i].pos.y; // WallStep origin
		float posZ1 = buildingHeight / 2; // Position of the ceiling
		float posZ2 = -buildingHeight / 2; // Position of the ground
		tempPos1.updatePosition(-buildingWidth / 2, posY2 - buildingDepth, posZ1);
		tempPos2.updatePosition(buildingWidth / 2, posY2 - buildingDepth, posZ1);
		tempPos3.updatePosition(buildingWidth / 2, posY2, posZ1);
		tempPos4.updatePosition(-buildingWidth / 2, posY2, posZ1);
		drawWall(posX, posY1, posZ1, 0, 0, buildingWidth, buildingDepth, buildingHeight, tempPos1, tempPos2, tempPos3, tempPos4, myGame, myCorridor.colorCeillingWalls, myCorridor.ceilingTexture, posBalls);

		// Ground
		tempPos1.updatePosition(-buildingWidth / 2, posY2 - buildingDepth, posZ2);
		tempPos2.updatePosition(buildingWidth / 2, posY2 - buildingDepth, posZ2);
		tempPos3.updatePosition(buildingWidth / 2, posY2, posZ2);
		tempPos4.updatePosition(-buildingWidth / 2, posY2, posZ2);
		glPushMatrix();
			glTranslatef(posX, posY1, posZ2);
			glScalef(buildingWidth, buildingDepth, buildingHeight);
			// The ground specifically is treated differently depending on the skin
			if (myGame.renderSkinId == 0 || myGame.renderSkinId == 3) {
				drawSquareLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.colorCeillingWalls, tempPos1, tempPos2, tempPos3, tempPos4);
			} else if (myGame.renderSkinId == 1) {
				drawTilesetLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.groundTexture, buildingWidth / 2, buildingDepth / 2, tempPos1, tempPos2, tempPos3, tempPos4);
			}
			else {
				drawTilesetLight(myGame.parameters.gameDepth, 1, posBalls, posPlayer, myCorridor.groundTexture, buildingWidth, buildingDepth, tempPos1, tempPos2, tempPos3, tempPos4);
			}
		glPopMatrix();

		// Left wall
		posX = -buildingWidth / 2; // Position of the left wall
		posZ1 = 0;
		tempPos1.updatePosition(posX, posY2 - buildingDepth, buildingHeight / 2);
		tempPos2.updatePosition(posX, posY2 - buildingDepth, -buildingHeight / 2);
		tempPos3.updatePosition(posX, posY2, -buildingHeight / 2);
		tempPos4.updatePosition(posX, posY2, buildingHeight / 2);
		drawWall(posX, posY1, posZ1, 0, 90, buildingWidth, buildingDepth, buildingHeight, tempPos1, tempPos2, tempPos3, tempPos4, myGame, myCorridor.colorSideWalls, myCorridor.leftWallTexture, posBalls);

		// Right wall
		posX = buildingWidth / 2; // Position of the right wall
		tempPos1.updatePosition(posX, posY2 - buildingDepth, buildingHeight / 2);
		tempPos2.updatePosition(posX, posY2 - buildingDepth, -buildingHeight / 2);
		tempPos3.updatePosition(posX, posY2, -buildingHeight / 2);
		tempPos4.updatePosition(posX, posY2, buildingHeight / 2);
		drawWall(posX, posY1, posZ1, 0, 90, buildingWidth, buildingDepth, buildingHeight, tempPos1, tempPos2, tempPos3, tempPos4, myGame, myCorridor.colorSideWalls, myCorridor.rightWallTexture, posBalls);

		// Draws the rings
		// The last ring of the corridor is covered above
		if (i != myWallSteps.size() - 1)
		{
			// Ring
			glPushMatrix();
				glTranslatef(0, posY2, 0);
				glScalef(buildingWidth, 1, buildingHeight);
				glRotatef(90, 1, 0, 0);
				glLineWidth(3.0);
				glBegin(GL_LINE_LOOP);
				
					// Top left corner
					tempPos1.updatePosition(-buildingWidth / 2, posY2, buildingHeight / 2);
					tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
					glColor4f(tempColor.r, tempColor.g, tempColor.b, 1.);
					glVertex2f(-0.5f, 0.5f);

					// Top right corner
					tempPos1.updatePosition(buildingWidth / 2, posY2, buildingHeight / 2);
					tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
					glColor4f(tempColor.r, tempColor.g, tempColor.b, 1.);
					glVertex2f(0.5f, 0.5f);

					// Bottom right corner
					tempPos1.updatePosition(buildingWidth / 2, posY2, -buildingHeight / 2);
					tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
					glColor4f(tempColor.r, tempColor.g, tempColor.b, 1.);
					glVertex2f(0.5f, -0.5f);

					// Bottom left corner
					tempPos1.updatePosition(-buildingWidth / 2, posY2, -buildingHeight / 2);
					tempColor = myCorridor.colorRings.displayColor(posBalls, posPlayer, tempPos1, myGame.parameters.gameDepth);
					glColor4f(tempColor.r, tempColor.g, tempColor.b, 1.);
					glVertex2f(-0.5f, -0.5f);

				glEnd();
				glLineWidth(1.0);
			glPopMatrix();
		}

		// Call of the function to draw the Walls of the wallStep
		drawWall(myWallSteps[i].walls, posBalls, posPlayer, myWallSteps[i].color, myCorridor.wallsTexture);

		// Call of the function to draw the bonus of the wallStep if it exists
		if (myWallSteps[i].bonus.alive == true)
		{
			drawBonusBox(myWallSteps[i].bonus, myGame, posBalls, posPlayer);
		}
	}
}

// Draws the obstacle walls of a wallStep
void drawWall(std::vector<Wall> myWalls, std::vector<Position> posBalls, Position posPlayer, Colors wallColor, GLuint wallsTexture)
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
				tempPos1.updatePosition(myWalls[i].pos.x, myWalls[i].pos.y, myWalls[i].pos.z - myWalls[i].height);
				// Bottom right corner
				tempPos2.updatePosition(myWalls[i].pos.x + myWalls[i].width, myWalls[i].pos.y, myWalls[i].pos.z - myWalls[i].height);
				// Top right corner
				tempPos3.updatePosition(myWalls[i].pos.x + myWalls[i].width, myWalls[i].pos.y, myWalls[i].pos.z);
				// Top left corner
				tempPos4.updatePosition(myWalls[i].pos.x, myWalls[i].pos.y, myWalls[i].pos.z);

				// Drawing of the walls varies according to the skin
				if (myGame.renderSkinId == 0) {
					drawSquareLight(myGame.parameters.gameDepth, 0.8, posBalls, posPlayer, wallColor, tempPos1, tempPos2, tempPos3, tempPos4);	
				} else if (myGame.renderSkinId == 1) {
					drawTilesetLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, wallsTexture, myWalls[i].width/4, myWalls[i].height/4,  tempPos1, tempPos2, tempPos3, tempPos4);		
				} else if (myGame.renderSkinId == 3) {
					drawSquareLight(myGame.parameters.gameDepth, 0.8, posBalls, posPlayer, Colors(1.,1.,1.), tempPos1, tempPos2, tempPos3, tempPos4);
				} else {
					drawTilesetLight(myGame.parameters.gameDepth, 0.75, posBalls, posPlayer, wallsTexture, myWalls[i].width/2, myWalls[i].height/2,  tempPos1, tempPos2, tempPos3, tempPos4);		
				}
			glPopMatrix();
		}
	}
}

// Draws the balls (Even if there is currently only one)
void drawBalls(std::vector<Ball> balls, GLuint ballsTexture)
{
	for (Ball ball : balls)
	{
		glPushMatrix();
			// if(myGame.balls[0].isLaunched) glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);
			// else glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);

			glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);
			glScalef(ball.radius, ball.radius, ball.radius);
			glColor3f(1., 1., 1.);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ballsTexture);
			GLUquadric* myQuadric = gluNewQuadric(); // Create a new quadric object
			gluQuadricTexture(myQuadric, GL_TRUE); // Enable texture coordinates for the quadric
			gluSphere(myQuadric, 1.0, NB_SEG_CIRCLE, NB_SEG_CIRCLE); // Render the sphere using the quadric
			gluDeleteQuadric(myQuadric);
			delete myQuadric;
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

// Draws the player's puck
void drawPlayer(Player myPlayer)
{
	glPushMatrix();
		glTranslatef(0, myPlayer.pos.y, 0);
		glTranslatef(myPlayer.pos.x, 0, myPlayer.pos.z);
		glScalef(myPlayer.width, 1, myPlayer.height);
		glRotatef(90, 1, 0, 0);
		glColor4f((float)178 / 255, (float)178 / 255, (float)178 / 255, 1.);
		
		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP); // Draws the outline of the puck
			glVertex2f(-0.5f, 0.5f);
			glVertex2f(0.5f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glVertex2f(-0.5f, -0.5f);
		glEnd();
		glLineWidth(1.0);

		// Draws the transparent interior of the puck
		glColor4f((float)178 / 255, (float)178 / 255, (float)178 / 255, .25);
		drawSquare();

	glPopMatrix();
}

// Draws an object box
void drawBonusBox(Bonus myBonus, Game myGame, std::vector<Position> posBalls, Position posPlayer)
{
    if (myBonus.pos.y - myGame.parameters.gameDepth >= 0) {

		// Calculation of the coordinates of the corners of the box
		GLfloat leftX = myBonus.pos.x - (myBonus.width / 2.);
		GLfloat rightX = myBonus.pos.x + (myBonus.width / 2.);
		GLfloat bottomZ = myBonus.pos.z - (myBonus.width / 2.);
		GLfloat topZ = myBonus.pos.z + (myBonus.width / 2.);
		GLfloat frontY = myBonus.pos.y - (myBonus.width / 2.);
		GLfloat backY = myBonus.pos.y + (myBonus.width / 2.);

		// Disabling the writing to the depth buffer to manually manage it temporarily
		glDepthMask(GL_FALSE);

		// Back side
		glPushMatrix();
			glTranslatef(myBonus.pos.x, backY, myBonus.pos.z);
			glScalef(myBonus.width, myBonus.width, myBonus.width);
			glRotatef(90, 1, 0, 0);
			tempPos1.updatePosition(leftX, backY, bottomZ);
			tempPos2.updatePosition(rightX, backY, bottomZ);
			tempPos3.updatePosition(rightX, backY, topZ);
			tempPos4.updatePosition(leftX, backY, topZ);
			if (myGame.renderSkinId == 0) {
				tempColor = myGame.corridor.colorRings.generateComplementaryColor();
                drawSquareLight(myGame.parameters.gameDepth, .5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);
            } else {
				tempColor.updateColors(1., 1., 1.);
                drawTextureLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, myGame.corridor.bonusBoxTexture, tempPos1, tempPos2, tempPos3, tempPos4);
            }
		glPopMatrix();

		// Depending on the position of the box in relation to the camera, the face to be drawn in priority changes
		bool isLeft = false;
		if (myBonus.pos.x < 0) {
			isLeft = true;
		}
		bool isTop = false;
		if (myBonus.pos.z > 0) {
			isTop = true;
		}

		// Loop that replicates a depth buffer
		for (int i = 0; i < 2; i++)
		{
			if ((isTop && i == 0) || (!isTop && i == 1) )
			{
				// Upper face
				tempColor.updateColors(1., 1., 1.);
				glPushMatrix();
					glTranslatef(myBonus.pos.x, myBonus.pos.y, topZ);
					glScalef(myBonus.width, myBonus.width, myBonus.width);
					tempPos1.updatePosition(leftX, frontY, topZ);
					tempPos2.updatePosition(rightX, frontY, topZ);
					tempPos3.updatePosition(rightX, backY, topZ);
					tempPos4.updatePosition(leftX, backY, topZ);
					if (myGame.renderSkinId == 0) {
						tempColor = myGame.corridor.colorSideWalls;
						drawSquareLight(myGame.parameters.gameDepth, .5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);
					} else {
						tempColor.updateColors(1., 1., 1.);
						drawTextureLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, myGame.corridor.bonusBoxTexture, tempPos1, tempPos2, tempPos3, tempPos4);
					}
				glPopMatrix();
			} else {
				// Lower face
				tempColor.updateColors(1., 1., 1.);
				glPushMatrix();
					glTranslatef(myBonus.pos.x, myBonus.pos.y, bottomZ);
					glScalef(myBonus.width, myBonus.width, myBonus.width);
					tempPos1.updatePosition(leftX, frontY, bottomZ);
					tempPos2.updatePosition(rightX, frontY, bottomZ);
					tempPos3.updatePosition(rightX, backY, bottomZ);
					tempPos4.updatePosition(leftX, backY, bottomZ);
					if (myGame.renderSkinId == 0) {
						tempColor = myGame.corridor.colorRings;
						drawSquareLight(myGame.parameters.gameDepth, .5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);
					} else {
						tempColor.updateColors(1., 1., 1.);
						tempColor = myGame.corridor.colorSideWalls;
						drawTextureLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, myGame.corridor.bonusBoxTexture, tempPos1, tempPos2, tempPos3, tempPos4);
					}
				glPopMatrix();
			}
			if ((isLeft && i == 0) || (!isLeft && i == 1) )
			{
				// Left face
				tempColor.updateColors(1., 1., 1.);
				glPushMatrix();
					glTranslatef(leftX, myBonus.pos.y, myBonus.pos.z);
					glScalef(myBonus.width, myBonus.width, myBonus.width);
					glRotatef(90, 0, 1, 0);
					tempPos1.updatePosition(leftX, frontY, topZ);
					tempPos2.updatePosition(leftX, frontY, bottomZ);
					tempPos3.updatePosition(leftX, backY, bottomZ);
					tempPos4.updatePosition(leftX, backY, topZ);
					if (myGame.renderSkinId == 0) {
						tempColor = myGame.corridor.colorCeillingWalls;
						drawSquareLight(myGame.parameters.gameDepth, .5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);
					} else {
						tempColor.updateColors(1., 1., 1.);
						drawTextureLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, myGame.corridor.bonusBoxTexture, tempPos1, tempPos2, tempPos3, tempPos4);
					}
				glPopMatrix();
			} else {
				// Right face
				tempColor.updateColors(1., 1., 1.);
				glPushMatrix();
					glTranslatef(rightX, myBonus.pos.y, myBonus.pos.z);
					glScalef(myBonus.width, myBonus.width, myBonus.width);
					glRotatef(90, 0, 1, 0);
					tempPos1.updatePosition(rightX, frontY, topZ);
					tempPos2.updatePosition(rightX, frontY, bottomZ);
					tempPos3.updatePosition(rightX, backY, bottomZ);
					tempPos4.updatePosition(rightX, backY, topZ);
					if (myGame.renderSkinId == 0) {
						tempColor = myGame.corridor.colorCeillingWalls;
						drawSquareLight(myGame.parameters.gameDepth, .5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);
					} else {
						tempColor.updateColors(1., 1., 1.);
						drawTextureLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, myGame.corridor.bonusBoxTexture, tempPos1, tempPos2, tempPos3, tempPos4);
					}
				glPopMatrix();
			}
		}

		// Front face
		glPushMatrix();
			glTranslatef(myBonus.pos.x, frontY, myBonus.pos.z);
			glScalef(myBonus.width, myBonus.width, myBonus.width);
			glRotatef(90, 1, 0, 0);
			tempPos1.updatePosition(leftX, frontY, bottomZ);
			tempPos2.updatePosition(rightX, frontY, bottomZ);
			tempPos3.updatePosition(rightX, frontY, topZ);
			tempPos4.updatePosition(leftX, frontY, topZ);
			if (myGame.renderSkinId == 0) {
				tempColor = myGame.corridor.colorRings.generateComplementaryColor();
                drawSquareLight(myGame.parameters.gameDepth, .5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);
            } else {
				tempColor.updateColors(1., 1., 1.);
                drawTextureLight(myGame.parameters.gameDepth, 1., posBalls, posPlayer, myGame.corridor.bonusBoxTexture, tempPos1, tempPos2, tempPos3, tempPos4);
            }
		glPopMatrix();

		glDepthMask(GL_TRUE);
	}
}

// Draws the interface displayed in game
void drawInterface(Game myGame, std::vector<GLuint> myTextures, std::vector<Position> posBalls, Position posPlayer) {

	// Drawing of the illuminated transparent strip
	glPushMatrix();
		glTranslatef(0, 0, 0);
		glTranslatef(-myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/2);
		glTranslatef((myGame.parameters.buildingWidth / 2), 0, -((myGame.parameters.buildingHeight/6)/2));
		glScalef(myGame.parameters.buildingWidth, 1, (myGame.parameters.buildingHeight/6));
		glRotatef(90, 1, 0, 0);
		tempColor.updateColors(0.2,0.2,0.2);
		tempPos1.updatePosition(-myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/3);
		tempPos2.updatePosition(myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/3);
		tempPos3.updatePosition(myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/2);
		tempPos4.updatePosition(-myGame.parameters.buildingWidth/2, 0, myGame.parameters.buildingHeight/2);
		drawSquareLight(0, 0.5, posBalls, posPlayer, tempColor, tempPos1, tempPos2, tempPos3, tempPos4);		
	glPopMatrix();

	// Lifes
	tempColor = myGame.corridor.colorRings.generateComplementaryColor();
	glColor3f(tempColor.r,tempColor.g,tempColor.b);
	for (int i = 0; i < myGame.lives; i++) // Loop to draw the correct number of lives
	{
		drawSimpleTexturedSquare(-5*myGame.parameters.buildingWidth/12+(i*myGame.parameters.buildingWidth/48),11*myGame.parameters.buildingHeight/24,myGame.parameters.buildingHeight/12,myGame.parameters.buildingHeight/12,myTextures[10], tempColor);
	}
	glDisable(GL_TEXTURE_2D);

	// Score
	int score = myGame.score;
	// Transformation of the score into individual figures per unit
	for (int i = 0; i < 6; i++)
	{
		int digit = score % 10;
  		score /= 10;
		drawSimpleTexturedSquare(5*myGame.parameters.buildingWidth/12-(i*myGame.parameters.buildingWidth/20),11*myGame.parameters.buildingHeight/24,myGame.parameters.buildingHeight/12,myGame.parameters.buildingHeight/12,myTextures[digit], tempColor);
	}
	glDisable(GL_TEXTURE_2D);
}

// Draws the starting menu
void drawStartingMenu(Game myGame, std::vector<GLuint> myTextures) {
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tempColor.updateColors(1,1,1);

	// Title
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, 5*myGame.parameters.buildingHeight/12, 2*myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/4, myTextures[11], tempColor);
	// Name
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/2, -myGame.parameters.buildingHeight/3, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[12], tempColor);
	
	// Start Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/6, myGame.parameters.buildingHeight/12, myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/6, myTextures[13], tempColor);
	// Exit Large Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/6, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/6, myTextures[14], tempColor);
}

// Draws the Game Over
void drawGameoverMenu(Game myGame, std::vector<GLuint> myTextures) {
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tempColor.updateColors(1,1,1);

	// General things

	// Title
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, 5*myGame.parameters.buildingHeight/12, 2*myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/4, myTextures[11], tempColor);
	// Name
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/2, -myGame.parameters.buildingHeight/3, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[12], tempColor);

	// Game Over
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/12, myGame.parameters.buildingWidth/2, myGame.parameters.buildingHeight/6, myTextures[17], tempColor);

	// Restart Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[15], tempColor);
	// Exit Small Button
	drawSimpleTexturedSquare(myGame.parameters.buildingWidth/12, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[16], tempColor);
}

// Draws the victory screen with the score
void drawVictoryMenu(Game myGame, std::vector<GLuint> myTextures) {
	glClearColor(0., 0., 0., 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tempColor.updateColors(1,1,1);

	// General things

	// Title
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, 5*myGame.parameters.buildingHeight/12, 2*myGame.parameters.buildingWidth/3, myGame.parameters.buildingHeight/4, myTextures[11], tempColor);
	// Name
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/2, -myGame.parameters.buildingHeight/3, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[12], tempColor);

	// Restart Button
	drawSimpleTexturedSquare(-myGame.parameters.buildingWidth/3, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[15], tempColor);
	// Exit Small Button
	drawSimpleTexturedSquare(myGame.parameters.buildingWidth/12, -myGame.parameters.buildingHeight/6, myGame.parameters.buildingWidth/4, myGame.parameters.buildingHeight/6, myTextures[16], tempColor);

	// Score
	tempColor.updateColors(57./255,181./255,74./255);
	int score = myGame.score;
	for (int i = 0; i < 6; i++)
	{
		int digit = score % 10;
  		score /= 10;
		drawSimpleTexturedSquare(myGame.parameters.buildingWidth/4-(i*myGame.parameters.buildingWidth/8.5),myGame.parameters.buildingHeight/12,myGame.parameters.buildingHeight/6,myGame.parameters.buildingHeight/6,myTextures[digit], tempColor);
	}
}