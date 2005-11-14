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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : StdMeshers_AutomaticLength.cxx
//  Author : Edward AGAPOV, OCC
//  Module : SMESH
//  $Header$

#include "StdMeshers_AutomaticLength.hxx"

#include "SMESH_Mesh.hxx"
#include "SMESHDS_Mesh.hxx"
#include "SMESH_Algo.hxx"

#include "utilities.h"

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

using namespace std;

//=============================================================================
/*!
 *  
 */
//=============================================================================

StdMeshers_AutomaticLength::StdMeshers_AutomaticLength(int hypId, int studyId,
	SMESH_Gen * gen):SMESH_Hypothesis(hypId, studyId, gen)
{
  _name = "AutomaticLength";
  _param_algo_dim = 1; // is used by SMESH_Regular_1D

  _mesh = 0;
}

//=============================================================================
/*!
 *  
 */
//=============================================================================

StdMeshers_AutomaticLength::~StdMeshers_AutomaticLength()
{
}

//================================================================================
/*!
 * \brief Return pointer to TopoDS_TShape
  * \param theShape - The TopoDS_Shape
  * \retval inline const TopoDS_TShape* - result
 */
//================================================================================

inline const TopoDS_TShape* getTShape(const TopoDS_Shape& theShape)
{
  return theShape.TShape().operator->();
}
//================================================================================
/*!
 * \brief Compute segment length for all edges
  * \param theMesh - The mesh
  * \param theTShapeToLengthMap - The map of edge to segment length
 */
//================================================================================

static void computeLengths( const SMESH_Mesh*                   theMesh,
                            map<const TopoDS_TShape*, double> & theTShapeToLengthMap)
{
  theTShapeToLengthMap.clear();

  SMESHDS_Mesh* aMesh = const_cast< SMESH_Mesh* > ( theMesh )->GetMeshDS();
  TopoDS_Shape aMainShape = aMesh->ShapeToMesh();

  // Find length of longest and shortest edge
  double Lmin = DBL_MAX, Lmax = -DBL_MAX;
  TopTools_IndexedMapOfShape edgeMap;
  TopExp::MapShapes( aMainShape, TopAbs_EDGE, edgeMap);
  for ( int i = 1; i <= edgeMap.Extent(); ++i )
  {
    TopoDS_Edge edge = TopoDS::Edge( edgeMap(i) );
    //if ( BRep_Tool::Degenerated( edge )) continue;

    Standard_Real L = SMESH_Algo::EdgeLength( edge );
    if ( L < DBL_MIN ) continue;

    if ( L > Lmax ) Lmax = L;
    if ( L < Lmin ) Lmin = L;

    // remember i-th edge length
    theTShapeToLengthMap.insert( make_pair( getTShape( edge ), L ));
  }

  // Compute S0

  // image attached to PAL10237

//   NbSeg
//     ^
//     |
//   10|\
//     | \
//     |  \
//     |   \
//    5|    --------
//     |
//     +------------>
//     1    10       Lmax/Lmin

  const int NbSegMin = 5, NbSegMax = 10; //  on axis NbSeg
  const double Lrat1 = 1., Lrat2 = 10.;  //  on axis Lmax/Lmin

  double Lratio = Lmax/Lmin;
  double NbSeg = NbSegMin;
  if ( Lratio < Lrat2 )
    NbSeg += ( Lrat2 - Lratio ) / ( Lrat2 - Lrat1 )  * ( NbSegMax - NbSegMin );

  double S0 = Lmin / (int) NbSeg;
  MESSAGE( "S0 = " << S0 << ", Lmin = " << Lmin << ", Nbseg = " << (int) NbSeg);

  // Compute segments length for all edges

  // S = S0 * f(L/Lmin) where f(x) = 1 + (2/Pi * 7 * atan(x/5) )
  // =>
  // S = S0 * ( 1 + 14/PI * atan( L / ( 5 * Lmin )))

  const double a14divPI = 14. / PI, a5xLmin = 5 * Lmin;
  map<const TopoDS_TShape*, double>::iterator tshape_length = theTShapeToLengthMap.begin();
  for ( ; tshape_length != theTShapeToLengthMap.end(); ++tshape_length )
  {
    double & L = tshape_length->second;
    L = S0 * ( 1. + a14divPI * atan( L / a5xLmin ));
  }
}

//=============================================================================
/*!
 *  
 */
//=============================================================================

double StdMeshers_AutomaticLength::GetLength(const SMESH_Mesh*   theMesh,
                                             const TopoDS_Shape& anEdge)
  throw(SALOME_Exception)
{
  if ( !theMesh ) throw SALOME_Exception(LOCALIZED("NULL Mesh"));

  if ( anEdge.IsNull() || anEdge.ShapeType() != TopAbs_EDGE )
    throw SALOME_Exception(LOCALIZED("Bad edge shape"));

  if ( theMesh != _mesh ) {
    computeLengths( theMesh, _TShapeToLength );
    _mesh = theMesh;
  }

  map<const TopoDS_TShape*, double>::iterator tshape_length =
    _TShapeToLength.find( getTShape( anEdge ));

  if ( tshape_length == _TShapeToLength.end() )
    return 1; // it is a dgenerated edge

  return tshape_length->second;
}

//=============================================================================
/*!
 *  
 */
//=============================================================================

ostream & StdMeshers_AutomaticLength::SaveTo(ostream & save)
{
  return save;
}

//=============================================================================
/*!
 *  
 */
//=============================================================================

istream & StdMeshers_AutomaticLength::LoadFrom(istream & load)
{
  return load;
}

//=============================================================================
/*!
 *  
 */
//=============================================================================

ostream & operator <<(ostream & save, StdMeshers_AutomaticLength & hyp)
{
  return hyp.SaveTo( save );
}

//=============================================================================
/*!
 *  
 */
//=============================================================================

istream & operator >>(istream & load, StdMeshers_AutomaticLength & hyp)
{
  return hyp.LoadFrom( load );
}
