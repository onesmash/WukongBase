workspace "wukongbase"
	configurations { "debug", "release" }
	platforms {"osx"}
	kind "StaticLib"
	language "C++"
	flags {"C++11"}
	targetdir "bin"
	filter "configurations:debug"
      defines { "DEBUG" }
      flags { "Symbols" }
	filter "configurations:delease"
      defines { "NDEBUG" }
      optimize "On"
						
	filter {}
		removefiles {".git/**"}
	
project "wukongbase"
	postbuildcommands {
		"{MKDIR} %{cfg.targetdir}/include/base/message_loop",
		"{MKDIR} %{cfg.targetdir}/include/base/thread",
		"{MKDIR} %{cfg.targetdir}/include/net/",
		"{COPY} base/*.h %{cfg.targetdir}/include/base",
		"{COPY} base/message_loop/*.h %{cfg.targetdir}/include/base/message_loop",
		"{COPY} base/thread/*.h %{cfg.targetdir}/include/base/thread",
		"{COPY} net/*.h %{cfg.targetdir}/include/net"
	}
	includedirs {".", "bin/include/libuv", "third_party/http_parser/"}
	files {"base/**.cpp", "net/*.cpp", "third_party/http_parser/http_parser/http_parser.c"}
	libdirs {"bin"}
	links {"c++", "uv"}

include "third_party/libuv/libuv.lua"

