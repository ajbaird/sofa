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

#include <sofa/core/loader/MeshLoader.h>
#include <OpenCTMPlugin/config.h>

namespace sofa::component::loader
{

/**
 * OpenCTMLoader class interfaces OpenCTM mesh reader with SOFA loader components.
 * For more information about the class API see doc: http://openctm.sourceforge.net/apidocs/
 */
class SOFA_SOFAOPENCTM_API OpenCTMLoader : public sofa::core::loader::MeshLoader
{
public:
    SOFA_CLASS(OpenCTMLoader,sofa::core::loader::MeshLoader);
protected:
    /// Default constructor of the component
    OpenCTMLoader();

public:
    /// Main Load method inherites from \sa sofa::core::loader::MeshLoader::load()
    virtual bool load();

    template <class T>
    static bool canCreate ( T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg )
    {
        return BaseLoader::canCreate (obj, context, arg);
    }

protected:
    /// Main internal method, implement the loading of OpenCTM mesh file.
    bool readOpenCTM(const char* filename);

public:
    // Data buffer for texture coordinates
    Data< type::vector<sofa::type::Vector2> > texCoords; ///< Texture coordinates of all faces, to be used as the parent data of a VisualModel texcoords data
};


} // namespace sofa::component::loader
