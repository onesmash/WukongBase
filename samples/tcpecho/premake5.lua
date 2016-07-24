
workspace "tcpecho"
	configurations { "debug", "release" }
	platforms {"osx"}
	kind "ConsoleApp"
	language "C++"
	flags {"C++11"}
	includedirs {"../../", "../../third_party/libuv"}
	filter "configurations:debug"
      defines { "DEBUG" }
      flags { "Symbols" }
	filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"
						
	filter {}
		prebuildcommands {
			"make -C ../../ config=" .. "%{cfg.buildcfg}_%{cfg.platform}" .. " target=wukongbase"
		}
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