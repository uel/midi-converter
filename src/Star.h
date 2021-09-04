#pragma once
#include "Particle.h"
#include <GL/gl.h>

///A fast moving star particle
class Star : public Particle
{
    GLbyte opacity;     ///< Alpha of star.
    GLfloat scale;      ///< Scale of the star.
    GLfloat xShift;     ///< Movement speed in x direction.
    GLfloat rotation;   ///< Rotation in degrees ?

    public:
    Star(GLfloat x, GLfloat y);

    /** Displays the particle using GL. */
    void display() const override;

    /** Moves and rotates the star. */
    void advance() override;
};