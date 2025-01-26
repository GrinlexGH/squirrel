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

#include <gtest/gtest.h>
#include <kalibri.hpp>
#include "Fixture.h"

using namespace kb;

TEST_F(KalibriTest, ImportScript) {
    DefaultVM::Set(vm);

    Script script;
    script.CompileString(" \
        import \"scripts/samplemodule\"\n \
        \
        gTest.EXPECT_FLOAT_EQ(3.1415, ::PI); \
        gTest.EXPECT_INT_EQ(10, ::RectArea(2, 5)); \
        gTest.EXPECT_FLOAT_EQ(12.566, ::CircleArea(2)); \
    ");
    if (Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << Error::Message(vm);
    }

    script.Run();
    if (Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << Error::Message(vm);
    }
}

TEST_F(KalibriTest, ImportScriptIntoTable) {
    DefaultVM::Set(vm);

    Script script;
    script.CompileString(" \
        import \"scripts/samplemodule\" as mod; \
        \
        gTest.EXPECT_FLOAT_EQ(3.1415, mod.PI); \
        gTest.EXPECT_INT_EQ(10, mod.RectArea(2, 5)); \
        gTest.EXPECT_FLOAT_EQ(12.566, mod.CircleArea(2)); \
    ");
    if (Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << Error::Message(vm);
    }

    script.Run();
    if (Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << Error::Message(vm);
    }
}

TEST_F(KalibriTest, RecursiveImport) {
    DefaultVM::Set(vm);

    Script script;
    script.CompileFile("scripts/a.nut");
    if (Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << Error::Message(vm);
    }

    script.Run();
    if (Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << Error::Message(vm);
    }
}

TEST_F(KalibriTest, ImportConsts) {
    DefaultVM::Set(vm);

    Script script;
    script.CompileString(" \
        import \"scripts/const\" as mod; \
        \
        gTest.EXPECT_INT_EQ(3, a); \
    ");
    if (Error::Occurred(vm)) {
        FAIL() << "Compile Failed: " << Error::Message(vm);
    }

    script.Run();
    if (Error::Occurred(vm)) {
        FAIL() << "Run Failed: " << Error::Message(vm);
    }
}
