//  SMESH SMESH : implementaion of SMESH idl descriptions
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SMESH_Algo.hxx
//  Author : Paul RASCLE, EDF
//  Module : SMESH
//  $Header$

#ifndef _SMESH_ALGO_HXX_
#define _SMESH_ALGO_HXX_

#include "SMESH_Hypothesis.hxx"
#include "SMESH_ComputeError.hxx"
#include "SMESH_Comment.hxx"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <GeomAbs_Shape.hxx>

#include <string>
#include <vector>
#include <list>

class SMESH_Gen;
class SMESH_Mesh;
class SMESH_HypoFilter;
class TopoDS_Vertex;
class TopoDS_Face;
class TopoDS_Shape;
class SMESHDS_Mesh;
class SMDS_MeshNode;
class SMESH_subMesh;
class SMESH_MesherHelper;


class SMESH_Algo:public SMESH_Hypothesis
{
public:
  /*!
   * \brief Creates algorithm
    * \param hypId - algorithm ID
    * \param studyId - study ID
    * \param gen - SMESH_Gen
   */
  SMESH_Algo(int hypId, int studyId, SMESH_Gen * gen);

  /*!
   * \brief Destructor
   */
  virtual ~ SMESH_Algo();

  /*!
   * \brief Saves nothing in a stream
    * \param save - the stream
    * \retval virtual std::ostream & - the stream
   */
  virtual std::ostream & SaveTo(std::ostream & save);

  /*!
   * \brief Loads nothing from a stream
    * \param load - the stream
    * \retval virtual std::ostream & - the stream
   */
  virtual std::istream & LoadFrom(std::istream & load);

  /*!
   * \brief Returns all types of compatible hypotheses
   */
  const std::vector < std::string > & GetCompatibleHypothesis();

  /*!
   * \brief Check hypothesis definition to mesh a shape
    * \param aMesh - the mesh
    * \param aShape - the shape
    * \param aStatus - check result
    * \retval bool - true if hypothesis is well defined
   */
  virtual bool CheckHypothesis(SMESH_Mesh&                          aMesh,
                               const TopoDS_Shape&                  aShape,
                               SMESH_Hypothesis::Hypothesis_Status& aStatus) = 0;
  /*!
   * \brief Computes mesh on a shape
    * \param aMesh - the mesh
    * \param aShape - the shape
    * \retval bool - is a success
    *
    * Algorithms that !NeedDescretBoundary() || !OnlyUnaryInput() are
    * to set SMESH_ComputeError returned by SMESH_submesh::GetComputeError()
    * to report problematic subshapes
   */
  virtual bool Compute(SMESH_Mesh & aMesh, const TopoDS_Shape & aShape) = 0;

  /*!
   * \brief Computes mesh without geometry
    * \param aMesh - the mesh
    * \param aHelper - helper that must be used for adding elements to \aaMesh
    * \retval bool - is a success
    *
    * The method is called if ( !aMesh->HasShapeToMesh() )
   */
  virtual bool Compute(SMESH_Mesh & aMesh, SMESH_MesherHelper* aHelper);

  /*!
   * \brief Returns a list of compatible hypotheses used to mesh a shape
    * \param aMesh - the mesh 
    * \param aShape - the shape
    * \param ignoreAuxiliary - do not include auxiliary hypotheses in the list
    * \retval const std::list <const SMESHDS_Hypothesis*> - hypotheses list
   * 
   *  List the hypothesis used by the algorithm associated to the shape.
   *  Hypothesis associated to father shape -are- taken into account (see
   *  GetAppliedHypothesis). Relevant hypothesis have a name (type) listed in
   *  the algorithm. This method could be surcharged by specific algorithms, in 
   *  case of several hypothesis simultaneously applicable.
   */
  virtual const std::list <const SMESHDS_Hypothesis *> &
  GetUsedHypothesis(SMESH_Mesh &         aMesh,
                    const TopoDS_Shape & aShape,
                    const bool           ignoreAuxiliary=true);
  /*!
   * \brief Returns a list of compatible hypotheses assigned to a shape in a mesh
    * \param aMesh - the mesh 
    * \param aShape - the shape
    * \param ignoreAuxiliary - do not include auxiliary hypotheses in the list
    * \retval const std::list <const SMESHDS_Hypothesis*> - hypotheses list
   * 
   *  List the relevant hypothesis associated to the shape. Relevant hypothesis
   *  have a name (type) listed in the algorithm. Hypothesis associated to
   *  father shape -are not- taken into account (see GetUsedHypothesis)
   */
  const list <const SMESHDS_Hypothesis *> &
  GetAppliedHypothesis(SMESH_Mesh &         aMesh,
                       const TopoDS_Shape & aShape,
                       const bool           ignoreAuxiliary=true);
  /*!
   * \brief Make the filter recognize only compatible hypotheses
   * \param theFilter - the filter to initialize
   * \param ignoreAuxiliary - make filter ignore compatible auxiliary hypotheses
   * \retval bool - true if the algo has compatible hypotheses
   */
  bool InitCompatibleHypoFilter( SMESH_HypoFilter & theFilter,
                                 const bool         ignoreAuxiliary) const;
  /*!
   * \brief Initialize my parameter values by the mesh built on the geometry
   *
   * Just return false as the algorithm does not hold parameters values
   */
  virtual bool SetParametersByMesh(const SMESH_Mesh* theMesh,
                                   const TopoDS_Shape& theShape);
  /*!
   * \brief return compute error
   */
  SMESH_ComputeErrorPtr GetComputeError() const;
  /*!
   * \brief initialize compute error
   */
  void InitComputeError();

public:
  // ==================================================================
  // Algo features influencing how Compute() is called:
  // in what turn and with what input shape
  // ==================================================================

  // SMESH_Hypothesis::GetDim();
  // 1 - dimention of target mesh

  bool OnlyUnaryInput() const { return _onlyUnaryInput; }
  // 2 - is collection of tesselatable shapes inacceptable as input;
  // "collection" means a shape containing shapes of dim equal
  // to GetDim().
  // Algo which can process a collection shape should expect
  // an input temporary shape that is neither MainShape nor
  // its child.

  bool NeedDescretBoundary() const { return _requireDescretBoundary; }
  // 3 - is a Dim-1 mesh prerequisite

  bool NeedShape() const { return _requireShape; }
  // 4 - is shape existance required

public:
  // ==================================================================
  // Methods to track non hierarchical dependencies between submeshes 
  // ==================================================================

  /*!
   * \brief Sets event listener to submeshes if necessary
    * \param subMesh - submesh where algo is set
   *
   * This method is called when a submesh gets HYP_OK algo_state.
   * After being set, event listener is notified on each event of a submesh.
   * By default non listener is set
   */
  virtual void SetEventListener(SMESH_subMesh* subMesh);
  
  /*!
   * \brief Allow algo to do something after persistent restoration
    * \param subMesh - restored submesh
   *
   * This method is called only if a submesh has HYP_OK algo_state.
   */
  virtual void SubmeshRestored(SMESH_subMesh* subMesh);
  
public:
  // ==================================================================
  // Common algo utilities
  // ==================================================================
  /*!
   * \brief Fill vector of node parameters on geometrical edge, including vertex nodes
   * \param theMesh - The mesh containing nodes
   * \param theEdge - The geometrical edge of interest
   * \param theParams - The resulting vector of sorted node parameters
   * \retval bool - false if not all parameters are OK
   */
  static bool GetNodeParamOnEdge(const SMESHDS_Mesh*     theMesh,
                                 const TopoDS_Edge&      theEdge,
                                 std::vector< double > & theParams);
  /*!
   * \brief Find out elements orientation on a geometrical face
   * \param theFace - The face correctly oriented in the shape being meshed
   * \param theMeshDS - The mesh data structure
   * \retval bool - true if the face normal and the normal of first element
   *                in the correspoding submesh point in different directions
   */
  static bool IsReversedSubMesh (const TopoDS_Face&  theFace,
                                 SMESHDS_Mesh*       theMeshDS);
  /*!
   * \brief Compute length of an edge
    * \param E - the edge
    * \retval double - the length
   */
  static double EdgeLength(const TopoDS_Edge & E);

  /*!
   * \brief Return continuity of two edges
    * \param E1 - the 1st edge
    * \param E2 - the 2nd edge
    * \retval GeomAbs_Shape - regularity at the junction between E1 and E2
   */
  static GeomAbs_Shape Continuity(const TopoDS_Edge & E1,
                                  const TopoDS_Edge & E2);

  /*!
   * \brief Return the node built on a vertex
    * \param V - the vertex
    * \param meshDS - mesh
    * \retval const SMDS_MeshNode* - found node or NULL
   */
  static const SMDS_MeshNode* VertexNode(const TopoDS_Vertex& V,
                                         SMESHDS_Mesh*        meshDS);

protected:

  /*!
   * \brief store error and comment and then return ( error == COMPERR_OK )
   */
  bool error(int error, const SMESH_Comment& comment = "");
  /*!
   * \brief To be used as error in previous method
   */
  SMESH_ComputeErrorName dfltErr() const { return COMPERR_ALGO_FAILED; }
  /*!
   * \brief store error and return error->IsOK()
   */
  bool error(SMESH_ComputeErrorPtr error);

protected:

  std::vector<std::string>              _compatibleHypothesis;
  std::list<const SMESHDS_Hypothesis *> _appliedHypList;
  std::list<const SMESHDS_Hypothesis *> _usedHypList;

  bool _onlyUnaryInput;
  bool _requireDescretBoundary;
  bool _requireShape;

  // quadratic mesh creation required,
  // is usually set trough SMESH_MesherHelper::IsQuadraticSubMesh()
  bool _quadraticMesh;

  int         _error;    //!< SMESH_ComputeErrorName or anything algo specific
  std::string _comment;  //!< any text explaining what is wrong in Compute()
};

#endif
