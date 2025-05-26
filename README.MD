# Kalibri
Kalibri is squirrel fork which adds some improvements and features into the language

--------------------------------------------------

**Features:**
- Full unicode support. No wchars. Now you can rid of _SC("") and write code calmly without thinking about encoding. Squirrel itself converts it to `wchar_t` when interacting with Windows os and back to `char` when it finishes doing this, so unicode will work properly.
- [Sqrat](https://scrat.sourceforge.net/) tools to bind symbols to the language.
- New `import` operator which allows you to import libraries

Uses C++ 17

--------------------------------------------------
**Import operator:**

You can use the new `import` keyword to import modules:
```squirrel
import "mymodule"
```
What `import` will do:

1. Tries to load `mymodule` nut script file. If `mymodule` nut script successfully loaded, executes the script and merges (if stackbase table is roottable) or replaces (if you use syntax like `mod <- import "mymodule"`) root or `mod` table with table with global variables, declared in `mymodule` file.
2. Tries to load `mymodule.nut` script file. If successfully loaded, does the same as in first item.
3. Tries to load `mymodule.dll`. If successfully loaded, tries to load `sqmodule_load` function. If successfully loaded, calls the `sqmodule_load()`.

In other situations throws error.

--------------------------------------------------
### TODO

- [x] `import` "folder/subfolder/module"
- [ ] (?) `import` "folder/subfolder" (import all files in folder)
- [ ] (?) `from` "folder/subfolder/module" `import` symbol
- [ ] stack dump from script
- [x] module destruction
- [ ] write some sample libraries
- [ ] write tests for dll importing
- [ ] (?) export only specific table from `.nut` script module by global `return` operator
- [x] (?) export whole file root table if there is no global `return`
- [x] handle recursive import
- [ ] (?) manual module deload like `deimport` or something like that

(?) - I'm not sure if it's worth adding this
