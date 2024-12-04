#include "vector.h"
#include <string>
#include <sstream>
#include <math.h>

Vec2::Vec2( void ) : x(0.0f), y(0.0f) {};
Vec2::Vec2( const Vec2 &v ) : x(v.x), y(v.y) {};
Vec2::Vec2( const float vx, const float vy ) : x(vx), y(vy) {};

bool Vec2::operator ==( const Vec2 &v ) const {
    return (x == v.x && y == v.y);
}

Vec2 Vec2::operator -( void ) const {
    return Vec2(-x, -y);
}

Vec2 Vec2::operator +( const Vec2& v ) const {
    return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator -( const Vec2& v ) const {
    return Vec2(x - v.x, y - v.y);
}

Vec2 Vec2::operator *( const float f ) const {
    return Vec2(x * f, y * f);
}

Vec2 Vec2::operator /( const float f ) const {
    return Vec2(x / f, y / f);
}

Vec2& Vec2::operator =( const Vec2& v ) {
    x = v.x;
    y = v.y;
    return *this;
}

float Vec2::Length( void ) const {
    return sqrt( (x * x) + (y * y) );
}

float Vec2::Distance( const Vec2 &v ) const {
    return sqrt( (x-v.x) * (x-v.x) + (y-v.y) * (y-v.y) );
}

Vec2& Vec2::Normalize( void ) {
    float invLen = 1.0f / Length();
    x *= invLen;
    y *= invLen;
    return *this;
}

float Vec2::Dot( const Vec2 &v ) const {
    return x*v.x + y*v.y;
}

std::string Vec2ToString(Vec2* v) {
    std::ostringstream out;
    out << "Vec2(" << v->x << ", " << v->y << ")";
    return out.str();
}
