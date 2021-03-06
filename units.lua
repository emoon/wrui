-- vim: expandtab ts=4 sw=4

local function get_src(dir, recursive)
    return FGlob {
        Dir = dir,
        Extensions = { ".cpp", ".c", ".h", ".s", ".m", ".sl" },
        Filters = {
            { Pattern = "[/\\]test[/\\]"; Config = "test-*" }, -- Directories named "test" and their subdirectories will be excluded from builds
            { Pattern = "%.s$"; Config = "amiga-*" },
            { Pattern = "[/\\]amiga[/\\]"; Config = "amiga-*" },
            { Pattern = "[/\\]windows[/\\]"; Config = "win32-*" },
            { Pattern = "[/\\]win32[/\\]"; Config = "win32-*" },
            { Pattern = "[/\\]mac[/\\]"; Config = "mac*-*" },
            { Pattern = "[/\\]linux[/\\]"; Config = "linux*-*" },
        },
        Recursive = recursive and true or false,
    }
end

-- Used to generate the moc cpp files as needed for .h that uses Q_OBJECT for QtTool(s)

DefRule {
	Name = "MocGeneration",
	Pass = "CodeGeneration",
	Command = "$(QT5)/bin/moc $(<) -o $(@)",

	Blueprint = {
		Source = { Required = true, Type = "string", Help = "Input filename", },
		OutName = { Required = true, Type = "string", Help = "Output filename", },
	},

	Setup = function (env, data)
		return {
			InputFiles    = { data.Source },
			OutputFiles   = { "$(OBJECTDIR)/_generated/" .. data.OutName },
		}
	end,
}

-- Used to send a list of header files

local function MocGenerationMulti(sources)
 local result = {}
 for _, src in ipairs(tundra.util.flatten(sources)) do
   result[#result + 1] = MocGeneration { Source = src, OutName = tundra.path.get_filename_base(src) .. "_moc.cpp" }
 end
 return result
end

StaticLibrary {
    Name = "glfw",

    Env = {
        CPPPATH = {
            "src/dear_imgui/external/glfw/src",
            "src/dear_imgui/external/glfw/include",
        },

        CPPDEFS = {
            { "_GLFW_WIN32", "_GLFW_WGL", "WIN32"; Config = "win64-*-*" },
            { "_GLFW_X11", "_GLFW_GFX", "LINUX"; Config = "linux-*-*" },
            { "_GLFW_NSGL", "MACOSX", "GLFW_INCLUDE_GLCOREARB", "_GLFW_COCOA", "GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED"; Config = "macosx-*-*" },
        },

        CCOPTS = { {
            "-Wno-reserved-id-macro", "-Wno-documentation", "-Wno-padded", "-Wno-double-promotion",
            "-Wno-documentation-unknown-command", "-Wno-missing-prototypes", "-Wno-cast-qual",
            "-Wno-extra-semi", "-Wno-format-nonliteral", "-Wno-sign-conversion", "-Wno-assign-enum", "-Wno-conversion",
            "-Wno-float-equal", "-Wno-strict-selector-match", "-Wno-direct-ivar-access", "-Wno-objc-interface-ivars",
            "-Wno-deprecated-declarations", "-Wno-bad-function-cast", "-Wno-sign-compare", "-Wno-shadow",
            "-Wno-selector", "-Wno-unused-macros", "-Wno-undeclared-selector", "-Wno-unused-parameter"; Config = "macosx-*-*" },
        },
    },

    Sources = {
        get_src("src/dear_imgui/external/glfw/src", true)
    },
}

StaticLibrary {
    Name = "imgui",

    Env = {
        CXXOPTS = { {
            "-Wno-c++11-long-long", "-Wno-sign-conversion", "-Wno-conversion", "-Wno-format-nonliteral", "-Wno-float-equal",
            "-Wno-global-constructors", "-Wno-sign-compare", "-Wno-shadow", "-Wno-old-style-cast",
            "-Wno-cast-align", "-Wno-missing-variable-declarations", "-Wno-disabled-macro-expansion", "-Wno-unused-parameter"; Config = "macosx-*-*" },
        },
    },

    Sources = {
        get_src("src/dear_imgui/external/imgui")
    },
}

SharedLibrary {
    Name = "wrui_dimgui",

    Env = {
        CPPPATH = {
            "src/dear_imgui/external/imgui",
            "src/dear_imgui/external/glfw/include",
        },
    },

    Sources = {
        get_src("src/dear_imgui/src")
    },

    Libs = { { "stdc++"; Config = { "linux-*-*", "mac*-*-*" }, }, },

    Frameworks = { "Cocoa", "OpenGL", "IOKit" },

    Depends = { "glfw", "imgui" },
}

SharedLibrary {
    Name = "wrui_qt",

    Env = {
        CPPPATH = {
            {
              "$(QT5)/lib/QtWidgets.framework/Headers",
              "$(QT5)/lib/QtGui.framework/Headers",
              "$(QT5)/lib/QtCore.framework/Headers"; Config = "macosx-*-*" },
        },

        CXXOPTS = {
            { "-F$(QT5)/lib"; Config = "macosx-*-*" },
        },

        SHLIBCOM = {
            {  "-Wl,-rpath,$(QT5)/lib", "-F$(QT5)/lib", "-lstdc++", Config = "macosx-clang-*" },
            {  "-Wl,-rpath,$(QT5)/lib", "-lstdc++", "-lm", Config = "linux-*-*" },
        },

        FRAMEWORKS = { "QtCore", "QtWidgets", "QtGui" },
    },

    Sources = {
		MocGenerationMulti {
			Glob {
				Dir = "src/qt",
				Extensions = { ".h" }
			},
		},

        get_src("src/qt")
    },
}

--[[
Program {
    Name = "button_dear_imgui",
    Sources = { "examples/button.c" },
    Libs = { { "opengl32.lib" ; Config = { "win32-*-*", "win64-*-*" } }, },
    Depends = { "wrui_dimgui", "glfw", "imgui" },
    Frameworks = { "Cocoa", "OpenGL", "IOKit" },
    Libs = { { "stdc++"; Config = { "linux-*-*", "mac*-*-*" }, }, },
}

Program {
    Name = "button_qt",

    Env = {
        PROGCOM = {
            { "-F$(QT5)/lib", "-lstdc++", "-rpath tundra-output$(SEP)macosx-clang-debug-default"; Config = "macosx-clang-*" },
        },
    },

    Sources = { "examples/button.c" },

    Depends = { "wrui_qt" },
    Frameworks = { "Cocoa", "QtCore", "QtWidgets", "QtGui" },
    Libs = { { "stdc++"; Config = { "linux-*-*", "mac*-*-*" }, }, },
}
--]]

Program {
    Name = "basic",
    Sources = { "examples/basic.c" },
}

-- Default "wrui_dimgui"
Default "wrui_qt"
Default "basic"

-- Default "button_dear_imgui"
--Default "button_qt"

