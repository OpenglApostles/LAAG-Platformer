#include <math.h>
#include "Vector.h"

vec3 vec3::cross(vec3 b) {
    return vec3(y * b.z - z * b.y,
                 z * b.x - x * b.z,
                 x * b.y - y * b.x);
}

float vec3::dot(vec3 b) {
    return x * b.x + y * b.y + z * b.z;
}

float vec3::len() {
    return sqrt(x * x + y * y + z * z);
}

float vec3::dist2(vec3 b) {
    return  (b.x - x) * (b.x - x) +
            (b.y - y) * (b.y - y) +
            (b.z - z) * (b.z - z);
}

float vec3::dist(vec3 b){
    return sqrt(dist2(b));
}

void vec3::norm() {
    float l = len();
    if(l == 0) l = .00001;
    x /= l;
    y /= l;
    z /= l;
}

float vec2::len() {
    return sqrt(x * x + y * y);
}

float vec2::dist2(vec2 b){
    return  (b.x - x) * (b.x - x) +
            (b.y - y) * (b.y - y);
}

float vec2::dist(vec2 b){
    return sqrt(dist2(b));
}
