#include "draw_scene.h"
#include "3D_tools.h"
#include "game_objects.h"
#include <vector>

using namespace std;

Colors tempColor = Colors();
Position posBottom = Position(0,0,0);

// Fonction qui dessine une grille et une origine
void drawFrame() {
    // WIDTH GRID
    for (int i = - building_width/2; i < building_width/2+1; i++)
    {          
        // Y AXIS
        glColor4f(0,1,0,0.2);
        glBegin(GL_LINES);
                glVertex3f((GLfloat) i, 0, building_height/2);
                glVertex3f((GLfloat) i, 0, -building_height/2);
        glEnd();

        // Z AXIS
        glColor4f(0,0,1,0.2);
        glBegin(GL_LINES);
            glVertex2f((GLfloat) i, building_depth/2);
            glVertex2f((GLfloat) i, -building_depth/2);
        glEnd();
        
    }
    // HEIGHT GRID
    for (int i = - building_height/2; i < building_height/2+1; i++)
    {
        // Y AXIS
        glColor4f(0,1,0,0.2);
        glBegin(GL_LINES);
                glVertex3f(building_width/2, 0, (GLfloat) i);
                glVertex3f(-building_width/2, 0, (GLfloat) i);
        glEnd();

        // X AXIS
        glColor4f(1,0,0,0.2);
        glBegin(GL_LINES);
                glVertex3f(0.0, building_depth/2, (GLfloat) i);
                glVertex3f(0.0, -building_depth/2, (GLfloat) i);
        glEnd();
        
    }

    // DEPTH GRID
    for (int i = - building_depth/2; i < building_depth/2+1; i++)
    {
        // X AXIS
        glColor4f(1,0,0,0.2);
        glBegin(GL_LINES);
                glVertex3f(0.0, (GLfloat) i, building_height/2);
                glVertex3f(0.0, (GLfloat) i, -building_height/2);
        glEnd();

        // Z AXIS
        glColor4f(0,0,1,0.2);
        glBegin(GL_LINES);
            glVertex2f(-building_width/2, (GLfloat) i);
            glVertex2f(building_width/2, (GLfloat) i);
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

// Obsolete
void drawTunnelPart(int tunnel_depth) {
	// Top wall
	glColor4f(0./255,0./255,69./255,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth/2+building_depth*tunnel_depth,building_height/2);
		glScalef(building_width, building_depth, building_height);
		drawSquare();
	glPopMatrix();

	// Right wall
	glColor4f(69./255,69./255,142./255,0.8);
	glPushMatrix();
		glTranslatef(building_width/2,building_depth/2+building_depth*tunnel_depth,0);
		glRotatef(90, 0, 1, 0);
		glScalef(building_height, building_depth, building_height);
		drawSquare();
	glPopMatrix();

	// Bottom wall
	glColor4f(69./255,69./255,105./255,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth/2+building_depth*tunnel_depth,-building_height/2);
		glScalef(building_width, building_depth, building_height);
		drawSquare();
	glPopMatrix();

	// Left wall
	glColor4f(69./255,105./255,178./255,0.8);
	glPushMatrix();
		glTranslatef(-building_width/2,building_depth/2+building_depth*tunnel_depth,0);
		glRotatef(90, 0, 1, 0);
		glScalef(building_height, building_depth, building_height);
		drawSquare();
	glPopMatrix();

	// Bottom
	glColor4f(0.,0.,0.,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*tunnel_depth,0);
		glScalef(building_width, 1, building_height);
		glRotatef(90, 1, 0, 0);
		
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

	// Wall test 1
	glColor4f(0.,0.,1.,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*tunnel_depth,0);
		glTranslatef(-building_width/2,0,0);
		glTranslatef(-(-building_width/3)/2,0,0);
		glScalef(building_width/3, 1, building_height);
		glRotatef(90, 1, 0, 0);
		if (tunnel_depth==5)
		{
			drawSquare();
		}
	glPopMatrix();

	// Wall test 2
	glColor4f(0.,1.,0,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*tunnel_depth,0);
		glTranslatef(0,0,building_height/2);
		glTranslatef(0,0,-(building_height/2)/2);
		glScalef(building_width, 1, building_height/2);
		glRotatef(90, 1, 0, 0);
		if (tunnel_depth==2)
		{
			drawSquare();
		}
	glPopMatrix();

	// Wall test 3
	glColor4f(1.,0.,0,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*tunnel_depth,0);
		glTranslatef(-building_width/2,0,building_height/2);
		glTranslatef((building_width/4)/2,0,-(building_height)/2);
		glScalef(building_width/4, 1, building_height);
		glRotatef(90, 1, 0, 0);
		if (tunnel_depth==0)
		{
			drawSquare();
		}
	glPopMatrix();

	// Wall test 4
	glColor4f(1.,1.,0,0.8);
	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*tunnel_depth,0);
		glTranslatef(-building_width/6,0,building_height/3);
		glTranslatef((building_width/3)/2,0,-(2*building_height/3)/2);
		glScalef(building_width/3, 1, 2*building_height/3);
		glRotatef(90, 1, 0, 0);
		if (tunnel_depth==7)
		{
			drawSquare();
		}
	glPopMatrix();
}

void drawCorridor(Corridor myCorridor, Position posBall, Position posPlayer) {
	// Bottom
	// glColor4f(myCorridor.colorSideWalls.r,myCorridor.colorSideWalls.g,myCorridor.colorSideWalls.b,0.8);

	posBottom.y=building_depth+building_depth*(myCorridor.numberOfSteps-1);
	tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, posBottom, game_depth);
	glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);

	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*(myCorridor.numberOfSteps-1),0);
		glScalef(building_width, 1, building_height);
		glRotatef(90, 1, 0, 0);
		drawSquare();
	glPopMatrix();

	// Last ring
	// glColor4f(myCorridor.colorRings.r,myCorridor.colorRings.g,myCorridor.colorRings.b,0.8);

	tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, posBottom, game_depth);
	glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);

	glPushMatrix();
		glTranslatef(0,building_depth+building_depth*(myCorridor.numberOfSteps-1)-(float) 1/100,0);
		glScalef(building_width, 1, building_height);
		glRotatef(90, 1, 0, 0);
		glLineWidth(6.0);
		glBegin(GL_LINE_LOOP);
			glVertex2f(-0.5f, 0.5f);
			glVertex2f(0.5f, 0.5f);
			glVertex2f(0.5f, -0.5f);
			glVertex2f(-0.5f, -0.5f);
		glEnd();
		glLineWidth(1.0);
	glPopMatrix();
	
	// Appel de la fonction pour dessiner les wallSteps du corridor
    drawWallStep(myCorridor.wallSteps, myCorridor, posBall, posPlayer);
}

void drawWallStep(std::vector<WallStep> myWallSteps, Corridor myCorridor, Position posBall, Position posPlayer) {
    for (int i = myWallSteps.size()-1; i > -1; i--)
    {
		// glColor4f(myCorridor.colorCeillingWalls.r,myCorridor.colorCeillingWalls.g,myCorridor.colorCeillingWalls.b,0.8);
		// myWallSteps[i].lightImpact(myCorridor.colorCeillingWalls, game_depth, ballTempY);
		// glColor4f(myWallSteps[i].displayColor.r,myWallSteps[i].displayColor.g,myWallSteps[i].displayColor.b,0.8);

		tempColor = myCorridor.colorCeillingWalls.displayColor(posBall, posPlayer, myWallSteps[i].pos, game_depth);
		glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);
		
		// Top wall
        glPushMatrix();
            glTranslatef(0,myWallSteps[i].depth-building_depth/2,building_height/2);
            glScalef(building_width, building_depth, building_height);
            drawSquare();
        glPopMatrix();

        // Bottom wall
        glPushMatrix();
            glTranslatef(0,myWallSteps[i].depth-building_depth/2,-building_height/2);
            glScalef(building_width, building_depth, building_height);
            drawSquare();
        glPopMatrix();

		// myWallSteps[i].lightImpact(myCorridor.colorSideWalls, game_depth, ballTempY);
		// glColor4f(myWallSteps[i].displayColor.r,myWallSteps[i].displayColor.g,myWallSteps[i].displayColor.b,0.8);
		// glColor4f(myCorridor.colorSideWalls.r,myCorridor.colorSideWalls.g,myCorridor.colorSideWalls.b,0.8);

		tempColor = myCorridor.colorSideWalls.displayColor(posBall, posPlayer, myWallSteps[i].pos, game_depth);
		glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);

        // Left wall
        glPushMatrix();
            glTranslatef(-building_width/2,myWallSteps[i].depth-building_depth/2,0);
            glRotatef(90, 0, 1, 0);
            glScalef(building_height, building_depth, building_height);
            drawSquare();
        glPopMatrix();

		// Right wall
        glPushMatrix();
            glTranslatef(building_width/2,myWallSteps[i].depth-building_depth/2,0);
            glRotatef(90, 0, 1, 0);
            glScalef(building_height, building_depth, building_height);
            drawSquare();
        glPopMatrix();

		if ((int) i != myWallSteps.size()-1)
		{
			// Ring
			// myWallSteps[i].lightImpact(myCorridor.colorRings, game_depth, ballTempY);
			// glColor4f(myWallSteps[i].displayColor.r,myWallSteps[i].displayColor.g,myWallSteps[i].displayColor.b,0.8);

			tempColor = myCorridor.colorRings.displayColor(posBall, posPlayer, myWallSteps[i].pos, game_depth);
			glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);

			// glColor4f(myCorridor.colorRings.r,myCorridor.colorRings.g,myCorridor.colorRings.b,0.8);
			glPushMatrix();
				glTranslatef(0,myWallSteps[i].depth,0);
				glScalef(building_width, 1, building_height);
				glRotatef(90, 1, 0, 0);
				glLineWidth(6.0);
				glBegin(GL_LINE_LOOP);
					glVertex2f(-0.5f, 0.5f);
					glVertex2f(0.5f, 0.5f);
					glVertex2f(0.5f, -0.5f);
					glVertex2f(-0.5f, -0.5f);
				glEnd();
				glLineWidth(1.0);
			glPopMatrix();
		}

		// Appel de la fonction pour dessiner les murs du wallStep
		// glColor4f(myWallSteps[i].color.r, myWallSteps[i].color.g, myWallSteps[i].color.b,0.8);

		tempColor = myWallSteps[i].color.displayColor(posBall, posPlayer, myWallSteps[i].pos, game_depth);
		glColor4f(tempColor.r,tempColor.g,tempColor.b,0.8);

		drawWall(myWallSteps[i].walls, posBall, posPlayer);
    }
}

void drawWall(std::vector<Wall> myWalls, Position posBall, Position posPlayer) {
    for (int i = 0; i < (int) myWalls.size(); i++)
    {
        if (myWalls[i].depth-game_depth >= 0)
		{
			// Wall
			glPushMatrix();
				// Placement du mur au fond du wallStep
				glTranslatef(0,myWalls[i].depth,0);
				// Placement du mur
				glTranslatef(myWalls[i].pos.x,0,myWalls[i].pos.z);
				// Compensation pour placer l'origine du mur en haut à gauche
				glTranslatef((myWalls[i].width)/2,0,-(myWalls[i].height)/2);
				// Taille du mur
				glScalef(myWalls[i].width, 1, myWalls[i].height);
				glRotatef(90, 1, 0, 0);
				drawSquare();
			glPopMatrix();
		}
    }
}

void drawPlayer(Player myPlayer) {
	glPushMatrix();
		// Placement du mur au fond du wallStep
		glTranslatef(0,myPlayer.depth,0);
		// Placement du mur
		glTranslatef(myPlayer.pos.x,0,myPlayer.pos.z);
		// Taille du mur
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

void drawBall(Ball myBall) {
	// Dessin de la balle
	glPushMatrix();
		// Placement de la balle
		glTranslatef(myBall.pos.x,myBall.pos.y,myBall.pos.z);
		// Taille du mur
		glScalef(myBall.radius*2, myBall.radius*2, myBall.radius*2);
		glColor4f((float) 69/255,(float) 69/255,(float) 142/255,1.);
		drawSphere();
	glPopMatrix();
}