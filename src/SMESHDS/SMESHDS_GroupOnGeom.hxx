//  SMESH SMESHDS : management of mesh data and SMESH document
//
//  Copyright (C) 2004  CEA
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
//  File   : SMESHDS_GroupOnGeom.hxx
//  Module : SMESH
//  $Header$

#ifndef _SMESHDS_GroupOnGeom_HeaderFile
#define _SMESHDS_GroupOnGeom_HeaderFile

#include "SMESH_SMESHDS.hxx"

#include "SMESHDS_GroupBase.hxx"
#include <TopoDS_Shape.hxx>
  
class SMESHDS_SubMesh;

class SMESHDS_EXPORT SMESHDS_GroupOnGeom: public SMESHDS_GroupBase
{
 public:

  SMESHDS_GroupOnGeom (const int                 theID,
                       const SMESHDS_Mesh*       theMesh,
                       const SMDSAbs_ElementType theType,
                       const TopoDS_Shape&       theShape);

  TopoDS_Shape GetShape() const { return myShape; }

  virtual bool Contains (const int theID);

  virtual bool Contains (const SMDS_MeshElement* elem);

  virtual SMDS_ElemIteratorPtr GetElements();

 private:

  TopoDS_Shape           myShape;
  const SMESHDS_SubMesh* mySubMesh;
};

#endif
