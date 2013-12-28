#include "../include/render_manager.h"
#include <fstream>
#include <SDL2/SDL_image.h>

using namespace manager_space;

void camera::setMat(int dim){
    if(dim == 2){
        proj = glm::ortho(0.f, (float)W, (float)H, 0.f);
        view = glm::mat4(1);
        model = glm::mat4(1);
    }
    calcMat();
}

void camera::calcMat(){
    mvp = proj * view * model;
}

camera::camera(int dim){
    setMat(dim);
}

camera::camera(){
    setMat(2);
}

render_obj::render_obj() {
    hasT = false;
    hasV = false;
    hasN = false;
}
render_obj::render_obj(GLuint _vao,GLuint _vbo) {
    hasT = false;
    hasV = false;
    hasN = false;
    vao = _vao;
    vbo = _vbo;
    hasV = true;
}
render_obj::render_obj(GLuint _vao,GLuint _vbo,GLuint _tbo) {
    hasT = false;
    hasV = false;
    hasN = false;
    vao = _vao;
    vbo = _vbo;
    hasV = true;
    hasT = true;
    tbo = _tbo;
}
render_obj::render_obj(GLuint _vao,GLuint _vbo,GLuint _tbo,GLuint _nbo) {
    hasT = false;
    hasV = false;
    hasN = false;
    vao = _vao;
    vbo = _vbo;
    hasV = true;
    hasT = true;
    tbo = _tbo;
    hasN = true;
    nbo = _nbo;
}

void render_obj::basicPlane(){
    GLfloat quad_tris[]= {
        0, 0,
        1, 0,
        0, 1,
        1, 1
    };

    GLfloat texdata[]= {
        0, 0,
        1, 0,
        0, 1,
        1, 1
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), quad_tris, GL_STATIC_DRAW);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), texdata, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer( GL_ARRAY_BUFFER, tbo );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

GLuint render_manager::getUniform(const char *s) {
    GLuint prog = getProg(in_use);
    return glGetUniformLocation(prog ,s);
}

void render_manager::drawImg(int w, int h) {

    glBindVertexArray(r.vao);
    glDrawArrays( GL_TRIANGLE_STRIP, 0,4);
    glBindVertexArray(0);
}

void render_obj::end() {
    glDeleteVertexArrays(1, &vao);
    if(hasV)
        glDeleteBuffers(1, &vbo);
    if(hasT)
        glDeleteBuffers(1, &tbo);
    if(hasN)
        glDeleteBuffers(1, &nbo);
}

void render_manager::putTex(GLuint t_id, int where, const char *var_name) {
    if(tex_in_use != t_id){
        tex_in_use = t_id;
        glActiveTexture(GL_TEXTURE0 + where);
        glBindTexture(GL_TEXTURE_2D, t_id);
        GLuint textureId  = glGetUniformLocation(getProg(in_use), var_name);
        glUniform1i(textureId, 0);
    }
}

render_manager::render_manager() {
    in_use = -1;
}

render_manager::~render_manager() {
    //dtor
}

void render_manager::end() {
    for(int i=0; i<prog.size(); i++) {
        glDeleteProgram(prog[i]);
    }
    prog.clear();
    for(int i=0;i<texture.size();i++){
        texture[i].end();
    }
    texture.clear();
    IMG_Quit();
}

void render_manager::loadPNG(const char *name){
    SDL_Surface *img = IMG_Load(name);
    GLuint img_id = surf2tex(img);
    texture.push_back(tex(img_id,img->w,img->h,img->format->BytesPerPixel));
    SDL_FreeSurface(img);
}

void render_manager::deletePNG(int ind){
    texture[ind].end();
    texture.erase(texture.begin() + ind);
}

void render_manager::loadShader(const char *vertexpath, const char *fragmentpath) {
    GLuint vertexid = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentid = glCreateShader(GL_FRAGMENT_SHADER);

    string vertexcode, fragmentcode, line;

    ifstream stream;
    stream.open(vertexpath);
    for(; getline(stream, line); vertexcode += "\n" + line);
    stream.close();

    stream.open(fragmentpath);
    for(; getline(stream, line); fragmentcode += "\n" + line);
    stream.close();

    GLint ret = GL_FALSE;
    int loglen = 0;
    GLchar *log;

    const char *vertexcodeptr = vertexcode.c_str();
    glShaderSource(vertexid, 1, &vertexcodeptr, NULL);
    glCompileShader(vertexid);

    glGetShaderiv(vertexid, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(vertexid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen) {
        output("error : vert prog");
        log = new GLchar[loglen];
        glGetShaderInfoLog(vertexid, loglen, NULL, log);
        output(log);
        delete[] log;
    }

    const char *fragmentcodeptr = fragmentcode.c_str();
    glShaderSource(fragmentid, 1, &fragmentcodeptr, NULL);
    glCompileShader(fragmentid);

    loglen = 0;

    glGetShaderiv(fragmentid, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(fragmentid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen) {
        output("error : frag prog");
        log = new GLchar[loglen];
        glGetShaderInfoLog(fragmentid, loglen, NULL, log);
        output(log);
        delete[] log;
    }

    GLuint programid = glCreateProgram();
    glAttachShader(programid, vertexid);
    glAttachShader(programid, fragmentid);
    glLinkProgram(programid);

    loglen = 0;

    glGetProgramiv(programid, GL_LINK_STATUS, &ret);
    glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &loglen);

    if(loglen) {
        output("error : whole shader prog");
        log = new GLchar[loglen];
        glGetProgramInfoLog(programid, loglen, 0, log);
        output(log);
        delete[] log;
    }

    glDeleteShader(vertexid);
    glDeleteShader(fragmentid);

    prog.push_back(programid);
}

void render_manager::useProg(int index) {
    if(in_use != index) {
        glUseProgram(prog[index]);
        in_use = index;
    }
}

void render_manager::init(){
    string path = SHADERS_PATH;
    this->loadShader((path + "simple.vert").c_str(), (path + "simple.frag").c_str());
    this->loadShader((path + "2dtext.vert").c_str(), (path + "2dtext.frag").c_str());
    this->loadShader((path + "2dimg.vert").c_str(), (path + "2dimg.frag").c_str());
    output("init image loader");
    if(!IMG_Init(IMG_INIT_PNG)){
        output(IMG_GetError());
    }
    c.setMat(2);
    r.basicPlane();
}

GLuint render_manager::getProg(int index) {
    return prog[index];
}
