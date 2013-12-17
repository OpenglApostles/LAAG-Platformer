#include "../include/ttf_manager.h"
#include <cmath>
#include <string>

glyph_metric::glyph_metric(){
}

ttf_manager::ttf_manager() {
    //ctor
}

ttf_manager::~ttf_manager() {
    //dtor
}

void ttf_manager::init() {
    if(!TTF_WasInit() && TTF_Init()==-1) {
        output(TTF_GetError());
        sdlDie();
    }
    c.setMat(2);
}

void ttf_manager::end() {
    font.end();
}

void ttf_manager::make(const char *name, int size, int a, int b) {
    TTF_Font *ttf_font=TTF_OpenFont(name, size);
    if(!ttf_font) {
        output(TTF_GetError());
    }
    std::string s;
    for(int i=a; i<=b; i++) {
        s+=char(i);
    }
    SDL_Surface *msg = TTF_RenderText_Blended(ttf_font, s.c_str(), {255, 255, 255});
    font = tex(surf2tex(msg),msg->w, msg->h, GL_BGR);

    //TTF_SetFontStyle()

    int tmp;
    metrics.assign(b-a+1, glyph_metric());
    //compute for integer
    float tot =0;
    for(int i=a; i<=b; i++) {
        int w= 0;
        TTF_GlyphMetrics(ttf_font,char(i), &tmp, &tmp, &tmp, &tmp, &w);
        if(i-a==0)
            metrics[i-a].w=w;
        else
            metrics[i-a].w=w+metrics[i-a-1].w;
        metrics[i-a].c=i;
        tot+=w;
    }
    //TTF_FONt
    //divide tex location
    for(int i=a; i<=b; i++) {
            metrics[i-a].wt = (float)metrics[i-a].w/tot;
    }

    r.basicPlane();

    SDL_FreeSurface(msg);
    TTF_CloseFont(ttf_font);
}

void ttf_manager::write_let(int w, int h, float xt1, float xt2) {
    ///
    glBindBufferARB(GL_ARRAY_BUFFER, r.vbo);
    float *ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if(ptr){
        ptr[2]=w;
        ptr[5]=h;
        ptr[6]=w;
        ptr[7]=h;
        glUnmapBufferARB(GL_ARRAY_BUFFER);
    }
    ///
    ///
    glBindBufferARB(GL_ARRAY_BUFFER, r.tbo);
    ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if(ptr){
        ptr[0]=xt1;
        ptr[2]=xt2;
        ptr[4]=xt1;
        ptr[6]=xt2;
        glUnmapBufferARB(GL_ARRAY_BUFFER);
    }
    ///
    glBindVertexArray(r.vao);
    glDrawArrays( GL_TRIANGLE_STRIP, 0,4);
    glBindVertexArray(0);
}

void ttf_manager::drawImg(int w, int h) {
    ///
    glBindBufferARB(GL_ARRAY_BUFFER, r.vbo);
    float *ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if(ptr){
        ptr[2]=w;
        ptr[5]=h;
        ptr[6]=w;
        ptr[7]=h;
        glUnmapBufferARB(GL_ARRAY_BUFFER);
    }
    ///
    ///
    glBindBufferARB(GL_ARRAY_BUFFER, r.tbo);
    ptr = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if(ptr){
        ptr[0]=0;
        ptr[2]=1;
        ptr[4]=0;
        ptr[6]=1;
        glUnmapBufferARB(GL_ARRAY_BUFFER);
    }
    ///
    glBindVertexArray(r.vao);
    glDrawArrays( GL_TRIANGLE_STRIP, 0,4);
    glBindVertexArray(0);
}
