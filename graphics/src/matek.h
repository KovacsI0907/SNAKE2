#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

class Vec2
{
public:
    float x, y;

    // Konstruktor
    Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    // Vektorok összeadása
    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    // Vektorok kivonása
    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    // Skalárral való szorzás
    Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }

    //Belsõ szorzás
    float DotProduct(const Vec2& other) const
    {
        return (x * other.x + y * other.y);
    }

    // Tagonkénti szorzás
    Vec2 ElementWiseMultiply(const Vec2& other) const
    {
        return Vec2(x * other.x, y * other.y);
    }

    // Keresztszorzás (két dimenziós vektorok esetében ez egy skalár)
    float CrossProduct(const Vec2& other) const
    {
        return (x * other.y - y * other.x);
    }
    //Hossz
    float Length() const
    {
        return (sqrt(x * x + y * y));
    }

    // Kiíratás
    friend ostream& operator<<(ostream& os, const Vec2& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }
};


class Vec3
{
public:
    float x, y, z;

    // Konstruktor
    Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    // Vektorok összeadása
    Vec3 operator+(const Vec3& other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    // Vektorok kivonása
    Vec3 operator-(const Vec3& other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    // Skalárral való szorzás
    Vec3 operator*(float scalar) const
    {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    //Belsõ szorzás
    float DotProduct(const Vec3& other) const
    {
        return (x * other.x + y * other.y + z * other.z);
    }


    // Tagonkénti szorzás
    Vec3 ElementWiseMultiply(const Vec3& other) const
    {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    // Keresztszorzás
    Vec3 DrossProduct(const Vec3& other) const
    {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    // Hossz
    float Length() const
    {
        return (sqrt(x * x + y * y + z * z));
    }

    // Kiíratás
    friend ostream& operator<<(ostream& os, const Vec3& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }
};


class Vec4
{
public:
    float x, y, z, w;

    // Konstruktor
    Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}

    // Vektorok összeadása
    Vec4 operator+(const Vec4& other) const
    {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Vektorok kivonása
    Vec4 operator-(const Vec4& other) const
    {
        return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Skalárral való szorzás
    Vec4 operator*(float scalar) const
    {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Tagonkénti szorzás
    Vec4 ElementWiseMultiply(const Vec4& other) const
    {
        return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    // Belsõ szorzat
    float DotProduct(const Vec4& other) const
    {
        return (x * other.x + y * other.y + z * other.z + w * other.w);
    }

    // Vektor hossza
    float Lenght() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    // Kiíratás
    friend ostream& operator<<(ostream& os, const Vec4& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }
};



class Mat4 {
private:
    float data[16];

public:
    // Konstruktor
    Mat4()
    {
        for (int i = 0; i < 16; ++i) 
        {
                data[i] = 0.0f;
        }
    }

    Mat4(float data0, float data1, float data2, float data3, float data4, float data5, float data6, float data7,
         float data8, float data9, float data10, float data11, float data12, float data13, float data14, float data15)
    {
        data[0] = data0;  data[1] = data1;  data[2] = data2;  data[3] = data3;
        data[4] = data4;  data[5] = data5;  data[6] = data6;  data[7] = data7;
        data[8] = data8;  data[9] = data9;  data[10] = data10;  data[11] = data11;
        data[12] = data12;  data[13] = data13;  data[14] = data14;  data[15] = data15;
    }

    // Get
    float get(int index) const
    {
        return data[index];
    }

    // Set
    void set(int index, float value)
    {
        data[index] = value;
    }

    // Összeadás
    Mat4 operator+(const Mat4& other) const
    {
        Mat4 result;
        for (int i = 0; i < 16; ++i)
        {
                result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    // Kivonás
    Mat4 operator-(const Mat4& other) const
    {
        Mat4 result;
        for (int i = 0; i < 16; ++i)
        {
                result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

    // Szorzás skalárral
    Mat4 operator*(float scalar) const
    {
        Mat4 result;
        for (int i = 0; i < 16; ++i)
        {
                result.data[i] = data[i] * scalar;
        }
        return result;
    }

    // Szorzás mátrixxal
    Mat4 operator*(const Mat4& other) const
    {
        Mat4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    result.data[i*4+j] += data[i*4+k] * other.data[k*4+j];
                }
            }
        }
        return result;
    }

    // Belsõ szorzat
    float DotProduct(const Mat4& other) const
    {
        float result = 0.0;
        for (int i = 0; i < 16; ++i)
        {
                result += data[i] * other.data[i];

        }
        return result;
    }

    // Transzponálás
    Mat4 Transpose() const
    {
        Mat4 result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.data[j*4+i] = data[i*4+j];
            }
        }
        return result;
    }

    // Determináns
    float Determinant() const
    {
        return data[0] * (data[5] * (data[10] * data[15] - data[11] * data[14]) -
            data[6] * (data[9] * data[15] - data[11] * data[13]) +
            data[7] * (data[9] * data[14] - data[10] * data[13])) -

            data[1] * (data[4] * (data[10] * data[15] - data[11] * data[14]) -
                data[6] * (data[8] * data[15] - data[11] * data[12]) +
                data[7] * (data[8] * data[14] - data[10] * data[12])) +

            data[2] * (data[4] * (data[9] * data[15] - data[11] * data[13]) -
                data[5] * (data[8] * data[15] - data[11] * data[12]) +
                data[7] * (data[8] * data[13] - data[9] * data[12])) -

            data[3] * (data[4] * (data[9] * data[14] - data[10] * data[13]) -
                data[5] * (data[8] * data[14] - data[10] * data[12]) +
                data[6] * (data[8] * data[13] - data[9] * data[12]));
    }
    

    // Inverz
    Mat4 Inverse() const
    {
        float det = Determinant();
        if (det < 1.0e-6)
            return Mat4();

        Mat4 adjoint;

        adjoint.set(0, 1 * (data[5] * (data[10] * data[15] - data[11] * data[14]) -
            data[6] * (data[9] * data[15] - data[11] * data[13]) +
            data[7] * (data[9] * data[14] - data[10] * data[13])));

        adjoint.set(1, -1 * (data[4] * (data[10] * data[15] - data[11] * data[14]) -
            data[6] * (data[8] * data[15] - data[11] * data[12]) +
            data[7] * (data[8] * data[14] - data[10] * data[12])));

        adjoint.set(2, 1 * (data[4] * (data[9] * data[15] - data[11] * data[13]) -
            data[5] * (data[8] * data[15] - data[11] * data[12]) +
            data[7] * (data[8] * data[13] - data[9] * data[12])));

        adjoint.set(3, -1 * (data[4] * (data[9] * data[14] - data[10] * data[13]) -
            data[5] * (data[8] * data[14] - data[10] * data[12]) +
            data[6] * (data[8] * data[13] - data[9] * data[12])));

        adjoint.set(4, -1 * (data[1] * (data[10] * data[15] - data[11] * data[14]) -
            data[2] * (data[9] * data[15] - data[11] * data[13]) +
            data[3] * (data[9] * data[14] - data[10] * data[13])));

        adjoint.set(5, 1 * (data[0] * (data[10] * data[15] - data[11] * data[14]) -
            data[2] * (data[8] * data[15] - data[11] * data[12]) +
            data[3] * (data[8] * data[14] - data[10] * data[12])));

        adjoint.set(6, -1 * (data[0] * (data[9] * data[15] - data[11] * data[13]) -
            data[1] * (data[8] * data[15] - data[11] * data[12]) +
            data[3] * (data[8] * data[13] - data[9] * data[12])));

        adjoint.set(7, 1 * (data[0] * (data[9] * data[14] - data[10] * data[13]) -
            data[1] * (data[8] * data[14] - data[10] * data[12]) +
            data[2] * (data[8] * data[13] - data[9] * data[12])));

        adjoint.set(8, 1 * (data[1] * (data[6] * data[15] - data[7] * data[14]) -
            data[2] * (data[5] * data[15] - data[7] * data[13]) +
            data[3] * (data[5] * data[14] - data[6] * data[13])));

        adjoint.set(9, -1 * (data[0] * (data[6] * data[15] - data[7] * data[14]) -
            data[2] * (data[4] * data[15] - data[7] * data[12]) +
            data[3] * (data[4] * data[14] - data[6] * data[12])));

        adjoint.set(10, 1 * (data[0] * (data[5] * data[15] - data[7] * data[13]) -
            data[1] * (data[4] * data[15] - data[7] * data[12]) +
            data[3] * (data[4] * data[13] - data[5] * data[12])));

        adjoint.set(11, -1 * (data[0] * (data[5] * data[14] - data[6] * data[13]) -
            data[1] * (data[4] * data[14] - data[6] * data[12]) +
            data[2] * (data[4] * data[13] - data[5] * data[12])));

        adjoint.set(12, -1 * (data[1] * (data[6] * data[11] - data[7] * data[10]) -
            data[2] * (data[5] * data[11] - data[7] * data[9]) +
            data[3] * (data[5] * data[10] - data[6] * data[9])));

        adjoint.set(13, 1 * (data[0] * (data[6] * data[11] - data[7] * data[10]) -
            data[2] * (data[4] * data[11] - data[7] * data[8]) +
            data[3] * (data[4] * data[10] - data[6] * data[8])));

        adjoint.set(14, -1 * (data[0] * (data[5] * data[11] - data[7] * data[9]) -
            data[1] * (data[4] * data[11] - data[7] * data[8]) +
            data[3] * (data[4] * data[9] - data[5] * data[8])));

        adjoint.set(15, 1 * (data[0] * (data[5] * data[10] - data[6] * data[9]) -
            data[1] * (data[4] * data[10] - data[6] * data[8]) +
            data[2] * (data[4] * data[9] - data[5] * data[8])));

        return adjoint * (1.0 / det);
    }

    // Kiíratás
    friend ostream& operator<<(ostream& os, const Mat4& mat)
    {
        os << "|" << mat.get(0) << ", " << mat.get(1) << ", " << mat.get(2) << ", " << mat.get(3) << "|" << endl <<
            "|" << mat.get(4) << ", " << mat.get(5) << ", " << mat.get(6) << ", " << mat.get(7) << "|" << endl <<
            "|" << mat.get(8) << ", " << mat.get(9) << ", " << mat.get(10) << ", " << mat.get(11) << "|" << endl <<
            "|" << mat.get(12) << ", " << mat.get(12) << ", " << mat.get(14) << ", " << mat.get(15) << "|" << endl;
        return os;
    }
};