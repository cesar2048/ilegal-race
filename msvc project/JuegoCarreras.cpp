// JuegoCarreras.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "JuegoCarreras.h"
#include <SDL.h>
#include <GL/glew.h>

/*
enum class State {
    RUNNING = 1,
    EXIT = 0
};

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Hello", 100, 100, 1080, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    SDL_GLContext glContext = SDL_GL_CreateContext(win);
    
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        char* msg = (char*) glewGetErrorString(error);
        MessageBoxA(NULL, msg, "Error caption", MB_ICONERROR);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    int width = 1080;
    int height = 720;
    float fieldOfView = 45.0f;
    // The following code is a fancy bit of math that is eqivilant to calling:
    // gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
    // We do it this way simply to avoid requiring glu.h
    GLfloat zNear = 0.1f;
    GLfloat zFar = 255.0f;
    GLfloat aspect = float(width) / float(height);
    GLfloat fH = tan(fieldOfView / 360.0 * 3.14159) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    int phase = 0;

    State s = State::RUNNING;
    while (s != State::EXIT) {

        // process all pending events
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_KEYDOWN) {
                if (evt.key.keysym.sym == SDLK_ESCAPE) {
                    s = State::EXIT;
                }
            } else if (evt.type == SDL_QUIT) {
                s = State::EXIT;
            }
        }

        // render
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableClientState(GL_COLOR_ARRAY);

        glBegin(GL_TRIANGLES);
        glColor3f(0.2f, 0.9f, 0.3f);

        float z = -100.0f + sin(6.28 / 100 * (phase++)) * 50;

        glVertex3f( -1.0f,   1.0f, z);
        glVertex3f( 10.0f, -10.0f, z);
        glVertex3f( 10.0f,   1.0f, z);

        glEnd();
        
        SDL_GL_SwapWindow(win);
    }

    return 0;
}
*/