IncludeDir                                = {}
IncludeDir["Glad"]                        = "%{wks.location}/vendor/Glad/include"
IncludeDir["SDL2"]                        = "%{wks.location}/vendor/SDL2/include"
IncludeDir["ScionUtilities"]              = "%{wks.location}/ScionUtilities/src"
IncludeDir["ScionWindows"]                = "%{wks.location}/ScionWindows/src"
IncludeDir["ScionRendering"]              = "%{wks.location}/ScionRendering/src"
IncludeDir["ScionLogger"]                 = "%{wks.location}/ScionLogger/src"
IncludeDir["ScionCore"]                   = "%{wks.location}/ScionCore/src"
IncludeDir["stb_image"]                   = "%{wks.location}/vendor/stb_image"
IncludeDir["glm"]                         = "%{wks.location}/vendor/glm"  
IncludeDir["entt"]                        = "%{wks.location}/vendor/entt/include"

LibraryDir                                = {}
LibraryDir["SDL2"]                        = "%{wks.location}/vendor/SDL2/lib/x64"

Library                                   = {}
Library["SDL2"]                           = "%{LibraryDir.SDL2}/SDL2.lib"