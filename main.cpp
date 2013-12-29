#include <fstream>
#include <cstdio>
#include <glm/glm.hpp>
#include "include/manager.h"
#define ELASTICITY_WALL .7f
#define ELASTICITY_OBJECTS 1.f
using namespace std;
using namespace manager_space;

glm::vec2 contct;

void draw(manager &manag) {
    manag.map.tree->draw();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec2 a = manag.map.a, b = manag.map.b, c = (a+b)/2.f;
    manag.drawImgCenter(4,c.x,c.y,c.x+50,c.y+50);

    manag.write(test, 0, 0);
    manag.write(manag.ms, W-font_size, 0);
    manag.write(manag.fps, W-font_size, 30);
    float cursor_size = W/40;
    manag.drawImg(1,manag.mx, manag.my, cursor_size, cursor_size);
    for(int i=0;i<manag.map.obj.size();i++){
        object o = *manag.map.obj[i];
        manag.drawImgCenter(o.body.tex_ind, o.p.x, o.p.y, o.body.dim.x* o.body.sc, o.body.dim.y* o.body.sc);
    }
    //manag.drawImgCenter(3, contct.x, contct.y, 10,10);
    SDL_GL_SwapWindow(manag.window);
}

void workPlayer(manager &manag){
    if(manag.actions[0]){
        manag.map.obj[0]->a.y=-1;
    }else
    if(manag.actions[1]){
        manag.map.obj[0]->a.y=1;
    }else{
        manag.map.obj[0]->a.y=0;
    }
    if(manag.actions[2]){
        manag.map.obj[0]->a.x=-1;
    }else
    if(manag.actions[3]){
        manag.map.obj[0]->a.x=1;
    }else{
        manag.map.obj[0]->a.x=0;
    }
}

void checkWorld(object &o, glm::vec2 a, glm::vec2 b, float e){
    if(o.p.x<a.x){
        o.v.x*=-1*e;
        o.p.x=a.x+EPS;
    }else
    if(o.p.x>b.x){
        o.v.x*=-1*e;
        o.p.x=b.x-EPS;
    }
    if(o.p.y<a.y){
        o.v.y*=-1*e;
        o.p.y=a.y+EPS;
    }else
    if(o.p.y>b.y){
        o.v.y*=-1*e;
        o.p.y=b.y-EPS;
    }
}

void workObjects(manager &manag){
    float acc_modulus = 1./5000., time = MS - manag.ms, spd = acc_modulus * time;
    glm::vec2 g = glm::vec2(0,1)*0.0008f;
    g=glm::vec2(0);
    for(int i=0;i<manag.map.obj.size();i++){
        object o = *manag.map.obj[i];
        if(o.movable){
            glm::vec2 v0 = o.v, a = o.a* acc_modulus + g;
            manag.map.obj[i]->v=v0 + (a)*time;
            manag.map.obj[i]->p+=v0 * time + a * time * time / 2.f;
        }
    }
}

void collisionResponse(object &o1, object &o2, glm::vec2 ax, glm::vec2 contct, float e){
    glm::vec2 vr = o2.v - o1.v;
    float j = (-(1 + e) * glm::dot(vr,ax))/
                (1/o1.mass + 1/o2.mass);//impulse
    o1.v-=(float)(j/o1.mass)*ax;
    o2.v+=(float)(j/o2.mass)*ax;
}

void checkPair(object &o1, object &o2){
    glm::vec2 ax, contact;
    float mtv;
    if(o1.check_col(o2, ax, mtv,contact)){
        contct = contact;
        o1.p+=ax*mtv/(float)((o1.mass+o2.mass)/o2.mass);
        o2.p-=ax*mtv/(float)((o1.mass+o2.mass)/o1.mass);
        collisionResponse(o1, o2, ax, contct, ELASTICITY_OBJECTS);
    }
}

void checkObjects(manager &manag){
    if(quad){
        for(int i=0;i<manag.map.obj.size();i++){
            object &o = *manag.map.obj[i];
            if(o.colidable){
                world w = manag.map;
                checkWorld(o, w.a, w.b, ELASTICITY_WALL);
            }
        }
        for(int i=0;i<manag.map.obj.size()-1;i++){
            for(int j=i+1;j<manag.map.obj.size();j++){
                object &o1 = *manag.map.obj[i], &o2 = *manag.map.obj[j];
                if(o1.colidable && o2.colidable){
                    checkPair(o1, o2);
                }
            }
        }
    }else{
        manag.map.tree->check();
    }
}

void work(manager &manag) {
    if(!quad)
    manag.map.removeAllMovableFromTree();
    workPlayer(manag);
    workObjects(manag);
    for(int i=0;i<manag.map.obj.size();i++){
        object &o = *(manag.map.obj[i]);
        o.bound = aabb2(o.body.a*o.body.sc+o.p,o.body.b*o.body.sc+o.p);
    }
    if(!quad)
    manag.map.insertAllMovableToTree();
    checkObjects(manag);
    manag.countFrames();
}

void initGame(manager &manag){
    manag.map.a = glm::vec2(W/4,H/4);
    manag.map.b = glm::vec2(3*W/4,3*H/4);
    manag.map.obj.push_back(new object(glm::vec2(W/2,H/2), glm::vec2(.1,.1), polygon("circle", 30), 30, true, true));
    for(int i=0;i<20;i++){
        int size = 30 + rand()%5;
        string type = "circle";
        if(i%3 == 0) type = "quad";
        if(i%3 == 1) type = "hex";
        //manag.map.obj.push_back(new object(glm::vec2(W/2,H/2), glm::vec2(0,0), polygon("quad", 20), 1, true, true));
        float randx = (rand()%RAND_MAX/(float)RAND_MAX),randy = (rand()%RAND_MAX/(float)RAND_MAX);
        manag.map.obj.push_back(new object(glm::vec2(W/4+randx*(2*W/4),H/4+randy*(2*H/4)), glm::vec2((.5-randx)/100,(.5-randy)/100), polygon(type, size), size, true, true));
    }
    manag.map.initTree(manag.map.a, manag.map.b);
    TOPLEFT = manag.map.a;
    BOTTOMRIGHT = manag.map.b;
    manag.map.insertAllInTree();
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
    output("init quadtree/objects");
    initGame(manag);

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
        int lt = SDL_GetTicks();
        manag.ms = MS - lt + manag.lasttime;
        manag.lasttime = lt;

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

        work(manag);

        draw(manag);

        int ticks = SDL_GetTicks();
        if(ticks - manag.lasttime < MS) {
            SDL_Delay( MS - (ticks - manag.lasttime ));
        }
    }

    end(manag);
    return 0;
}
