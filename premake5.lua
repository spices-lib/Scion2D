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
include "ScionRendering"
include "ScionLogger"
include "ScionCore"
include "ScionSounds"
include "ScionPhysics"

group "Dependencies"
include "vendor/Glad"
include "vendor/Lua"
include "vendor/box2d"
include "vendor/imgui"
group ""