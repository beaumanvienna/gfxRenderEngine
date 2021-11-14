
-- Team Engine 2021

-- sndfile
ICONV_DIR       = "iconv"

project "iconv"
    kind "StaticLib"
    language "C"

    targetdir (path.join(ICONV_DIR, "build/%{cfg.buildcfg}"))
    objdir (path.join(ICONV_DIR, "build/%{cfg.buildcfg}/%{prj.name}"))

    includedirs
    {
        path.join(ICONV_DIR, ""),
        path.join(ICONV_DIR, "include"),
        path.join(ICONV_DIR, "srclib"),
        path.join(ICONV_DIR, "libcharset")
    }
    files
    {
        path.join(ICONV_DIR,"libcharset/lib/localcharset.c"),
        --path.join(ICONV_DIR,"libcharset/lib/relocatable-stub.c"),
        path.join(ICONV_DIR,"lib/iconv.c"),
        path.join(ICONV_DIR,"lib/relocatable.c"),
        --path.join(ICONV_DIR,"srclib/allocator.c"),
        --path.join(ICONV_DIR,"srclib/areadlink.c"),
        --path.join(ICONV_DIR,"srclib/binary-io.c"),
        --path.join(ICONV_DIR,"srclib/careadlinkat.c"),
        --path.join(ICONV_DIR,"srclib/dirname-lgpl.c"),
        --path.join(ICONV_DIR,"srclib/basename-lgpl.c"),
        --path.join(ICONV_DIR,"srclib/stripslash.c"),
        --path.join(ICONV_DIR,"srclib/getprogname.c"),
        --path.join(ICONV_DIR,"srclib/malloca.c"),
        --path.join(ICONV_DIR,"srclib/progname.c"),
        --path.join(ICONV_DIR,"srclib/safe-read.c"),
        --path.join(ICONV_DIR,"srclib/stat-time.c"),
        --path.join(ICONV_DIR,"srclib/unistd.c"),
        --path.join(ICONV_DIR,"srclib/uniwidth/width.c"),
        --path.join(ICONV_DIR,"srclib/xmalloc.c"),
        --path.join(ICONV_DIR,"srclib/xstrdup.c"),
        --path.join(ICONV_DIR,"srclib/xreadlink.c"),
        --path.join(ICONV_DIR,"srclib/canonicalize-lgpl.c"),
        --path.join(ICONV_DIR,"srclib/error.c"),
        --path.join(ICONV_DIR,"srclib/malloc.c"),
        --path.join(ICONV_DIR,"srclib/msvc-inval.c"),
        --path.join(ICONV_DIR,"srclib/msvc-nothrow.c"),
        --path.join(ICONV_DIR,"srclib/raise.c"),
        --path.join(ICONV_DIR,"srclib/read.c"),
        --path.join(ICONV_DIR,"srclib/readlink.c"),
        --path.join(ICONV_DIR,"srclib/sigprocmask.c"),
        --path.join(ICONV_DIR,"srclib/stat.c"),
        --path.join(ICONV_DIR,"srclib/stat-w32.c"),
        --path.join(ICONV_DIR,"srclib/stdio-write.c"),
        --path.join(ICONV_DIR,"srclib/strerror.c"),
        --path.join(ICONV_DIR,"srclib/strerror-override.c"),
    }

    filter "system:windows"
        files
        {
        }
        includedirs
        {
            "win"
        }
        defines
        {
            "WIN32",
            "_WINDOWS",
            "BUILDING_LIBCHARSET",
            "HAVE_CONFIG_H",
            "DEPENDS_ON_LIBICONV",
            "DEPENDS_ON_LIBINTL"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter { "configurations:Dist" }
        defines { "NDEBUG" }
        optimize "On"
