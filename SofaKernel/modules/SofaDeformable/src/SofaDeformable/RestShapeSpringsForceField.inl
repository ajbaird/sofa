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
#include <SofaDeformable/RestShapeSpringsForceField.h>

#include <sofa/core/behavior/MultiMatrixAccessor.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/type/RGBAColor.h>

namespace sofa::component::forcefield
{

using helper::WriteAccessor;
using helper::ReadAccessor;
using core::behavior::BaseMechanicalState;
using core::behavior::MultiMatrixAccessor;
using core::behavior::ForceField;
using defaulttype::BaseMatrix;
using core::VecCoordId;
using core::MechanicalParams;
using type::Vector3;
using type::Vec4f;
using type::vector;
using core::visual::VisualParams;

template<class DataTypes>
RestShapeSpringsForceField<DataTypes>::RestShapeSpringsForceField()
    : d_points(initData(&d_points, "points", "points controlled by the rest shape springs"))
    , d_stiffness(initData(&d_stiffness, "stiffness", "stiffness values between the actual position and the rest shape position"))
    , d_angularStiffness(initData(&d_angularStiffness, "angularStiffness", "angularStiffness assigned when controlling the rotation of the points"))
    , d_pivotPoints(initData(&d_pivotPoints, "pivot_points", "global pivot points used when translations instead of the rigid mass centers"))
    , d_external_points(initData(&d_external_points, "external_points", "points from the external Mechancial State that define the rest shape springs"))
    , d_recompute_indices(initData(&d_recompute_indices, true, "recompute_indices", "Recompute indices (should be false for BBOX)"))
    , d_drawSpring(initData(&d_drawSpring,false,"drawSpring","draw Spring"))
    , d_springColor(initData(&d_springColor, sofa::type::RGBAColor::green(), "springColor","spring color. (default=[0.0,1.0,0.0,1.0])"))
    , l_restMState(initLink("external_rest_shape", "rest_shape can be defined by the position of an external Mechanical State"))
{
}

template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::parse(core::objectmodel::BaseObjectDescription *arg)
{
    const char* attr = arg->getAttribute("external_rest_shape") ;
    if( attr != nullptr && attr[0] != '@')
    {
        msg_error() << "RestShapeSpringsForceField have changed since 17.06. The parameter 'external_rest_shape' is now a Link. To fix your scene you need to add and '@' in front of the provided path. See PR#315" ;
    }

    Inherit::parse(arg) ;
}

template<class Type>
inline bool needAngularStiffness() { return false; }

template<>
inline bool needAngularStiffness<sofa::defaulttype::Rigid3Types>(){ return true; }

template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::bwdInit()
{
    ForceField<DataTypes>::init();

    if (d_stiffness.getValue().empty())
    {
        msg_info() << "No stiffness is defined, assuming equal stiffness on each node, k = 100.0 ";

        VecReal stiffs;
        stiffs.push_back(100.0);
        d_stiffness.setValue(stiffs);
    }

    if (l_restMState.get() == nullptr)
    {
        useRestMState = false;
        msg_info() << "no external rest shape used";

        if(!l_restMState.empty())
        {
            msg_warning() << "external_rest_shape in node " << this->getContext()->getName() << " not found";
        }
    }
    else
    {
        msg_info() << "external rest shape used";
        useRestMState = true;
    }

    recomputeIndices();

    BaseMechanicalState* state = this->getContext()->getMechanicalState();
    if(!state)
    {
        msg_warning() << "MechanicalState of the current context returns null pointer";
    }
    else
    {
        assert(state);
        matS.resize(state->getMatrixSize(),state->getMatrixSize());
    }

    /// Compile time condition to check if we are working with a Rigid3Types or a type that does not
    /// need the Angular Stiffness parameters.
    if(needAngularStiffness<DataTypes>())
    {
        sofa::helper::ReadAccessor<Data<VecReal>> s = d_stiffness;
        sofa::helper::WriteOnlyAccessor<Data<VecReal>> as = d_angularStiffness;

        if (as.size() < s.size())
        {
            msg_info() << "'stiffness' is larger than 'angularStiffness', add the default value (100.0) to the missing entries.";

            for(size_t i = as.size();i<s.size();i++)
            {
                as.push_back(100.0);
            }
        }else if (as.size() > s.size())
        {
            msg_info() << "'stiffness' is smaller than 'angularStiffness', clamp the extra values in angularStiffness.";
            as.resize(s.size());
        }
    }

    lastUpdatedStep = -1.0;
}


template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::reinit()
{
    if (!checkOutOfBoundsIndices())
    {
        m_indices.clear();
    }
    else
    {
        msg_info() << "Indices successfully checked";
    }

    if (d_stiffness.getValue().empty())
    {
        msg_info() << "No stiffness is defined, assuming equal stiffness on each node, k = 100.0 " ;

        VecReal stiffs;
        stiffs.push_back(100.0);
        d_stiffness.setValue(stiffs);
    }
    else
    {
        const VecReal &k = d_stiffness.getValue();
        if ( k.size() != m_indices.size() )
        {
            msg_warning() << "Size of stiffness vector is not correct (" << k.size() << "), should be either 1 or " << m_indices.size() << msgendl
                          << "First value of stiffness will be used";
        }
    }

}

template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::recomputeIndices()
{
    m_indices.clear();
    m_ext_indices.clear();

    for (sofa::Index i = 0; i < d_points.getValue().size(); i++)
    {
        m_indices.push_back(d_points.getValue()[i]);
    }

    for (sofa::Index i = 0; i < d_external_points.getValue().size(); i++)
    {
        m_ext_indices.push_back(d_external_points.getValue()[i]);
    }

    if (m_indices.empty())
    {
        // no point are defined, default case: points = all points
        msg_info() << "No point are defined. Change to default case: points = all points";
        for (sofa::Index i = 0; i < this->mstate->getSize(); i++)
        {
            m_indices.push_back(i);
        }
    }

    if (m_ext_indices.empty())
    {
        if (useRestMState)
        {
            for (sofa::Index i = 0; i < getExtPosition()->getValue().size(); i++)
            {
                m_ext_indices.push_back(i);
            }
        }
        else
        {
            for (sofa::Index i = 0; i < m_indices.size(); i++)
            {
                m_ext_indices.push_back(m_indices[i]);
            }
        }
    }

    if (!checkOutOfBoundsIndices())
    {
        msg_error() << "The dimension of the source and the targeted points are different ";
        m_indices.clear();
    }
    else
    {
        msg_info() << "Indices successfully checked";
    }
}

template<class DataTypes>
bool RestShapeSpringsForceField<DataTypes>::checkOutOfBoundsIndices()
{
    if (!checkOutOfBoundsIndices(m_indices, this->mstate->getSize()))
    {
        msg_error() << "Out of Bounds m_indices detected. ForceField is not activated.";
        return false;
    }
    if (!checkOutOfBoundsIndices(m_ext_indices, sofa::Size(getExtPosition()->getValue().size())))
    {
        msg_error() << "Out of Bounds m_ext_indices detected. ForceField is not activated.";
        return false;
    }
    if (m_indices.size() != m_ext_indices.size())
    {
        msg_error() << "Dimensions of the source and the targeted points are different. ForceField is not activated.";
        return false;
    }
    return true;
}

template<class DataTypes>
bool RestShapeSpringsForceField<DataTypes>::checkOutOfBoundsIndices(const VecIndex &indices, const sofa::Size dimension)
{
    for (sofa::Index i = 0; i < indices.size(); i++)
    {
        if (indices[i] >= dimension)
        {
            return false;
        }
    }
    return true;
}

template<class DataTypes>
const typename RestShapeSpringsForceField<DataTypes>::DataVecCoord* RestShapeSpringsForceField<DataTypes>::getExtPosition() const
{
    return (useRestMState ? l_restMState->read(VecCoordId::position()) : this->mstate->read(VecCoordId::restPosition()));
}

template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::addForce(const MechanicalParams*  mparams , DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv&  v )
{
    SOFA_UNUSED(mparams);
    SOFA_UNUSED(v);

    WriteAccessor< DataVecDeriv > f1 = f;
    ReadAccessor< DataVecCoord > p1 = x;
    ReadAccessor< DataVecCoord > p0 = *getExtPosition();
    const VecReal& k = d_stiffness.getValue();

    f1.resize(p1.size());

    if (d_recompute_indices.getValue())
    {
        recomputeIndices();
    }

    if ( k.size()!= m_indices.size() )
    {
        // Stiffness is not defined on each point, first stiffness is used
        const Real k0 = k[0];

        for (sofa::Index i=0; i<m_indices.size(); i++)
        {
            const sofa::Index index = m_indices[i];
            const sofa::Index ext_index = m_ext_indices[i];

            Deriv dx = p1[index] - p0[ext_index];
            f1[index] -=  dx * k0 ;
        }
    }
    else
    {
        // Heterogeneous stiffness for each spring
        for (sofa::Index i=0; i<m_indices.size(); i++)
        {
            const sofa::Index index = m_indices[i];
            const sofa::Index ext_index = m_ext_indices[i];

            Deriv dx = p1[index] - p0[ext_index];
            f1[index] -=  dx * k[i];
        }
    }
}

template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx)
{
    WriteAccessor< DataVecDeriv > df1 = df;
    ReadAccessor< DataVecDeriv > dx1 = dx;
    Real kFactor = (Real)sofa::core::mechanicalparams::kFactorIncludingRayleighDamping(mparams, this->rayleighStiffness.getValue());
    const VecReal& k = d_stiffness.getValue();

    if (k.size() != m_indices.size())
    {
        // Stiffness is not defined on each point, first stiffness is used
        const Real k0 = k[0] * kFactor;

        for (sofa::Index i=0; i<m_indices.size(); i++)
        {
            df1[m_indices[i]] -=  dx1[m_indices[i]] * k0;
        }
    }
    else
    {
        for (sofa::Index i=0; i<m_indices.size(); i++)
        {
            df1[m_indices[i]] -=  dx1[m_indices[i]] * k[i] * kFactor ;
        }
    }
}

// draw for standard types (i.e Vec<1,2,3>)
template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::draw(const VisualParams *vparams)
{
    if (!vparams->displayFlags().getShowForceFields() || !d_drawSpring.getValue())
        return;  /// \todo put this in the parent class

    if constexpr (DataTypes::spatial_dimensions > 3)
    {
        msg_error() << "Draw function not implemented for this DataType";
        return;
    }

    vparams->drawTool()->saveLastState();
    vparams->drawTool()->setLightingEnabled(false);

    ReadAccessor< DataVecCoord > p0 = *getExtPosition();
    ReadAccessor< DataVecCoord > p  = this->mstate->read(VecCoordId::position());

    const VecIndex& indices = m_indices;
    const VecIndex& ext_indices = (useRestMState ? m_ext_indices : m_indices);

    std::vector<Vector3> vertices;

    for (sofa::Index i=0; i<indices.size(); i++)
    {
        const sofa::Index index = indices[i];
        const sofa::Index ext_index = ext_indices[i];

        Vector3 v0(0.0, 0.0, 0.0);
        Vector3 v1(0.0, 0.0, 0.0);
        for(sofa::Index j=0 ; j<DataTypes::spatial_dimensions ; j++)
        {
            v0[j] = p[index][j];
            v1[j] = p0[ext_index][j];
        }

        vertices.push_back(v0);
        vertices.push_back(v1);
    }

    //todo(dmarchal) because of https://github.com/sofa-framework/sofa/issues/64
    vparams->drawTool()->drawLines(vertices,5, d_springColor.getValue());
    vparams->drawTool()->restoreLastState();
}

template<class DataTypes>
void RestShapeSpringsForceField<DataTypes>::addKToMatrix(const MechanicalParams* mparams, const MultiMatrixAccessor* matrix )
{
    MultiMatrixAccessor::MatrixRef mref = matrix->getMatrix(this->mstate);
    BaseMatrix* mat = mref.matrix;
    unsigned int offset = mref.offset;
    Real kFact = (Real)sofa::core::mechanicalparams::kFactorIncludingRayleighDamping(mparams, this->rayleighStiffness.getValue());

    const VecReal& k = d_stiffness.getValue();
    const sofa::Size N = Coord::total_size;

    sofa::Index curIndex = 0;

    if (k.size() != m_indices.size())
    {
        const Real k0 = -k[0] * (Real)kFact;

        for (sofa::Index index = 0; index < m_indices.size(); index++)
        {
            curIndex = m_indices[index];

            for(sofa::Index i = 0; i < N; i++)
            {
                mat->add(offset + N * curIndex + i, offset + N * curIndex + i, k0);
            }
        }
    }
    else
    {
        for (sofa::Index index = 0; index < m_indices.size(); index++)
        {
            curIndex = m_indices[index];

            for(sofa::Index i = 0; i < N; i++)
            {
                mat->add(offset + N * curIndex + i, offset + N * curIndex + i, -kFact * k[index]);
            }
        }
    }
}

} // namespace sofa::component::forcefield
