/* Mednafen - Multi-system Emulator
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
// Mednafen development team until 2020
// https://mednafen.github.io/
// License: GPL-2.0-or-later
//Engine Copyright (c) 2021 Engine Development Team 
//https://github.com/beaumanvienna/gfxRenderEngine/blob/master/LICENSE

#include "main.h"

#include <trio/trio.h>

#include "video.h"

#include "opengl.h"
#include "shader.h"

extern unsigned int gMainBuffer[4096 * 4096];
extern unsigned int mednafenWidth;
extern unsigned int mednafenHeight;
extern int mednafenTextureIDs[4];
extern bool mednafenTextures;

void OpenGL_Blitter::ReadPixels(MDFN_Surface *surface, const MDFN_Rect *rect)
{
    p_glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitchinpix);
    p_glReadPixels(rect->x, gl_screen_h - rect->h - rect->y, rect->w, rect->h, PixelFormat, PixelType, surface->pixels);

    for(int y = 0; y < surface->h / 2; y++)
    {
        #ifdef _MSC_VER
        uint32* tmp_buffer = new uint32[surface->w];
        #else
        uint32 tmp_buffer[surface->w];
        #endif // _MSC_VER

        memcpy(tmp_buffer, &surface->pixels[y * surface->pitchinpix], surface->pitchinpix * sizeof(uint32));
        memcpy(&surface->pixels[y * surface->pitchinpix], &surface->pixels[(surface->h - 1 - y) * surface->pitchinpix], surface->pitchinpix * sizeof(uint32));
        memcpy(&surface->pixels[(surface->h - 1 - y) * surface->pitchinpix], tmp_buffer, surface->pitchinpix * sizeof(uint32));
    }
}


void OpenGL_Blitter::BlitRaw(const MDFN_Surface *surface, const MDFN_Rect *rect, const MDFN_Rect *dest_rect, const bool source_alpha)
{
    unsigned int tmpwidth;
    unsigned int tmpheight;

    if(SupportNPOT)
    {
        tmpwidth = rect->w;
        tmpheight = rect->h;
    }
    else
    {
        tmpwidth = round_up_pow2(rect->w);
        tmpheight = round_up_pow2(rect->h);
    }

    if(tmpwidth > MaxTextureSize || tmpheight > MaxTextureSize)
    {
        MDFN_Rect neo_rect;
        MDFN_Rect neo_dest_rect;

        for(int32 xseg = 0; xseg < rect->w; xseg += MaxTextureSize)
        {
            for(int32 yseg = 0; yseg < rect->h; yseg += MaxTextureSize)
            {
                neo_rect.x = rect->x + xseg;
                neo_rect.w = rect->w - xseg;

                if(neo_rect.w > MaxTextureSize)
                {
                    neo_rect.w = MaxTextureSize;
                }

                neo_rect.y = rect->y + yseg;
                neo_rect.h = rect->h - yseg;

                if(neo_rect.h > MaxTextureSize)
                {
                    neo_rect.h = MaxTextureSize;
                }

                neo_dest_rect.x = dest_rect->x + xseg * dest_rect->w / rect->w;
                neo_dest_rect.y = dest_rect->y + yseg * dest_rect->h / rect->h;
                neo_dest_rect.w = neo_rect.w * dest_rect->w / rect->w;
                neo_dest_rect.h = neo_rect.h * dest_rect->h / rect->h;
                BlitRaw(surface, &neo_rect, &neo_dest_rect, source_alpha);
            }
        }
    }
    else
    {
        // Don't move the source_alpha stuff out of this else { }, otherwise it will break the recursion necessary to work around maximum texture size limits.
        if(source_alpha)
        {
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }   
        p_glBindTexture(GL_TEXTURE_2D, textures[3]);
        p_glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitchinpix);

        p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        p_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rect->w, rect->h, PixelFormat, PixelType, surface->pixels + rect->x + rect->y * surface->pitchinpix);

        p_glBegin(GL_QUADS);

        p_glTexCoord2f(0.0f, 1.0f * rect->h / tmpheight);  // Bottom left of our picture.
        p_glVertex2f(dest_rect->x, dest_rect->y + dest_rect->h);

        p_glTexCoord2f((float)rect->w / tmpwidth, 1.0f * rect->h / tmpheight); // Bottom right of our picture.
        p_glVertex2f(dest_rect->x + dest_rect->w, dest_rect->y + dest_rect->h);

        p_glTexCoord2f((float)rect->w / tmpwidth, 0.0f);    // Top right of our picture.
        p_glVertex2f(dest_rect->x + dest_rect->w,  dest_rect->y);

        p_glTexCoord2f(0.0f, 0.0f);     // Top left of our picture.
        p_glVertex2f(dest_rect->x, dest_rect->y);

        p_glEnd();

        if(source_alpha)
        {
            p_glDisable(GL_BLEND);
        }
    }
}

static INLINE void MakeSourceCoords(const MDFN_Rect *src_rect, float sc[4][2], const int32 tmpwidth, const int32 tmpheight, const float y_fudge)
{
    // Upper left
    sc[0][0] = (float)src_rect->x / tmpwidth;                           // X
    sc[0][1] = (float)(src_rect->y + y_fudge) / tmpheight;              // Y

    // Upper right
    sc[1][0] = (float)(src_rect->x + src_rect->w) / tmpwidth;           // X
    sc[1][1] = (float)(src_rect->y + y_fudge) / tmpheight;              // Y

    // Lower right
    sc[2][0] = (float)(src_rect->x + src_rect->w) / tmpwidth;           // X
    sc[2][1] = (float)(src_rect->y + y_fudge + src_rect->h) / tmpheight;// Y

    // Lower left
    sc[3][0] = (float)src_rect->x / tmpwidth;                           // X
    sc[3][1] = (float)(src_rect->y + y_fudge + src_rect->h) / tmpheight;// Y
}

static INLINE void MakeDestCoords(const MDFN_Rect *dest_rect, int dest_coords[4][2], const unsigned rotated)
{
    signed dco = 0;

    if(rotated == MDFN_ROTATE90)
    {
        dco = 1;
    }
    else if(rotated == MDFN_ROTATE270)
    {
        dco = 3;
    }
    else if(rotated == MDFN_ROTATE180)
    {
        dco = 2;
    }

    // Upper left
    dest_coords[dco][0] = dest_rect->x;
    dest_coords[dco][1] = dest_rect->y;
    dco = (dco + 1) & 3;

    // Upper Right
    dest_coords[dco][0] = dest_rect->x + dest_rect->w;
    dest_coords[dco][1] = dest_rect->y;
    dco = (dco + 1) & 3;

    // Lower right
    dest_coords[dco][0] = dest_rect->x + dest_rect->w;
    dest_coords[dco][1] = dest_rect->y + dest_rect->h;
    dco = (dco + 1) & 3;

    // Lower left
    dest_coords[dco][0] = dest_rect->x;
    dest_coords[dco][1] = dest_rect->y + dest_rect->h;
    dco = (dco + 1) & 3;
}

INLINE void OpenGL_Blitter::DrawQuad(float src_coords[4][2], int dest_coords[4][2])
{
    // Lower left
    p_glTexCoord2f(src_coords[3][0], src_coords[3][1]);
    p_glVertex2f(dest_coords[3][0], dest_coords[3][1]);

    // Lower right
    p_glTexCoord2f(src_coords[2][0], src_coords[2][1]);
    p_glVertex2f(dest_coords[2][0], dest_coords[2][1]);

    // Upper right
    p_glTexCoord2f(src_coords[1][0], src_coords[1][1]);
    p_glVertex2f(dest_coords[1][0], dest_coords[1][1]);

    // Upper left
    p_glTexCoord2f(src_coords[0][0], src_coords[0][1]);
    p_glVertex2f(dest_coords[0][0], dest_coords[0][1]);
}


void OpenGL_Blitter::DrawLinearIP(const unsigned UsingIP, const unsigned rotated, const MDFN_Rect *tex_src_rect, const MDFN_Rect *dest_rect, const uint32 tmpwidth, const uint32 tmpheight)
{
    MDFN_Rect tmp_sr = *tex_src_rect;
    MDFN_Rect tmp_dr = *dest_rect;
    float tmp_sc[4][2];
    int tmp_dc[4][2];

    int32 start_pos;
    int32 bound_pos;
    bool rotate_side = (rotated == MDFN_ROTATE90 || rotated == MDFN_ROTATE270);
    bool reversi;
    bool dr_y;
    bool sr_y;

    if((UsingIP == VIDEOIP_LINEAR_Y) ^ rotate_side)
    {
        start_pos = dest_rect->x;
        bound_pos = dest_rect->x + dest_rect->w;
        dr_y = false;
        sr_y = rotate_side;
    }
    else
    {
        start_pos = dest_rect->y;
        bound_pos = dest_rect->y + dest_rect->h;
        dr_y = true;
        sr_y = !rotate_side;
    }

    reversi = (rotated == MDFN_ROTATE270 && UsingIP == VIDEOIP_LINEAR_X) || (rotated == MDFN_ROTATE90 && UsingIP == VIDEOIP_LINEAR_Y);

    for(int i = start_pos; i < bound_pos; i++)
    {
        int sr_goon = i - start_pos;

        if(dr_y)
        {
            tmp_dr.y = i;
            tmp_dr.h = 1;
        }
        else
        {
            tmp_dr.x = i;
            tmp_dr.w = 1;
        }

        if(reversi)
        {
            sr_goon = (bound_pos - start_pos) - 1 - sr_goon;
        }

        if(sr_y)
        {
            tmp_sr.y = sr_goon * tex_src_rect->h / (rotate_side ? dest_rect->w : dest_rect->h);
            tmp_sr.h = 1;
        }
        else
        {
            tmp_sr.x = sr_goon * tex_src_rect->w / (rotate_side ? dest_rect->h : dest_rect->w);
            tmp_sr.w = 1;
        }

        MakeSourceCoords(&tmp_sr, tmp_sc, tmpwidth, tmpheight, 0);
        MakeDestCoords(&tmp_dr, tmp_dc, rotated);

        DrawQuad(tmp_sc, tmp_dc);
    }
}
uint32 *src_pixies;
void OpenGL_Blitter::Blit(const MDFN_Surface *src_surface, const MDFN_Rect *src_rect, const MDFN_Rect *dest_rect, const MDFN_Rect *original_src_rect, int InterlaceField, int UsingIP, int rotated)
{
    MDFN_Rect tex_src_rect = *src_rect;
    float src_coords[4][2];
    int dest_coords[4][2];
    unsigned int tmpwidth;
    unsigned int tmpheight;

    const bool ShaderIlace = (InterlaceField >= 0) && shader && shader->ShaderNeedsProperIlace();

    if(shader)
    {
        if(shader->ShaderNeedsBTIP())
        {
            UsingIP = VIDEOIP_BILINEAR;
        }
        else
        {
            UsingIP = VIDEOIP_OFF;
        }
    }

    if(tex_src_rect.w == 0 || tex_src_rect.h == 0 || dest_rect->w == 0 || dest_rect->h == 0 || original_src_rect->w == 0 || original_src_rect->h == 0)
    {
        printf("[BUG] OpenGL blitting nothing? --- %d:%d %d:%d %d:%d\n", tex_src_rect.w, tex_src_rect.h, dest_rect->w, dest_rect->h, original_src_rect->w, original_src_rect->h);
        return;
    }

    src_pixies = src_surface->pixels + tex_src_rect.x + tex_src_rect.y * src_surface->pitchinpix;

    int iterator = 0;
    int destIterator = 0;
    if (src_pixies)
    {
        
        for (int rows = 0; rows < 2*mednafenHeight; rows++)
        {
            for (int columns = 0; columns < mednafenWidth; columns++) 
            {
                iterator++;
                if (!(rows & 1))
                {
                    gMainBuffer[destIterator] = src_pixies[iterator] | 0xff000000;
                    destIterator++;
                }
            }
        }
    }
    tex_src_rect.x = 0;
    tex_src_rect.y = 0;
    tex_src_rect.h >>= ShaderIlace;

    MakeDestCoords(dest_rect, dest_coords, rotated);
    p_glBindTexture(GL_TEXTURE_2D, textures[0]);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, UsingIP ? GL_LINEAR : GL_NEAREST);
    p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, UsingIP ? GL_LINEAR : GL_NEAREST);

    if(SupportNPOT)
    {
        tmpwidth = tex_src_rect.w;
        tmpheight = tex_src_rect.h;

        if(tmpwidth != last_w || tmpheight != last_h)
        {
            mednafenWidth = tmpwidth;
            mednafenHeight = tmpheight;
            p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            last_w = tmpwidth;
            last_h = tmpheight;
        }
    }
    else
    {
        bool ImageSizeChange = FALSE;

        tmpwidth = round_up_pow2(tex_src_rect.w);
        tmpheight = round_up_pow2(tex_src_rect.h);

        // If the required GL texture size has changed, resize the texture! :b
        if(tmpwidth != round_up_pow2(last_w) || tmpheight != round_up_pow2(last_h))
        {
            mednafenWidth = tmpwidth;
            mednafenHeight = tmpheight;
            p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            ImageSizeChange = TRUE;
        }

        // If the dimensions of our image stored in the texture have changed...
        if(tex_src_rect.w != last_w || tex_src_rect.h != last_h)
        {
            ImageSizeChange = TRUE;
        }

        // Only clean up if we're using pixel shaders and/or bilinear interpolation
        if(ImageSizeChange && (shader || UsingIP))
        {
            uint32 neo_dbs = DummyBlackSize;

            if(tex_src_rect.w != tmpwidth && neo_dbs < tex_src_rect.h)
            {
                neo_dbs = tex_src_rect.h;
            }

            if(tex_src_rect.h != tmpheight && neo_dbs < tex_src_rect.w)
            {
                neo_dbs = tex_src_rect.w;
            }

            if(neo_dbs != DummyBlackSize)
            {
                if(DummyBlack)
                {
                 delete[] DummyBlack;
                 DummyBlack = NULL;
                 DummyBlackSize = 0;
                }

                try
                {
                    DummyBlack = new uint32[neo_dbs];
                    memset(DummyBlack, 0, sizeof(uint32) * neo_dbs);
                    DummyBlackSize = neo_dbs;
                }
                catch(...)
                {
                }
            }

            if(DummyBlack) // If memory allocation failed for some reason, don't clean the texture. :(
            {
                if(tex_src_rect.w < tmpwidth)
                {
                 //puts("X");
                 p_glPixelStorei(GL_UNPACK_ROW_LENGTH, 1);
                 p_glTexSubImage2D(GL_TEXTURE_2D, 0, tex_src_rect.w, 0, 1, tex_src_rect.h, GL_RGBA, GL_UNSIGNED_BYTE, DummyBlack);
                }
                if(tex_src_rect.h < tmpheight)
                {
                 //puts("Y");
                 p_glPixelStorei(GL_UNPACK_ROW_LENGTH, tex_src_rect.w);
                 p_glTexSubImage2D(GL_TEXTURE_2D, 0, 0, tex_src_rect.h, tex_src_rect.w, 1, GL_RGBA, GL_UNSIGNED_BYTE, DummyBlack);
                }
            } 
        }

        last_w = tex_src_rect.w;
        last_h = tex_src_rect.h;
    }

    MakeSourceCoords(&tex_src_rect, src_coords, tmpwidth, tmpheight, (ShaderIlace & InterlaceField) * -0.5);
}

void OpenGL_Blitter::Cleanup(void)
{
    if(textures[0])
    {
        p_glDeleteTextures(4, &textures[0]);
    }

    textures[0] = textures[1] = textures[2] = textures[3] = 0;
    mednafenTextures = false;

    if(DummyBlack)
    {
        delete[] DummyBlack;
        DummyBlack = NULL;
    }
    DummyBlackSize = 0;

    if(shader)
    {
        delete shader;
        shader = NULL;
    }
}

OpenGL_Blitter::~OpenGL_Blitter()
{
    Cleanup();
}

static bool CheckExtension(const char *extensions, const char *testval)
{
    const char *extparse = extensions;
    const size_t testval_len = strlen(testval);

    while((extparse = strstr(extparse, testval)))
    {
        if(extparse == extensions || *(extparse - 1) == ' ')
        {
            if(extparse[testval_len] == ' ' || extparse[testval_len] == 0)
            {
                return true;
            }
        }
     extparse += testval_len;
    }
    return false;
}

static bool CheckAlternateFormat(const uint32 version_h)
{
    return false;
}

/* Rectangle, left, right(not inclusive), top, bottom(not inclusive). */
OpenGL_Blitter::OpenGL_Blitter(int scanlines, ShaderType pixshader, const ShaderParams& shader_params, int *rs, int *gs, int *bs, int *as)
{
    try
    {
        const char *extensions;
        const char *vendor;
        const char *renderer;
        const char *version;
        uint32 version_h;

        MaxTextureSize = 0;
        SupportNPOT = false;
        SupportARBSync = false;
        PixelFormat = 0;
        PixelType = 0;
        for(unsigned i = 0; i < 4; i++)
        {
            textures[i] = 0;
        }

        using_scanlines = 0;
        last_w = 0;
        last_h = 0;

        OSDLastWidth = 0;
        OSDLastHeight = 0;

        shader = NULL;

        DummyBlack = NULL;
        DummyBlackSize = 0;

        gl_screen_w = 0;
        gl_screen_h = 0;

        #define LFG(x) p_##x = x
        #define LFGN(x) p_##x = x

        LFG(glGetError);
        LFG(glBindTexture);
        LFGN(glColorTableEXT);
        LFG(glTexImage2D);
        LFG(glBegin);
        LFG(glVertex2f);
        LFG(glTexCoord2f);
        LFG(glEnd);
        LFG(glEnable);
        LFG(glBlendFunc);
        LFG(glGetString);
        LFG(glViewport);
        LFG(glGenTextures);
        LFG(glDeleteTextures);
        LFG(glTexParameteri);
        LFG(glClearColor);
        LFG(glLoadIdentity);
        LFG(glClear);
        LFG(glMatrixMode);
        LFG(glDisable);
        LFG(glPixelStorei);
        LFG(glTexSubImage2D);
        LFG(glFinish);
        LFG(glOrtho);
        LFG(glPixelTransferf);
        LFG(glColorMask);
        LFG(glTexEnvf);
        LFG(glGetIntegerv);
        LFG(glTexGend);
        LFG(glRasterPos2i);
        LFG(glDrawPixels);
        LFG(glPixelZoom);
        LFG(glAccum);
        LFG(glClearAccum);
        LFG(glGetTexLevelParameteriv);
        LFG(glPushMatrix);
        LFG(glPopMatrix);
        LFG(glRotated);
        LFG(glScalef);
        LFG(glReadPixels);

        vendor = (const char *)p_glGetString(GL_VENDOR);
        renderer = (const char *)p_glGetString(GL_RENDERER);
        version = (const char *)p_glGetString(GL_VERSION);

        {
            int major = 0, minor = 0;
            trio_sscanf(version, "%d.%d", &major, &minor);
            if(minor < 0) minor = 0;
            if(minor > 255) minor = 255;

            version_h = (major << 8) | minor;
        }

        MDFN_printf(_("OpenGL Implementation: %s %s %s\n"), vendor, renderer, version);

        MDFN_printf(_("Checking extensions:\n"));
        MDFN_indent(1);

        SupportNPOT = FALSE;
        SupportARBSync = false;

        if (glewGetExtension("GL_ARB_texture_non_power_of_two"))
        {
            MDFN_printf(_("GL_ARB_texture_non_power_of_two found.\n"));
            SupportNPOT = TRUE;
        }

        if (glewGetExtension("GL_ARB_sync"))
        {
            MDFN_printf(_("GL_ARB_sync found.\n"));
            LFG(glFenceSync);
            LFG(glIsSync);
            LFG(glDeleteSync);
            LFG(glClientWaitSync);
            LFG(glWaitSync);
            LFG(glGetInteger64v);
            LFG(glGetSynciv);
            SupportARBSync = true;
        }

        MDFN_indent(-1);

        p_glGenTextures(4, &textures[0]);
        for(int i = 0; i < 4; i++)
        {
            mednafenTextureIDs[i] = textures[i];
        }
        mednafenTextures = true;
        using_scanlines = 0;

        shader = NULL;

        if(pixshader != SHADER_NONE)
        {
            LFG(glCreateShaderObjectARB);
            LFG(glShaderSourceARB);
            LFG(glCompileShaderARB);
            LFG(glCreateProgramObjectARB);
            LFG(glAttachObjectARB);
            LFG(glLinkProgramARB);
            LFG(glUseProgramObjectARB);
            LFG(glUniform1fARB);
            LFG(glUniform2fARB);
            LFG(glUniform3fARB);
            LFG(glUniform1iARB);
            LFG(glUniform2iARB);
            LFG(glUniform3iARB);
            LFG(glUniformMatrix2fvARB);
            LFG(glUniform1fvARB);
            LFG(glUniform2fvARB);
            LFG(glUniform3fvARB);
            LFG(glUniform4fvARB);
            LFG(glActiveTextureARB);
            LFG(glGetInfoLogARB);
            LFG(glGetUniformLocationARB);
            LFG(glDeleteObjectARB);
            LFG(glDetachObjectARB);

            LFG(glGetObjectParameterivARB);

            shader = new OpenGL_Blitter_Shader(this, pixshader, shader_params);
            SupportNPOT = 0;      // Our pixel shaders don't work right with NPOT textures(for them to do so would probably necessitate rewriting them to use texelFetch)
            p_glActiveTextureARB(GL_TEXTURE0_ARB);

            if(pixshader == SHADER_GOAT && shader_params.goat_slen)
            {
                scanlines = 0;
            }
        }

        if(SupportNPOT)
        {
            MDFN_printf(_("Using non-power-of-2 sized textures.\n"));
        }
        else
        {
            MDFN_printf(_("Using power-of-2 sized textures.\n"));
        }

        if(scanlines)
        {
            int slcount;

            using_scanlines = scanlines;
            p_glBindTexture(GL_TEXTURE_2D, textures[1]);
            p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

            std::unique_ptr<uint8[]> buf(new uint8[64 * (256 * 2) * 4]);

            slcount = 0;
            for(int y=0;y<(256 * 2);y++)
            {
                for(int x=0;x<64;x++)
                {
                   int sl_alpha;
   
                   if(slcount)
                   {
                       sl_alpha = 0xFF - (0xFF * abs(scanlines) / 100);
                   }
                   else
                   {
                       sl_alpha = 0xFF;
                   }
   
                   buf[y*64*4+x*4]=0;
                   buf[y*64*4+x*4+1]=0;
                   buf[y*64*4+x*4+2]=0;
                   buf[y*64*4+x*4+3] = sl_alpha;
                }
                slcount ^= 1;
            }
            p_glPixelStorei(GL_UNPACK_ROW_LENGTH, 64);
            p_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 256 * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf.get());
        }
        p_glBindTexture(GL_TEXTURE_2D, textures[3]);
        p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        p_glBindTexture(GL_TEXTURE_2D, textures[0]);

        p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        p_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        p_glBindTexture(GL_TEXTURE_2D, textures[2]);

        p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        p_glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

        p_glEnable(GL_TEXTURE_2D);
        p_glClearColor(0.1f, 0.2f, 0.9f, 0.0f);    // Background color to black.
        p_glMatrixMode(GL_MODELVIEW);

        p_glLoadIdentity();
        p_glFinish();

        p_glDisable(GL_TEXTURE_1D);
        p_glDisable(GL_FOG);
        p_glDisable(GL_LIGHTING);
        p_glDisable(GL_LOGIC_OP);
        p_glDisable(GL_DITHER);
        p_glDisable(GL_COLOR_MATERIAL);
        p_glDisable(GL_NORMALIZE);
        p_glDisable(GL_SCISSOR_TEST);
        p_glDisable(GL_STENCIL_TEST);
        p_glDisable(GL_ALPHA_TEST);
        p_glDisable(GL_DEPTH_TEST);

        p_glPixelTransferf(GL_RED_BIAS, 0);
        p_glPixelTransferf(GL_GREEN_BIAS, 0);
        p_glPixelTransferf(GL_BLUE_BIAS, 0);
        p_glPixelTransferf(GL_ALPHA_BIAS, 0);

        p_glPixelTransferf(GL_RED_SCALE, 1);
        p_glPixelTransferf(GL_GREEN_SCALE, 1);
        p_glPixelTransferf(GL_BLUE_SCALE, 1);
        p_glPixelTransferf(GL_ALPHA_SCALE, 1);

        p_glPixelTransferf(GL_MAP_COLOR, GL_FALSE);

        last_w = 0;
        last_h = 0;

        OSDLastWidth = OSDLastHeight = 0;


        MDFN_printf(_("Checking maximum texture size...\n"));
        MDFN_indent(1);
        p_glBindTexture(GL_TEXTURE_2D, textures[0]);
        // Assume maximum texture width is the same as maximum texture height to greatly simplify things
        MaxTextureSize = 32768;

        while(MaxTextureSize)
        {
            GLint width_test = 0;
            p_glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA, MaxTextureSize, MaxTextureSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            p_glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width_test);

            if((unsigned int)width_test == MaxTextureSize)
            {
                break;
            }

            MaxTextureSize >>= 1;
        }
        MDFN_printf(_("Apparently it is at least: %d x %d\n"), MaxTextureSize, MaxTextureSize);

        if(MaxTextureSize < 256)
        {
            MDFN_printf(_("Warning:  Maximum texture size is reported as being less than 256, but we can't handle that.\n"));
            MaxTextureSize = 256;
        }

        DummyBlack = NULL;
        DummyBlackSize = 0;

        MDFN_indent(-1);

        if(!CheckAlternateFormat(version_h))
        {
            #ifdef LSB_FIRST
            *rs = 0;
            *gs = 8;
            *bs = 16;
            *as = 24;
            #else
            *rs = 24;
            *gs = 16;
            *bs = 8;
            *as = 0;
            #endif
            PixelFormat = GL_RGBA;
            PixelType = GL_UNSIGNED_BYTE;
            MDFN_printf(_("Using GL_RGBA, GL_UNSIGNED_BYTE for texture source data.\n"));
        }
        else
        {
            *as = 24;
            *rs = 16;
            *gs = 8;
            *bs = 0;
            PixelFormat = GL_BGRA;
            PixelType = GL_UNSIGNED_INT_8_8_8_8_REV;
            MDFN_printf(_("Using GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV for texture source data.\n"));
        }
    }
    catch(...)
    {
        Cleanup();
        throw;
    }
}

void OpenGL_Blitter::SetViewport(int w, int h)
{
    p_glFinish();
    gl_screen_w = w;
    gl_screen_h = h;

    p_glMatrixMode(GL_MODELVIEW);
    p_glLoadIdentity();
    // x,y specify LOWER left corner of the viewport.
    p_glViewport(0, 0, gl_screen_w, gl_screen_h);
    p_glOrtho(0, gl_screen_w, gl_screen_h, 0, -1.0, 1.0);
}

void OpenGL_Blitter::ClearBackBuffer(void)
{
 //if(1)
 //{
  p_glClearAccum(0.0, 0.0, 0.0, 1.0);
  p_glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
 //}
 //else
 //{
 // p_glClear(GL_COLOR_BUFFER_BIT);
 //}
}

