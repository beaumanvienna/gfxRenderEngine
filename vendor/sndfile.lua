
-- Team Engine 2021

-- sndfile
SNDFILE_DIR       = "sndfile"

project "sndfile"
    kind "StaticLib"
    language "C"

    targetdir (path.join(SNDFILE_DIR, "build/%{cfg.buildcfg}"))
    objdir (path.join(SNDFILE_DIR, "build/%{cfg.buildcfg}/%{prj.name}"))

    includedirs
    {
        path.join(SNDFILE_DIR, "src/")
    }
    files
    {
        path.join(SNDFILE_DIR,"src/pcm.c"),
        path.join(SNDFILE_DIR,"src/command.c"),
        path.join(SNDFILE_DIR,"src/file_io.c"),
        path.join(SNDFILE_DIR,"src/common.c"),
        path.join(SNDFILE_DIR,"src/ulaw.c"),
        path.join(SNDFILE_DIR,"src/alaw.c"),
        path.join(SNDFILE_DIR,"src/float32.c"),
        path.join(SNDFILE_DIR,"src/double64.c"),
        path.join(SNDFILE_DIR,"src/ima_adpcm.c"),
        path.join(SNDFILE_DIR,"src/ms_adpcm.c"),
        path.join(SNDFILE_DIR,"src/gsm610.c"),
        path.join(SNDFILE_DIR,"src/dwvw.c"),
        path.join(SNDFILE_DIR,"src/vox_adpcm.c"),
        path.join(SNDFILE_DIR,"src/interleave.c"),
        path.join(SNDFILE_DIR,"src/strings.c"),
        path.join(SNDFILE_DIR,"src/dither.c"),
        path.join(SNDFILE_DIR,"src/cart.c"),
        path.join(SNDFILE_DIR,"src/broadcast.c"),
        path.join(SNDFILE_DIR,"src/audio_detect.c"),
        path.join(SNDFILE_DIR,"src/ima_oki_adpcm.c"),
        path.join(SNDFILE_DIR,"src/alac.c"),
        path.join(SNDFILE_DIR,"src/chunk.c"),
        path.join(SNDFILE_DIR,"src/ogg.c"),
        path.join(SNDFILE_DIR,"src/chanmap.c"),
        path.join(SNDFILE_DIR,"src/id3.c"),
        path.join(SNDFILE_DIR,"src/sndfile.c"),
        path.join(SNDFILE_DIR,"src/aiff.c"),
        path.join(SNDFILE_DIR,"src/au.c"),
        path.join(SNDFILE_DIR,"src/avr.c"),
        path.join(SNDFILE_DIR,"src/caf.c"),
        path.join(SNDFILE_DIR,"src/dwd.c"),
        path.join(SNDFILE_DIR,"src/flac.c"),
        path.join(SNDFILE_DIR,"src/g72x.c"),
        path.join(SNDFILE_DIR,"src/htk.c"),
        path.join(SNDFILE_DIR,"src/ircam.c"),
        path.join(SNDFILE_DIR,"src/macos.c"),
        path.join(SNDFILE_DIR,"src/mat4.c"),
        path.join(SNDFILE_DIR,"src/mat5.c"),
        path.join(SNDFILE_DIR,"src/nist.c"),
        path.join(SNDFILE_DIR,"src/paf.c"),
        path.join(SNDFILE_DIR,"src/pvf.c"),
        path.join(SNDFILE_DIR,"src/raw.c"),
        path.join(SNDFILE_DIR,"src/rx2.c"),
        path.join(SNDFILE_DIR,"src/sd2.c"),
        path.join(SNDFILE_DIR,"src/sds.c"),
        path.join(SNDFILE_DIR,"src/svx.c"),
        path.join(SNDFILE_DIR,"src/txw.c"),
        path.join(SNDFILE_DIR,"src/voc.c"),
        path.join(SNDFILE_DIR,"src/wve.c"),
        path.join(SNDFILE_DIR,"src/w64.c"),
        path.join(SNDFILE_DIR,"src/wavlike.c"),
        path.join(SNDFILE_DIR,"src/wav.c"),
        path.join(SNDFILE_DIR,"src/xi.c"),
        path.join(SNDFILE_DIR,"src/mpc2k.c"),
        path.join(SNDFILE_DIR,"src/rf64.c"),
        path.join(SNDFILE_DIR,"src/ogg_vorbis.c"),
        path.join(SNDFILE_DIR,"src/ogg_speex.c"),
        path.join(SNDFILE_DIR,"src/ogg_pcm.c"),
        path.join(SNDFILE_DIR,"src/ogg_opus.c"),
        path.join(SNDFILE_DIR,"src/ogg_vcomment.c"),
        path.join(SNDFILE_DIR,"src/nms_adpcm.c"),
        path.join(SNDFILE_DIR,"src/mpeg.c"),
        path.join(SNDFILE_DIR,"src/mpeg_decode.c"),
        path.join(SNDFILE_DIR,"src/mpeg_l3_encode.c"),
        path.join(SNDFILE_DIR,"src/GSM610/add.c"),
        path.join(SNDFILE_DIR,"src/GSM610/code.c"),
        path.join(SNDFILE_DIR,"src/GSM610/decode.c"),
        path.join(SNDFILE_DIR,"src/GSM610/gsm_create.c"),
        path.join(SNDFILE_DIR,"src/GSM610/gsm_decode.c"),
        path.join(SNDFILE_DIR,"src/GSM610/gsm_destroy.c"),
        path.join(SNDFILE_DIR,"src/GSM610/gsm_encode.c"),
        path.join(SNDFILE_DIR,"src/GSM610/gsm_option.c"),
        path.join(SNDFILE_DIR,"src/GSM610/long_term.c"),
        path.join(SNDFILE_DIR,"src/GSM610/lpc.c"),
        path.join(SNDFILE_DIR,"src/GSM610/preprocess.c"),
        path.join(SNDFILE_DIR,"src/GSM610/rpe.c"),
        path.join(SNDFILE_DIR,"src/GSM610/short_term.c"),
        path.join(SNDFILE_DIR,"src/GSM610/table.c"),
        path.join(SNDFILE_DIR,"src/G72x/g721.c"),
        path.join(SNDFILE_DIR,"src/G72x/g723_16.c"),
        path.join(SNDFILE_DIR,"src/G72x/g723_24.c"),
        path.join(SNDFILE_DIR,"src/G72x/g723_40.c"),
        path.join(SNDFILE_DIR,"src/G72x/g72x.c"),
        path.join(SNDFILE_DIR,"src/ALAC/ALACBitUtilities.c"),
        path.join(SNDFILE_DIR,"src/ALAC/ag_dec.c"),
        path.join(SNDFILE_DIR,"src/ALAC/ag_enc.c"),
        path.join(SNDFILE_DIR,"src/ALAC/dp_dec.c"),
        path.join(SNDFILE_DIR,"src/ALAC/dp_enc.c"),
        path.join(SNDFILE_DIR,"src/ALAC/matrix_dec.c"),
        path.join(SNDFILE_DIR,"src/ALAC/matrix_enc.c"),
        path.join(SNDFILE_DIR,"src/ALAC/alac_decoder.c"),
        path.join(SNDFILE_DIR,"src/ALAC/alac_encoder.c"),



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
            "_WINDOWS"
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
