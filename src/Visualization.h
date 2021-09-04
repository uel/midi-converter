#pragma once
#include "MidiJson.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <list>
#include <variant>
#include <memory>
#include "Star.h"
#include "Smoke.h"

///Generates visualizations with OpenGL from a Midi object
class Visualization
{
    static std::shared_ptr<Visualization> v;    ///< Main object of class

    ///A representation of a key/note on the screen, all units are in gl units.
    struct Key
    {
        float width;
        float height;
        float top;
        float left;
        uint8_t note;  ///< The note number
        Key(float width, float height, float top, float left, uint8_t note)
        : width(width), height(height), top(top), left(left), note(note) {}
    };

    MidiJson mid;           ///< The MidiJson object that is played.
    std::list<Key> keys;    ///< All key objects ordered by start.
    int windowWidth;        ///< Window height updated by reshape.
    int windowHeight;       ///< Window width updated by reshape.
    bool isActive[88];      ///< Indicates which key is active.
    const float ups = 0.5;  ///< Number of units a key should travel each seconds, this will change the size of the keys, tempo remains the same.
    const size_t frameRateMillis = 25;    ///< Number of milliseconds between redisplays.
    const size_t maxParticleCount = 2000;   ///< Maximum number of particles to be displayed.
    size_t particleIndex = 0;   ///< Next particle to be over written.
    std::vector<std::variant<Smoke, Star>> particles;   ///< Continuous vector of particle variants.

    /** Calculates a new projection on window size change.*/
    static void reshape(GLint width, GLint height);

    /** Timer function that will be called by GLUT.
     * calls display every "time" milliseconds in a loop. */
    static void timer(int time);

    /** C compatible display function, calls v->display. */
    static void displayFunc();

    /** Main display function to be called by glut. */
    void display();

    /** Displays the keyboard with activated notes. */
    void displayKeyboard();

    /** Displays notes contained in activeKeys. */
    void displayNotes();

    /** Fills the variant vector with "maxParticleCount" dead particles. */
    void initiateParticles();

    /** Calls display and advance on every living particle in "particles". */
    void displayParticles();

    /** Generate new particles that corespond to key "k". */
    void addKeyParticles(Key k);

    /** Tries to open an audio device with audio_callback, return false if it fails.
     *  Visualization will continue without sound if no device available. */
    bool initiateAudio();

    /** Fills up "keys" from mid. Calculates the note positions. */
    void initiateNotes();

    /** Displays a bordered rectangle with the parameters. */
    void rectangle(float width, float height, float top, float left, float borderWidth = 0);

    public:
    /** Closes the audio stream. */
    ~Visualization();

    /** Starts the visualization of "m". */
    static void Start(MidiJson m, int argc, char **argv);
};