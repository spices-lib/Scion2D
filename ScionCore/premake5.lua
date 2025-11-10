project "ScionCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	flags { "MultiProcessorCompile" }

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	defines
	{
	}
	

	includedirs
	{
		"src",
		"%{IncludeDir.ScionLogger}",
		"%{IncludeDir.ScionRendering}",
		"%{IncludeDir.ScionUtilities}",
		"%{IncludeDir.ScionWindows}",
		"%{IncludeDir.ScionSounds}",
		"%{IncludeDir.ScionPhysics}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.sol2}",
		"%{IncludeDir.Lua}", 
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.Box2D}",
	}

	links
	{
		"%{Library.SDL2}",
		"ScionLogger",
		"ScionRendering",
		"ScionWindows",
		"ScionUtilities",
		"ScionSounds",
		"ScionPhysics",
		"LuaLibrary",
		"Box2D"
	}

	filter "system:windows"
		systemversion   "latest" 
		editAndContinue "Off"

		defines
		{}

		links
		{}

	filter "configurations:Debug"

		defines 
		{}

		links
		{}

		runtime "Debug"
		symbols "On"

	filter "configurations:Release"

		defines 
		{}

		links
		{}

		runtime "Release"
		optimize "On"