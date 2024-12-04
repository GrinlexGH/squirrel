#pragma once

#include <string>

class Vec2 {
public:
    float x, y;

    Vec2(void);
    Vec2(const Vec2& v);
    Vec2(const float vx, const float vy);

    bool operator ==(const Vec2& v) const;
    Vec2 operator -(void) const;
    Vec2 operator +(const Vec2& v) const;
    Vec2 operator -(const Vec2& v) const;
    Vec2 operator *(const float f) const;
    Vec2 operator /(const float f) const;
    Vec2& operator =(const Vec2& v);

    float Length(void) const;
    float Distance(const Vec2& v) const;
    Vec2& Normalize(void);
    float Dot(const Vec2& v) const;
};

std::string Vec2ToString(Vec2* v);
