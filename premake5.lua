workspace "GZEngine"
	configurations {"Debug", "Release", "Dist"}
	startproject "GZEditor"
    architecture "x64"
    staticruntime "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include_maps={}
include_maps["spdlog"] = "%{wks.location}/GZEngine/vendor/spdlog/include"
include_maps["SDL3"] = "%{wks.location}/GZEngine/vendor/SDL3/include"

group "Dependencies"
-- include "GZEngine/vendor/SDL3" // Too hard to build ourselves, use the correct lib instead
group ""

-- defines { "_CRT_SECURE_NO_WARNINGS" }

project "GZEngine"
	location "GZEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}/")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}/")

	-- precompiled headers
	pchheader "gzpch.h"
	pchsource "%{prj.location}/src/gzpch.cpp"

	files 
	{ 
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		"%{include_maps.SDL3}/**.h",
	}

	-- Include path
	includedirs
	{
		".",
		"%{prj.location}/", -- for pcheader
		"%{prj.location}/src/",
		"%{prj.location}/src/**",
		include_maps.spdlog,
		include_maps.SDL3,
		-- "%{prj.location}/vendor/**"
	}

	libdir_map={}

	-- Different Config????
	libdir_map["SDL3"] = "%{prj.location}/vendor/SDL3/lib/windows"

	libdirs { libdir_map.SDL3 }

	links { "SDL3" }

	filter { "system:windows", "action:vs2022" }
	        buildoptions { "/utf-8" }

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"GZ_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "GZ_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GZ_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GZ_DIST"
		optimize "On"


project "GZEditor"
	location "GZEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}/")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}/")
	files
	{
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
	}

	-- Include path
	includedirs
	{
		"%{wks.location}/src/",

		"%{wks.location}/GZEngine/",
		-- "%{wks.location}/GZEngine/vendor/**",
		include_maps.spdlog,
		include_maps.SDL3,
	}

	links
	{
		"GZEngine"
	}

	filter { "system:windows", "action:vs2022" }
	        buildoptions { "/utf-8" }

	filter { "system:windows" }
		systemversion "latest"
		
		defines
		{
			"GZ_PLATFORM_WINDOWS"
		}

		-- Copy dll to editor location
		

	filter { "system:windows", "configurations:Debug" }

		postbuildcommands {
		    '{COPYFILE} "%{wks.location}/GZEngine/vendor/SDL3/lib/windows/SDL3.dll" "%{cfg.targetdir}"',
		    '{COPYFILE} "%{wks.location}/GZEngine/vendor/SDL3/lib/windows/SDL3.pdb" "%{cfg.targetdir}"',
	    }

	-- Need to have a release version of the SDL3, will trigger error if not set 

	filter "configurations:Debug"
		defines "GZ_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GZ_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GZ_DIST"
		optimize "On"