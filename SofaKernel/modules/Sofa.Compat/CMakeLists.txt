cmake_minimum_required(VERSION 3.12)
project(Sofa.Compat LANGUAGES CXX)

set(SOFACOMPATSRC_ROOT "src")

set(HEADER_FILES
    # ${SOFACOMPATSRC_ROOT}/config.h.in 
    ${SOFACOMPATSRC_ROOT}/sofa/helper/vectorData.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/vectorLinks.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/vector.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/vector_device.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/SVector.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/stable_vector.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/vector_algebra.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/vector_algorithm.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/fixed_array.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/fixed_array_algorithms.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/Quater.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/Quater.inl
    ${SOFACOMPATSRC_ROOT}/sofa/helper/DualQuat.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/DualQuat.inl
    ${SOFACOMPATSRC_ROOT}/sofa/helper/LCPSolver.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/StateMask.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Trackball.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/template.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Texture.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/FrameBufferObject.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/GLSLShader.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Transformation.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/RAII.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/BasicShapes.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Capture.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/VideoRecorderFFMPEG.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Axis.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/glText.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/glText.inl
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Color.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/gl/Cylinder.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/types/RGBAColor.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/types/RGBAColor_fwd.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/types/Material.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/types/PrimitiveGroup.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/system/gl.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/system/glu.h
    ${SOFACOMPATSRC_ROOT}/sofa/helper/system/atomic.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Vec.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Mat.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/MatSym.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Mat_solve_Cholesky.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Mat_solve_LU.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Mat_solve_SVD.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Quat.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Quat.inl
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/BoundingBox.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Color.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Frame.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/Ray.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/RGBAColor.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/BaseVector.h
    ${SOFACOMPATSRC_ROOT}/sofa/defaulttype/BaseMatrix.h
    ${SOFACOMPATSRC_ROOT}/sofa/core/visual/DrawTool.h
    ${SOFACOMPATSRC_ROOT}/sofa/core/visual/DrawToolGL.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/CompressedRowSparseMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/DiagonalMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/FullMatrix.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/FullVector.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/RotationMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/matrix_bloc_traits.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/CompressedRowSparseMatrix.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/FullMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/FullVector.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/MatrixExpr.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseLinearSolver/SparseMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/EigenBaseSparseMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/EigenBaseSparseMatrix_MT.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/EigenMatrixManipulator.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/EigenSparseMatrix.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/EigenVector.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/EigenVectorWrapper.h
    ${SOFACOMPATSRC_ROOT}/SofaEigen2Solver/SVDLinearSolver.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologyData.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologyData.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologyDataHandler.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologyDataHandler.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologySparseData.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologySparseData.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologySubsetData.h
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologySubsetData.inl
    ${SOFACOMPATSRC_ROOT}/SofaBaseTopology/TopologySubsetIndices.h

)

set(SOURCE_FILES
    ${SOFACOMPATSRC_ROOT}/sofa/initSofaCompat.cpp
)

sofa_find_package(Sofa.Config REQUIRED)
# sofa_find_package(SofaFramework REQUIRED)
# sofa_find_package(Sofa.Type REQUIRED)
# sofa_find_package(Sofa.Geometry REQUIRED)
# sofa_find_package(Sofa.Topology REQUIRED)
# sofa_find_package(SofaHelper REQUIRED)
# sofa_find_package(SofaDefaulttype REQUIRED)
# sofa_find_package(SofaCore REQUIRED)
# sofa_find_package(SofaSimulationCore REQUIRED)
# sofa_find_package(Sofa.GL QUIET)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC Sofa.Config)
# target_link_libraries(${PROJECT_NAME} PUBLIC Sofa.Type Sofa.Geometry Sofa.Topology)
# target_link_libraries(${PROJECT_NAME} PUBLIC SofaHelper SofaDefaulttype SofaCore SofaSimulationCore)
# if(Sofa.GL_FOUND)
#     target_link_libraries(${PROJECT_NAME} PUBLIC Sofa.GL)
# endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER SofaFramework) # IDE folder

sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${Sofa_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR "${PROJECT_NAME}"
    OPTIMIZE_BUILD_DIR 0
)
