#pragma once
#include "Particle.h"
#include <GL/gl.h>

///A Smoke particle displayed in GL
class Smoke : public Particle
{
    GLbyte color[4];    ///< RGBA color in bytes
    GLfloat radius;     ///< Channel of the midi event, 4 bits.
    GLfloat xState;     ///< Inner state counter for advance function
    GLfloat xShift;     ///< Speed in x direction

    public:
    Smoke(){};
    Smoke(GLfloat x, GLfloat y);

    /** Displays the particle using GL. */
    void display() const override;

    /** Calculates the position and alpha. */
    void advance() override;
};