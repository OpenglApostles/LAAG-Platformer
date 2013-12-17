#ifndef TTF_MANAGER_H
#define TTF_MANAGER_H
#include "manager_space.h"
#include "render_manager.h"
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <glm/glm.hpp>

using namespace manager_space;

class glyph_metric {
    public:
    float wt;
    int w;
    char c;

    glyph_metric();
};

class ttf_manager {
public:
    ttf_manager();
    virtual ~ttf_manager();
    void end();
    void init();
    void make(const char *name, int size, int a, int b);
    void write_let(int w, int h,float xt1, float yt1);
    void drawImg(int w, int h);
    camera c;
    int height;
    std::vector<glyph_metric> metrics;
    tex font;
    render_obj r;
};

#endif // TTF_MANAGER_H
