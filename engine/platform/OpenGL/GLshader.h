/* Engine Copyright (c) 2021 Engine Development Team 
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

#pragma once

#include "engine.h"
#include "platform.h"
#include "shader.h"

enum shaderStatus
{
    SHADER_OK                           =  1,
    SHADER_STATUS_UNDEFINED             =  0,
    SHADER_ERROR_COMPILE_FAILED         = -1,
    SHADER_ERROR_COULD_NOT_LOAD_FILE    = -2,
    SHADER_ERROR_EMPTY_FILE             = -3,
    SHADER_ERROR_LINKING_FAILED         = -4,
    SHADER_ERROR_VALIDATION_FAILED      = -5,
    SHADER_ERROR_CREATION_FAILED        = -6,
    SHADER_ID_INVALID                   = -7
};

enum uniformStatus
{
    UNDEFINED_UNIFORM_LOCATION = -1
};

class GLShader
{
public:
    GLShader(const int type, const std::string fileName);
    ~GLShader();
    
    void Bind();
    void Unbind() const;
    bool Compile();
    bool IsOK() { return m_ShaderIsLoaded; }
    int ID() { return m_RendererID; }
    
private:
    uint m_RendererID;
    std::string m_FileName;
    int m_Type;
    bool m_ShaderIsLoaded;
    std::string m_ShaderSourceCode;
    int m_ShaderStatus;
    
    bool LoadFromFile();
};

class GLShaderProgram: public ShaderProgram
{

public:

    GLShaderProgram();
    ~GLShaderProgram();
    virtual int AddShader(const ShaderProgramTypes type, const std::string& shaderFileName) override;
    virtual int Build() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual bool IsOK() const override { return m_ShaderStatus == SHADER_OK; }
    virtual void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) override;
    virtual void SetUniform1i(const std::string& name, int i0) override;
    virtual void SetUniform1iv(const std::string& name, int count, int* i0) override;
    virtual void SetUniformMat4f(const std::string& name, const glm::mat4& modelViewProjection) override;
    
private:

    uint m_RendererID;
    bool m_ShadersAreLoaded;
    int m_ShaderStatus;
    std::vector<GLShader> m_Shaders;
    
    //caching uniforms
    int GetUniformLocation(const std::string& name);
    std::map<std::string, int> uniformLocationCache;
    
};
