-- Lua lib
project "LuaLibrary"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    systemversion "latest"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "src/lapi.c",
        "src/lauxlib.c",
        "src/lbaselib.c",
        "src/lcode.c",
        "src/lcorolib.c",
        "src/lctype.c",
        "src/ldblib.c",
        "src/ldebug.c",
        "src/ldo.c",
        "src/ldump.c",
        "src/lfunc.c",
        "src/lgc.c",
        "src/linit.c",
        "src/liolib.c",
        "src/llex.c",
        "src/lmathlib.c",
        "src/lmem.c",
        "src/loadlib.c",
        "src/lobject.c",
        "src/lopcodes.c",
        "src/loslib.c",
        "src/lparser.c",
        "src/lstate.c",
        "src/lstring.c",
        "src/lstrlib.c",
        "src/ltable.c",
        "src/ltablib.c",
        "src/ltm.c",
        "src/lundump.c",
        "src/lutf8lib.c",
        "src/lvm.c",
        "src/lzio.c",
    }
    
    includedirs
    {
        "src",
    }

    -- Platform specific configurations
    filter "system:linux"
        defines { "LUA_USE_LINUX" }
        links { "dl" }
        linkoptions { "-Wl,-E" }

    filter "system:emscripten"
        defines { "LUA_USE_LINUX" }
        links { "dl" }
        
        buildoptions {
            "-s WASM=1",
            "-matomics",
            "-mbulk-memory"
        }
        linkoptions { 
            "-Wl,-E",
            "-s WASM=1"
        }

    filter "system:macosx"
        defines { "LUA_USE_MACOSX", "LUA_USE_READLINE" }
        links { "readline" }
    
    filter "system:bsd"
        defines { "LUA_USE_POSIX", "LUA_USE_DLOPEN" }
        links { "dl" }
        linkoptions { "-Wl,-E" }
    
    filter "system:windows"
    
    filter "system:ios"
        defines { "LUA_USE_IOS" }
    
    filter "system:solaris"
        defines { "LUA_USE_POSIX", "LUA_USE_DLOPEN", "_REENTRANT" }
        links { "dl" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- Executable lua
project "LuaInterpreter"
    kind "ConsoleApp"
    language "C"
    staticruntime "On"
    systemversion "latest"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "src/lua.c",
    }
    
    includedirs
    {
        "src",
    }
    
    links
    {
        "LuaLibrary",
    }

    -- Platform specific configurations
    filter "system:linux"
        defines { "LUA_USE_LINUX" }
        links { "dl" }
        linkoptions { "-Wl,-E" }

    filter "system:emscripten"
        defines { "LUA_USE_LINUX" }
        links { "dl" }
        buildoptions {
            "-s WASM=1",
            "-matomics",
            "-mbulk-memory"
        }
        linkoptions { 
            "-Wl,-E",
            "-s WASM=1"
        }
        
    filter "system:macosx"
        defines { "LUA_USE_MACOSX", "LUA_USE_READLINE" }
        links { "readline" }
    
    filter "system:bsd"
        defines { "LUA_USE_POSIX", "LUA_USE_DLOPEN" }
        links { "dl" }
        linkoptions { "-Wl,-E" }
    
    filter "system:windows"
    
    filter "system:ios"
        defines { "LUA_USE_IOS" }
    
    filter "system:solaris"
        defines { "LUA_USE_POSIX", "LUA_USE_DLOPEN", "_REENTRANT" }
        links { "dl" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- Executable luac
project "LuaCompiler"
    kind "ConsoleApp"
    language "C"
    staticruntime "On"
    systemversion "latest"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "src/luac.c",
    }
    
    includedirs
    {
        "src",
    }
    
    links
    {
        "LuaLibrary",
    }

    filter "system:linux"
        defines { "LUA_USE_LINUX" }
        links { "dl" }
        linkoptions { "-Wl,-E" }

    filter "system:emscripten"
        defines { "LUA_USE_LINUX" }
        links { "dl" }
        buildoptions {
            "-s WASM=1",
            "-matomics",
            "-mbulk-memory"
        }
        linkoptions { 
            "-Wl,-E",
            "-s WASM=1"
        }
    
    filter "system:macosx"
        defines { "LUA_USE_MACOSX", "LUA_USE_READLINE" }
        links { "readline" }
    
    filter "system:bsd"
        defines { "LUA_USE_POSIX", "LUA_USE_DLOPEN" }
        links { "dl" }
        linkoptions { "-Wl,-E" }
    
    filter "system:windows"
    
    filter "system:ios"
        defines { "LUA_USE_IOS" }
    
    filter "system:solaris"
        defines { "LUA_USE_POSIX", "LUA_USE_DLOPEN", "_REENTRANT" }
        links { "dl" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"