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
#include <SofaMiscCollision/BarycentricStickContact.inl>
#include <SofaMeshCollision/BarycentricContactMapper.h>
#include <SofaMeshCollision/IdentityContactMapper.h>

using namespace sofa::core::collision ;

namespace sofa
{

namespace component
{

namespace collision
{

Creator<Contact::Factory, BarycentricStickContact<SphereCollisionModel<sofa::defaulttype::Vec3Types>, SphereCollisionModel<sofa::defaulttype::Vec3Types>> > SphereSphereStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<SphereCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>> > SpherePointStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<PointCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>> > PointPointStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<LineCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>> > LinePointStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<LineCollisionModel<sofa::defaulttype::Vec3Types>, LineCollisionModel<sofa::defaulttype::Vec3Types>> > LineLineStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<LineCollisionModel<sofa::defaulttype::Vec3Types>, SphereCollisionModel<sofa::defaulttype::Vec3Types>> > LineSphereStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, SphereCollisionModel<sofa::defaulttype::Vec3Types>> > TriangleSphereStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>> > TrianglePointStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, LineCollisionModel<sofa::defaulttype::Vec3Types>> > TriangleLineStickContactClass("stick",true);
Creator<Contact::Factory, BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, TriangleCollisionModel<sofa::defaulttype::Vec3Types>> > TriangleTriangleStickContactClass("stick",true);

template class SOFA_MISC_COLLISION_API BarycentricStickContact<SphereCollisionModel<sofa::defaulttype::Vec3Types>, SphereCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<SphereCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<PointCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<LineCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<LineCollisionModel<sofa::defaulttype::Vec3Types>, LineCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<LineCollisionModel<sofa::defaulttype::Vec3Types>, SphereCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, SphereCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, PointCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, LineCollisionModel<sofa::defaulttype::Vec3Types>>;
template class SOFA_MISC_COLLISION_API BarycentricStickContact<TriangleCollisionModel<sofa::defaulttype::Vec3Types>, TriangleCollisionModel<sofa::defaulttype::Vec3Types>>;

} // namespace collision

} // namespace component

} // namespace sofa

