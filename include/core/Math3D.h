#ifndef YAGE_CORE_MATH3D_H
#define YAGE_CORE_MATH3D_H

#include "core/Config.h"
#include "core/NonCopyable.h"

namespace ye
{
#define YE_PI 3.14159265358979323846
#define YE_TORADIAN(degree) ((degree) * YE_PI / 180.0)
#define YE_TODEGREE(radian) ((radian) * 180.0 / YE_PI)

    template <typename T, uint32_t D>
    class YE_TEMPLATE_API Vector : public NonCopyable
    {
    public:
        T Dot(Vector<T, D> const& r) const
        {
            T result = T(0);
            for (uint32_t i = 0; i < D; ++i)
            {
                result += (*this)[i] * r[i];
            }

            return result;
        }

        T Length() const { return std::sqrt(Dot(*this)); }

        Vector<T, D> Normalized() const { return *this / Length(); }

        Vector<T, D> operator+(Vector<T, D> const& r) const
        {
            Vector<T, D> result;
            for (uint32_t i = 0; i < D; i++)
                result[i] = values[i] + r[i];

            return result;
        }

        Vector<T, D> operator-(Vector<T, D> const& r) const
        {
            Vector<T, D> result;
            for (uint32_t i = 0; i < D; i++)
                result[i] = values[i] - r[i];

            return result;
        }

        Vector<T, D> operator*(T const& r) const
        {
            Vector<T, D> result;
            for (uint32_t i = 0; i < D; i++)
                result[i] = values[i] * r;

            return result;
        }

        Vector<T, D> operator/(T const& r) const
        {
            Vector<T, D> result;
            for (uint32_t i = 0; i < D; i++)
                result[i] = values[i] / r;

            return result;
        }

        Vector<T, D>& operator+=(Vector<T, D> const& r)
        {
            for (uint32_t i = 0; i < D; i++)
                (*this)[i] = values[i] + r[i];

            return *this;
        }

        Vector<T, D>& operator-=(Vector<T, D> const& r)
        {
            for (uint32_t i = 0; i < D; i++)
                (*this)[i] = values[i] - r[i];

            return *this;
        }

        Vector<T, D>& operator*=(T const& r)
        {
            for (uint32_t i = 0; i < D; i++)
                (*this)[i] = values[i] * r;

            return *this;
        }

        Vector<T, D>& operator/=(T const& r)
        {
            for (uint32_t i = 0; i < D; i++)
                (*this)[i] = values[i] / r;

            return *this;
        }

        bool operator==(Vector<T, D> const& r) const
        {
            for (uint32_t i = 0; i < D; i++)
                if ((*this)[i] != r[i])
                    return false;
            return true;
        }

        bool operator!=(Vector<T, D> const& r) const { return !operator==(r); }


        T& operator[](const uint32_t i) { return values[i]; }
        T operator[](const uint32_t i) const { return values[i]; }

    private:
        T values[D];
    };

    template <typename T>
    class YE_TEMPLATE_API Vector2 : public Vector<T, 2>
    {
    public:
        Vector2() = default;

        explicit Vector2(Vector2<T> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
        }

        Vector2(T x, T y)
        {
            (*this)[0] = x;
            (*this)[1] = y;
        }

        Vector2& operator =(Vector2<T> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];

            return *this;
        }

        Vector2(Vector2<T>&& r)
        {
            *this = r;
        }

        T Cross(Vector2<T> const& r) const
        {
            return GetX() * r.GetY() - GetY() * r.GetX();
        }

        T GetX() const { return (*this)[0]; }
        T GetY() const { return (*this)[1]; }

        void SetX(T const& x) { (*this)[0] = x; }
        void SetY(T const& y) { (*this)[1] = y; }

        void Set(T const& x, T const& y)
        {
            SetX(x);
            SetY(y);
        }
    };

    template <typename T>
    class YE_TEMPLATE_API Vector3 : public Vector<T, 3>
    {
    public:
        Vector3() = default;

        explicit Vector3(Vector<T, 3> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
            (*this)[2] = r[2];
        }

        explicit Vector3(Vector3<T> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
            (*this)[2] = r[2];
        }

        Vector3(T x, T y, T z)
        {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }

        Vector3& operator =(Vector3<T> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
            (*this)[2] = r[2];

            return *this;
        }

        Vector3(Vector3<T>&& r) noexcept
        {
            *this = r;
        }

        Vector3& operator=(Vector3&& r) noexcept
        {
            *this = r;

            return *this;
        }

        Vector3<T> Cross(Vector3<T> const& r) const
        {
            T x = (*this)[1] * r[2] - (*this)[2] * r[1];
            T y = (*this)[2] * r[0] - (*this)[0] * r[2];
            T z = (*this)[0] * r[1] - (*this)[1] * r[0];

            return Vector3<T>(x, y, z);
        }

        Vector3<T> Rotate(T angle, Vector3<T> const& axis) const
        {
            throw std::exception();
        }

        Vector2<T> GetXY() const { return Vector2<T>(GetX(), GetY()); }
        Vector2<T> GetYZ() const { return Vector2<T>(GetY(), GetZ()); }
        Vector2<T> GetZX() const { return Vector2<T>(GetZ(), GetX()); }

        Vector2<T> GetYX() const { return Vector2<T>(GetY(), GetX()); }
        Vector2<T> GetZY() const { return Vector2<T>(GetZ(), GetY()); }
        Vector2<T> GetXZ() const { return Vector2<T>(GetX(), GetZ()); }


        float GetX() const { return (*this)[0]; }
        float GetY() const { return (*this)[1]; }
        float GetZ() const { return (*this)[2]; }

        void SetX(const float x) { (*this)[0] = x; }
        void SetY(const float y) { (*this)[1] = y; }
        void SetZ(const float z) { (*this)[2] = z; }

        void Set(const float x, const float y, const float z)
        {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
    };

    template <typename T>
    class Vector4 : public Vector<T, 4>
    {
    public:
        Vector4() = default;

        explicit Vector4(Vector4<T> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
            (*this)[2] = r[2];
            (*this)[3] = r[3];
        }

        Vector4(T x, T y, T z, T w)
        {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
            (*this)[3] = w;
        }

        Vector4& operator =(Vector4<T> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
            (*this)[2] = r[2];
            (*this)[3] = r[3];

            return *this;
        }

        T GetX() const { return (*this)[0]; }
        T GetY() const { return (*this)[1]; }
        T GetZ() const { return (*this)[2]; }
        T GetW() const { return (*this)[3]; }

        void SetX(T const& x) { (*this)[0] = x; }
        void SetY(T const& y) { (*this)[1] = y; }
        void SetZ(T const& z) { (*this)[2] = z; }
        void SetW(T const& w) { (*this)[3] = w; }

        void Set(T const& x, T const& y, T const& z, T const& w)
        {
            SetX(x);
            SetY(y);
            SetZ(z);
            SetW(w);
        }
    };

    template <typename T, uint32_t D>
    class YE_TEMPLATE_API Matrix
    {
    public:
        Matrix<T, D> InitIdentity()
        {
            for (uint32_t i = 0; i < D; i++)
            {
                for (uint32_t j = 0; j < D; j++)
                {
                    if (i == j)
                        m[i][j] = T(1);
                    else
                        m[i][j] = T(0);
                }
            }

            return *this;
        }

        Matrix<T, D> Transpose() const
        {
            Matrix<T, D> t;
            for (auto j = 0; j < D; j++)
            {
                for (auto i = 0; i < D; i++)
                {
                    t[i][j] = m[j][i];
                }
            }
            return t;
        }

        Matrix<T, D> operator*(Matrix<T, D> const& r) const
        {
            Matrix<T, D> ret;
            for (uint32_t i = 0; i < D; i++)
            {
                for (uint32_t j = 0; j < D; j++)
                {
                    ret.m[i][j] = T(0);
                    for (uint32_t k = 0; k < D; k++)
                        ret.m[i][j] += m[k][j] * r.m[i][k];
                }
            }
            return ret;
        }

        void Set(const uint32_t x, const uint32_t y, T val) { m[x][y] = val; }

        T const* operator[](const int32_t index) const { return m[index]; }
        T* operator[](const int32_t index) { return m[index]; }

    private:
        T m[D][D];
    };

    template <typename T>
    class YE_TEMPLATE_API Matrix3 : public Matrix<T, 3>
    {
    public:
        Matrix3() = default;

        template <int32_t D>
        explicit Matrix3(Matrix<T, D> const& r)
        {
            if (D < 3)
            {
                this->InitIdentity();

                for (uint32_t i = 0; i < D; i++)
                    for (uint32_t j = 0; j < D; j++)
                        (*this)[i][j] = r[i][j];
            }
            else
            {
                for (uint32_t i = 0; i < 3; i++)
                    for (uint32_t j = 0; j < 3; j++)
                        (*this)[i][j] = r[i][j];
            }
        }
    };

    template <typename T>
    class YE_TEMPLATE_API Matrix4 : public Matrix<T, 4>
    {
    public:
        Matrix4() = default;

        template <uint32_t D>
        explicit Matrix4(Matrix<T, D> const& r)
        {
            if (D < 4)
            {
                this->InitIdentity();

                for (uint32_t i = 0; i < D; i++)
                    for (uint32_t j = 0; j < D; j++)
                        (*this)[i][j] = r[i][j];
            }
            else
            {
                for (uint32_t i = 0; i < 4; i++)
                    for (uint32_t j = 0; j < 4; j++)
                        (*this)[i][j] = r[i][j];
            }
        }

        Matrix4<T> InitPersective(T fov, T aspectRatio, T zNear, T zFar)
        {
            T const zRange = zNear - zFar;
            T tanHalfFOV = std::tanf(fov / T(2));

            (*this)[0][0] = T(1) / (tanHalfFOV * aspectRatio);
            (*this)[1][0] = T(0);
            (*this)[2][0] = T(0);
            (*this)[3][0] = T(0);
            (*this)[0][1] = T(0);
            (*this)[1][1] = T(1) / tanHalfFOV;
            (*this)[2][1] = T(0);
            (*this)[3][1] = T(0);
            (*this)[0][2] = T(0);
            (*this)[1][2] = T(0);
            (*this)[2][2] = (-zNear - zFar) / zRange;
            (*this)[3][2] = T(2) * zFar * zNear / zRange;
            (*this)[0][3] = T(0);
            (*this)[1][3] = T(0);
            (*this)[2][3] = T(1);
            (*this)[3][3] = T(0);

            return *this;
        }
    };

    class YE_API Quaternion : public Vector4<float>
    {
    public:
        explicit Quaternion(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 1.0f)
        {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
            (*this)[3] = w;
        }

        explicit Quaternion(Vector4<float> const& r)
        {
            (*this)[0] = r[0];
            (*this)[1] = r[1];
            (*this)[2] = r[2];
            (*this)[3] = r[3];
        }

        Quaternion(Quaternion const& quaternion)
        {
            (*this)[0] = quaternion[0];
            (*this)[1] = quaternion[1];
            (*this)[2] = quaternion[2];
            (*this)[3] = quaternion[3];
        }

        Quaternion(Vector3<float> const& axis, const float angle)
        {
            const auto sinHalfAngle = std::sinf(angle / 2);
            const auto cosHalfAngle = std::cosf(angle / 2);

            (*this)[0] = axis.GetX() * sinHalfAngle;
            (*this)[1] = axis.GetY() * sinHalfAngle;
            (*this)[2] = axis.GetZ() * sinHalfAngle;
            (*this)[3] = cosHalfAngle;
        }

        Quaternion Conjugate() const { return Quaternion(-GetX(), -GetY(), -GetZ(), GetW()); }

        Quaternion operator*(Quaternion const& r) const
        {
            auto const _w = (GetW() * r.GetW()) - (GetX() * r.GetX()) - (GetY() * r.GetY()) - (GetZ() * r.GetZ());
            auto const _x = (GetX() * r.GetW()) + (GetW() * r.GetX()) + (GetY() * r.GetZ()) - (GetZ() * r.GetY());
            auto const _y = (GetY() * r.GetW()) + (GetW() * r.GetY()) + (GetZ() * r.GetX()) - (GetX() * r.GetZ());
            auto const _z = (GetZ() * r.GetW()) + (GetW() * r.GetZ()) + (GetX() * r.GetY()) - (GetY() * r.GetX());

            return Quaternion(_x, _y, _z, _w);
        }
    };

    typedef Vector3<uint32_t> Vector3u;

    typedef Vector2<float> Vector2f;
    typedef Vector3<float> Vector3f;
    typedef Vector4<float> Vector4f;

    typedef Matrix3<float> Matrix3f;
    typedef Matrix4<float> Matrix4f;
}

#endif
