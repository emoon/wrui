require "tundra.syntax.glob"
local native = require('tundra.native')

local win64 = {
    Env = {
		QT5 = native.getenv("QT5_WIN32", ""),
        GENERATE_PDB = "1",
        CCOPTS = {
            "/FS", "/W4", "/WX", "/I.",
            { "/Od"; Config = "*-*-debug" },
            { "/O2"; Config = "*-*-release" },
        },
        CXXOPTS = {
            "/W4", "/WX", "/I.",
            { "/Od"; Config = "*-*-debug" },
            { "/O2"; Config = "*-*-release" },
        },
        PROGOPTS = {
            "/INCREMENTAL:NO" -- Disable incremental linking. It doesn't work properly in our use case (nearly all code in libs) and causes log spam.
        },

    },

    ReplaceEnv = {
        ["OBJCCOM"] = "meh",
    },
}

local macosx = {
    Env = {
    	-- TODO: Query this from qmake
    	QT5="/Users/danielcollin/code/other/Qt/5.7/clang_64";
        CCOPTS = {
            "-Weverything", "-I.", "-Werror",
            { "-O0", "-g"; Config = "*-*-debug" },
            { "-O3"; Config = "*-*-release" },
        },
        CXXOPTS = {
            "-std=c++11", -- "-Weverything", "-I.", "-Werror"
        },
    },
    Frameworks = { "Cocoa" },
}

local linux = {
    Env = {
        CCOPTS = {
            "-Wpedantic", "-Werror", "-I.", "-Wall",
            { "-O0", "-g"; Config = "*-*-debug" },
            { "-O3"; Config = "*-*-release" },
        },
    },
}

Build {
    Configs = {
        Config { Name = "macosx-clang", DefaultOnHost = "macosx", Inherit = macosx, Tools = { "clang-osx" } },
        Config { Name = "win64-msvc", DefaultOnHost = { "windows" }, Inherit = win64, Tools = { "msvc",  } },
        Config { Name = "linux-gcc", DefaultOnHost = { "linux" }, Inherit = linux, Tools = { "gcc", } },
    },

    Units = {
        "units.lua",
    },
}
