Kalibri is squirrel fork which adds some improvements and features into the language

--------------------------------------------------

Features:
- Full unicode support. No wchars. Now you can rid of _SC("") and write code calmly without thinking about encoding. Squirrel itself converts it to `wchar_t` when interacting with Windows os and back to `char` when it finishes doing this, so unicode will work properly.
- [Scrat](https://scrat.sourceforge.net/) tools to bind symbols to the language.

Uses C++ 17

### TODO

- [ ] `import` `from` `as`
- [x] Module destruction
- [ ] Write some sample libraries
