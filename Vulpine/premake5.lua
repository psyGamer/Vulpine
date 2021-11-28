project "Vulpine"
	kind		  "StaticLib"
	staticruntime "off"
	
	language   "C++"
	cppdialect "C++17"	
	
	pchheader "vppch.h"
	pchsource "src/vppch.cpp"

	targetdir("../Bin/" .. outputdir .. "/%{prj.name}")
	objdir("../Bin/temp/" .. outputdir .. "/%{prj.name}")
		
	dependson "GLFW"
		
	includedirs 
	{
		"$(VULKAN_SDK)/Include",
		"vendor/glfw/include",
		"vendor/glm",
		"vendor/spdlog/include",
		
		"src",
		"src/Vulpine",
		"vendor"
	}
	
	libdirs "$(VULKAN_SDK)/Lib"

	links 
	{
		"vulkan-1",
		"GLFW"
	}
		
	files 
	{
		"src/**.cpp",
		"src/**.tpp",
		"src/**.hpp",
		"src/**.h",
		
		"vendor/glm/**.hpp",
		"vendor/glm/**.inl",
	}
	
	filter "configurations:Debug"
		defines { "VP_DEBUG", "VP_VULKAN_VALIDATION" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines { "VP_RELEASE", "VP_VULKAN_VALIDATION" }
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		defines "VP_DIST"
		runtime "Release"
		optimize "on"
	