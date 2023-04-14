#include "draw_scene.h"
#include "3D_tools.h"

float building_width = 36.0f;
float building_height = building_width/2;

// Fonction qui dessine une grille et une origine
void drawFrame() {
    //GRID
    for (int i = - building_width/2; i < building_width/2+1; i++)
    {          
        // Z
        // glColor3f((GLfloat)0 / 255, (GLfloat)0 / 255, (GLfloat)63 / 255);
        // glColor4f(0,0,1,0.1);
        // glBegin(GL_LINES);
        //     glVertex2f((GLfloat) i, building_width/2);
        //     glVertex2f((GLfloat) i, -building_width/2);
        // glEnd();
        // glBegin(GL_LINES);
        //     glVertex2f(-building_width/2, (GLfloat) i);
        //     glVertex2f(building_width/2, (GLfloat) i);
        // glEnd();

        // X
        // glColor3f((GLfloat)63 / 255, (GLfloat)0 / 255, (GLfloat)0 / 255);
        // glColor4f(1,0,0,0.1);
        // glBegin(GL_LINES);
        //         glVertex3f(0.0, building_width/2, (GLfloat) i);
        //         glVertex3f(0.0, -building_width/2, (GLfloat) i);
        // glEnd();
        // glBegin(GL_LINES);
        //         glVertex3f(0.0, (GLfloat) i, building_width/2);
        //         glVertex3f(0.0, (GLfloat) i, -building_width/2);
        // glEnd();

        // Y
        // glColor3f((GLfloat)0 / 255, (GLfloat)63 / 255, (GLfloat)0 / 255);
        glColor4f(0,1,0,0.1);
        glBegin(GL_LINES);
                glVertex3f(building_width/2, 0, (GLfloat) i);
                glVertex3f(-building_width/2, 0, (GLfloat) i);
        glEnd();
        glBegin(GL_LINES);
                glVertex3f((GLfloat) i, 0, building_height/2);
                glVertex3f((GLfloat) i, 0, -building_height/2);
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

void drawBase() {
    glColor3f((GLfloat) 235/255,(GLfloat) 207/255, (GLfloat) 52/255);
    glPushMatrix();
        glScalef(2,2,10);
        drawCone();
    glPopMatrix();

    glPushMatrix();
        glScalef(3,3,1);
        //glColor3f(0,1,0);
        drawCircle();
    glPopMatrix();
}

void drawArm() {
    glColor3f((GLfloat) 245/255,(GLfloat) 164/255, (GLfloat) 66/255);
    glPushMatrix();
        glScalef(2,2,2);
        drawSphere();
    glPopMatrix();

    glPushMatrix();
        glRotatef(90.0f, 1, 1, 0);
        glScalef(1,1,10);
        drawCone();
    glPopMatrix();

    glPushMatrix();
        glRotatef(-90.0f, 1, 1, 0);
        glScalef(1,1,10);
        drawCone();
    glPopMatrix();
}

void drawPan() {
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex3f(0,0.0,0.0);
        glVertex3f(3.0,0.0,-5.0);
    glEnd();

    glPushMatrix();
        glRotatef(120.0f, 0, 0, 1);
        glBegin(GL_LINES);
            glVertex3f(0,0.0,0.0);
            glVertex3f(3.0,0.0,-5.0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glRotatef(240.0f, 0, 0, 1);
        glBegin(GL_LINES);
            glVertex3f(0,0.0,0.0);
            glVertex3f(3.0,0.0,-5.0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(0,1,0);
        glTranslatef(0,0,-5);
        glScalef(3,3,1);
        drawCircle();
    glPopMatrix();
}

// void drawFrame(){
//     //REPERE 3D
//     glBegin(GL_LINES);
//         //axe x
//         glColor3f(1,0,0);
//         glVertex3f(2.0,0.0,0.0);
//         glVertex3f(-2.0,0.0,0.0);

//         //axe y
//         glColor3f(0,1,0);
//         glVertex3f(0.0,2.0,0.0);
//         glVertex3f(0.0,-2.0,0.0);

//         //axe z
//         glColor3f(0,0,1);
//         glVertex3f(0.0,0.0,2.0);
//         glVertex3f(0.0,0.0,-2.0);
//     glEnd();
// }
