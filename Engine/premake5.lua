project "Engine"
	kind		  "StaticLib"
	staticruntime "on"
	
	language   "C++"
	cppdialect "C++17"	
	
	targetdir("../Bin/" .. outputdir .. "/%{prj.name}")
	objdir("../Bin/temp/" .. outputdir .. "/%{prj.name}")
		
	includedirs 
	{
		"$(VULKAN_SDK)/Include",
		"vendor/glfw/include",
		"vendor/glm",
		
		"src",
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
		buildoptions "/MTd"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		buildoptions "/MT"
		runtime "Release"
		optimize "on"
	