#include "../include/world.h"

world::world()
{
    //ctor
}

world::~world()
{
    //dtor
}

void world::end(){
    tree->kill();
    delete tree;
    for(int i=0;i<obj.size();i++){
        delete obj[i];
    }
    obj.clear();
}

void world::initTree(glm::vec2 a, glm::vec2 b){
    tree = new quadtree(aabb2(a, b), 0);
}

void world::insertAllInTree(){
    for(int i=0;i<obj.size();i++){
        tree->insert(obj[i], obj[i]->bound);
    }
}

void world::insertAllMovableToTree(){
    for(int i=0;i<obj.size();i++){
        if(obj[i]->movable)
            tree->insert(obj[i], obj[i]->bound);
    }
}

void world::insertInTree(object *o){
    tree->insert(o, o->bound);
}

void world::removeAllFromTree(){
    for(int i=0;i<obj.size();i++){
        tree->remove(obj[i], obj[i]->bound);
    }
}

void world::removeAllMovableFromTree(){
    for(int i=0;i<obj.size();i++){
        if(obj[i]->movable)
            tree->remove(obj[i], obj[i]->bound);
    }
}

void world::removeFromTree(object *o){
    tree->remove(o, o->bound);
}
