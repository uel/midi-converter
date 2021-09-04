#pragma once
#include <GL/gl.h>
#include <random>

///A virtual class of any particle with a position, to be simulated and displayed.
class Particle
{
    protected:
    GLfloat x, y;   ///< Position of particle in GL units
    bool alive = false; ///< State of particle
    Particle(){};   
    Particle(GLfloat x, GLfloat y) : x(x), y(y), alive(true) {};

    /** Returns a random float between 0 and 1. */
    static float randFloat() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }

    public:
    /** Displays the particle using GL. */
    virtual void display() const = 0;

    /** Advances the simulation of the particle. */
    virtual void advance() = 0;

    bool isAlive() const { return alive; };
};