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
#if 0
	// You can run your own script by doing this
	kb::DefaultVM::Set(v); // after this line specifying vm in next objects is unnecessary
	kb::Script script(v);
	script.CompileString("\
		v <- Vec2(); \
		v.x = 1.2; \
		v.y = 3.4; \
		v *= 2.0; \
		print(v.x + \"\\n\"); \
		print(v.y + \"\\n\"); \
		print(\"\"+v + \"\\n\"); \
		print(v.Length() + \"\\n\"); \
	");
	if (kb::Error::Occurred(v)) {
		sq_getprintfunc(v)(v, "Compile Failed: %s", kb::Error::Message(v));
	}
	script.Run();
	if (kb::Error::Occurred(v))
	{
		sq_getprintfunc(v)(v, "Run Failed: %s", kb::Error::Message(v));
	}
#endif

	return SQ_OK;
}
