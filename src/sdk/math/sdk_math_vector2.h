//
// Created by sexey on 01.12.2025.
//

#ifndef DXX_DLC_SDK_MATH_VECTOR2_H
#define DXX_DLC_SDK_MATH_VECTOR2_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <numbers>

namespace sdk::math
{
    struct vec2_s
    {
        float x{ 0.f }, y{ 0.f };

        constexpr vec2_s() noexcept = default;
        constexpr vec2_s(float _x, float _y) noexcept : x(_x), y(_y) {}
        constexpr explicit vec2_s(float v) noexcept : x(v), y(v) {}

        [[nodiscard]] constexpr float operator[](std::size_t i) const noexcept
        {
            assert(i < 2 && "vec2_s index out of bounds");
            if (i == 0) return x;
            return y;
        }

        [[nodiscard]] constexpr float& operator[](std::size_t i) noexcept
        {
            assert(i < 2 && "vec2_s index out of bounds");
            if (i == 0) return x;
            return y;
        }

        [[nodiscard]] constexpr vec2_s operator-() const noexcept { return { -x, -y }; }
        [[nodiscard]] constexpr vec2_s operator+() const noexcept { return *this; }

        [[nodiscard]] constexpr vec2_s operator+(const vec2_s& v) const noexcept { return { x + v.x, y + v.y }; }
        [[nodiscard]] constexpr vec2_s operator-(const vec2_s& v) const noexcept { return { x - v.x, y - v.y }; }
        [[nodiscard]] constexpr vec2_s operator*(const vec2_s& v) const noexcept { return { x * v.x, y * v.y }; }
        [[nodiscard]] constexpr vec2_s operator/(const vec2_s& v) const noexcept
        {
            assert(v.x != 0.f && v.y != 0.f && "Division by zero");
            return { x / v.x, y / v.y };
        }

        [[nodiscard]] constexpr vec2_s operator+(float s) const noexcept { return { x + s, y + s }; }
        [[nodiscard]] constexpr vec2_s operator-(float s) const noexcept { return { x - s, y - s }; }
        [[nodiscard]] constexpr vec2_s operator*(float s) const noexcept { return { x * s, y * s }; }
        [[nodiscard]] constexpr vec2_s operator/(float s) const noexcept
        {
            assert(s != 0.f && "Division by zero");
            const float inv = 1.f / s;
            return { x * inv, y * inv };
        }

        [[nodiscard]] friend constexpr vec2_s operator*(float s, const vec2_s& v) noexcept { return v * s; }
        [[nodiscard]] friend constexpr vec2_s operator+(float s, const vec2_s& v) noexcept { return v + s; }
        [[nodiscard]] friend constexpr vec2_s operator-(float s, const vec2_s& v) noexcept { return { s - v.x, s - v.y }; }

        constexpr vec2_s& operator+=(const vec2_s& v) noexcept { x += v.x; y += v.y; return *this; }
        constexpr vec2_s& operator-=(const vec2_s& v) noexcept { x -= v.x; y -= v.y; return *this; }
        constexpr vec2_s& operator*=(const vec2_s& v) noexcept { x *= v.x; y *= v.y; return *this; }
        constexpr vec2_s& operator/=(const vec2_s& v) noexcept
        {
            assert(v.x != 0.f && v.y != 0.f && "Division by zero");
            x /= v.x; y /= v.y;
            return *this;
        }

        constexpr vec2_s& operator+=(float s) noexcept { x += s; y += s; return *this; }
        constexpr vec2_s& operator-=(float s) noexcept { x -= s; y -= s; return *this; }
        constexpr vec2_s& operator*=(float s) noexcept { x *= s; y *= s; return *this; }
        constexpr vec2_s& operator/=(float s) noexcept
        {
            assert(s != 0.f && "Division by zero");
            const float inv = 1.f / s;
            x *= inv; y *= inv;
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(const vec2_s&) const noexcept = default;

        [[nodiscard]] constexpr bool is_near(const vec2_s& other, float tolerance = 1e-5f) const noexcept
        {
            return (*this - other).length_sqr() <= tolerance * tolerance;
        }

        [[nodiscard]] constexpr float dot(const vec2_s& v) const noexcept
        {
            return x * v.x + y * v.y;
        }

        // В 2D "cross product" возвращает скаляр (z-компонента в 3D)
        [[nodiscard]] constexpr float cross(const vec2_s& v) const noexcept
        {
            return x * v.y - y * v.x;
        }

        [[nodiscard]] constexpr float length_sqr() const noexcept { return x * x + y * y; }
        [[nodiscard]] float length() const noexcept { return std::sqrt(length_sqr()); }
        [[nodiscard]] float length_safe() const noexcept { return std::hypot(x, y); }

        [[nodiscard]] float dist_to(const vec2_s& v) const noexcept { return (*this - v).length(); }
        [[nodiscard]] constexpr float dist_to_sqr(const vec2_s& v) const noexcept { return (*this - v).length_sqr(); }

        [[nodiscard]] constexpr bool is_zero() const noexcept
        {
            return x == 0.f && y == 0.f;
        }

        [[nodiscard]] constexpr bool is_near_zero(float tolerance = 1e-6f) const noexcept
        {
            return length_sqr() <= tolerance * tolerance;
        }

        [[nodiscard]] bool is_valid() const noexcept
        {
            return std::isfinite(x) && std::isfinite(y);
        }

        [[nodiscard]] vec2_s normalized() const noexcept
        {
            const float len_sq = length_sqr();
            if (len_sq < 1e-12f)
                return {};
            const float inv_len = 1.f / std::sqrt(len_sq);
            return { x * inv_len, y * inv_len };
        }

        float normalize() noexcept
        {
            const float len_sq = length_sqr();
            if (len_sq < 1e-12f) {
                x = y = 0.f;
                return 0.f;
            }
            const float len = std::sqrt(len_sq);
            const float inv_len = 1.f / len;
            x *= inv_len;
            y *= inv_len;
            return len;
        }

        [[nodiscard]] static constexpr vec2_s lerp(const vec2_s& a, const vec2_s& b, float t) noexcept
        {
            return a + (b - a) * t;
        }

        [[nodiscard]] vec2_s project_onto(const vec2_s& onto) const noexcept
        {
            const float denom = onto.length_sqr();
            if (denom < 1e-12f)
                return {};

            return onto * (dot(onto) / denom);
        }

        [[nodiscard]] constexpr vec2_s reflect(const vec2_s& normal) const noexcept
        {
            return *this - normal * (2.f * dot(normal));
        }

        constexpr void clamp(float min_val, float max_val) noexcept
        {
            x = std::clamp(x, min_val, max_val);
            y = std::clamp(y, min_val, max_val);
        }

        [[nodiscard]] constexpr vec2_s clamped(float min_val, float max_val) const noexcept
        {
            return {
                std::clamp(x, min_val, max_val),
                std::clamp(y, min_val, max_val)
            };
        }

        [[nodiscard]] static constexpr vec2_s min(const vec2_s& a, const vec2_s& b) noexcept
        {
            return { std::min(a.x, b.x), std::min(a.y, b.y) };
        }

        [[nodiscard]] static constexpr vec2_s max(const vec2_s& a, const vec2_s& b) noexcept
        {
            return { std::max(a.x, b.x), std::max(a.y, b.y) };
        }

        [[nodiscard]] float to_angle() const noexcept;

        [[nodiscard]] static vec2_s from_angle(float angle_deg) noexcept;

        [[nodiscard]] static constexpr vec2_s zero() noexcept { return { 0.f, 0.f }; }
        [[nodiscard]] static constexpr vec2_s one() noexcept { return { 1.f, 1.f }; }
        [[nodiscard]] static constexpr vec2_s up() noexcept { return { 0.f, 1.f }; }
        [[nodiscard]] static constexpr vec2_s right() noexcept { return { 1.f, 0.f }; }
    };

    static_assert(std::is_standard_layout_v<vec2_s>, "vec2_s must be standard layout");
    static_assert(std::is_trivially_copyable_v<vec2_s>, "vec2_s must be trivially copyable");
    static_assert(sizeof(vec2_s) == sizeof(float) * 2, "vec2_s size mismatch");
    static_assert(alignof(vec2_s) >= alignof(float), "vec2_s alignment mismatch");
    static_assert(offsetof(vec2_s, x) == 0, "vec2_s::x offset mismatch");
    static_assert(offsetof(vec2_s, y) == sizeof(float), "vec2_s::y offset mismatch");

    inline float vec2_s::to_angle() const noexcept
    {
        constexpr float rad2deg = 180.f / std::numbers::pi_v<float>;
        return std::atan2(y, x) * rad2deg;
    }

    inline vec2_s vec2_s::from_angle(float angle_deg) noexcept
    {
        constexpr float deg2rad = std::numbers::pi_v<float> / 180.f;
        const float rad = angle_deg * deg2rad;

        return { std::cos(rad), std::sin(rad) };
    }

    [[nodiscard]] inline constexpr float dot(const vec2_s& a, const vec2_s& b) noexcept
    {
        return a.dot(b);
    }

    [[nodiscard]] inline constexpr float cross(const vec2_s& a, const vec2_s& b) noexcept
    {
        return a.cross(b);
    }

    [[nodiscard]] inline float distance(const vec2_s& a, const vec2_s& b) noexcept
    {
        return a.dist_to(b);
    }

    [[nodiscard]] inline constexpr float distance_sqr(const vec2_s& a, const vec2_s& b) noexcept
    {
        return a.dist_to_sqr(b);
    }

    [[nodiscard]] inline vec2_s normalize(const vec2_s& v) noexcept
    {
        return v.normalized();
    }

    [[nodiscard]] inline constexpr vec2_s lerp(const vec2_s& a, const vec2_s& b, float t) noexcept
    {
        return vec2_s::lerp(a, b, t);
    }

    using vector2 = vec2_s;
}

#endif //DXX_DLC_SDK_MATH_VECTOR2_H