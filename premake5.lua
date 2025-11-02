workspace "Scion2D"
	architecture "x64"
	startproject "ScionEditor"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "lib.lua"

include "ScionEditor"
include "ScionUtilities"
include "ScionWindows"

group "Dependencies"
include "vendor/Glad"
group ""