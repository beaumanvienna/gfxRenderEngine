
-- Team Engine 2021

-- pthread
PTHREAD_DIR       = "pthread"

project "pthread"
    kind "StaticLib"
    language "C"

    targetdir (path.join(PTHREAD_DIR, "build/%{cfg.buildcfg}"))
    objdir (path.join(PTHREAD_DIR, "build/%{cfg.buildcfg}/%{prj.name}"))

    includedirs
    {
        path.join(PTHREAD_DIR, "src")
    }
    files
    {
        --path.join(PTHREAD_DIR,"src/pthread.c"),
        path.join(PTHREAD_DIR,"src/attr.c"),
        path.join(PTHREAD_DIR,"src/barrier.c"),
        path.join(PTHREAD_DIR,"src/cancel.c"),
        path.join(PTHREAD_DIR,"src/cleanup.c"),
        path.join(PTHREAD_DIR,"src/condvar.c"),
        path.join(PTHREAD_DIR,"src/create.c"),
        path.join(PTHREAD_DIR,"src/dll.c"),
        path.join(PTHREAD_DIR,"src/errno.c"),
        path.join(PTHREAD_DIR,"src/exit.c"),
        path.join(PTHREAD_DIR,"src/fork.c"),
        path.join(PTHREAD_DIR,"src/global.c"),
        path.join(PTHREAD_DIR,"src/misc.c"),
        path.join(PTHREAD_DIR,"src/mutex.c"),
        path.join(PTHREAD_DIR,"src/nonportable.c"),
        path.join(PTHREAD_DIR,"src/private.c"),
        path.join(PTHREAD_DIR,"src/rwlock.c"),
        path.join(PTHREAD_DIR,"src/sched.c"),
        path.join(PTHREAD_DIR,"src/semaphore.c"),
        path.join(PTHREAD_DIR,"src/signal.c"),
        path.join(PTHREAD_DIR,"src/spin.c"),
        path.join(PTHREAD_DIR,"src/sync.c"),
        path.join(PTHREAD_DIR,"src/tsd.c")
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
            "_WINDOWS",
            "HAVE_PTW32_CONFIG_H",
            "__CLEANUP_C",
            "PTW32_BUILD_INLINED",
            "PTW32_STATIC_LIB"
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
