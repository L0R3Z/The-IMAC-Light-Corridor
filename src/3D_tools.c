#include "3D_tools.h"
#include "draw_scene.h"

using namespace std;

/* Camera parameters and functions */
float theta = -90.0f;                                                                                     // Angle between x axis and viewpoint (45.0f originally)
float phy = 90.0f;                                                                                         // Angle between z axis and viewpoint (60.0f originally)
float dist_zoom = (myGame.parameters.buildingHeight) / (2 * tan(toRad(myGame.parameters.aperture / 2))); // Distance between origin and viewpoint (30.0f originally)
// Le 60 c'est dans window resized maintenant aperture
// + 4 sur building height est du padding

void setCamera()
{
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0., 0.,-10.392305);
    glRotatef(-90,1.,0.,0.);
    // -10.39... correspond à dist_zoom
} 

/* Convert degree to radians */
float toRad(float deg)
{
    return deg * M_PI / 180.0f;
}

void drawSquare()
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glEnd();
}

void drawCircle()
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    float step_rad = 2 * M_PI / (float)NB_SEG_CIRCLE;
    for (int i = 0; i <= NB_SEG_CIRCLE; i++)
    {
        glVertex3f(cos(i * step_rad), sin(i * step_rad), 0.0f);
    }
    glEnd();
}

void drawCone()
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 1.0);
    float step_rad = 2 * M_PI / (float)NB_SEG_CIRCLE;
    for (int i = 0; i <= NB_SEG_CIRCLE; i++)
    {
        glVertex3f(cos(i * step_rad), sin(i * step_rad), 0.0f);
    }
    glEnd();
}

void drawSphere()
{
    gluSphere(gluNewQuadric(), 1.0, NB_SEG_CIRCLE, NB_SEG_CIRCLE);
}