import("examplelib")

v <- Vec2();
v.x = 1.2;
v.y = 3.4;
v *= 2.0;
print(v.x + "\n");
print(v.y + "\n");
print("" + v + "\n");
print(v.Length() + "\n");

mod <- ::import("scripts/mymodule", {});

instance <- mod.MyClass();
::print(instance.a);
::print(instance.b);

res <- mod.AddTwo(5, 3);

::print(mod.M_PI);
