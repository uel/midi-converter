#include "Star.h"

Star::Star(GLfloat x, GLfloat y) : Particle(x, y)
{
    opacity = 127;
    scale = randFloat()*2;
    xShift = (randFloat()-0.5)*0.01;
}

void Star::display() const 
{
    glColor4b(127, 127, 0, opacity);

    glRotatef(rotation, x, y, 0);

    glBegin (GL_TRIANGLES);
    glVertex3f(x+0.004*scale, y+0.005*scale, 0);
    glVertex3f(x+0, y+0.02*scale, 0);
    glVertex3f(x-0.004*scale, y+0.005*scale, 0);
    glEnd();

    glBegin (GL_POLYGON); 
    glVertex3f(x+0.005*scale, y+0.005*scale, 0);
    glVertex3f(x+0.015*scale, y+0.005*scale, 0);
    glVertex3f(x+0.006*scale, y-0.004*scale, 0);
    glVertex3f(x+0.009*scale, y-0.02*scale, 0);
    glVertex3f(x+0.0, y-0.007*scale, 0);
    glVertex3f(x-0.009*scale, y-0.02*scale, 0);
    glVertex3f(x-0.006*scale, y-0.004*scale, 0);
    glVertex3f(x-0.015*scale, y+0.005*scale, 0);
    glEnd();

    glLoadIdentity();
}

void Star::advance()
{
    x += xShift;
    y += 0.01;
    rotation += 10;
    opacity -= 5;
    if ( opacity <= 0 )
        alive = false;
}