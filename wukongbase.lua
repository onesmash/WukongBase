workspace "wukongbase"
	configurations { "Debug", "Release" }
	platforms {"OSX"}
	kind "StaticLib"
	language "C++"
	flags {"C++11"}
	targetdir "bin"
	filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
	filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
						
	filter {}
		removefiles {".git/**"}
	
project "wukongbase"
	includedirs {".", "third_party/libuv/"}
	files {"base/**.cpp", "net/*.cpp"}
	libdirs {"bin"}
	links {"c++", "uv"}

include "third_party/libuv/libuv.lua"

