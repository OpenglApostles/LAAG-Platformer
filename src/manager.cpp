#include "..\include\manager.h"

#include <cstdlib>
#include <sstream>
#include <SDL2/SDL_image.h>

using namespace manager_space;

manager::manager() {
}

manager::~manager() {
    SDL_DestroyWindow(window);
}

void manager::initImg() {
    gl.loadPNG("data/img/player.png");
    gl.loadPNG("data/img/mouse.png");
    gl.loadPNG("data/img/logo.png");
    gl.loadPNG("data/img/ball.png");
    gl.loadPNG("data/img/wall.png");
    //gl.loadPNG("data/img/test.png");
}

void manager::drawImg(int i, float x, float y, float w, float h) {
    if(gl.in_use != IMG_SHADER) {
        gl.useProg(IMG_SHADER);
    }
    gl.putTex(gl.texture[i].id, 0, "tex");
    gl.c.view = glm::translate(glm::mat4(1), glm::vec3(x , y, 0));
    gl.c.model = glm::scale(glm::mat4(1), glm::vec3(w,h,1));
    gl.c.calcMat();
    glUniformMatrix4fv(gl.getUniform("mvp"), 1, GL_FALSE, &gl.c.mvp[0][0]);
    gl.drawImg(w, h);
}

void manager::drawImgCenter(int i, float x, float y, float w, float h) {
    if(gl.in_use != IMG_SHADER) {
        gl.useProg(IMG_SHADER);
    }
    gl.putTex(gl.texture[i].id, 0, "tex");
    x-=w/2;
    y-=h/2;
    gl.c.view = glm::translate(glm::mat4(1), glm::vec3(x , y, 0));
    gl.c.model = glm::scale(glm::mat4(1), glm::vec3(w,h,1));
    gl.c.calcMat();
    glUniformMatrix4fv(gl.getUniform("mvp"), 1, GL_FALSE, &gl.c.mvp[0][0]);
    gl.drawImg(w, h);
}

void manager::drawImgReal(int i, float x, float y) {
    if(gl.in_use != IMG_SHADER) {
        gl.useProg(IMG_SHADER);
    }
    gl.putTex(gl.texture[i].id, 0, "tex");
    int w = gl.texture[i].w, h = gl.texture[i].h;
    gl.c.view = glm::translate(glm::mat4(1), glm::vec3(x , y, 0));
    gl.c.model = glm::scale(glm::mat4(1), glm::vec3(w,h,1));
    gl.c.calcMat();
    glUniformMatrix4fv(gl.getUniform("mvp"), 1, GL_FALSE, &gl.c.mvp[0][0]);
    gl.drawImg(w, h);
}

void setIcon(SDL_Window *window){
    SDL_Surface *surface;
    surface = IMG_Load("data/img/logo.png");
    SDL_SetWindowIcon(window, surface);

    SDL_FreeSurface(surface);
}

void manager::init() {
    window = SDL_CreateWindow(
                 GAME_TITLE,
                 SDL_WINDOWPOS_CENTERED,
                 SDL_WINDOWPOS_CENTERED,
                 W,
                 H,
                 SDL_WINDOW_OPENGL |SDL_WINDOW_RESIZABLE
             );
    setIcon(window);
    SDL_ShowCursor(0);
    ttf_manag.init();
    for(int i=0;i<4;i++){
        actions[i]=false;
    }
}

void manager::initOpengl() {
    context = SDL_GL_CreateContext(window);
    if(context == NULL) {
        sdlDie();
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(1);

    glewExperimental = true,glewInit();
    glEnable(GL_TEXTURE);
    glEnable(GL_BLEND);
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void manager::end() {
    ttf_manag.end();
    gl.end();
    SDL_GL_DeleteContext(context);
    if(window != NULL)
        SDL_DestroyWindow(window);
}

void manager::write(std::string s, float x, float y) {
    if(gl.in_use != FONT_SHADER) {
        gl.useProg(FONT_SHADER);
    }
    gl.putTex(ttf_manag.font.id, 0, "text");

    float prec=0;
    for(int i=0; i<s.size(); i++) {
        int loc = int(s[i])-32;
        int x1, x2;
        float xt1, xt2;
        //position
        if(loc == 0)
            x1 = 0;
        else
            x1 = ttf_manag.metrics[loc-1].w;
        x2 = ttf_manag.metrics[loc].w;
        //texture
        if(loc == 0)
            xt1 = 0;
        else
            xt1 = ttf_manag.metrics[loc-1].wt;
        xt2 = ttf_manag.metrics[loc].wt;

        ttf_manag.c.view = glm::translate(glm::mat4(1), glm::vec3(x + prec, y, 0));
        ttf_manag.c.calcMat();
        glUniformMatrix4fv(gl.getUniform("mvp"), 1, GL_FALSE, &ttf_manag.c.mvp[0][0]);
        prec+=x2-x1;

        ttf_manag.write_let(x2-x1, ttf_manag.font.h, xt1, xt2);
    }
}

void manager::write(int s, float x, float y) {
    std::stringstream stream;
    string ss;
    stream<<s;
    stream>>ss;
    write(ss, x, y);
}

void manager::countFrames() {
    if(SDL_GetTicks() - last_sec >=1000) {
        last_sec = SDL_GetTicks();
        fps = frames;
        frames = 0;
    }
    frames++;
}

//SDL
//{

void manager::windowShow() {
}

void manager::windowHide() {
}

void manager::windowMove(int x, int y) {
}

void manager::windowResize(int x, int y) {
}

void manager::windowMinimize() {
}

void manager::windowMaximize() {
}

void manager::windowRestore() {
}

void manager::windowMouseEnter() {
}

void manager::windowMouseLeave() {
}

void manager::windowFocusGain() {
}

void manager::windowFocusLost() {
}

void manager::windowClose() {
    running = false;
}

void manager::keyboardDown(SDL_Keysym key) {
    //manager_space::test+=key.sym;
    switch(key.sym) {
    case SDLK_ESCAPE: {
        running = false;
        break;
    }
    case SDLK_f: {
        fullscreen = !fullscreen;
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(0, &current);
        int screenWidth = current.w ,screenHeight = current.h;
        SDL_SetWindowSize(window, screenWidth, screenHeight);
        SDL_SetWindowFullscreen(window,fullscreen?SDL_WINDOW_FULLSCREEN:SDL_WINDOW_FULLSCREEN_DESKTOP);
        W = screenWidth;
        H = screenHeight;
        gl.c.setMat(2);
        break;
    }
    case SDLK_w:{
        actions[0] = true;
        break;
    }
    case SDLK_s:{
        actions[1] = true;
        break;
    }
    case SDLK_a:{
        actions[2] = true;
        break;
    }
    case SDLK_d:{
        actions[3] = true;
        break;
    }
    case SDLK_b:{
        if(FPS == 10){
            FPS = 60;
        }else FPS = 10;
        MS = 1000/FPS;
        break;
    }
    default:break;
    }
}

void manager::keyboardUp(SDL_Keysym key) {
    switch(key.sym) {
    case SDLK_ESCAPE: {
        running = false;
        break;
    }
    case SDLK_w:{
        actions[0] = false;
        break;
    }
    case SDLK_s:{
        actions[1] = false;
        break;
    }
    case SDLK_a:{
        actions[2] = false;
        break;
    }
    case SDLK_d:{
        actions[3] = false;
        break;
    }
    }
}

void manager::mouseMove(int x, int y) {
    mx = x;
    my = y;
}

void manager::mouseButtonDown(SDL_MouseButtonEvent e) {
    switch(e.button) {
    case SDL_BUTTON_LEFT: {
        break;
    }

    case SDL_BUTTON_MIDDLE: {
        break;
    }

    case SDL_BUTTON_RIGHT: {
        break;
    }

    case SDL_BUTTON_X1: {
        break;
    }

    case SDL_BUTTON_X2: {
        break;
    }
    }
}

void manager::mouseButtonUp(SDL_MouseButtonEvent e) {
    switch(e.button) {
    case SDL_BUTTON_LEFT: {
        break;
    }

    case SDL_BUTTON_MIDDLE: {
        break;
    }

    case SDL_BUTTON_RIGHT: {
        break;
    }

    case SDL_BUTTON_X1: {
        break;
    }

    case SDL_BUTTON_X2: {
        break;
    }
    }
}

// x negative left - positive right
// y negative up - positive down
void manager::mouseWheel(int x, int y) {
}

//}
