//  SMESH SMDS : implementaion of Salome mesh data structure
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

#ifndef _SMDS_PolygonalFaceOfNodes_HeaderFile
#define _SMDS_PolygonalFaceOfNodes_HeaderFile

#include "SMDS_MeshFace.hxx"
//#include "SMDS_FaceOfNodes.hxx"
#include "SMDS_MeshNode.hxx"
#include "SMDS_Iterator.hxx"

#include <iostream>

//class SMDS_PolygonalFaceOfNodes:public SMDS_FaceOfNodes
class SMDS_PolygonalFaceOfNodes:public SMDS_MeshFace
{
 public:
  SMDS_PolygonalFaceOfNodes (std::vector<const SMDS_MeshNode *> nodes);

  virtual SMDSAbs_ElementType GetType() const;
  virtual bool IsPoly() const { return true; };

  bool ChangeNodes (std::vector<const SMDS_MeshNode *> nodes);

  bool ChangeNodes (const SMDS_MeshNode* nodes[],
                    const int            nbNodes);
  // to support the same interface, as SMDS_FaceOfNodes

  virtual int NbNodes() const;
  virtual int NbEdges() const;
  virtual int NbFaces() const;

  virtual void Print (std::ostream & OS) const;

 protected:
  virtual SMDS_ElemIteratorPtr elementsIterator (SMDSAbs_ElementType type) const;

 private:
  std::vector<const SMDS_MeshNode *> myNodes;
};

#endif
