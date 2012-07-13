solution "roguelike"
    configurations { "Debug", "Release" }
	flags { "ExtraWarnings", "FatalWarnings" }

    --includedirs { "" }
    --libdirs { "" }

    -- Platform-specific options
    configuration { "gmake" }
		buildoptions { "-std=c99", "-pedantic" }

	configuration { "windows" }
		defines { "WINDOWS" }

    configuration { "linux" }
		defines { "LINUX" }
        links { "m", "rt" }

    -- Configuration-specific options
    configuration "Debug"
        defines { "DEBUG" }
        flags   { "Symbols" }

    configuration "Release"
        defines { "NDEBUG","RELEASE_BUILD" }
        flags   { "Optimize" }

    -- Client project
    project "game"
        kind "ConsoleApp"
        language "C"
        files { "src/**.h", "src/**.c" }

        configuration "windows"
            -- TODO: Remove hard-coded paths
            libdirs     { "C:/codelibs/pdcurses/lib/" }
            includedirs { "C:/codelibs/pdcurses/include/" }
            links       { "pdcurses" }

        configuration "linux"
            links { "ncurses" }
