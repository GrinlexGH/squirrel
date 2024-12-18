Kalibri is squirrel fork which adds some improvements and features into the language

--------------------------------------------------

Features:
- Full unicode support. No wchars. Now you can rid of _SC("") and write code calmly without thinking about encoding. Squirrel itself converts it to `wchar_t` when interacting with Windows os and back to `char` when it finishes doing this, so unicode will work properly.
- [Scrat](https://scrat.sourceforge.net/) tools to bind symbols to the language.

Uses C++ 17

### TODO

- [ ] `import` folder.subfolder.module
- [ ] `import` folder.subfolder (import all files in folder)
- [ ] `from` folder.subfolder.module `import` symbol
- [ ] `from` folder.subfolder `import` module
- [ ] `import` `as`
- [ ] stack dump from script
- [x] Module destruction
- [ ] Write some sample libraries
- [ ] export only specific table from `.nut` script module by global `return` operator
- [ ] export whole file root table if there is no global `return`
