#ifndef MANAGER_H
#define MANAGER_H
#include "world.h"
#include "manager_space.h"
#include "render_manager.h"
#include "ttf_manager.h"
#include <string>

class manager {
public:
    //FUNCTIONS
    manager();
    void init();
    void initOpengl();
    void end();
    ~manager();
    void windowShow();
    void windowHide();
    void windowMove(int x, int y);
    void windowResize(int x, int y);
    void windowMinimize();
    void windowMaximize();
    void windowRestore();
    void windowMouseEnter();
    void windowMouseLeave();
    void windowFocusGain();
    void windowFocusLost();
    void windowClose();
    void keyboardDown(SDL_Keysym key);
    void keyboardUp(SDL_Keysym key);
    void mouseMove(int x, int y);
    void mouseButtonDown(SDL_MouseButtonEvent e);
    void mouseButtonUp(SDL_MouseButtonEvent e);
    void mouseWheel(int x, int y);
    void initImg();
    void drawImg(int i, float x, float y, float w, float h);
    void drawImgReal(int i, float x, float y);
    void drawImgCenter(int i, float x, float y, float w, float h);
    void countFrames();

    bool runs() {
        return running;
    }
    void write(int s, float x, float y);
    void write(std::string s, float x, float y);
    //VARIABLES
    double ms;
    int lasttime, last_sec, frames, fps;
    render_manager gl;
    ttf_manager ttf_manag;
    SDL_Window *window;
    SDL_GLContext context;
    int mx, my;
    bool actions[4];
    world map;
};

extern bool quad;

#endif // MANAGER_H
