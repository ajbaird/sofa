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
#include <SofaBaseMechanics/BarycentricMapping.h>
#include <SofaBaseMechanics/BarycentricMappers/BarycentricMapperTriangleSetTopology.h>
using sofa::component::mapping::BarycentricMapperTriangleSetTopology;
using sofa::component::mapping::BarycentricMapping;

#include <SofaBaseTopology/TriangleSetTopologyContainer.h>
#include <SofaBaseTopology/TetrahedronSetTopologyContainer.h>
#include <sofa/core/topology/BaseMeshTopology.h>
using sofa::component::topology::TriangleSetTopologyContainer;
using sofa::component::topology::TetrahedronSetTopologyContainer;
using sofa::core::topology::BaseMeshTopology;

#include <sofa/testing/BaseTest.h>
using sofa::testing::BaseTest;


using sofa::type::Vector3;
using sofa::type::Vec3u;
using sofa::core::objectmodel::New;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;
using sofa::simulation::Simulation ;
using sofa::simulation::Node ;
using sofa::simulation::setSimulation ;
using sofa::core::objectmodel::New ;
using sofa::core::objectmodel::BaseData ;

#include <SofaBaseMechanics/MechanicalObject.h>
using sofa::component::container::MechanicalObject ;

#include <sofa/simulation/Node.h>

using sofa::defaulttype::Vec3dTypes;

template <class In, class Out>
struct BarycentricMapperTriangleSetTopologyTest :  public BaseTest, public BarycentricMapperTriangleSetTopology<In,Out>
{
    typedef BarycentricMapperTriangleSetTopology<In,Out> Inherit;
    typedef typename In::Real Real;

    using Inherit::m_hashTable;
    using Inherit::m_hashTableSize;
    using Inherit::m_gridCellSize;
    using Inherit::m_convFactor;
    using Inherit::m_fromTopology;
    using Inherit::d_map;

    using Inherit::computeHashTable;
    using Inherit::getGridIndices;
    using Inherit::initHashing;
    using Inherit::init;

    typename In::VecCoord m_in;
    typename Out::VecCoord m_out;
    TriangleSetTopologyContainer::SPtr m_topology;

    void SetUp() override
    {
        m_in.push_back(Vector3(0.5, 1.5, 0.0));
        m_in.push_back(Vector3(1.5, 0.0, 2.5));
        m_in.push_back(Vector3(-0.5, -1.5, 0.0));

        m_out.push_back(Vector3{-0.5, -1.5, 0.0});
        m_out.push_back(Vector3{0.5, 0.0, -10.0});

        m_topology = New<TriangleSetTopologyContainer>();
        m_fromTopology = m_topology.get();
        m_fromTopology->addTriangle(0, 1, 2);

        initHashing(m_in);
    }

    void scene_test(){
        Simulation* simu;
        setSimulation(simu = new DAGSimulation());

        typename BarycentricMapping<In,Out>::SPtr thisObject = New<BarycentricMapping<In,Out>>();
        thisObject->setName("barycentricMapping");
        EXPECT_TRUE(thisObject->getName() == "barycentricMapping");

        Node::SPtr node = simu->createNewGraph("root");
        Node::SPtr nodeMapping = node->createChild("nodeToMap");
        TriangleSetTopologyContainer::SPtr triangleContainer = New<TriangleSetTopologyContainer>();
        TetrahedronSetTopologyContainer::SPtr tetraContainer = New<TetrahedronSetTopologyContainer>();
        MechanicalObject<Vec3dTypes>::SPtr mecanical = New<MechanicalObject<Vec3dTypes>>();

        node->addObject(tetraContainer);
        node->addObject(mecanical);
        node->addChild(nodeMapping);
        nodeMapping->addObject(triangleContainer);
        nodeMapping->addObject(thisObject);

        EXPECT_NO_THROW(simu->init(node.get()));
    }

    void init_test()
    {
        init(m_out,m_in);
        EXPECT_EQ(d_map.getValue().size(),2);
    }

    void initHashing_test()
    {
        Real min =(m_in[0]-m_in[1]).norm();
        Real max =(m_in[2]-m_in[1]).norm();
        EXPECT_LE(m_gridCellSize,max);
        EXPECT_GE(m_gridCellSize,min);

        EXPECT_EQ(m_convFactor,1./m_gridCellSize);
    }
};


typedef BarycentricMapperTriangleSetTopologyTest< Vec3dTypes, Vec3dTypes> BarycentricMapperTriangleSetTopologyTest_d;


TEST_F(BarycentricMapperTriangleSetTopologyTest_d, init)
{
    EXPECT_NO_THROW(init_test());
}

TEST_F(BarycentricMapperTriangleSetTopologyTest_d, initHashing)
{
    initHashing_test();
}


