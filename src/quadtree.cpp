#include "../include/quadtree.h"
#include <iostream>
#include "../include/manager_space.h"

void quadtree::kill() {
    count = 0;
    if(!obj.empty())
        obj.clear();
    if(has) {
        for(int i=0; i<4; i++) {
            children[i]->kill();
            delete children[i];
        }
    }
    has = false;
}

quadtree::quadtree() {
    count=0;
}

quadtree::quadtree(aabb2 b, int d) {
    has=0;
    boundary = b;
    depth = d;
    count=0;
}

std::vector<short> quadtree::whereTo(aabb2 p) {
    std::vector<short> rez;
    glm::vec2 a = p.a, b = p.b, c=boundary.c;
    if(a.x< c.x) {
        if(a.y< c.y)
            rez.push_back(0);
        if(b.y>= c.y)
            rez.push_back(2);
    }
    if(b.x>= c.x) {
        if(a.y< c.y)
            rez.push_back(1);
        if(b.y>= c.y)
            rez.push_back(3);
    }
    return rez;
}

void quadtree::insert(object *a ,aabb2 p) {
    count++;
    if(!has && count > MAX_NODE_CAPACITY && depth< MAX_DEPTH) {
        obj.push_back(a);
        subdivide();
        return;
    }
    if(has) {
        std::vector<short> rez = whereTo(p);
        for(int i=0; i<rez.size(); i++) {
            children[rez[i]]->insert(a, p);
        }
    } else {
        obj.push_back(a);
    }
}

void quadtree::subdivide() {
    has=true;
    children[0]=new quadtree(aabb2(boundary.a,boundary.c),depth+1);
    children[1]=new quadtree(aabb2(glm::vec2(boundary.c.x,boundary.a.y),glm::vec2(boundary.b.x,boundary.c.y)),depth+1);
    children[2]=new quadtree(aabb2(glm::vec2(boundary.a.x,boundary.c.y),glm::vec2(boundary.c.x,boundary.b.y)),depth+1);
    children[3]=new quadtree(aabb2(boundary.c,boundary.b),depth+1);
    for(int i=0; i<obj.size(); i++) {
        std::vector<short> rez = whereTo(obj[i]->bound);
        for(int j=0; j<rez.size(); j++) {
            children[rez[j]]->insert(obj[i],obj[i]->bound);
        }
    }
    obj.clear();
}

void quadtree::queryRange(aabb2 range) {
}

void quadtree::collect(std::vector<object*> &here) {
    int n = here.size();
    for(int i=0; i<obj.size(); i++) {
        bool present = false;
        for(int j=0; j<n; j++) {
            if(here[j] == obj[i]) {
                present = true;
                obj.erase(obj.begin()+i);
                break;
            }
        }
        if(!present)
            here.push_back(obj[i]);
    }
    obj.clear();
}

void quadtree::remove(object*  a, aabb2 rem) {
    count--;
    if(has && count < MIN_NODE_CAPACITY) {
        kill();
    }
    if(has) {
        std::vector<short> rez = whereTo(rem);
        for(int i=0; i<rez.size(); i++) {
            children[rez[i]]->remove(a, rem);
        }
    } else {
        for(int i=0; i<obj.size(); i++) {
            if(obj[i]==a) {
                obj.erase(obj.begin() + i);
                return;
            }
        }
    }
}

void quadtree::checkObj() {
    if (has && count) {
        for(int i=0; i<4; i++)
            if(children[i]->count)
                children[i]->checkObj();
    } else if(count) {
        for(int i=0; i<obj.size()-1; i++) {
            for(int j=i+1; j<obj.size(); j++) {
                checkPair(*obj[i],*obj[j]);
            }
        }
    }
}

void checkWorld(object &o, glm::vec2 a, glm::vec2 b, float e);

void quadtree::checkWall(int w) {
    if (has && count) {
        for(int i=0;i<4;i++){
            if(children[i]->count)
            children[i]->checkWall(0);
        }
    } else if(count) {
        for(int i=0; i<obj.size(); i++) {
            checkWorld(*obj[i],manager_space::TOPLEFT, manager_space::BOTTOMRIGHT,.8);
        }
    }
}

void quadtree::check() {
    checkWall(-1);
    checkObj();
}

void quadtree::draw() {
    glm::vec2 a= glm::vec2(boundary.a.x+1,boundary.a.y+1), b=glm::vec2(boundary.b.x-1,boundary.b.y-1);
    aabb2 box(a,b);
    for(int i=0; i<obj.size(); i++) {
        aabb2 jt = obj[i]->bound;
    }
    if(has) {
        for(int i=0; i<4; i++)
            children[i]->draw();
    }
}
