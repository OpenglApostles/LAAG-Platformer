#ifndef OBJECT_H
#define OBJECT_H
#include "polygon.h"
#include <glm/glm.hpp>

struct aabb2 {
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 c;

    aabb2(glm::vec2 a1, glm::vec2 b1);

    aabb2() {}

    bool containsPoint(glm::vec2 p);

    bool intersectsaabb2(aabb2 o);
};

class object{
    public:
        object();
        object(glm::vec2 _p, glm::vec2 _a, polygon _body, double _mass, bool _movable, bool _colidable);
        virtual ~object();
        void move();
        bool check_col(object o, glm::vec2 &ax, float &mtv, glm::vec2 &contact);
    bool movable, colidable;
    double mass;
    glm::vec2 p, v, a;
    aabb2 bound;
    polygon body;
};


#endif // OBJECT_H
