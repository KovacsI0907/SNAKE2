#include <vectors.h>

mat4 ScaleMatrix(vec3 s) {
    return mat4(
        s.x, 0.0f, 0.0f, 0.0f,
        0.0f, s.y, 0.0f, 0.0f,
        0.0f, 0.0f, s.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat4 TranslateMatrix(vec3 t) {
    return mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        t.x, t.y, t.z, 1.0f
    );
}

mat4 RotationMatrix(vec3 axis, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    float t = 1.0 - c;

    axis = normalize(axis);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return mat4(
        t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0.0f,
        t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0.0f,
        t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

vec4 operator*(const vec4& v, const mat4& m) {
    return vec4(
        v.x * m.rows[0][0] + v.y * m.rows[1][0] + v.z * m.rows[2][0] + v.w * m.rows[3][0],
        v.x * m.rows[0][1] + v.y * m.rows[1][1] + v.z * m.rows[2][1] + v.w * m.rows[3][1],
        v.x * m.rows[0][2] + v.y * m.rows[1][2] + v.z * m.rows[2][2] + v.w * m.rows[3][2],
        v.x * m.rows[0][3] + v.y * m.rows[1][3] + v.z * m.rows[2][3] + v.w * m.rows[3][3]
    );
}

vec2 normalize(const vec2& v) {
    float len = v.length();
    return { v.x / len, v.y / len };
}

vec3 normalize(const vec3& v) {
    float len = v.length();
    return { v.x / len, v.y / len, v.z / len };
}

vec4 normalize(const vec4& v) {
    float len = v.length();
    return { v.x / len, v.y / len, v.z / len, v.w / len };
}

float dot(const vec2& a, const vec2& b) {
    return a.x * b.x + a.y * b.y;
}

float dot(const vec3& a, const vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float dot(const vec4& a, const vec4& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vec3 cross(const vec3& a, const vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

