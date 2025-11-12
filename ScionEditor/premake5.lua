project "ScionEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	flags { "MultiProcessorCompile" }

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
		
		"%{IncludeDir.stb_image}/stb_image.h",
	}

	defines
	{
		"STB_IMAGE_IMPLEMENTATION"
	}
	

	includedirs
	{
		"src",
		"%{IncludeDir.ScionUtilities}",
		"%{IncludeDir.ScionWindows}",
		"%{IncludeDir.ScionRendering}",
		"%{IncludeDir.ScionSounds}",
		"%{IncludeDir.ScionLogger}",
		"%{IncludeDir.ScionCore}",
		"%{IncludeDir.ScionPhysics}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.sol2}",
		"%{IncludeDir.Lua}", 
		"%{IncludeDir.Box2D}", 
		"%{IncludeDir.ImGui}", 
	}

	links
	{
		"Glad",
		"%{Library.SDL2}",
		"ScionUtilities",
		"ScionWindows",
		"ScionSounds",
		"ScionRendering",
		"ScionLogger",
		"ScionCore",
		"ScionPhysics",
		"Box2D",
		"ImGui"
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