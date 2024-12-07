#include <kalibri.hpp>
#include <sqstdio.h>

#include "vector.h"

SQRESULT sqmodule_load(HSQUIRRELVM v, HSQAPI api)
{
    kb::Class<Vec2> vec2(v, "Vec2");

    vec2
    // Variables
    .Var("x", &Vec2::x)
    .Var("y", &Vec2::y)
    .Ctor()
    .Ctor<const Vec2 &>()
    .Ctor<float, float>()

    // Operators
    .Func("_add", &Vec2::operator +)
    .Func("_mul", &Vec2::operator *)
    .Func("_div", &Vec2::operator /)

    // Function Disambiguation
    .Func<Vec2(Vec2::*)(void) const>("_unm", &Vec2::operator -)
    .Func<Vec2(Vec2::*)(const Vec2&) const>("_sub", &Vec2::operator -)

    // Member Functions
    .Func("Length", &Vec2::Length)
    .Func("Distance", &Vec2::Distance)
    .Func("Normalize", &Vec2::Normalize)
    .Func("Dot", &Vec2::Dot)

    // Global Static Function bound as a member function
    .GlobalFunc("_tostring", &Vec2ToString)
    ;

    kb::RootTable(v).Bind("Vec2", vec2);
    return SQ_OK;
}
