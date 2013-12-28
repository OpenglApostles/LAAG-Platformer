#ifndef POLYGON_H
#define POLYGON_H
#include <glm/glm.hpp>
#include <vector>
#include <string>

class polygon
{
    public:
        polygon();
        polygon(std::string type, double _sc);
        void computeDim();
        bool check_col(polygon p, glm::vec2 &ax, float &mtv, glm::vec2 &contact);
        virtual ~polygon();
        std::vector<glm::vec2> p, n;
        glm::vec2 dim;
        double sc;
};

#endif // POLYGON_H
