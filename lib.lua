IncludeDir                                = {}
IncludeDir["Glad"]                        = "%{wks.location}/vendor/Glad/include"
IncludeDir["SDL2"]                        = "%{wks.location}/vendor/SDL2/include"
IncludeDir["ScionUtilities"]              = "%{wks.location}/ScionUtilities/src"
IncludeDir["ScionWindows"]                = "%{wks.location}/ScionWindows/src"
IncludeDir["ScionRendering"]              = "%{wks.location}/ScionRendering/src"
IncludeDir["ScionLogger"]                 = "%{wks.location}/ScionLogger/src"
IncludeDir["ScionCore"]                   = "%{wks.location}/ScionCore/src"
IncludeDir["ScionSounds"]                 = "%{wks.location}/ScionSounds/src"
IncludeDir["ScionPhysics"]                = "%{wks.location}/ScionPhysics/src"
IncludeDir["stb_image"]                   = "%{wks.location}/vendor/stb_image"
IncludeDir["glm"]                         = "%{wks.location}/vendor/glm"  
IncludeDir["entt"]                        = "%{wks.location}/vendor/entt/include"
IncludeDir["sol2"]                        = "%{wks.location}/vendor/sol2/include"
IncludeDir["Lua"]                         = "%{wks.location}/vendor/Lua/src"
IncludeDir["Box2D"]                       = "%{wks.location}/vendor/Box2D/include"

LibraryDir                                = {}
LibraryDir["SDL2"]                        = "%{wks.location}/vendor/SDL2/lib/x64"

Library                                   = {}
Library["SDL2"]                           = "%{LibraryDir.SDL2}/SDL2.lib"
Library["SDL_mixer"]                      = "%{LibraryDir.SDL2}/SDL2_mixer.lib"