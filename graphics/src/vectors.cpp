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

vec4 operator*(const vec4& v, const mat4& m) {
    return vec4(
        v.x * m.rows[0][0] + v.y * m.rows[1][0] + v.z * m.rows[2][0] + v.w * m.rows[3][0],
        v.x * m.rows[0][1] + v.y * m.rows[1][1] + v.z * m.rows[2][1] + v.w * m.rows[3][1],
        v.x * m.rows[0][2] + v.y * m.rows[1][2] + v.z * m.rows[2][2] + v.w * m.rows[3][2],
        v.x * m.rows[0][3] + v.y * m.rows[1][3] + v.z * m.rows[2][3] + v.w * m.rows[3][3]
    );
}