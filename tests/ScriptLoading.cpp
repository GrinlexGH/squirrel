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

TEST_F(KalibriTest, LoadScriptFromString) {
    //
    // Compile and run from string
    //

    DefaultVM::Set(vm);

    Script script;
    script.CompileString(_SC(" \
        x <- 1 + 2; \
        gTest.EXPECT_STR_EQ(x, 3); \
        "));
    if (kb::Error::Occurred(vm)) {
        FAIL() << _SC("Script Compile Failed: ") << kb::Error::Message(vm);
    }

    script.Run();
    if (kb::Error::Occurred(vm)) {
        FAIL() << _SC("Script Run Failed: ") << kb::Error::Message(vm);
    }
}

TEST_F(KalibriTest, LoadScriptFromFile) {
    //
    // Compile and run from file
    //

    DefaultVM::Set(vm);

    Script script;
    script.CompileFile(_SC("scripts/hello.nut"));
    if (kb::Error::Occurred(vm)) {
        FAIL() << _SC("Script Compile Failed: ") << kb::Error::Message(vm);
    }

    script.Run();
    if (kb::Error::Occurred(vm)) {
        FAIL() << _SC("Script Run Failed: ") << kb::Error::Message(vm);
    }
}

TEST_F(KalibriTest, LoadScriptBytecode) {
    //
    // Compile script, load script from bytecode and run it
    //

    DefaultVM::Set(vm);

    std::string bytecode;

    {
        Script script;
        script.CompileString(_SC("x <- 40 + 2;"));
        bytecode = script.SaveBytecode();
        if (bytecode.empty()) {
            FAIL() << _SC("Script SaveBytecode Failed!");
        }
    }

    {
        Script script;
        if (!script.LoadBytecode(bytecode)) {
            FAIL() << _SC("Script LoadBytecode Failed!");
        }
        std::string errMsg;
        if (!script.Run(errMsg)) {
            FAIL() << _SC("Script Run Failed: ") << errMsg;
        }
    }
}
