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
		
		"%{IncludeDir.SOIL}/SOIL.h",
	}

	defines
	{
	}
	

	includedirs
	{
		"src",
		"%{IncludeDir.ScionUtilities}",
		"%{IncludeDir.ScionWindows}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.SOIL}",
	}

	links
	{
		"Glad",
		"ScionUtilities",
		"ScionWindows"
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