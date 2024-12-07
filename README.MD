Kalibri is based on SQUIRREL 3.2 stable

--------------------------------------------------

Features:
1. Full unicode support. No wchars. Now you can rid of _SC("") and write code calmly without thinking about encoding. Squirrel itself converts it to `wchar_t` when interacting with Windows os and back to `char` when it finishes doing this, so unicode will work properly.
2. [Scrat](https://scrat.sourceforge.net/) tools to bind symbols to the language.

Uses C++ 17
