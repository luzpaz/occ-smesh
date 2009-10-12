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
//  File   : SMDS_MeshNodeIDFactory.cxx
//  Author : Jean-Michel BOULCOURT
//  Module : SMESH
//
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "SMDS_MeshNodeIDFactory.hxx"
#include "SMDS_MeshElement.hxx"
#include "SMDS_Mesh.hxx"

#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>

using namespace std;

//=======================================================================
//function : SMDS_MeshNodeIDFactory
//purpose  : 
//=======================================================================
SMDS_MeshNodeIDFactory::SMDS_MeshNodeIDFactory():
  SMDS_MeshIDFactory(),
  myMin(0), myMax(0)
{
}

//=======================================================================
//function : BindID
//purpose  : 
//=======================================================================
bool SMDS_MeshNodeIDFactory::BindID(int ID, SMDS_MeshElement * elem)
{
  updateMinMax (ID);
  return true;
}

//=======================================================================
//function : MeshElement
//purpose  : 
//=======================================================================
SMDS_MeshElement* SMDS_MeshNodeIDFactory::MeshElement(int ID)
{
  if ((ID<0) || (ID>myMax))
    return NULL;
  const SMDS_MeshElement* elem = GetMesh()->FindNode(ID);
  return (SMDS_MeshElement*)(elem);
}

//=======================================================================
//function : ReleaseID
//purpose  : 
//=======================================================================
void SMDS_MeshNodeIDFactory::ReleaseID(const int ID)
{
  SMDS_MeshIDFactory::ReleaseID(ID);
  if (ID == myMax)
    myMax = 0;     // --- force updateMinMax
  if (ID == myMin)
    myMax = 0;     // --- force updateMinMax
}

//=======================================================================
//function : GetMaxID
//purpose  : 
//=======================================================================

int SMDS_MeshNodeIDFactory::GetMaxID() const
{
  if (myMax == 0)
    updateMinMax();
  return myMax;
}

//=======================================================================
//function : GetMinID
//purpose  : 
//=======================================================================

int SMDS_MeshNodeIDFactory::GetMinID() const
{
  if (myMax == 0)
    updateMinMax();
  return myMin;
}

//=======================================================================
//function : updateMinMax
//purpose  : 
//=======================================================================

void SMDS_MeshNodeIDFactory::updateMinMax() const
{
  myMesh->updateNodeMinMax();
  myMin = myMesh->MinNodeID();
  myMax = myMesh->MaxNodeID();
}

SMDS_ElemIteratorPtr SMDS_MeshNodeIDFactory::elementsIterator() const
{
    return myMesh->elementsIterator(SMDSAbs_Node);
}

void SMDS_MeshNodeIDFactory::Clear()
{
  myMin = myMax = 0;
  SMDS_MeshIDFactory::Clear();
}
