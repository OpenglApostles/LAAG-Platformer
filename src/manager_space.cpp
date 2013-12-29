#include "../include/manager_space.h"
#include <cstdlib>
#include <stdio.h>
#include <SDL2/SDL_image.h>

tex::tex() {
}

tex::tex(GLuint _id, int _w, int _h, int _col) {
    id = _id;
    w = _w;
    h = _h;
    col = _col;
}

void tex::end() {
    glDeleteTextures(1, &id);
}

namespace manager_space {
///font stuff{
std::string test="test";
int font_size = 36;
const char *font_name="data/fonts/UbuntuMono-R.ttf";
///}
///imgs{
std::string IMG_PATH = "data/img/";
///}
std::string SHADERS_PATH = "data/shaders/";
glm::vec2 TOPLEFT, BOTTOMRIGHT;
unsigned int W = 800 , H = 600;
char* GAME_TITLE= {"Test"};
float FPS = 60, MS = 1000/FPS, EPS = 1e-10;

bool running = true, fullscreen = false;
GLuint FONT_SHADER =1, IMG_SHADER=2;

void output(const char* c) {
    printf("%s\n", c);
}

GLuint surf2tex(SDL_Surface *surf) {
    int colors = surf->format->BytesPerPixel , texture_format = GL_RGB;
    if (colors == 4) {   // alpha
        if (surf->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    } else {             // no alpha
        if (surf->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, colors, surf->w, surf->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surf->pixels);

    return texture;
}

void sdlDie() {
    output(SDL_GetError());
    SDL_Quit();
    exit(1);
}
}
