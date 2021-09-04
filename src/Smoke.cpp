#include "Smoke.h"
#include <iostream>

Smoke::Smoke(GLfloat x, GLfloat y) : Particle(x, y)
{
    radius = 0.005+0.0075*randFloat();
   
    xState = randFloat()*M_PI;
    xShift = randFloat()*0.5 - 0.25;

    GLfloat br = 63+randFloat()*63;
    color[0] = 0.63*br;  color[1] = br*0.35;  color[2] = br*0.90;

    color[3] =  GLbyte(100);
}

void Smoke::display() const
{
    GLfloat cx = x + sin(xState)/50 + (xState/40)*xShift;
    glColor4b(color[0], color[1], color[2], color[3]);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, y, 0);
    for (float i = 0; i <= 10; i++)
    {
        glVertex3f(cx+radius*cos((i/5)*M_PI), y+radius*sin((i/5)*M_PI), 0);
    }
    glEnd();
}

void Smoke::advance()
{
    xState += 0.2;
    y += 0.005;
    color[3] -= 10/xState;
    if ( color[3] <= 0 )
        alive = false;
}

