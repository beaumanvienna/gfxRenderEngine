
-- Team Engine 2021

ZLIB_DIR       = "zlib"

project "zlib"
    kind "StaticLib"
    language "C"

    targetdir (path.join(ZLIB_DIR, "build/%{cfg.buildcfg}"))
    objdir (path.join(ZLIB_DIR, "build/%{cfg.buildcfg}/%{prj.name}"))

    includedirs
    {
        path.join(ZLIB_DIR, "")
    }
    files
    {
        path.join(ZLIB_DIR,"adler32.c"),
        path.join(ZLIB_DIR,"compress.c"),
        path.join(ZLIB_DIR,"crc32.c"),
        path.join(ZLIB_DIR,"deflate.c"),
        path.join(ZLIB_DIR,"gzclose.c"),
        path.join(ZLIB_DIR,"gzlib.c"),
        path.join(ZLIB_DIR,"gzread.c"),
        path.join(ZLIB_DIR,"gzwrite.c"),
        path.join(ZLIB_DIR,"infback.c"),
        path.join(ZLIB_DIR,"inffast.c"),
        path.join(ZLIB_DIR,"inflate.c"),
        path.join(ZLIB_DIR,"inftrees.c"),
        path.join(ZLIB_DIR,"trees.c"),
        path.join(ZLIB_DIR,"uncompr.c"),
        path.join(ZLIB_DIR,"zutil.c")


    }

    filter "system:windows"
        files
        {
        }
        includedirs
        {
        }
        defines
        {
            "WIN32",
            "_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
