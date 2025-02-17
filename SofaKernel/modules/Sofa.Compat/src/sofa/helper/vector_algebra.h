/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#pragma once

#include <sofa/type/vector_algebra.h>

SOFA_DEPRECATED_HEADER("v21.06", "v21.12", "sofa/type/vector_algebra.h")

namespace sofa::helper
{
    template<class V1, class V2>
    SReal dot(const V1& vector1, const V2& vector2)
    {
        return sofa::type::dot(vector1, vector2);
    }

    /// Norm of a vector
    template<class V>
    SReal norm(const V& v)
    {
        return sofa::type::norm(v);
    }

    /// Vector operation: result = ax + y
    template<class V1, class Scalar, class V2, class V3>
    void axpy(V1& result, Scalar a, const V2& x, const V3& y)
    {
         sofa::type::axpy(result, a, x, y);
    }

} // namespace sofa::helper
