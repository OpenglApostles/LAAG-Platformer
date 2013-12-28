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
    dim.x = Max.x - Min.x;
    dim.y = Max.y - Min.y;
}

polygon::polygon(std::string type, double _sc){
    if(type == "quad"){
        p.push_back(glm::vec2(-.5,-.5));
        p.push_back(glm::vec2(.5,-.5));
        p.push_back(glm::vec2(.5,.5));
        p.push_back(glm::vec2(-.5,.5));
        n.push_back(glm::vec2(0,-1));
        n.push_back(glm::vec2(1,0));
        n.push_back(glm::vec2(0,1));
        n.push_back(glm::vec2(-1,0));
        sc = _sc;
        this->computeDim();
    }
}

polygon::~polygon()
{
    //dtor
}
