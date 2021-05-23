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

#include <vector>
#include <map>

#include "engine.h"
#include "platform.h"
#include "glm.hpp"

class ShaderProgram
{
    
public:
    
    enum ShaderProgramTypes
    {
        VERTEX_SHADER,  
        FRAGMENT_SHADER
    };

public:

    ~ShaderProgram() {}
    virtual int AddShader(const ShaderProgramTypes type, const std::string& shaderFileName) = 0;
    virtual int Build() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual bool IsOK() const = 0;
    virtual void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;
    virtual void setUniform1i(const std::string& name, int i0) = 0;
    virtual void setUniform1iv(const std::string& name, int count, int* i0) = 0;
    virtual void setUniformMat4f(const std::string& name, const glm::mat4& modelViewProjection) = 0;

    static std::shared_ptr<ShaderProgram> Create();

};
