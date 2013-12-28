#ifndef OBJECT_H
#define OBJECT_H
#include "polygon.h"
#include <glm/glm.hpp>

class object
{
    public:
        object();
        object(glm::vec2 _p, glm::vec2 _a, polygon _body, double _mass, bool _movable, bool _colidable);
        virtual ~object();
        void move();
        bool check_col(object o, glm::vec2 &ax, float &mtv, glm::vec2 &contact);
    bool movable, colidable;
    double mass;
    glm::vec2 p, v, a;
    polygon body;
};

#endif // OBJECT_H
