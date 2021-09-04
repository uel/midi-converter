#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Visualization.h"
#include "Particle.h"
#include "Star.h"
#include "Smoke.h"

float position[] = {0, 0.7, 1, 2, 2.6, 3, 3.8, 4, 5, 5.7, 6, 6.7};

float ww = 2/(float)51;
float Note2Left(int note)
{
    return 7*ww*(note/12) + position[note%12]*ww - 1;
}

bool NoteType(int note)
{
    int m = note % 12;
    switch (m)
    {
        case 0:
        case 2:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        return true;
    default:
        return false;
        break;
    }
}

std::shared_ptr<Visualization> Visualization::v;

void Visualization::displayFunc()
{
    v->display();
}

void Visualization::reshape(GLint width, GLint height)
{
    v->windowWidth = width;
    v->windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();            
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0/(width/(GLfloat)height));
}

void Visualization::displayKeyboard()
{
    glClearColor(0.188, 0.188, 0.188, 1);

    glColor3f(0.1, 0.1, 0.1);
    rectangle(2, ww*9, -0.98, -1);

    for (size_t i = 0; i < 88; i++)
        if ( NoteType(i) )
        {
            if ( isActive[i])
                glColor3f(0.61, 0.9, 0.33);
            else
                glColor3f(1, 1, 0.9);
            rectangle(ww, ww*9, -1, Note2Left(i));
        }
    
    for (size_t i = 0; i < 88; i++)
    {
        if ( !NoteType(i) )
        {
            if ( isActive[i])
                glColor3f(0.42, 0.66, 0.18);
            else
                glColor3f(0.1, 0.1, 0.1);
            rectangle(ww*0.6, ww*5, -1+4*ww, Note2Left(i));
        }
    }
}

void Visualization::displayNotes()
{
    for (auto& i : keys)
        i.top-=((frameRateMillis*ups)/1000);

    for (auto i = keys.begin(); i != keys.end() && i->top < (windowHeight/(float)windowWidth);)
    {   
        if ( NoteType(i->note) )
            glColor3f(0.61, 0.9, 0.33);
        else
            glColor3f(0.42, 0.66, 0.18);
        rectangle(i->width, i->height, i->top, i->left, 0.03);
        
        if ( i->top < ww*8-1)
        {
            addKeyParticles(*i);
            isActive[i->note] = true;
        }

        if ( i->top + i->height < ww*8-1 )
        {
            isActive[i->note] = false;
            i = keys.erase(i);
        }
        else
            i++;
    }
}

void Visualization::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();     //Double buffering

    displayNotes();
    displayParticles();
    displayKeyboard();

    glPopMatrix();
    glutSwapBuffers();

    if ( keys.empty() )
        glutDestroyWindow(glutGetWindow());
}

void Visualization::rectangle(float width, float height, float top, float left, float borderWidth)
{
    glBegin(GL_POLYGON);
        glVertex3f(left, top, 0.0);
        glVertex3f(left, top+height, 0.0);
        glVertex3f(left+width, top+height, 0.0);
        glVertex3f(left+width, top, 0.0);
    glEnd();

    glColor3f(0, 0, 0);
    glLineWidth(borderWidth);
    glBegin(GL_LINES);
        glVertex3f(left, top, 0.0);
        glVertex3f(left, top+height, 0.0);
        glVertex3f(left+width, top+height, 0.0);
        glVertex3f(left+width, top, 0.0);
    glEnd();
}

void Visualization::displayParticles()
{
    for (auto &&i : particles)
    {
        Particle* p;
        std::visit([&](auto& v){p = static_cast<Particle*>(&v);}, i);
        if ( p->isAlive() )
        {
            p->advance();
            p->display();
        }
    }
}

void Visualization::addKeyParticles(Key k)
{
    for (size_t i = 0; i < 3; i++)
    {
        particles[particleIndex%maxParticleCount] = Smoke(k.left, ww*8-1+0.06);
        particleIndex++;
    }

    if (!(particleIndex%25))
    {
        particles[particleIndex%maxParticleCount] = Star(k.left, ww*8-1+0.06);
        particleIndex++;
    }
    
    particleIndex = particleIndex%maxParticleCount;
}

void Visualization::timer(int time) {
   glutPostRedisplay();      
   glutTimerFunc(Visualization::v->frameRateMillis, Visualization::timer, 0);
}

void Visualization::initiateParticles()
{
    particles.resize(maxParticleCount);
    for (size_t i = 0; i < maxParticleCount; i++)
        particles[i] = Smoke(); //empty constructor = dead
}

void Visualization::initiateNotes()
{
    std::sort(v->mid.notes.begin(), mid.notes.end(), [](Note& l, Note& r){ return l.begin < r.begin; });

    for (auto i : mid.notes)
    {
        if ( i.note >= 21 && i.note < 88+21 )
        {
            float w = NoteType(i.note-21) ? ww : 0.6*ww;
            Key k(w, i.duration*ups, i.begin*ups, Note2Left(i.note-21), i.note-21);
            keys.push_back(k);
        }
    }
}


bool Visualization::initiateAudio()
{
    if( SDL_Init(SDL_INIT_AUDIO) < 0 )
    {
        return false;
    }

	SDL_AudioSpec spec; 
	spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 2048;       //Aproximate size of buffer in audio_call
	spec.userdata = v->isActive;

    #include "Audio.h"
    spec.callback = audio_callback;
	
	if ( SDL_OpenAudio(&spec, NULL) < 0 )
        return false;
	
	SDL_PauseAudio(0);

    return true;
} 

Visualization::~Visualization()
{
    SDL_PauseAudio(1);
    SDL_CloseAudio();
}

void Visualization::Start(MidiJson m, int argc, char **argv)
{
    v = std::make_shared<Visualization>();
    v->mid = m;
    v->initiateNotes();
    v->initiateParticles();
    if ( !v->initiateAudio())
        std::cout << "Unable to open audio." << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1280, 720);
    std::string windowName = "Playing - " + std::string(argv[2]);
    glutCreateWindow(windowName.c_str());
    glutDisplayFunc(displayFunc);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshape);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glutMainLoop();
}