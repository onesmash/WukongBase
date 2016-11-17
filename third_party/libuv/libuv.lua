project "uv"
	kind "Makefile"
	language "C++"
    local cwd = os.getcwd()
    local target = path.getabsolute("%{cfg.targetdir}")
	buildcommands {
        "cd " .. cwd .. "/libuv && ./autogen.sh && ./configure --prefix=" .. target .. "; cd -",
        "make -C " .. cwd .. "/libuv",
        "make -C " .. cwd .. "/libuv install",
        "{MKDIR} %{wks.location}/%{cfg.targetdir}/include/libuv/include", 
        "{COPY} " .. cwd .. "/bin/include/ %{wks.location}/%{cfg.targetdir}/include/libuv/include",
        "{COPY} " .. cwd .. "/bin/lib/libuv.a %{wks.location}/%{cfg.targetdir}/"
    }
    cleancommands {
        "make -C " .. cwd .. "/libuv distclean",
        "rm -rf " .. target
    }