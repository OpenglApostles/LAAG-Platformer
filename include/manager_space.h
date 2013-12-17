#ifndef MANAGER_SPACE_H
#define MANAGER_SPACE_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

class tex{
    public :
    int col;
    int w, h;
    GLuint id;
    tex();
    tex(GLuint id, int w, int h, int col);
    void end();
};

namespace manager_space{
    extern std::string test, SHADERS_PATH;
    extern unsigned int W, H ;
    extern char* GAME_TITLE;
    extern int FPS, font_size, inv_font_size;
    extern bool running, fullscreen;
    extern const char *font_name;
    extern GLuint FONT_SHADER, IMG_SHADER;

    void output(const char* c);

    GLuint surf2tex(SDL_Surface *surf);

    void sdlDie();
}

#endif // MANAGER_SPACE_H
