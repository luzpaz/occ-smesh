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
//  File   : SMESH_Quadrangle_2D.hxx
//  Author : Paul RASCLE, EDF
//  Module : SMESH
//  $Header$

#ifndef _SMESH_QUADRANGLE_2D_HXX_
#define _SMESH_QUADRANGLE_2D_HXX_

#include "SMESH_2D_Algo.hxx"
#include "SMESH_Mesh.hxx"
#include "Utils_SALOME_Exception.hxx"

typedef struct uvPtStruct
{
  double param;
  double normParam;
  double u; // original 2d parameter
  double v;
  double x; // 2d parameter, normalized [0,1]
  double y; 
	const SMDS_MeshNode * node;
} UVPtStruct;

typedef struct faceQuadStruct
{
  int nbPts[4];
  TopoDS_Edge edge[4];
  double first[4];
  double last[4];
  bool isEdgeForward[4];
  UVPtStruct* uv_edges[4];
  UVPtStruct* uv_grid;
} FaceQuadStruct;

class SMESH_Quadrangle_2D:
  public SMESH_2D_Algo
{
public:
  SMESH_Quadrangle_2D(int hypId, int studyId, SMESH_Gen* gen);
  virtual ~SMESH_Quadrangle_2D();

  virtual bool CheckHypothesis(SMESH_Mesh& aMesh,
			       const TopoDS_Shape& aShape);

  virtual bool Compute(SMESH_Mesh& aMesh,
		       const TopoDS_Shape& aShape)
    throw (SALOME_Exception);

  FaceQuadStruct* CheckAnd2Dcompute(SMESH_Mesh& aMesh,
				    const TopoDS_Shape& aShape)
    throw (SALOME_Exception);

  void QuadDelete(FaceQuadStruct* quad);

  ostream & SaveTo(ostream & save);
  istream & LoadFrom(istream & load);
  friend ostream & operator << (ostream & save, SMESH_Quadrangle_2D & hyp);
  friend istream & operator >> (istream & load, SMESH_Quadrangle_2D & hyp);

protected:

  void SetNormalizedGrid(SMESH_Mesh& aMesh,
			 const TopoDS_Shape& aShape,
			 FaceQuadStruct* quad)
    throw (SALOME_Exception);

  UVPtStruct* LoadEdgePoints(SMESH_Mesh& aMesh,
			     const TopoDS_Face& F,
			     const TopoDS_Edge& E,
			     double first,
			     double last);
// 			     bool isForward);

//   FaceQuadStruct _quadDesc;
};

#endif
