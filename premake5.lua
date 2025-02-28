workspace "GZ"
	configurations {"Debug", "Release", "Dist"}
	startproject "GZEditor"
    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GZEngine"
	location "GZEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/".. outputdir .."/%{prj.name}")
	objdir ("bin-obj/".. outputdir .."/%{prj.name}")
	files 
	{ 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	-- Include path

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"GZ_PLATFORM_WINDOWS",
			"GZ_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/GZEditor")
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
	
	targetdir ("bin/".. outputdir .."/%{prj.name}")
	objdir ("bin-obj/".. outputdir .."/%{prj.name}")
	files 
	{ 
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	-- Include path
	includedirs
	{
		"GZEngine/"
	}

	links
	{
		"GZEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"GZ_PLATFORM_WINDOWS"
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