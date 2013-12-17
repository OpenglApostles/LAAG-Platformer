#include <fstream>
#include <cstdio>
#include <glm/glm.hpp>
#include "include/manager.h"

using namespace std;
using namespace manager_space;

void draw(manager &manag) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    manag.write(manager_space::test, 0, 0);
    int time = SDL_GetTicks() - manag.lasttime;
    manag.write(manager_space::FPS - time, W-font_size, 0);
    manag.write(manag.fps, W-font_size, 30);
    for(int i=0;i<50;i++)
    manag.drawImg(0,manag.mx, manag.my);
    SDL_GL_SwapWindow(manag.window);
}

void work(manager &manag) {
    manag.countFrames();
}

void init(manager &manag) {
    output("init sdl");
    if(SDL_Init( SDL_INIT_EVERYTHING )<0) {
        sdlDie();
    }

    manag.init();

    if (manag.window == NULL) {
        sdlDie();
    }

    output("init opengl");
    manag.initOpengl();

    output("init images");
    manag.initImg();

    output("init shaders");
    manag.gl.init();

    output("init ttf fonts");
    manag.ttf_manag.make(font_name, font_size, 32, 126);

    SDL_DisableScreenSaver();
    glClearColor(.1, .2, .3, 1);
}

void end(manager &manag) {
    manag.end();
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char **argv) {
    manager manag;
    init(manag);
    SDL_Event event;


    while(manag.runs()) {
        manag.lasttime = SDL_GetTicks();

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_SHOWN:
                    manag.windowShow();
                    break;
                case SDL_WINDOWEVENT_HIDDEN:
                    manag.windowHide();
                    break;
                case SDL_WINDOWEVENT_MOVED:
                    manag.windowMove(event.window.data1,event.window.data2);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    manag.windowResize(event.window.data1, event.window.data2);
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    manag.windowMinimize();
                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:
                    manag.windowMaximize();
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    manag.windowRestore();
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    manag.windowMouseEnter();
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    manag.windowMouseLeave();
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    manag.windowFocusGain();
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    manag.windowFocusLost();
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    manag.windowClose();
                    break;
                default:
                    break;
                }
                break;
            }

            case SDL_KEYDOWN: {
                manag.keyboardDown(event.key.keysym);
                break;
            }

            case SDL_KEYUP: {
                manag.keyboardUp(event.key.keysym);
                break;
            }

            case SDL_MOUSEMOTION: {
                manag.mouseMove(event.motion.x, event.motion.y);
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                manag.mouseMove(event.motion.x, event.motion.y);
                manag.mouseButtonDown(event.button);
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                manag.mouseMove(event.motion.x, event.motion.y);
                manag.mouseButtonUp(event.button);
                break;
            }

            case SDL_MOUSEWHEEL: {
                manag.mouseWheel(event.motion.x, event.motion.y);
                break;
            }

            }
        }

        draw(manag);

        work(manag);
    }

    end(manag);
    return 0;
}
