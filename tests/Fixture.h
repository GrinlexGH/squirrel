#if !defined(KB_TEST_FIXTURE_H)
#define KB_TEST_FIXTURE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <squirrel.h>
#include <sqstdio.h>
#include <kalibri.hpp>
#include <kalibri/kbunicode.hpp>
#include <gtest/gtest.h>

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace kb {

class KalibriTest : public ::testing::Test {
protected:
    HSQUIRRELVM vm;

    virtual void SetUp() {
#if _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
#endif

        vm = sq_open(1024);
#if SQUIRREL_VERSION_NUMBER >= 300
        sq_setprintfunc(vm, printfunc, printfunc);
#else
        sq_setprintfunc(vm, printfunc);
#endif

        sq_newclosure(vm, errorhandler,0);
        sq_seterrorhandler(vm);

        BindTestFunctions();
    }

    virtual void TearDown() {
        sq_close(vm);
    }

    static void printfunc(HSQUIRRELVM v,const SQChar *s,...) {
        va_list vl;
        va_start(vl, s);
        vprintf(s, vl);
        va_end(vl);
    }

    static SQInteger errorhandler(HSQUIRRELVM v) {
        const SQChar *sErr = 0;
        if(sq_gettop(v)>=1) {
            if(SQ_SUCCEEDED(sq_getstring(v,2,&sErr)))   {
                ADD_FAILURE() << _SC("A Script Error Occured: ") << sErr;
            }
            else {
                ADD_FAILURE() << _SC("An Unknown Script Error Occured.") << sErr;
            }
        }
        return 0;
    }

    // Test Macros

    // Assert
    static void SQ_ASSERT_TRUE(bool a) {
        ASSERT_TRUE(a);
    }

    static void SQ_ASSERT_FALSE(bool a) {
        ASSERT_FALSE(a);
    }

    static void SQ_ASSERT_INT_EQ(int a, int b) {
        ASSERT_EQ(a, b);
    }

    static void SQ_ASSERT_INT_NE(int a, int b) {
        ASSERT_NE(a, b);
    }

    static void SQ_ASSERT_FLOAT_EQ(float a, float b) {
        ASSERT_FLOAT_EQ(a, b);
    }

    static void SQ_ASSERT_FLOAT_NE(float a, float b) {
        ASSERT_NE(a, b);
    }

    static void SQ_ASSERT_STR_EQ(const string& a, const string& b) {
        ASSERT_STREQ(a.c_str(), b.c_str());
    }

    static void SQ_ASSERT_STR_NE(const string& a, const string& b) {
        ASSERT_STRNE(a.c_str(), b.c_str());
    }

    // Expect
    static void SQ_EXPECT_TRUE(bool a) {
        EXPECT_TRUE(a);
    }

    static void SQ_EXPECT_FALSE(bool a) {
        EXPECT_FALSE(a);
    }

    static void SQ_EXPECT_INT_EQ(int a, int b) {
        EXPECT_EQ(a, b);
    }

    static void SQ_EXPECT_INT_NE(int a, int b) {
        EXPECT_NE(a, b);
    }

    static void SQ_EXPECT_FLOAT_EQ(float a, float b) {
        EXPECT_FLOAT_EQ(a, b);
    }

    static void SQ_EXPECT_FLOAT_NE(float a, float b) {
        EXPECT_NE(a, b);
    }

    static void SQ_EXPECT_STR_EQ(string a, string b) {
        EXPECT_STREQ(a.c_str(), b.c_str());
    }

    static void SQ_EXPECT_STR_NE(string a, string b) {
        EXPECT_STRNE(a.c_str(), b.c_str());
    }

    void BindTestFunctions() {

        RootTable(vm).Bind(_SC("gTest"), Table(vm)
                           .Func(_SC("ASSERT_TRUE"), &SQ_ASSERT_TRUE)
                           .Func(_SC("ASSERT_FALSE"), &SQ_ASSERT_FALSE)
                           .Func(_SC("ASSERT_INT_EQ"), &SQ_ASSERT_INT_EQ)
                           .Func(_SC("ASSERT_INT_NE"), &SQ_ASSERT_INT_NE)
                           .Func(_SC("ASSERT_FLOAT_EQ"), &SQ_ASSERT_FLOAT_EQ)
                           .Func(_SC("ASSERT_FLOAT_NE"), &SQ_ASSERT_FLOAT_NE)
                           .Func(_SC("ASSERT_STR_EQ"), &SQ_ASSERT_STR_EQ)
                           .Func(_SC("ASSERT_STR_NE"), &SQ_ASSERT_STR_NE)

                           .Func(_SC("EXPECT_TRUE"), &SQ_EXPECT_TRUE)
                           .Func(_SC("EXPECT_FALSE"), &SQ_EXPECT_FALSE)
                           .Func(_SC("EXPECT_INT_EQ"), &SQ_EXPECT_INT_EQ)
                           .Func(_SC("EXPECT_INT_NE"), &SQ_EXPECT_INT_NE)
                           .Func(_SC("EXPECT_FLOAT_EQ"), &SQ_EXPECT_FLOAT_EQ)
                           .Func(_SC("EXPECT_FLOAT_NE"), &SQ_EXPECT_FLOAT_NE)
                           .Func(_SC("EXPECT_STR_EQ"), &SQ_EXPECT_STR_EQ)
                           .Func(_SC("EXPECT_STR_NE"), &SQ_EXPECT_STR_NE)
                          );

    }
};

}

#endif
