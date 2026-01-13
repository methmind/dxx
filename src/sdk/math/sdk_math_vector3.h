#ifndef DXX_DLC_SDK_MATH_VECTOR3_H
#define DXX_DLC_SDK_MATH_VECTOR3_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <numbers>

namespace sdk::math
{
    struct vec3_s
    {
        float x{ 0.f }, y{ 0.f }, z{ 0.f };

        constexpr vec3_s() noexcept = default;
        constexpr vec3_s(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
        constexpr explicit vec3_s(float v) noexcept : x(v), y(v), z(v) {}

        [[nodiscard]] constexpr float operator[](std::size_t i) const noexcept
        {
            assert(i < 3 && "vec3_s index out of bounds");
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        [[nodiscard]] constexpr float& operator[](std::size_t i) noexcept
        {
            assert(i < 3 && "vec3_s index out of bounds");
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        [[nodiscard]] constexpr vec3_s operator-() const noexcept { return { -x, -y, -z }; }
        [[nodiscard]] constexpr vec3_s operator+() const noexcept { return *this; }

        [[nodiscard]] constexpr vec3_s operator+(const vec3_s& v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
        [[nodiscard]] constexpr vec3_s operator-(const vec3_s& v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
        [[nodiscard]] constexpr vec3_s operator*(const vec3_s& v) const noexcept { return { x * v.x, y * v.y, z * v.z }; }

        [[nodiscard]] constexpr vec3_s operator/(const vec3_s& v) const noexcept
        {
            assert(v.x != 0.f && v.y != 0.f && v.z != 0.f && "Division by zero");
            return { x / v.x, y / v.y, z / v.z };
        }

        [[nodiscard]] constexpr vec3_s operator+(float s) const noexcept { return { x + s, y + s, z + s }; }
        [[nodiscard]] constexpr vec3_s operator-(float s) const noexcept { return { x - s, y - s, z - s }; }
        [[nodiscard]] constexpr vec3_s operator*(float s) const noexcept { return { x * s, y * s, z * s }; }

        [[nodiscard]] constexpr vec3_s operator/(float s) const noexcept
        {
            assert(s != 0.f && "Division by zero");
            const float inv = 1.f / s;  // Одно деление вместо трёх
            return { x * inv, y * inv, z * inv };
        }

        [[nodiscard]] friend constexpr vec3_s operator*(float s, const vec3_s& v) noexcept { return v * s; }
        [[nodiscard]] friend constexpr vec3_s operator+(float s, const vec3_s& v) noexcept { return v + s; }
        [[nodiscard]] friend constexpr vec3_s operator-(float s, const vec3_s& v) noexcept { return { s - v.x, s - v.y, s - v.z }; }

        constexpr vec3_s& operator+=(const vec3_s& v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
        constexpr vec3_s& operator-=(const vec3_s& v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
        constexpr vec3_s& operator*=(const vec3_s& v) noexcept { x *= v.x; y *= v.y; z *= v.z; return *this; }

        constexpr vec3_s& operator/=(const vec3_s& v) noexcept
        {
            assert(v.x != 0.f && v.y != 0.f && v.z != 0.f && "Division by zero");
            x /= v.x; y /= v.y; z /= v.z;
            return *this;
        }

        constexpr vec3_s& operator+=(float s) noexcept { x += s; y += s; z += s; return *this; }
        constexpr vec3_s& operator-=(float s) noexcept { x -= s; y -= s; z -= s; return *this; }
        constexpr vec3_s& operator*=(float s) noexcept { x *= s; y *= s; z *= s; return *this; }

        constexpr vec3_s& operator/=(float s) noexcept
        {
            assert(s != 0.f && "Division by zero");
            const float inv = 1.f / s;
            x *= inv; y *= inv; z *= inv;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const vec3_s&) const noexcept = default;

        [[nodiscard]] constexpr bool is_near(const vec3_s& other, float tolerance = 1e-5f) const noexcept
        {
            return (*this - other).length_sqr() <= tolerance * tolerance;
        }

        [[nodiscard]] constexpr float dot(const vec3_s& v) const noexcept
        {
            return x * v.x + y * v.y + z * v.z;
        }

        [[nodiscard]] constexpr vec3_s cross(const vec3_s& v) const noexcept
        {
            return {
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
        }

        [[nodiscard]] constexpr float length_sqr() const noexcept { return x * x + y * y + z * z; }
        [[nodiscard]] constexpr float length_2d_sqr() const noexcept { return x * x + y * y; }

        [[nodiscard]] float length() const noexcept { return std::sqrt(length_sqr()); }
        [[nodiscard]] float length_2d() const noexcept { return std::sqrt(length_2d_sqr()); }

        [[nodiscard]] float length_safe() const noexcept { return std::hypot(x, y, z); }
        [[nodiscard]] float length_2d_safe() const noexcept { return std::hypot(x, y); }

        [[nodiscard]] float dist_to(const vec3_s& v) const noexcept { return (*this - v).length(); }
        [[nodiscard]] constexpr float dist_to_sqr(const vec3_s& v) const noexcept { return (*this - v).length_sqr(); }

        [[nodiscard]] constexpr bool is_zero() const noexcept
        {
            return x == 0.f && y == 0.f && z == 0.f;
        }

        [[nodiscard]] constexpr bool is_near_zero(float tolerance = 1e-6f) const noexcept
        {
            return length_sqr() <= tolerance * tolerance;
        }

        [[nodiscard]] bool is_valid() const noexcept
        {
            return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
        }

        [[nodiscard]] vec3_s normalized() const noexcept
        {
            const float len_sq = length_sqr();
            if (len_sq < 1e-12f)
                return {};

            const float inv_len = 1.f / std::sqrt(len_sq);
            return { x * inv_len, y * inv_len, z * inv_len };
        }

        float normalize() noexcept
        {
            const float len_sq = length_sqr();
            if (len_sq < 1e-12f) {
                x = y = z = 0.f;
                return 0.f;
            }

            const float len = std::sqrt(len_sq);
            const float inv_len = 1.f / len;
            x *= inv_len;
            y *= inv_len;
            z *= inv_len;
            return len;
        }

        [[nodiscard]] static constexpr vec3_s lerp(const vec3_s& a, const vec3_s& b, float t) noexcept
        {
            return a + (b - a) * t;
        }

        [[nodiscard]] vec3_s project_onto(const vec3_s& onto) const noexcept
        {
            const float denom = onto.length_sqr();
            if (denom < 1e-12f)
                return {};
            return onto * (dot(onto) / denom);
        }

        [[nodiscard]] constexpr vec3_s reflect(const vec3_s& normal) const noexcept
        {
            return *this - normal * (2.f * dot(normal));
        }

        constexpr void clamp(float min_val, float max_val) noexcept
        {
            x = std::clamp(x, min_val, max_val);
            y = std::clamp(y, min_val, max_val);
            z = std::clamp(z, min_val, max_val);
        }

        [[nodiscard]] constexpr vec3_s clamped(float min_val, float max_val) const noexcept
        {
            return {
                std::clamp(x, min_val, max_val),
                std::clamp(y, min_val, max_val),
                std::clamp(z, min_val, max_val)
            };
        }

        [[nodiscard]] static constexpr vec3_s min(const vec3_s& a, const vec3_s& b) noexcept
        {
            return { std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z) };
        }

        [[nodiscard]] static constexpr vec3_s max(const vec3_s& a, const vec3_s& b) noexcept
        {
            return { std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z) };
        }

        void sanitize_angles() noexcept
        {
            x = std::clamp(x, -89.f, 89.f);
            y = std::remainder(y, 360.f);
            z = 0.f;
        }

        [[nodiscard]] vec3_s to_angles() const noexcept;
        [[nodiscard]] static vec3_s from_angles(float pitch, float yaw) noexcept;

        [[nodiscard]] static constexpr vec3_s zero() noexcept { return { 0.f, 0.f, 0.f }; }
        [[nodiscard]] static constexpr vec3_s one() noexcept { return { 1.f, 1.f, 1.f }; }
        [[nodiscard]] static constexpr vec3_s up() noexcept { return { 0.f, 0.f, 1.f }; }
        [[nodiscard]] static constexpr vec3_s forward() noexcept { return { 1.f, 0.f, 0.f }; }
        [[nodiscard]] static constexpr vec3_s right() noexcept { return { 0.f, 1.f, 0.f }; }
    };

    static_assert(std::is_standard_layout_v<vec3_s>, "vec3_s must be standard layout");
    static_assert(std::is_trivially_copyable_v<vec3_s>, "vec3_s must be trivially copyable");
    static_assert(sizeof(vec3_s) == sizeof(float) * 3, "vec3_s size mismatch");
    static_assert(alignof(vec3_s) >= alignof(float), "vec3_s alignment mismatch");
    static_assert(offsetof(vec3_s, x) == 0, "vec3_s::x offset mismatch");
    static_assert(offsetof(vec3_s, y) == sizeof(float), "vec3_s::y offset mismatch");
    static_assert(offsetof(vec3_s, z) == sizeof(float) * 2, "vec3_s::z offset mismatch");

    inline vec3_s vec3_s::to_angles() const noexcept
    {
        constexpr float rad2deg = 180.f / std::numbers::pi_v<float>;

        const float len_2d = length_2d();

        if (len_2d < 1e-6f) {
            // Вертикальный вектор
            return { (z > 0.f) ? -90.f : 90.f, 0.f, 0.f };
        }

        const float yaw   = std::atan2(y, x) * rad2deg;
        const float pitch = std::atan2(-z, len_2d) * rad2deg;

        return { pitch, yaw, 0.f };
    }

    inline vec3_s vec3_s::from_angles(float pitch, float yaw) noexcept
    {
        constexpr float deg2rad = std::numbers::pi_v<float> / 180.f;

        const float pitch_rad = pitch * deg2rad;
        const float yaw_rad   = yaw * deg2rad;

        const float sp = std::sin(pitch_rad);
        const float cp = std::cos(pitch_rad);
        const float sy = std::sin(yaw_rad);
        const float cy = std::cos(yaw_rad);

        return { cp * cy, cp * sy, -sp };
    }

    [[nodiscard]] inline constexpr float dot(const vec3_s& a, const vec3_s& b) noexcept
    {
        return a.dot(b);
    }

    [[nodiscard]] inline constexpr vec3_s cross(const vec3_s& a, const vec3_s& b) noexcept
    {
        return a.cross(b);
    }

    [[nodiscard]] inline float distance(const vec3_s& a, const vec3_s& b) noexcept
    {
        return a.dist_to(b);
    }

    [[nodiscard]] inline constexpr float distance_sqr(const vec3_s& a, const vec3_s& b) noexcept
    {
        return a.dist_to_sqr(b);
    }

    [[nodiscard]] inline vec3_s normalize(const vec3_s& v) noexcept
    {
        return v.normalized();
    }

    [[nodiscard]] inline constexpr vec3_s lerp(const vec3_s& a, const vec3_s& b, float t) noexcept
    {
        return vec3_s::lerp(a, b, t);
    }

    using vector3 = vec3_s;
} // namespace sdk::math

#endif // DXX_DLC_SDK_MATH_VECTOR3_H