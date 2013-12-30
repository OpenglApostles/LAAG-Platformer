#ifndef QUADTREE_H
#define QUADTREE_H
#include "object.h"
#include <vector>
#define MAX_NODE_CAPACITY 5
#define MIN_NODE_CAPACITY 6
#define MAX_DEPTH 5

#include "manager_space.h"

struct quadtree {
    aabb2 boundary;
    std::vector<object*> obj;
    quadtree* children[4];
    bool has;
    int depth, count;

    void cleanUp();

    quadtree();

    quadtree(aabb2 b, int d);

    std::vector<short> whereTo(aabb2 p);

    void insert(object *a ,aabb2 p);

    void subdivide();

    void queryRange(aabb2 range);

    void collect(std::vector<object*> &here);

    void kill();

    void remove(object*  a, aabb2 rem);

    void checkObj();

    void checkWall(int w);

    void check();

    void draw();
};

void checkPair(object &o1, object &o2);
void checkWorld(object &o, glm::vec2 a, glm::vec2 b, float e);

#endif // QUADTREE_H
