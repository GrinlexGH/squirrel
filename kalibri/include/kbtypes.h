//
// kbTypes: Type Translators
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_KALIBRI_TYPES_H_)
#define _KALIBRI_TYPES_H_

#include <squirrel.h>
#include <string>
#include <cstddef> // std::nullptr_t

#include "kbclasstype.h"
#include "kbutil.h"

namespace kb {

/// @cond DEV

// copied from http://www.experts-exchange.com/Programming/Languages/CPP/A_223-Determing-if-a-C-type-is-convertable-to-another-at-compile-time.html
template <typename T1, typename T2>
struct is_convertible
{
private:
    struct True_ { char x[2]; };
    struct False_ { };

    static True_ helper(T2 const &);
    static False_ helper(...);

    static T1* dummy;

public:
    static bool const YES = (
        sizeof(True_) == sizeof(is_convertible::helper(*dummy))
    );
};

template <typename T, bool b>
struct popAsInt
{
    T value;
    popAsInt(HSQUIRRELVM vm, SQInteger idx)
    {
        SQObjectType value_type = sq_gettype(vm, idx);
        switch(value_type) {
        case OT_BOOL:
            SQBool sqValueb;
            sq_getbool(vm, idx, &sqValueb);
            value = static_cast<T>(sqValueb);
            break;
        case OT_INTEGER:
            SQInteger sqValue;
            sq_getinteger(vm, idx, &sqValue);
            value = static_cast<T>(sqValue);
            break;
        case OT_FLOAT:
            SQFloat sqValuef;
            sq_getfloat(vm, idx, &sqValuef);
            value = static_cast<T>(static_cast<int>(sqValuef));
            break;
        default:
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("integer")));
            value = static_cast<T>(0);
            break;
        }
    }
};

template <typename T>
struct popAsInt<T, false>
{
    T value;  // cannot be initialized because unknown constructor parameters
    popAsInt(HSQUIRRELVM /*vm*/, SQInteger /*idx*/)
    {
        // keep the current error message already set previously, do not touch that here
    }
};

template <typename T>
struct popAsFloat
{
    T value;
    popAsFloat(HSQUIRRELVM vm, SQInteger idx)
    {
        SQObjectType value_type = sq_gettype(vm, idx);
        switch(value_type) {
        case OT_BOOL:
            SQBool sqValueb;
            sq_getbool(vm, idx, &sqValueb);
            value = static_cast<T>(sqValueb);
            break;
        case OT_INTEGER:
            SQInteger sqValue; \
            sq_getinteger(vm, idx, &sqValue);
            value = static_cast<T>(sqValue);
            break;
        case OT_FLOAT:
            SQFloat sqValuef;
            sq_getfloat(vm, idx, &sqValuef);
            value = static_cast<T>(sqValuef);
            break;
        default:
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("float")));
            value = 0;
            break;
        }
    }
};

/// @endcond

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as copies
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/// \remarks
/// This specialization requires T to have a default constructor.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var {

    T value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQTRY()
        T* ptr = ClassType<T>::GetInstance(vm, idx);
        if (ptr != NULL) {
            value = *ptr;
#if !defined (KALIBRI_NO_ERROR_CHECKING)
        } else if (std::is_convertible_v<T, SQInteger>) { /* value is likely of integral type like enums */
            SQCLEAR(vm); // clear the previous error
            value = popAsInt<T, std::is_convertible_v<T, SQInteger>>(vm, idx).value;
#endif
        } else {
            // initialize value to avoid warnings
            value = popAsInt<T, std::is_convertible_v<T, SQInteger>>(vm, idx).value;
        }
        SQCATCH(vm) {
#if defined (KALIBRI_USE_EXCEPTIONS)
            SQUNUSED(e); // avoid "unreferenced local variable" warning
#endif
            if (std::is_convertible_v<T, SQInteger>) { /* value is likely of integral type like enums */
                value = popAsInt<T, std::is_convertible_v<T, SQInteger>>(vm, idx).value;
            } else {
                SQRETHROW(vm);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T& val) {
        if (ClassType<T>::hasClassData(vm))
            ClassType<T>::PushInstanceCopy(vm, val);
        else /* try integral type */
            pushAsInt<T, std::is_convertible_v<T, SQInteger>>().push(vm, val);
    }

private:

    template <class T2, bool b>
    struct pushAsInt {
        void push(HSQUIRRELVM vm, const T2& /*value*/) {
            assert(false); // fails because called before a kb::Class for T exists and T is not convertible to SQInteger
            sq_pushnull(vm);
        }
    };

    template <class T2>
    struct pushAsInt<T2, true> {
        void push(HSQUIRRELVM vm, const T2& val) {
            sq_pushinteger(vm, static_cast<SQInteger>(val));
        }
    };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as references
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<T&> {

    T& value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(*ClassType<T>::GetInstance(vm, idx)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVarR to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, T& value) {
        if (ClassType<T>::hasClassData(vm))
            ClassType<T>::PushInstance(vm, &value);
        else /* try integral type */
            pushAsInt<T, std::is_convertible_v<T, SQInteger>>().push(vm, value);
    }

private:

    template <class T2, bool b>
    struct pushAsInt {
        void push(HSQUIRRELVM vm, const T2& /*value*/) {
            assert(false); // fails because called before a kb::Class for T exists and T is not convertible to SQInteger
            sq_pushnull(vm);
        }
    };

    template <class T2>
    struct pushAsInt<T2, true> {
        void push(HSQUIRRELVM vm, const T2& val) {
            sq_pushinteger(vm, static_cast<SQInteger>(val));
        }
    };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as pointers
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<T*> {

    T* value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, T* val) {
        ClassType<T>::PushInstance(vm, val);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as pointers to const data
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<T* const> {

    T* const value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, T* const val) {
        ClassType<T>::PushInstance(vm, val);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as const references
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<const T&> {

    const T& value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(*ClassType<T>::GetInstance(vm, idx)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T& val) {
        ClassType<T>::PushInstanceCopy(vm, val);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as const pointers
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<const T*> {

    const T* value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T* val) {
        ClassType<T>::PushInstance(vm, const_cast<T*>(val));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push class instances to and from the stack as const pointers to const data
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct Var<const T* const> {

    const T* const value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) : value(ClassType<T>::GetInstance(vm, idx, true)) {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const T* const val) {
        ClassType<T>::PushInstance(vm, const_cast<T*>(val));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get (as copies) and push (as references) class instances to and from the stack as a SharedPtr
///
/// \tparam T Type of instance (usually doesnt need to be defined explicitly)
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void PushVarR(HSQUIRRELVM vm, T& value);
template<class T>
struct Var<SharedPtr<T> > {

    SharedPtr<T> value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as the given type
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        if (sq_gettype(vm, idx) != OT_NULL) {
            Var<T> instance(vm, idx);
            SQCATCH_NOEXCEPT(vm) {
                return;
            }
            value.Init(new T(instance.value));
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a class object on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SharedPtr<T>& val) {
        PushVarR(vm, *val);
    }
};

// Integer types
#define KALIBRI_INTEGER( type ) \
 template<> \
 struct Var<type> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsInt<type, true>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushinteger(vm, static_cast<SQInteger>(value)); \
     } \
 };\
 \
 template<> \
 struct Var<const type&> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsInt<type, true>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushinteger(vm, static_cast<SQInteger>(value)); \
     } \
 };

KALIBRI_INTEGER(unsigned int)
KALIBRI_INTEGER(signed int)
KALIBRI_INTEGER(unsigned long)
KALIBRI_INTEGER(signed long)
KALIBRI_INTEGER(unsigned short)
KALIBRI_INTEGER(signed short)
KALIBRI_INTEGER(unsigned char)
KALIBRI_INTEGER(signed char)
KALIBRI_INTEGER(unsigned long long)
KALIBRI_INTEGER(signed long long)

#ifdef _MSC_VER
#if defined(__int64)
KALIBRI_INTEGER(unsigned __int64)
KALIBRI_INTEGER(signed __int64)
#endif
#endif

// Float types
#define KALIBRI_FLOAT( type ) \
 template<> \
 struct Var<type> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsFloat<type>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushfloat(vm, static_cast<SQFloat>(value)); \
     } \
 }; \
 \
 template<> \
 struct Var<const type&> { \
     type value; \
     Var(HSQUIRRELVM vm, SQInteger idx) { \
         value = popAsFloat<type>(vm, idx).value; \
     } \
     static void push(HSQUIRRELVM vm, const type& value) { \
         sq_pushfloat(vm, static_cast<SQFloat>(value)); \
     } \
 };

KALIBRI_FLOAT(float)
KALIBRI_FLOAT(double)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push nullptr to and from the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<std::nullptr_t> {

    std::nullptr_t value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as an nullptr
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
#if !defined (KALIBRI_NO_ERROR_CHECKING)
        SQObjectType value_type = sq_gettype(vm, idx);
        if (value_type != OT_NULL) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("null")));
        }
#endif
        value = nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put an nullptr on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const std::nullptr_t value) {
        sq_pushnull(vm);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push bools to and from the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<bool> {

    bool value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a bool
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQBool sqValue;
        sq_tobool(vm, idx, &sqValue);
        value = (sqValue != 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a bool on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const bool& val) {
        sq_pushbool(vm, static_cast<SQBool>(val));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const bool references to and from the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const bool&> {

    bool value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a bool
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQBool sqValue;
        sq_tobool(vm, idx, &sqValue);
        value = (sqValue != 0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a bool on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const bool& val) {
        sq_pushbool(vm, static_cast<SQBool>(val));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push SQChar arrays to and from the stack (usually is a char array)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<SQChar*> {
private:

    HSQOBJECT obj; /* hold a reference to the object holding value during the Var struct lifetime*/
    HSQUIRRELVM v;

public:

    SQChar* value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a character array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        sq_tostring(vm, idx);
        sq_getstackobj(vm, -1, &obj);
        sq_getstring(vm, -1, (const SQChar**)&value);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
        v = vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Var()
    {
        if(v && !sq_isnull(obj)) {
            sq_release(v, &obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a character array on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    /// \param len   Length of the string (defaults to finding the length by searching for a terminating null-character)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SQChar* val, SQInteger len = -1) {
        sq_pushstring(vm, val, len);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const SQChar arrays to and from the stack (usually is a const char array)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const SQChar*> {
private:

    HSQOBJECT obj; /* hold a reference to the object holding value during the Var struct lifetime*/
    HSQUIRRELVM v;

public:

    const SQChar* value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a character array
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        sq_tostring(vm, idx);
        sq_getstackobj(vm, -1, &obj);
        sq_getstring(vm, -1, &value);
        sq_addref(vm, &obj);
        sq_pop(vm,1);
        v = vm;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ~Var()
    {
        if(v && !sq_isnull(obj)) {
            sq_release(v, &obj);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a character array on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    /// \param len   Length of the string (defaults to finding the length by searching for a terminating null-character)
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SQChar* val, SQInteger len = -1) {
        sq_pushstring(vm, val, len);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push strings to and from the stack (string is usually std::string)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<string> {

    string value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a string
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        const SQChar* ret;
        sq_tostring(vm, idx);
        sq_getstring(vm, -1, &ret);
        value = string(ret, sq_getsize(vm, -1));
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a string on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const string& val) {
        sq_pushstring(vm, val.c_str(), val.size());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push const string references to and from the stack as copies (strings are always copied)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<const string&> {

    string value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a string
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        const SQChar* ret;
        sq_tostring(vm, idx);
        sq_getstring(vm, -1, &ret);
        value = string(ret, sq_getsize(vm, -1));
        sq_pop(vm,1);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a string on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const string& val) {
        sq_pushstring(vm, val.c_str(), val.size());
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to get and push SQUserPointer to and from the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct Var<SQUserPointer> {

    SQUserPointer value; ///< The actual value of get operations

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Attempts to get the value off the stack at idx as a SQUserPointer
    ///
    /// \param vm  Target VM
    /// \param idx Index trying to be read
    ///
    /// \remarks
    /// This function MUST have its Error handled if it occurred.
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Var(HSQUIRRELVM vm, SQInteger idx) {
        SQUserPointer sqValue;
#if !defined (KALIBRI_NO_ERROR_CHECKING)
        if (SQ_FAILED(sq_getuserpointer(vm, idx, &sqValue))) {
            SQTHROW(vm, FormatTypeError(vm, idx, _SC("userpointer")));
        }
#else
        sq_getuserpointer(vm, idx, &sqValue);
#endif
        value = sqValue;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Called by kb::PushVar to put a SQUserPointer on the stack
    ///
    /// \param vm    Target VM
    /// \param value Value to push on to the VM's stack
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void push(HSQUIRRELVM vm, const SQUserPointer value) {
        sq_pushuserpointer(vm, value);
    }
};

// Non-referencable type definitions
template<class T> struct is_referencable {static const bool value = true;};

#define KALIBRI_MAKE_NONREFERENCABLE( type ) \
 template<> struct is_referencable<type> {static const bool value = false;};

KALIBRI_MAKE_NONREFERENCABLE(unsigned int)
KALIBRI_MAKE_NONREFERENCABLE(signed int)
KALIBRI_MAKE_NONREFERENCABLE(unsigned long)
KALIBRI_MAKE_NONREFERENCABLE(signed long)
KALIBRI_MAKE_NONREFERENCABLE(unsigned short)
KALIBRI_MAKE_NONREFERENCABLE(signed short)
KALIBRI_MAKE_NONREFERENCABLE(unsigned char)
KALIBRI_MAKE_NONREFERENCABLE(signed char)
KALIBRI_MAKE_NONREFERENCABLE(unsigned long long)
KALIBRI_MAKE_NONREFERENCABLE(signed long long)
KALIBRI_MAKE_NONREFERENCABLE(float)
KALIBRI_MAKE_NONREFERENCABLE(double)
KALIBRI_MAKE_NONREFERENCABLE(bool)
KALIBRI_MAKE_NONREFERENCABLE(string)

#ifdef _MSC_VER
#if defined(__int64)
KALIBRI_MAKE_NONREFERENCABLE(unsigned __int64)
KALIBRI_MAKE_NONREFERENCABLE(signed __int64)
#endif
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a value on to a given VM's stack
///
/// \param vm    VM that the variable will be pushed on to the stack of
/// \param value The actual value being pushed
///
/// \tparam T Type of value (usually doesnt need to be defined explicitly)
///
/// \remarks
/// What this function does is defined by kalibri::Var template specializations,
/// and thus you can create custom functionality for it by making new template specializations.
/// When making a custom type that is not referencable, you must use KALIBRI_MAKE_NONREFERENCABLE( type )
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVar(HSQUIRRELVM vm, T* value) {
    Var<T*>::push(vm, value);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a value on to a given VM's stack
///
/// \param vm    VM that the variable will be pushed on to the stack of
/// \param value The actual value being pushed
///
/// \tparam T Type of value (usually doesnt need to be defined explicitly)
///
/// \remarks
/// What this function does is defined by kalibri::Var template specializations,
/// and thus you can create custom functionality for it by making new template specializations.
/// When making a custom type that is not referencable, you must use KALIBRI_MAKE_NONREFERENCABLE( type )
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVar(HSQUIRRELVM vm, const T& value) {
    Var<T>::push(vm, value);
}


/// @cond DEV
template<class T, bool b>
struct PushVarR_helper {
    inline static void push(HSQUIRRELVM vm, T value) {
        PushVar<T>(vm, value);
    }
};
template<class T>
struct PushVarR_helper<T, false> {
    inline static void push(HSQUIRRELVM vm, const T& value) {
        PushVar<const T&>(vm, value);
    }
};
/// @endcond


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pushes a reference on to a given VM's stack (some types cannot be referenced and will be copied instead)
///
/// \param vm    VM that the reference will be pushed on to the stack of
/// \param value The actual referenced value being pushed
///
/// \tparam T Type of value (usually doesnt need to be defined explicitly)
///
/// \remarks
/// What this function does is defined by kalibri::Var template specializations,
/// and thus you can create custom functionality for it by making new template specializations.
/// When making a custom type that is not referencable, you must use KALIBRI_MAKE_NONREFERENCABLE( type )
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void PushVarR(HSQUIRRELVM vm, T& value) {
    if (!is_pointer<T>::value && is_referencable<typename remove_cv<T>::type>::value) {
        Var<T&>::push(vm, value);
    } else {
        PushVarR_helper<T, is_pointer<T>::value>::push(vm, value);
    }
}

}

#endif
