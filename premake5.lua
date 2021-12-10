workspace "Vulpine"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	
	startproject "Sandbox"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Vulpine/vendor/GLFW"
	include "Vulpine/vendor/benchmark"
group ""

include "Vulpine"
include "Sandbox"

newaction {
	trigger = "clean",
	description = "Remove all binaries and generated files",
	
	execute = function()
		print("Removing compiled shaders...")
		os.remove("**.spv")
		
		print("Removing binaries...")
		
		os.rmdir("./bin")
		
		print("Removing intermediate binaries...")
		
		os.rmdir("./bin-int")
		
		print("Removing project files...")
		
		os.rmdir("./.vs")
		
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		
		print("Done.")
	end
}
