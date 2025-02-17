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
#include <SofaGeneralObjectInteraction/BoxStiffSpringForceField.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/type/RGBAColor.h>
#include <sofa/type/Vec.h>
#include <sofa/type/vector.h>
#include <map>


namespace sofa::component::interactionforcefield
{

template <class DataTypes>
BoxStiffSpringForceField<DataTypes>::BoxStiffSpringForceField(MechanicalState* object1, MechanicalState* object2, double ks, double kd)
    : StiffSpringForceField<DataTypes>(object1, object2, ks, kd),
      box_object1( initData( &box_object1, Vec6(0,0,0,1,1,1), "box_object1", "Box for the object1 where springs will be attached") ),
      box_object2( initData( &box_object2, Vec6(0,0,0,1,1,1), "box_object2", "Box for the object2 where springs will be attached") ),
      factorRestLength( sofa::core::objectmodel::Base::initData( &factorRestLength, (SReal)1.0, "factorRestLength", "Factor used to compute the rest length of the springs generated")),
	  forceOldBehavior(initData(&forceOldBehavior, true, "forceOldBehavior", "Keep using the old behavior"))
{
}

template <class DataTypes>
BoxStiffSpringForceField<DataTypes>::BoxStiffSpringForceField(double ks, double kd)
    : StiffSpringForceField<DataTypes>(ks, kd),
      box_object1( initData( &box_object1, Vec6(0,0,0,1,1,1), "box_object1", "Box for the object1 where springs will be attached") ),
      box_object2( initData( &box_object2, Vec6(0,0,0,1,1,1), "box_object2", "Box for the object2 where springs will be attached") ),
      factorRestLength( sofa::core::objectmodel::Base::initData( &factorRestLength, (SReal)1.0, "factorRestLength", "Factor used to compute the rest length of the springs generated")),
	  forceOldBehavior(initData(&forceOldBehavior, true, "forceOldBehavior", "Keep using the old behavior"))
{
}

template <class DataTypes>
void BoxStiffSpringForceField<DataTypes>::init()
{
	if(forceOldBehavior.getValue())
	{
		 msg_warning("BoxStiffSpringForceField") << "The behavior of the component has changed."
												 << " If you want to use the old behavior you should add the parameter \"forceOldBehavior=true\" to your scene."
												 << " If you want to remove this warning and use the new behavior you need to add \"forceOldBehavior=false\"." << "\n";
	}
}

template <class DataTypes>
void BoxStiffSpringForceField<DataTypes>::bwdInit()
{
    Inherit::bwdInit();
    sofa::type::vector<Index> indices1;
    Vec6& b1=*(box_object1.beginEdit());

    if (b1[0] > b1[3]) std::swap(b1[0],b1[3]);
    if (b1[1] > b1[4]) std::swap(b1[1],b1[4]);
    if (b1[2] > b1[5]) std::swap(b1[2],b1[5]);
    box_object1.endEdit();

    this->mstate1->getIndicesInSpace( indices1, b1[0],b1[3],b1[1],b1[4],b1[2],b1[5] );

    sofa::type::vector<Index> indices2;
    Vec6& b2=*(box_object2.beginEdit());
    if (b2[0] > b2[3]) std::swap(b2[0],b2[3]);
    if (b2[1] > b2[4]) std::swap(b2[1],b2[4]);
    if (b2[2] > b2[5]) std::swap(b2[2],b2[5]);
    box_object2.endEdit();

    this->mstate2->getIndicesInSpace( indices2, b2[0],b2[3],b2[1],b2[4],b2[2],b2[5] );


    const VecCoord& x1 = this->mstate1->read(core::ConstVecCoordId::position())->getValue();
    const VecCoord& x2 = this->mstate2->read(core::ConstVecCoordId::position())->getValue();

    //Attach springs using with priority the shortest distance between points
    Real min_dist=0;
    if (indices1.size() < indices2.size())
    {
        sofa::type::vector< std::map<Real, Index> > distance_spring(indices1.size());
        for(unsigned int i = 0; i < indices1.size(); ++i)
        {
            for(unsigned int j = 0; j < indices2.size(); ++j)
            {
                distance_spring[i][(Real)sqrt((x1[indices1[i]] - x2[indices2[j]]).norm2())] = j;
            }
            if (i==0 || min_dist> distance_spring[i].begin()->first) min_dist = distance_spring[i].begin()->first;

        }
        sofa::type::vector< bool > indice_unused(indices2.size(),true);

        for(unsigned int i = 0; i<indices1.size(); ++i)
        {
            auto it = distance_spring[i].begin();
            for (; it!=distance_spring[i].end(); it++)
            {
                if (indice_unused[it->second])
                {
                    indice_unused[it->second] = false;
					if(forceOldBehavior.getValue())
						this->addSpring(indices1[i], indices2[it->second], this->getStiffness()*it->first/min_dist, this->getDamping(), it->first*factorRestLength.getValue() );
					else
						this->addSpring(indices1[i], indices2[it->second], this->getStiffness(), this->getDamping(), it->first*factorRestLength.getValue() );
                    break;
                }
            }
        }
    }
    else
    {
        sofa::type::vector< std::map<Real, Index> > distance_spring(indices2.size());
        for(unsigned int i = 0; i < indices2.size(); ++i)
        {
            for(unsigned int j = 0; j < indices1.size(); ++j)
            {
                distance_spring[i][(Real)sqrt((x1[indices1[j]] - x2[indices2[i]]).norm2())] = j;
            }

            if (i==0 || min_dist> distance_spring[i].begin()->first) min_dist = distance_spring[i].begin()->first;
        }
        sofa::type::vector< bool > indice_unused(indices1.size(),true);

        for(unsigned int i = 0; i<indices2.size(); ++i)
        {
            auto it = distance_spring[i].begin();
            for (; it!=distance_spring[i].end(); it++)
            {
                if (indice_unused[it->second])
                {
                    indice_unused[it->second] = false;
					if(forceOldBehavior.getValue())
						this->addSpring(indices1[i], indices2[it->second], this->getStiffness()*it->first/min_dist, this->getDamping(), it->first*factorRestLength.getValue() );
					else
						this->addSpring( indices1[it->second], indices2[i], this->getStiffness(), this->getDamping(), it->first*factorRestLength.getValue() );
                    break;
                }
            }
        }
    }
}



template <class DataTypes>
void BoxStiffSpringForceField<DataTypes>::draw(const core::visual::VisualParams* vparams)
{
    if (!vparams->displayFlags().getShowInteractionForceFields())
        return;

    Inherit::draw(vparams);

    vparams->drawTool()->saveLastState();

    sofa::type::RGBAColor color;
    std::vector<sofa::type::Vector3> vertices;

    ///draw the constraint box
    const Vec6& b1=box_object1.getValue();
    const Real& Xmin1=b1[0];
    const Real& Xmax1=b1[3];
    const Real& Ymin1=b1[1];
    const Real& Ymax1=b1[4];
    const Real& Zmin1=b1[2];
    const Real& Zmax1=b1[5];

    const Vec6& b2=box_object2.getValue();
    const Real& Xmin2=b2[0];
    const Real& Xmax2=b2[3];
    const Real& Ymin2=b2[1];
    const Real& Ymax2=b2[4];
    const Real& Zmin2=b2[2];
    const Real& Zmax2=b2[5];

    color = sofa::type::RGBAColor(0,0.5,0.5,1);
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymin1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymin1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymin1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymin1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymin1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymax1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymax1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymax1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymax1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymax1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymax1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymin1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymin1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymin1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymin1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymax1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymin1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymin1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmin1,Ymax1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymax1,Zmax1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymax1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymin1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymax1,Zmin1));
    vertices.push_back(sofa::type::Vector3(Xmax1,Ymax1,Zmax1));
    vparams->drawTool()->drawLines(vertices,1,color);
    vertices.clear();

    color = sofa::type::RGBAColor(0.5,0.5,0,1);

    vertices.push_back(sofa::type::Vector3(Xmin2,Ymin2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymin2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymin2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymin2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymin2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymax2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymax2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymax2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymax2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymax2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymax2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymin2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymin2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymin2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymin2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymax2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymin2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymin2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmin2,Ymax2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymax2,Zmax2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymax2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymin2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymax2,Zmin2));
    vertices.push_back(sofa::type::Vector3(Xmax2,Ymax2,Zmax2));

    vparams->drawTool()->drawLines(vertices,1,color);

    vparams->drawTool()->restoreLastState();
}


} //namespace sofa::component::interactionforcefield
