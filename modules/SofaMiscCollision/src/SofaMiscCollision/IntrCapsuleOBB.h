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
// File modified from GeometricTools
// http://www.geometrictools.com/


#pragma once
#include <SofaMiscCollision/config.h>

#include <SofaBaseCollision/Intersector.h>
#include <SofaMiscCollision/CapsuleModel.h>
#include <SofaMiscCollision/RigidCapsuleModel.h>
#include <SofaMiscCollision/OBBModel.h>

namespace sofa::component::collision
{
/**
  *TDataTypes is the capsule type and TDataTypes2 the OBB type.
  */
template <typename TDataTypes,typename TDataTypes2>
class TIntrCapsuleOBB : public Intersector<typename TDataTypes::Real>
{
public:
    typedef TCapsule<TDataTypes> IntrCap;
    typedef typename IntrCap::Real Real;
    typedef typename IntrCap::Coord Coord;
    typedef TOBB<TDataTypes2> Box;
    typedef sofa::type::Vec<3,Real> Vec3;

    TIntrCapsuleOBB (const IntrCap& capsule, const Box & box);

    bool Find (Real dmax);
private:
    // The objects to intersect.
    const IntrCap* _cap;
    const Box * mBox;

    // Information about the intersection set.
    using Intersector<Real>::_is_colliding;
    using Intersector<Real>::_pt_on_first;
    using Intersector<Real>::_pt_on_second;
    using Intersector<Real>::mContactTime;
    using Intersector<Real>::_sep_axis;
};

typedef TIntrCapsuleOBB<sofa::defaulttype::Vec3Types, sofa::defaulttype::Rigid3Types> IntrCapsuleOBB;

#if  !defined(SOFA_COMPONENT_COLLISION_INTRCAPSULEOBB_CPP)
extern template class SOFA_MISC_COLLISION_API TIntrCapsuleOBB<sofa::defaulttype::Vec3Types, sofa::defaulttype::Rigid3Types>;
extern template class SOFA_MISC_COLLISION_API TIntrCapsuleOBB<sofa::defaulttype::Rigid3Types, sofa::defaulttype::Rigid3Types>;

#endif

} // namespace sofa::component::collision
