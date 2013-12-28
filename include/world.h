#ifndef WORLD_H
#define WORLD_H
#include "object.h"
#include <glm/glm.hpp>

class world {
public:
    world();
    virtual ~world();
    glm::vec2 a, b;//wolrd boundary
    std::vector<object> obj;
};

#endif // WORLD_H
