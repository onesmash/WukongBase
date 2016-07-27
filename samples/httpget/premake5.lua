workspace "httpget"
	configurations { "debug", "release" }
	platforms {"osx"}
	kind "ConsoleApp"
	language "C++"
	flags {"C++11"}
	includedirs {"../../bin/include", "../../bin/include/libuv"}
	filter "configurations:debug"
      defines { "DEBUG" }
      flags { "Symbols" }
	filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"
						
	filter {}
		libdirs {"../../bin"}
		links {"c++", "uv", "wukongbase"}
	
project "httpget"
	prebuildcommands {
		"make -C ../../ config=" .. "%{cfg.buildcfg}_%{cfg.platform}" .. " target=wukongbase"
	}
	files {
		"main.cpp"
	}