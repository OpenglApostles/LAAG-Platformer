#ifndef VECTORHEADER
#define VECTORHEADER

struct vec2 {
    float x, y;

    float &operator[](int i) {
        if(i)
            return y;
        return x;
    }

    vec2() {}

    vec2(float x1, float y1) {
        x = x1;
        y = y1;
    }

    vec2 operator/(float value) {
        return vec2(x / value, y / value);
    }

    void operator+=(float value) {
        x += value;
        y += value;
    }

    void operator+=(vec2 value) {
        x += value.x;
        y += value.y;
    }

    void operator-=(float value) {
        x -= value;
        y -= value;
    }

    void operator-=(vec2 value) {
        x -= value.x;
        y -= value.y;
    }

    void operator*=(float value){
        x *= value;
        y *= value;
    }

    vec2 operator-(vec2 value) {
        return vec2(x - value.x, y - value.y);
    }

    vec2 operator*(float scale) {
        return vec2(x * scale, y * scale);
    }

    vec2 operator+(const vec2 &other) {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2 &other) const {
        return vec2(x - other.x, y - other.y);
    }

    vec2 operator-() const {
        return vec2(-x, -y);
    }

    float len();
    float dist2(vec2);
    float dist(vec2);
};

static vec2 zero2f = vec2(0, 0);

struct vec3 {
    float x, y, z;

    float &operator[](int i) {
        if(i == 0) return x;
        else if(i == 1) return y;
        else return z;
    }

    vec3() {}

    vec3(float x1, float y1, float z1) {
        x = x1;
        y = y1;
        z = z1;
    }

    vec3 operator/(float value) {
        return vec3(x / value, y / value, z / value);
    }

    void operator+=(vec3 value) {
        x += value.x;
        y += value.y;
        z += value.z;
    }

    void operator+=(vec2 value) {
        x += value.x;
        y += value.y;
    }

    void operator+=(float value) {
        x += value;
        y += value;
        z += value;
    }

    void operator-=(vec3 value) {
        x -= value.x;
        y -= value.y;
        z -= value.z;
    }

    void operator-=(vec2 value) {
        x -= value.x;
        y -= value.y;
    }

    void operator-=(float value) {
        x -= value;
        y -= value;
        z -= value;
    }

    bool operator!=(vec3 value) {
        return x != value.x or y != value.y or z != value.z;
    }

    vec3 operator-(vec3 value) {
        return vec3(x - value.x, y - value.y, z - value.z);
    }

    vec3 operator-(vec2 value) {
        return vec3(x - value.x, y - value.y, z);
    }

    vec3 operator*(float scale) {
        return vec3(x * scale, y * scale, z * scale);
    }

    void operator*=(float value) {
        x *= value;
        y *= value;
        z *= value;
    }

    vec3 operator+(const vec3 &other) {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator+(const vec2 &other) {
        return vec3(x + other.x, y + other.y, z);
    }

    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    bool operator==(vec3 v) {
        return x == v.x and y == v.y and z == v.z;
    }

    float len();
    void norm();
    float dot(vec3);
    vec3 cross(vec3);
    float dist2(vec3);
    float dist(vec3);
};

struct vec4 {
    float x, y, z, w;

    float &operator[](int i) {
        if(i == 0) return x;
        else if(i == 1) return y;
        else if(i == 2) return z;
        else return w;
    }

    vec4() {}

    vec4(float x1, float y1, float z1, float w1) {
        x = x1;
        y = y1;
        z = z1;
        w = w1;
    }

    vec4 operator/(float value) {
        return vec4(x / value, y / value, z / value, w / value);
    }

    void operator+=(vec4 value) {
        x += value.x;
        y += value.y;
        z += value.z;
        w += value.w;
    }

    void operator+=(vec2 value) {
        x += value.x;
        y += value.y;
    }

    void operator+=(float value) {
        x += value;
        y += value;
        z += value;
        w += value;
    }

    void operator-=(vec4 value) {
        x -= value.x;
        y -= value.y;
        z -= value.z;
        w -= value.w;
    }

    void operator-=(vec2 value) {
        x -= value.x;
        y -= value.y;
    }

    void operator-=(float value) {
        x -= value;
        y -= value;
        z -= value;
        w -= value;
    }

    bool operator!=(vec4 value) {
        return x != value.x or y != value.y or z != value.z or w != value.w;
    }

    vec4 operator-(vec4 value) {
        return vec4(x - value.x, y - value.y, z - value.z, w - value.w);
    }

    vec4 operator*(float scale) {
        return vec4(x * scale, y * scale, z * scale, w * scale);
    }

    void operator*=(float value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
    }

    vec4 operator+(const vec4 &other) {
        return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vec4 operator-() const {
        return vec4(-x, -y, -z, -w);
    }

    bool operator==(vec4 v) {
        return x == v.x and y == v.y and z == v.z and w == v.w;
    }
};

static vec4 zero4f = vec4(0, 0, 0, 0);

#endif
