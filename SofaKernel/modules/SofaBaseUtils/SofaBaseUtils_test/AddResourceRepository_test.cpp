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
#include <sofa/testing/BaseTest.h>
using sofa::testing::BaseTest;

#include <sofa/helper/BackTrace.h>

#include <sofa/simulation/Node.h>
#include <sofa/simulation/Simulation.h>
#include <SofaSimulationGraph/DAGSimulation.h>
#include <SofaSimulationCommon/SceneLoaderXML.h>

#include <SofaBaseUtils/AddResourceRepository.h>
#include <SofaBase/initSofaBase.h>

namespace sofa
{

const std::string& START_STR("<Node name=\"root\"  >");
const std::string& END_STR("</Node>");

struct AddResourceRepository_test : public BaseTest
{
    sofa::simulation::Node::SPtr m_root;
    std::string m_testRepoDir;

    void SetUp() override
    {
        sofa::component::initSofaBase();

        m_testRepoDir = std::string(SOFABASEUTILS_TEST_RESOURCES_DIR) + std::string("/repo");
    }

    void buildScene(const std::string& repoType, const std::string& repoPath)
    {
        std::string addRepoStr = "<" + repoType + " path=\""+ repoPath + "\" />";

        std::string scene = START_STR + addRepoStr + END_STR;
        std::cout << scene << std::endl;

        m_root = sofa::simulation::SceneLoaderXML::loadFromMemory(
                "scene", scene.c_str(), scene.size());

        EXPECT_NE(m_root, nullptr);
    }
};


TEST_F(AddResourceRepository_test, AddDataRepository_RepoExists)
{
    std::string existFilename("somefilesomewhere.txt");
    std::string nopeFilename("somefilesomewherebutdoesnotexist.txt");

    EXPECT_MSG_EMIT(Error) ;

    EXPECT_FALSE(helper::system::DataRepository.findFile(existFilename));
    EXPECT_FALSE(helper::system::DataRepository.findFile(nopeFilename));

    buildScene("AddDataRepository", m_testRepoDir);

    EXPECT_FALSE(helper::system::DataRepository.findFile(nopeFilename));

    EXPECT_MSG_NOEMIT(Error);
    EXPECT_TRUE(helper::system::DataRepository.findFile(existFilename));
}

TEST_F(AddResourceRepository_test, AddDataRepository_RepoDoesNotExist)
{
    EXPECT_MSG_EMIT(Error) ;
    buildScene("AddDataRepository", "/blabla/Repo_not_existing");
}


} // namespace sofa
