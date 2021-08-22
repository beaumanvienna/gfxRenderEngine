
-- Team Engine 2021

SDL_MIXER_DIR       = "sdl_mixer"
SDL_MIXER_INCLUDE   = SDL_MIXER_DIR.."/include"
SDL2_DIR            = "sdl"

project "sdl_mixer"
    kind "StaticLib"
    language "C"
    
    targetdir (path.join(SDL_MIXER_DIR, "build/%{_ACTION}-%{cfg.platform}-%{cfg.buildcfg}"))
    objdir (path.join(SDL_MIXER_DIR, "build/%{_ACTION}-%{cfg.platform}-%{cfg.buildcfg}/%{prj.name}"))
    
    defines "HAVE_LIBC"
    includedirs
    { 
        path.join(SDL_MIXER_DIR, "include"),
        path.join(SDL_MIXER_DIR, "src/"),
        path.join(SDL_MIXER_DIR, "src/codecs"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity"),
        path.join(SDL_MIXER_DIR, "src/codecs/native_midi"),
        path.join(SDL_MIXER_DIR, "external/flac-1.3.3/include"),
        path.join(SDL2_DIR, "include")
    }
    files
    {
    
        path.join(SDL_MIXER_DIR, "version.rc"),
        
        path.join(SDL_MIXER_DIR, "src/utils.c"),
        path.join(SDL_MIXER_DIR, "src/effects_internal.c"),
        path.join(SDL_MIXER_DIR, "src/effect_position.c"),
        path.join(SDL_MIXER_DIR, "src/effect_stereoreverse.c"),
        path.join(SDL_MIXER_DIR, "src/mixer.c"),
        path.join(SDL_MIXER_DIR, "src/music.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/load_aiff.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/load_voc.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/mp3utils.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_cmd.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_flac.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_fluidsynth.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_mad.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_mikmod.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_modplug.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_mpg123.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_nativemidi.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_ogg.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_opus.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_timidity.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/music_wav.c"),
                    
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/common.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/instrum.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/instrum.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/mix.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/output.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/playmidi.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/readmidi.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/resample.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/timidity.c"),
        path.join(SDL_MIXER_DIR, "src/codecs/timidity/tables.c"),

        path.join(SDL_MIXER_DIR, "src/codecs/native_midi/native_midi_common.c"),
        

    }

    filter "system:windows"
        --links { "setupapi", "winmm", "imm32", "version" }
        files
        {
            path.join(SDL_MIXER_DIR, "src/codecs/native_midi/native_midi_win32.c"),
        }
        includedirs
        { 
            path.join(SDL_MIXER_DIR, "VisualC/external/include/")
        }
        defines
        {
            "SDL_DISABLE_WINDOWS_IME",
            "WIN32",
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
            
            "MUSIC_MID_NATIVE",
            "MUSIC_MID_TIMIDITY",
            "MUSIC_MP3_MPG123",
            "MUSIC_FLAC",
            "MUSIC_OGG",
            "MUSIC_OPUS",
            "MUSIC_MOD_MODPLUG",
            "MUSIC_WAV",
            
            --DLL_EXPORT;
            -- NDEBUG;
            -- OGG_DYNAMIC="libvorbisfile-3.dll";
            -- OPUS_DYNAMIC="libopusfile-0.dll"
            -- FLAC_DYNAMIC="libFLAC-8.dll";
            -- MPG123_DYNAMIC="libmpg123-0.dll";;;
        }
        --links { "user32", "gdi32", "winmm", "imm32", "ole32", "oleaut32", "version", "uuid" }

    

