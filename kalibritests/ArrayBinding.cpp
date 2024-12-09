//
// Copyright 2013 Li-Cheng (Andy) Tai
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

#include <gtest/gtest.h>
#include <kalibri.hpp>
#include "Fixture.h"


using namespace kb;

TEST_F(KalibriTest, ArrayGet) {
    static const SQChar *sq_code = "\
        local i; \
        for (i = 0; i < 12; i++) \
            a.append(i); \
        \
    ";
    DefaultVM::Set(vm);

    Array array(vm);
    RootTable(vm).Bind("a", array);

    Script script;
    script.CompileString(sq_code);
    if (kb::Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << kb::Error::Message(vm);
    }

    script.Run();
    if (kb::Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << kb::Error::Message(vm);
    }

    const int length = static_cast<const int>(array.Length());
    EXPECT_EQ(length, 12);
    for (int i = 0; i < length; i++)
    {
        SharedPtr<int> t = array.GetValue<int>(i);
        EXPECT_EQ(t != NULL, 1);
        EXPECT_EQ(*t, i);

        SharedPtr<float> f;
        f = array.GetValue<float>(i);
        EXPECT_EQ(f != NULL, 1);
        EXPECT_EQ((float)i, *f);
    }

    std::vector<int> t(length);
    array.GetArray(t.data(), static_cast<int>(t.size()));
    EXPECT_FALSE(kb::Error::Occurred(vm));
    for (int i = 0; i < length; i++)
    {
        EXPECT_EQ(t[i], i);
    }

    std::vector<double> d(length);
    array.GetArray(d.data(), static_cast<int>(d.size()));
    EXPECT_FALSE(kb::Error::Occurred(vm));
    for (int i = 0; i < length; i++)
    {
        EXPECT_EQ(d[i], (double)i);
    }

    double d2[15];
    array.GetArray(d2, sizeof(d2) / sizeof(d2[0]));
    EXPECT_TRUE(kb::Error::Occurred(vm));
    kb::Error::Clear(vm);

    double d3[5];
    array.GetArray(d3, sizeof(d3) / sizeof(d3[0]));
    EXPECT_FALSE(kb::Error::Occurred(vm));
}


void touch_element(kb::Array& a, int index, int val)
{
    a.SetValue(index, val);
}

void touch_element2(kb::Array a, int index, int val)
{
    a.SetValue(index, val);
}

TEST_F(KalibriTest, PassingArrayIn) {
    static const int SIZE = 56;
    static const SQChar *sq_code = "\
        local i; \
        for (i = 0; i < a.len(); i++) \
            touch_element2(a, i, 5 - i); \
        \
        for (i = 0; i < a.len(); i++) \
            gTest.EXPECT_INT_EQ(a[i], 5 - i); \
        \
        for (i = 0; i < a.len(); i++) \
            touch_element(a, i, -i); \
        \
        for (i = 0; i < a.len(); i++) \
            gTest.EXPECT_INT_EQ(a[i], -i); \
        \
    ";
    DefaultVM::Set(vm);

    RootTable().Func("touch_element", &touch_element);
    RootTable().Func("touch_element2", &touch_element2);

    int i;
    Array array(vm, SIZE);
    RootTable(vm).Bind("a", array);
    for (i = 0; i < SIZE; i++)
        touch_element(array, i, i);

    int length = static_cast<int>(array.Length());
    EXPECT_EQ(length, SIZE);
    for (i = 0; i < length; i++)
    {
        SharedPtr<int> t = array.GetValue<int>(i);
        EXPECT_EQ(t != NULL, 1);
        EXPECT_EQ(*t, i);
    }

    Script script;
    script.CompileString(sq_code);
    if (kb::Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << kb::Error::Message(vm);
    }

    script.Run();
    if (kb::Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << kb::Error::Message(vm);
    }

    length = static_cast<int>(array.Length());
    EXPECT_EQ(length, SIZE);
    for (i = 0; i < length; i++)
    {
        SharedPtr<int> t = array.GetValue<int>(i);
        EXPECT_EQ(t != NULL, 1);
        EXPECT_EQ(*t, -i);
    }
}


TEST_F(KalibriTest, PassingArrayIn2) {
    static const int SIZE = 56;
    static const SQChar *sq_code = "\
        local i; \
        local a2 = array(12); \
        for (i = 0; i < a2.len(); i++) \
            touch_element2(a2, i, 1 - i); \
        \
        for (i = 0; i < a2.len(); i++) \
            gTest.EXPECT_INT_EQ(a2[i], 1 - i); \
        \
        for (i = 0; i < a2.len(); i++) \
            touch_element(a2, i, 1 + i); \
        \
        for (i = 0; i < a2.len(); i++) \
            gTest.EXPECT_INT_EQ(a2[i], 1 + i); \
        \
    ";
    DefaultVM::Set(vm);

    RootTable().Func("touch_element2", &touch_element2);
    RootTable().Func("touch_element", &touch_element);

    Script script;
    script.CompileString(sq_code);
    if (kb::Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << kb::Error::Message(vm);
    }

    script.Run();
    if (kb::Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << kb::Error::Message(vm);
    }
}
