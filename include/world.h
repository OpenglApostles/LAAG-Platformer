#ifndef WORLD_H
#define WORLD_H
#include "object.h"
#include "quadtree.h"
#include <glm/glm.hpp>
#include <vector>

class world {
public:
    world();
    virtual ~world();
    void end();
    void initTree(glm::vec2 a, glm::vec2 b);
    void insertAllInTree();
    void insertAllMovableToTree();
    void insertInTree(object *o);
    void removeAllFromTree();
    void removeAllMovableFromTree();
    void removeFromTree(object *o);
    glm::vec2 a, b;//wolrd boundary
    std::vector<object*> obj;
    quadtree *tree;
};

#endif // WORLD_H
