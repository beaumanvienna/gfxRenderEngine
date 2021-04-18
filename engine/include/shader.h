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

bool LoadShaderFromFile(std::string& shader, const std::string filename);

int CreateShader(const std::string& vertextShader, const std::string& fragmentShader);

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

extern int gShaderStatus;
