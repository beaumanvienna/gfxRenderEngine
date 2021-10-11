/* Copyright (c) 2013-2020 PPSSPP project
   https://github.com/hrydgard/ppsspp/blob/master/LICENSE.TXT
   
   Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/gfxRenderEngine

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "common.h"
#include "context.h"
#include "matrix.h"
#include "drawBuffer.h"
#include "textureAtlas.h"
#include "screen.h"
#include "marley/marley.h"
#include "renderCommand.h"

inline SCREEN_UI::Style MakeStyle(uint32_t fg, uint32_t bg)
{
    SCREEN_UI::Style s;
    s.background = SCREEN_UI::Drawable(bg);
    s.fgColor = fg;

    return s;
}

SCREEN_UIContext::SCREEN_UIContext()
{
    fontStyle_ = new SCREEN_UI::FontStyle();
    m_ContextWidth  = Engine::m_Engine->GetContextWidth();
    m_ContextHeight = Engine::m_Engine->GetContextHeight();
    m_HalfContextWidth  = m_ContextWidth  * 0.5f;
    m_HalfContextHeight = m_ContextHeight * 0.5f;
    bounds_ = Bounds(0, 0, m_ContextWidth, m_ContextHeight);
    uidrawbuffer_ = new SCREEN_DrawBuffer();
    
    UIThemeInit();

    ui_theme.checkOn         = SCREEN_ScreenManager::m_SpritesheetUI->GetSprite(I_CHECKEDBOX);
    ui_theme.checkOff        = SCREEN_ScreenManager::m_SpritesheetUI->GetSprite(I_SQUARE);
    ui_theme.whiteImage      = SCREEN_ScreenManager::m_SpritesheetUI->GetSprite(I_WHITE);
    ui_theme.sliderKnob      = SCREEN_ScreenManager::m_SpritesheetUI->GetSprite(I_CIRCLE);
    ui_theme.dropShadow4Grid = SCREEN_ScreenManager::m_SpritesheetUI->GetSprite(I_DROP_SHADOW);
    
    theme = &ui_theme;
}

void SCREEN_UIContext::UIThemeInit()
{
    if (CoreSettings::m_UITheme == THEME_RETRO)
    {
        ui_theme.uiFont = SCREEN_UI::FontStyle(FontID("RETRO24"), "", 22);
        ui_theme.uiFontSmall = SCREEN_UI::FontStyle(FontID("RETRO24"), "", 18);
        ui_theme.uiFontSmaller = SCREEN_UI::FontStyle(FontID("RETRO24"), "", 8);
    
        ui_theme.itemStyle = MakeStyle(RETRO_COLOR_FONT_FOREGROUND, 0x80000000);
        ui_theme.itemFocusedStyle = MakeStyle(0xFFFFFFFF, 0xA0000000); // active icons
        ui_theme.itemDownStyle = MakeStyle(0xFFFFFFFF, 0xB0000000);
        ui_theme.itemDisabledStyle = MakeStyle(0xffEEEEEE, 0x55E0D4AF);
        ui_theme.itemHighlightedStyle = MakeStyle(0xFFFFFFFF, 0x55ffffff); //

        ui_theme.buttonStyle = MakeStyle(RETRO_COLOR_FONT_FOREGROUND, 0x70000000); // inactive button
        ui_theme.buttonFocusedStyle = MakeStyle(RETRO_COLOR_FONT_FOREGROUND, RETRO_COLOR_FONT_BACKGROUND); // active button
        ui_theme.buttonDownStyle = MakeStyle(0xFFFFFFFF, 0xFFBD9939);
        ui_theme.buttonDisabledStyle = MakeStyle(0x80EEEEEE, 0x55E0D4AF);
        ui_theme.buttonHighlightedStyle = MakeStyle(0xFFFFFFFF, 0x55BDBB39);

        ui_theme.headerStyle.fgColor = RETRO_COLOR_FONT_FOREGROUND;
        ui_theme.infoStyle = MakeStyle(RETRO_COLOR_FONT_FOREGROUND, 0x00000000U);

        ui_theme.popupTitle.fgColor = RETRO_COLOR_FONT_FOREGROUND;
        ui_theme.popupStyle = MakeStyle(0xFFFFFFFF, 0xFF303030);
        
        m_Font = FontID{ "RETRO24" };
    }
    else
    {
        ui_theme.uiFont = SCREEN_UI::FontStyle(FontID("UBUNTU24"), "", 26);
        ui_theme.uiFontSmall = SCREEN_UI::FontStyle(FontID("UBUNTU24"), "", 24);
        ui_theme.uiFontSmaller = SCREEN_UI::FontStyle(FontID("UBUNTU24"), "", 24);
        
        ui_theme.itemStyle = MakeStyle(0xFFFFFFFF, 0x55000000);
        ui_theme.itemFocusedStyle = MakeStyle(0xFFFFFFFF, 0xA0000000);
        ui_theme.itemDownStyle = MakeStyle(0xFFFFFFFF, 0xFFBD9939);
        ui_theme.itemDisabledStyle = MakeStyle(0x80EEEEEE, 0x55E0D4AF);
        ui_theme.itemHighlightedStyle = MakeStyle(0xFFFFFFFF, 0x55BDBB39);

        ui_theme.buttonStyle = MakeStyle(0xFFFFFFFF, 0x55000000);
        ui_theme.buttonFocusedStyle = MakeStyle(0xFFFFFFFF, 0xB0000000);
        ui_theme.buttonDownStyle = MakeStyle(0xFFFFFFFF, 0xFFBD9939);
        ui_theme.buttonDisabledStyle = MakeStyle(0x80EEEEEE, 0x55E0D4AF);
        ui_theme.buttonHighlightedStyle = MakeStyle(0xFFFFFFFF, 0x55BDBB39);

        ui_theme.headerStyle.fgColor = 0xFFFFFFFF;
        ui_theme.infoStyle = MakeStyle(0xFFFFFFFF, 0x00000000U);

        ui_theme.popupTitle.fgColor = 0xFFE3BE59;
        ui_theme.popupStyle = MakeStyle(0xFFFFFFFF, 0xFF303030);
        
        m_Font = FontID{ "UBUNTU24" };
    }
}

//void SCREEN_UIContext::Init(SCREEN_Draw::SCREEN_DrawContext *thin3d, SCREEN_Draw::SCREEN_Pipeline *uipipe, 
//                            SCREEN_Draw::SCREEN_Pipeline *uipipenotex, SCREEN_DrawBuffer *uidrawbuffer,
//                            SCREEN_DrawBuffer *uidrawbufferTop)
//{
//    using namespace SCREEN_Draw;
//    draw_ = thin3d;
//    sampler_ = draw_->CreateSamplerState({ SCREEN_TextureFilter::LINEAR, SCREEN_TextureFilter::LINEAR, SCREEN_TextureFilter::LINEAR });
//    ui_pipeline_ = uipipe;
//    ui_pipeline_notex_ = uipipenotex;
//    uidrawbuffer_ = uidrawbuffer;
//    uidrawbufferTop_ = uidrawbufferTop;
//    textDrawer_ = SCREEN_TextDrawer::Create(thin3d);  // May return nullptr if no implementation is available for this platform.
//}
//
//void SCREEN_UIContext::BeginFrame()
//{
//    if (!uitexture_) {
//        uitexture_ = CreateTextureFromFile(draw_, "ui_atlas.zim", ImageFileType::ZIM, false);
//        _dbg_assert_msg_(uitexture_, "Failed to load ui_atlas.zim.\n\nPlace it in the directory \"assets\" under your PPSSPP directory.");
//    }
//    uidrawbufferTop_->SetCurZ(0.0f);
//    uidrawbuffer_->SetCurZ(0.0f);
//    ActivateTopScissor();
//}
//
//void SCREEN_UIContext::Begin()
//{
//    BeginPipeline(ui_pipeline_, sampler_);
//}
//
//void SCREEN_UIContext::BeginNoTex()
//{
//    draw_->BindSamplerStates(0, 1, &sampler_);
//    UIBegin(ui_pipeline_notex_);
//}
//
//void SCREEN_UIContext::BeginPipeline(SCREEN_Draw::SCREEN_Pipeline *pipeline, SCREEN_Draw::SCREEN_SamplerState *samplerState)
//{
//    draw_->BindSamplerStates(0, 1, &samplerState);
//    RebindTexture();
//    UIBegin(pipeline);
//}
//
//void SCREEN_UIContext::RebindTexture() const 
//{
//    if (uitexture_)
//        draw_->BindTexture(0, uitexture_->GetTexture());
//}

void SCREEN_UIContext::Flush()
{
    #ifdef LINUX
        #warning "fix me"
    #endif
    MarleyApp::Marley::m_Application->Flush();
}

//void SCREEN_UIContext::SetCurZ(float curZ)
//{
//    SCREEN_ui_draw2d.SetCurZ(curZ);
//    SCREEN_ui_draw2d_front.SetCurZ(curZ);
//}
//
void SCREEN_UIContext::PushScissor(const Bounds &bounds)
{
    Flush();
    Bounds clipped = TransformBounds(bounds);
    if (scissorStack_.size())
    {
        clipped.Clip(scissorStack_.back());
    }
    else
    {
        clipped.Clip(bounds_);
    }
    scissorStack_.push_back(clipped);
    ActivateTopScissor();
}

void SCREEN_UIContext::PopScissor()
{
    Flush();
    scissorStack_.pop_back();
    ActivateTopScissor();
}

Bounds SCREEN_UIContext::GetScissorBounds()
{
    if (!scissorStack_.empty())
    {
        return scissorStack_.back();
    }
    else
    {
        return bounds_;
    }
}

Bounds SCREEN_UIContext::GetLayoutBounds() const 
{
    Bounds bounds = GetBounds();

    return bounds;
}

void SCREEN_UIContext::ActivateTopScissor()
{
    Bounds bounds;
    if (scissorStack_.size()) 
    {
        float scale_x = Engine::m_Engine->GetWindowWidth() / Engine::m_Engine->GetContextWidth();
        float scale_y = Engine::m_Engine->GetWindowHeight() / Engine::m_Engine->GetContextHeight();
        bounds = scissorStack_.back();
        int x = floorf(scale_x * bounds.x);
        int y = Engine::m_Engine->GetWindowHeight() - floorf(scale_y * (bounds.y + bounds.h));
        int w = std::max(0.0f, ceilf(scale_x * bounds.w));
        int h = std::max(0.0f, ceilf(scale_y * bounds.h));
        RenderCommand::SetScissor(x, y, w, h);
    } 
    else 
    {
        RenderCommand::SetScissor(0, 0, Engine::m_Engine->GetWindowWidth(), Engine::m_Engine->GetWindowHeight());
    }
}

void SCREEN_UIContext::SetFontScale(float scaleX, float scaleY)
{
    fontScaleX_ = scaleX;
    fontScaleY_ = scaleY;
}

void SCREEN_UIContext::SetFontStyle(const SCREEN_UI::FontStyle& fontStyle)
{
    *fontStyle_ = fontStyle;
}

void SCREEN_UIContext::MeasureText(const SCREEN_UI::FontStyle &style, float scaleX, float scaleY, const char *str, float *x, float *y, int align) const 
{
    MeasureTextCount(style, scaleX, scaleY, str, (int)strlen(str), x, y, align);
}

void SCREEN_UIContext::MeasureTextCount(const SCREEN_UI::FontStyle &style, float scaleX, float scaleY, const char *str, int count, float *x, float *y, int align) const 
{
    float sizeFactor = (float)style.sizePts / 24.0f;
    Draw()->SetFontScale(scaleX * sizeFactor, scaleY * sizeFactor);
    Draw()->MeasureTextCount(style.atlasFont, str, count, x, y);
}

void SCREEN_UIContext::MeasureTextRect(const SCREEN_UI::FontStyle &style, float scaleX, float scaleY, const char *str, int count, const Bounds &bounds, float *x, float *y, int align) const
{
    float sizeFactor = (float)style.sizePts / 24.0f;
    Draw()->SetFontScale(scaleX * sizeFactor, scaleY * sizeFactor);
    Draw()->MeasureTextRect(m_Font, str, count, bounds, x, y, align);
}

void SCREEN_UIContext::DrawText(const char *str, float x, float y, uint32_t color, int align)
{
    float sizeFactor = (float)fontStyle_->sizePts / 24.0f;
    Draw()->SetFontScale(fontScaleX_ * sizeFactor, fontScaleY_ * sizeFactor);
    Draw()->DrawText(fontStyle_->atlasFont, str, x, y, color, align);
}

//void SCREEN_UIContext::DrawTextShadow(const char *str, float x, float y, uint32_t color, int align)
//{
//    uint32_t alpha = (color >> 1) & 0xFF000000;
//    DrawText(str, x + 2, y + 2, alpha, align);
//    DrawText(str, x, y, color, align);
//}
//
void SCREEN_UIContext::DrawTextRect(const char *str, const Bounds &bounds, uint32_t color, int align)
{    
    float sizeFactor = (float)fontStyle_->sizePts / 24.0f;
    Draw()->SetFontScale(fontScaleX_ * sizeFactor, fontScaleY_ * sizeFactor);
    Draw()->DrawTextRect(m_Font, str, bounds.x, bounds.y, bounds.w, bounds.h, color, align);
    
}
namespace MarleyApp
{
    extern Sprite* whiteImage;
}

void SCREEN_UIContext::FillRect(const SCREEN_UI::Drawable &drawable, const Bounds &bounds)
{
    if ((drawable.color & 0xFF000000) == 0)
    {
        return;
    }
    
    std::shared_ptr<Renderer> renderer = Engine::m_Engine->GetRenderer();
    switch (drawable.type)
    {
        case SCREEN_UI::DRAW_SOLID_COLOR:
            uidrawbuffer_->DrawImageStretch(theme->whiteImage, bounds.x, bounds.y, bounds.x2(), bounds.y2(), drawable.color);
            break;
        case SCREEN_UI::DRAW_4GRID:
            // uidrawbuffer_->DrawImage4Grid(drawable.image, bounds.x, bounds.y, bounds.x2(), bounds.y2(), drawable.color);
            LOG_CORE_ERROR("not supported: case SCREEN_UI::DRAW_4GRID");
            break;
        case SCREEN_UI::DRAW_STRETCH_IMAGE:
            // uidrawbuffer_->DrawImageStretch(drawable.image, bounds.x, bounds.y, bounds.x2(), bounds.y2(), drawable.color);
            LOG_CORE_ERROR("not supported: case SCREEN_UI::DRAW_STRETCH_IMAGE");
            break;
        case SCREEN_UI::DRAW_NOTHING:
        {
            if (MarleyApp::debugUI)
            {
                glm::vec3 scaleVec = glm::vec3(bounds.w, bounds.h, 0.0f);
                glm::vec3 translation = glm::vec3(m_HalfContextWidth - bounds.centerX(), m_HalfContextHeight - bounds.centerY(), 0.0f);
                glm::mat4 transformationMatrix = Translate(translation) * Scale(scaleVec);
                
                // transformed position
                glm::mat4 position = transformationMatrix * MarleyApp::whiteImage->GetScaleMatrix();
                glm::vec4 color(0.8f, 0.1f, 0.1f, 0.5f);
                renderer->Draw(MarleyApp::whiteImage, position, -0.4f, color);
            }
            
            break;
        }
    } 
}

//void SCREEN_UIContext::PushTransform(const UITransform &transform)
//{
//    Flush();
//
//    using namespace SCREEN_Lin;
//
//    SCREEN_Matrix4x4 m = Draw()->GetDrawMatrix();
//    const SCREEN_Vec3 &t = transform.translate;
//    SCREEN_Vec3 scaledTranslate = SCREEN_Vec3(
//        t.x * m.xx + t.y * m.xy + t.z * m.xz + m.xw,
//        t.x * m.yx + t.y * m.yy + t.z * m.yz + m.yw,
//        t.x * m.zx + t.y * m.zy + t.z * m.zz + m.zw);
//
//    m.translateAndScale(scaledTranslate, transform.scale);
//    Draw()->PushDrawMatrix(m);
//    Draw()->PushAlpha(transform.alpha);
//
//    transformStack_.push_back(transform);
//}
//
//void SCREEN_UIContext::PopTransform()
//{
//    Flush();
//
//    transformStack_.pop_back();
//
//    Draw()->PopDrawMatrix();
//    Draw()->PopAlpha();
//}

Bounds SCREEN_UIContext::TransformBounds(const Bounds &bounds)
{
    if (!transformStack_.empty()) {
        const UITransform t = transformStack_.back();
        Bounds translated = bounds.Offset(t.translate.x, t.translate.y);

        float scaledX = (translated.x - m_HalfContextWidth) * t.scale.x + m_HalfContextWidth;
        float scaledY = (translated.y - m_HalfContextHeight) * t.scale.y + m_HalfContextHeight;

        return Bounds(scaledX, scaledY, translated.w * t.scale.x, translated.h * t.scale.y);
    }

    return bounds;
}
