project "uv"
	kind "StaticLib"
	language "C++"
	includedirs {"libuv/include", "libuv/src"}
	files {"libuv/src/**.h", "libuv/src/**.c"}
	filter "platforms:osx"
		local cwd = os.getcwd()
		postbuildcommands {
			"{MKDIR} %{wks.location}/%{cfg.targetdir}/include/libuv/", 
			"{COPY} " .. cwd .. "/libuv/include/ %{wks.location}/%{cfg.targetdir}/include/libuv/"
		}
		removefiles {"libuv/src/unix/aix.c",
						"libuv/src/unix/android-ifaddrs.c",
						"libuv/src/unix/freebsd.c",
						"libuv/src/unix/linux*.c",
						"libuv/src/unix/netbsd.c",
						"libuv/src/unix/openbsd.c",
						"libuv/src/unix/pthread-fixes.c",
						"libuv/src/unix/sunos.c",
						"libuv/src/win/**"}