project "Sandbox"
	kind		  "ConsoleApp"
	staticruntime "on"
	
	language   "C++"
	cppdialect "C++17"	
	
	targetdir("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/Bin/temp/" .. outputdir .. "/%{prj.name}")
		
	includedirs 
	{	
		"%{wks.location}/Vulpine/src",
		
		"%{wks.location}/Vulpine/vendor",
		"%{wks.location}/Vulpine/vendor/glm",
		"%{wks.location}/Vulpine/vendor/spdlog/include"
	}

	links 
	{
		"Vulpine"
	}
		
	files 
	{
		"src/**.cpp",
		"src/**.h",
	}
	
	filter "configurations:Debug"
		buildoptions "/MTd"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		buildoptions "/MT"
		runtime "Release"
		optimize "on"