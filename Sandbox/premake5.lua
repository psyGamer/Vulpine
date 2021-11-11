project "Sandbox"
	kind		  "ConsoleApp"
	staticruntime "off"
	
	language   "C++"
	cppdialect "C++17"	
	
	targetdir("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/Bin/temp/" .. outputdir .. "/%{prj.name}")
		
	dependson "Vulpine"
		
	includedirs 
	{	
		"%{wks.location}/Vulpine/src",
		
		"%{wks.location}/Vulpine/vendor",
		"%{wks.location}/Vulpine/vendor/glm",
		"%{wks.location}/Vulpine/vendor/spdlog/include",

		"%{wks.location}/Vulpine/vendor/glfw/include", -- Temporary
		"$(VULKAN_SDK)/Include" -- Temporary
	}

	libdirs 
	{
		"$(VULKAN_SDK)/Lib",

		"%{wks.location}/Bin/" .. outputdir .. "/Vulpine",
		"%{wks.location}/Vulpine/vendor/GLFW/Bin/" .. outputdir .. "/GLFW",
	}

	links
	{
		"vulkan-1.lib",
		"glfw.lib",

		"Vulpine.lib"
	}

	files 
	{
		"src/**.cpp",
		"src/**.h",
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"