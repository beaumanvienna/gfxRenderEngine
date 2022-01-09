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

#include "scabb/rayTracing/sphere.h"

namespace ScabbApp
{

    bool Sphere::Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
    {
        glm::vec3 oc = ray.GetOrigin() - m_Center;
        auto a = glm::length2(ray.GetDirection());
        auto half_b = glm::dot(oc, ray.GetDirection());
        auto c = glm::length2(oc) - m_Radius * m_Radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0)
        {
            return false;
        }
        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < tMin || tMax < root)
        {
            root = (-half_b + sqrtd) / a;
            if (root < tMin || tMax < root)
            {
                return false;
            }
        }

        record.m_DistanceToRayOrigin = root;
        record.m_Point = ray.At(record.m_DistanceToRayOrigin);
        glm::vec3 outwardNormal = (record.m_Point - m_Center) / m_Radius;
        record.SetFaceNormal(ray, outwardNormal);

        return true;
    }
}
