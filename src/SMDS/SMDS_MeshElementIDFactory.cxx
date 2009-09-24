//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  SMESH SMDS : implementaion of Salome mesh data structure
//  File   : SMDS_MeshElementIDFactory.cxx
//  Author : Jean-Michel BOULCOURT
//  Module : SMESH
//
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "SMDS_MeshElementIDFactory.hxx"
#include "SMDS_MeshElement.hxx"
#include "SMDS_Mesh.hxx"

#include "utilities.h"

#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>

using namespace std;

//=======================================================================
//function : SMDS_MeshElementIDFactory
//purpose  : 
//=======================================================================
SMDS_MeshElementIDFactory::SMDS_MeshElementIDFactory():
  SMDS_MeshNodeIDFactory()
{
    myIDElements.clear();
    myVtkIndex.clear();
    myVtkCellTypes.clear();
    myVtkCellTypes.reserve(SMDSEntity_Last);
    myVtkCellTypes[SMDSEntity_Node]            = VTK_VERTEX;
    myVtkCellTypes[SMDSEntity_0D]              = VTK_VERTEX;
    myVtkCellTypes[SMDSEntity_Edge]            = VTK_LINE;
    myVtkCellTypes[SMDSEntity_Quad_Edge]       = VTK_QUADRATIC_EDGE;
    myVtkCellTypes[SMDSEntity_Triangle]        = VTK_TRIANGLE;
    myVtkCellTypes[SMDSEntity_Quad_Triangle]   = VTK_QUADRATIC_TRIANGLE;
    myVtkCellTypes[SMDSEntity_Quadrangle]      = VTK_QUAD;
    myVtkCellTypes[SMDSEntity_Quad_Quadrangle] = VTK_QUADRATIC_TRIANGLE;
    myVtkCellTypes[SMDSEntity_Polygon]         = VTK_POLYGON;
    myVtkCellTypes[SMDSEntity_Quad_Polygon]    = VTK_POLYGON; // -PR- verifer
    myVtkCellTypes[SMDSEntity_Tetra]           = VTK_TETRA;
    myVtkCellTypes[SMDSEntity_Quad_Tetra]      = VTK_QUADRATIC_TETRA;
    myVtkCellTypes[SMDSEntity_Pyramid]         = VTK_PYRAMID;
    myVtkCellTypes[SMDSEntity_Quad_Pyramid]    = VTK_CONVEX_POINT_SET;
    myVtkCellTypes[SMDSEntity_Hexa]            = VTK_HEXAHEDRON;
    myVtkCellTypes[SMDSEntity_Quad_Hexa]       = VTK_QUADRATIC_HEXAHEDRON;
    myVtkCellTypes[SMDSEntity_Penta]           = VTK_WEDGE;
    myVtkCellTypes[SMDSEntity_Quad_Penta]      = VTK_QUADRATIC_WEDGE;
    myVtkCellTypes[SMDSEntity_Polyhedra]       = VTK_CONVEX_POINT_SET;
    myVtkCellTypes[SMDSEntity_Quad_Polyhedra]  = VTK_CONVEX_POINT_SET;
}

//=======================================================================
//function : BindID
//purpose  : 
//=======================================================================
bool SMDS_MeshElementIDFactory::BindID(int ID, SMDS_MeshElement * elem)
{
  if ((ID < myIDElements.size()) && myIDElements[ID] >= 0) // --- already bound
  {
    MESSAGE(" --------------------------------- already bound "<< ID << " " << myIDElements[ID]);
      return false;
  }

  if (ID >= myIDElements.size()) // --- resize local vector
  {
    //MESSAGE(" ------------------- resize myIDElements " << ID << " --> " << ID+SMDS_Mesh::chunkSize);
    myIDElements.resize(ID+SMDS_Mesh::chunkSize,-1); // fill new elements with -1
  }

  // --- retreive nodes ID

  vector<int> nodeIds;
  SMDS_ElemIteratorPtr it = elem->nodesIterator();
  while(it->more())
  {
      int nodeId = it->next()->GetID();
      //MESSAGE("   node in cell " << ID << " : " <<nodeId)
      nodeIds.push_back(nodeId);
  }

  // --- insert cell in vtkUnstructuredGrid

  vtkUnstructuredGrid * grid = myMesh->getGrid();
  int typ = GetVtkCellType(elem->GetType());
  int cellId = grid->InsertNextLinkedCell(typ, nodeIds.size(), &nodeIds[0]);

  // --- fill local vector

  myIDElements[ID] = cellId;
  //MESSAGE("smds:" << ID << " vtk:" << cellId );

  if (cellId >= myVtkIndex.size()) // --- resize local vector
  {
    //MESSAGE(" --------------------- resize myVtkIndex " << cellId << " --> " << cellId+SMDS_Mesh::chunkSize);
    myVtkIndex.resize(cellId+SMDS_Mesh::chunkSize, -1);
  }
  myVtkIndex[cellId] = ID;

  elem->myID=ID;
  SMDS_MeshCell *cell = dynamic_cast<SMDS_MeshCell*>(elem);
  assert(cell);
  cell->setVtkId(cellId);
  updateMinMax (ID);
  return true;
}

//=======================================================================
//function : MeshElement
//purpose  : 
//=======================================================================
SMDS_MeshElement* SMDS_MeshElementIDFactory::MeshElement(int ID)
{
  if ((ID<0) || (ID>myMax) || (myIDElements[ID]<0))
    return NULL;
  const SMDS_MeshElement* elem = GetMesh()->FindElement(ID);
  return (SMDS_MeshElement*)(elem);
}

//=======================================================================
//function : ReleaseID
//purpose  : 
//=======================================================================
void SMDS_MeshElementIDFactory::ReleaseID(const int ID)
{
  myIDElements[ID] = -1;
  SMDS_MeshIDFactory::ReleaseID(ID);
  if (ID == myMax)
    myMax = 0;
  if (ID == myMin)
    myMax = 0;
}

//=======================================================================
//function : updateMinMax
//purpose  : 
//=======================================================================

void SMDS_MeshElementIDFactory::updateMinMax() const
{
  myMin = IntegerLast();
  myMax = 0;
  for (int i=0; i<myIDElements.size(); i++)
      if (int id=myIDElements[i] >=0)
      {
        if (id > myMax) myMax = id;
        if (id < myMin) myMin = id;
      }
  if (myMin == IntegerLast())
    myMin = 0;
}

//=======================================================================
//function : elementsIterator
//purpose  : Return an iterator on elements of the factory
//=======================================================================

SMDS_ElemIteratorPtr SMDS_MeshElementIDFactory::elementsIterator() const
{
    return myMesh->elementsIterator(SMDSAbs_All);
}

void SMDS_MeshElementIDFactory::Clear()
{
  myIDElements.clear();
  myMin = myMax = 0;
  SMDS_MeshIDFactory::Clear();
}

int SMDS_MeshElementIDFactory::GetVtkCellType(int SMDSType)
{
    assert((SMDSType >=0) && (SMDSType< SMDSEntity_Last));
    return myVtkCellTypes[SMDSType];
}
