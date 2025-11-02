IncludeDir                                = {}
IncludeDir["Glad"]                        = "%{wks.location}/vendor/Glad/include"
IncludeDir["SDL2"]                        = "%{wks.location}/vendor/SDL2/include"
IncludeDir["ScionUtilities"]              = "%{wks.location}/ScionUtilities/src"
IncludeDir["ScionWindows"]                = "%{wks.location}/ScionWindows/src"
IncludeDir["SOIL"]                        = "%{wks.location}/vendor/SOIL"

LibraryDir                                = {}
LibraryDir["SDL2"]                        = "%{wks.location}/vendor/SDL2/lib/x64"

Library                                   = {}
Library["SDL2"]                           = "%{LibraryDir.SDL2}/SDL2.lib"