workspace "wukongbase"
	configurations { "debug", "release" }
	platforms {"osx"}
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
	includedirs {".", "third_party/libuv/", "third_party/http_parser/"}
	files {"base/**.cpp", "net/*.cpp", "third_party/http_parser/http_parser/http_parser.c"}
	libdirs {"bin"}
	links {"c++", "uv"}

include "third_party/libuv/libuv.lua"

