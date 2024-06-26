#ifndef VECTORS_H
#define VECTORS_H

#include <cmath>
#include <cstring>

struct vec2 {
    float x, y;

    vec2() : x(0), y(0) {}
    vec2(float x, float y) : x(x), y(y) {}

    vec2 operator+(const vec2& other) const {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2& other) const {
        return vec2(x - other.x, y - other.y);
    }

    vec2 operator*(float scalar) const {
        return vec2(x * scalar, y * scalar);
    }

    vec2 operator/(float scalar) const {
        return vec2(x / scalar, y / scalar);
    }

    float dot(const vec2& other) const {
        return x * other.x + y * other.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    vec2 normalize() const {
        float len = length();
        return vec2(x / len, y / len);
    }
};

struct vec3 {
    float x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(const vec2& v, float z) : x(v.x), y(v.y), z(z) {}

    vec2 xy() const {
        return vec2(x, y);
    }

    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator*(float scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3 operator/(float scalar) const {
        return vec3(x / scalar, y / scalar, z / scalar);
    }

    float dot(const vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3 cross(const vec3& other) const {
        return vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec3 normalize() const {
        float len = length();
        return vec3(x / len, y / len, z / len);
    }
};

struct vec4 {
    float x, y, z, w;

    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    vec4(const vec2& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
    vec4(const vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

    vec3 xyz() const {
        return vec3(x, y, z);
    }

    vec2 xy() const {
        return vec2(x, y);
    }

    vec4 operator+(const vec4& other) const {
        return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vec4 operator-(const vec4& other) const {
        return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    vec4 operator*(float scalar) const {
        return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    vec4 operator/(float scalar) const {
        return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    float dot(const vec4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    vec4 normalize() const {
        float len = length();
        return vec4(x / len, y / len, z / len, w / len);
    }
};

struct mat4 {
    float rows[4][4];

    mat4() {
        std::memset(rows, 0, 16 * sizeof(float));
    }

    mat4(float diagonal) {
        std::memset(rows, 0, 16 * sizeof(float));
        rows[0][0] = rows[1][1] = rows[2][2] = rows[3][3] = diagonal;
    }

    mat4(float v00, float v01, float v02, float v03,
        float v10, float v11, float v12, float v13,
        float v20, float v21, float v22, float v23,
        float v30, float v31, float v32, float v33) {
        rows[0][0] = v00; rows[0][1] = v01; rows[0][2] = v02; rows[0][3] = v03;
        rows[1][0] = v10; rows[1][1] = v11; rows[1][2] = v12; rows[1][3] = v13;
        rows[2][0] = v20; rows[2][1] = v21; rows[2][2] = v22; rows[2][3] = v23;
        rows[3][0] = v30; rows[3][1] = v31; rows[3][2] = v32; rows[3][3] = v33;
    }

    static mat4 identity() {
        return mat4(1.0f);
    }

    mat4 operator*(const mat4& other) const {
        mat4 result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.rows[row][col] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.rows[row][col] += rows[row][k] * other.rows[k][col];
                }
            }
        }
        return result;
    }

    vec4 operator*(const vec4& v) const {
        return vec4(
            rows[0][0] * v.x + rows[0][1] * v.y + rows[0][2] * v.z + rows[0][3] * v.w,
            rows[1][0] * v.x + rows[1][1] * v.y + rows[1][2] * v.z + rows[1][3] * v.w,
            rows[2][0] * v.x + rows[2][1] * v.y + rows[2][2] * v.z + rows[2][3] * v.w,
            rows[3][0] * v.x + rows[3][1] * v.y + rows[3][2] * v.z + rows[3][3] * v.w
        );
    }

    float* operator[](int index) {
        return rows[index];
    }

    const float* operator[](int index) const {
        return rows[index];
    }
};

mat4 ScaleMatrix(vec3 s);

mat4 TranslateMatrix(vec3 t);

vec4 operator*(const vec4& v, const mat4& m);

#endif // VECTORS_H