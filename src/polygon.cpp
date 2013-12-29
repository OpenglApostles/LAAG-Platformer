#include "../include/polygon.h"

polygon::polygon()
{
    //ctor
}

void polygon::computeDim(){
    glm::vec2 Min(p[0]), Max(Min);
    for(int i=0;i<p.size();i++){
        if(p[i].x < Min.x)Min.x = p[i].x;
        if(p[i].y < Min.y)Min.y = p[i].y;
        if(p[i].x > Max.x)Max.x = p[i].x;
        if(p[i].y > Max.y)Max.y = p[i].y;
    }
    a = Min;
    b = Max;
    dim.x = Max.x - Min.x;
    dim.y = Max.y - Min.y;
}

std::vector<glm::vec2> computeNormals(std::vector<glm::vec2> p){
    std::vector<glm::vec2> n;
    for(int i=0;i<p.size();i++){
        n.push_back(p[i]-p[(i+1)%p.size()]);
        float aux = n[i].x;
        n[i].x = -n[i].y;
        n[i].y = aux;
        n[i] = glm::normalize(n[i]);
    }
    return n;
}

polygon::polygon(std::string type, float _sc){
    if(type == "quad"){
        p.push_back(glm::vec2(-.5,-.5));
        p.push_back(glm::vec2(.5,-.5));
        p.push_back(glm::vec2(.5,.5));
        p.push_back(glm::vec2(-.5,.5));
        n= computeNormals(p);
        sc = _sc;
        this->computeDim();
        tex_ind = 5;
    }else
    if(type == "hex"){
        float one= .166666666667, five=.5;
        p.push_back(glm::vec2(-one,-five));
        p.push_back(glm::vec2(one,-five));
        p.push_back(glm::vec2(five,-one));
        p.push_back(glm::vec2(five,one));
        p.push_back(glm::vec2(one,five));
        p.push_back(glm::vec2(-one,five));
        p.push_back(glm::vec2(-five,one));
        p.push_back(glm::vec2(-five,-one));
        n= computeNormals(p);
        sc = _sc;
        this->computeDim();
        tex_ind = 6;
    }else
    if(type == "circle"){
        p.push_back(glm::vec2(.5,0));
        sc = _sc;
        a = glm::vec2(-.5,-.5);
        b = glm::vec2(.5,.5);
        dim.x = b.x - a.x;
        dim.y = b.y - a.y;
        tex_ind = 3;
    }
}

polygon::~polygon()
{
    //dtor
}
