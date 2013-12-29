#ifndef POLYGON_H
#define POLYGON_H
#include <glm/glm.hpp>
#include <vector>
#include <string>

class polygon
{
    public:
        polygon();
        polygon(std::string type, float _sc);
        void computeDim();
        bool check_col(polygon p, glm::vec2 &ax, float &mtv, glm::vec2 &contact);
        virtual ~polygon();
        std::vector<glm::vec2> p, n;
        glm::vec2 dim, a, b;
        float sc;
        int tex_ind;
};

#endif // POLYGON_H
