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

#include "scabb/rayTracing/material.h"
#include "scabb/rayTracing/aux.h"

namespace ScabbApp
{
    glm::vec3 Reflect(const glm::vec3& vector, const glm::vec3& normal)
    {
        return vector - 2*glm::dot(vector, normal)*normal;
    }

    bool Lambertian::Scatter(const Ray& rayIn, const HitRecord& record,
                             glm::color& attenuation, Ray& scattered) const
    {
        auto scatterDirection = record.m_Normal + RandomUnitVector();
        scattered = Ray(record.m_Point, scatterDirection);
        attenuation = m_Albedo;

        // Catch degenerate scatter direction
        if (NearZero(scatterDirection))
        {
            scatterDirection = record.m_Normal;
        }

        return true;
    }

    bool Metal::Scatter(const Ray& rayIn, const HitRecord& record,
                             glm::color& attenuation, Ray& scattered) const
    {
        glm::vec3 reflected = Reflect(glm::normalize(rayIn.GetDirection()), record.m_Normal);
        scattered = Ray(record.m_Point, reflected);
        attenuation = m_Albedo;
        return (glm::dot(scattered.GetDirection(), record.m_Normal) > 0);
    }
}
