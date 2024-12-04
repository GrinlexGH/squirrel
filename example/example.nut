import("examplelib")

v <- Vec2();
v.x = 1.2;
v.y = 3.4;
v *= 2.0;
print(v.x + "\n"); // Will print "1.2"
print(v.y + "\n");
print("" + v + "\n");
print(v.Length() + "\n");

mod <- ::import("mymodule", {});

print(mod.M_PI);
print(mod.RectArea(2, 5));
print(mod.CircleArea(2));
