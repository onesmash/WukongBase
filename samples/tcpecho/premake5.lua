
workspace "tcpecho"
	configurations { "Debug", "Release" }
	platforms {"OSX"}
	kind "ConsoleApp"
	language "C++"
	flags {"C++11"}
	includedirs {"../../", "../../third_party/libuv"}
	filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
	filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
						
	filter {}
		libdirs {"../../bin"}
		links {"c++", "uv", "wukongbase"}
	
project "client"
	files {
		"client.cpp"
	}

project "server"
	files {
		"server.cpp"
	}