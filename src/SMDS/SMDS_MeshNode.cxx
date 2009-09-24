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
//
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "SMDS_MeshNode.hxx"
#include "SMDS_SpacePosition.hxx"
#include "SMDS_IteratorOfElements.hxx"
#include "SMDS_Mesh.hxx"
#include <vtkUnstructuredGrid.h>

#define protected public
#include <vtkCellLinks.h>
#define protected protected

#include "utilities.h"

using namespace std;

//=======================================================================
//function : SMDS_MeshNode
//purpose  : 
//=======================================================================

SMDS_MeshNode::SMDS_MeshNode(int id, int meshId, int shapeId, double x, double y, double z):
  SMDS_MeshElement(id, meshId, shapeId),
  myPosition(SMDS_SpacePosition::originSpacePosition())
{
  //MESSAGE("Node " << myID << " (" << x << ", " << y << ", " << z << ")");
  SMDS_Mesh* mesh = SMDS_Mesh::_meshList[myMeshId];
  vtkUnstructuredGrid * grid = mesh->getGrid();
  vtkPoints *points = grid->GetPoints();
  //int nbp = points->GetNumberOfPoints();
  points->InsertPoint(myID, x, y, z);
  if (myID >= mesh->myCellLinksSize)
  {
      //points->SetNumberOfPoints(myID+SMDS_Mesh::chunkSize);
      vtkCellLinks *cellLinks = grid->GetCellLinks();

//      int imax = cellLinks->Size;
//      for (int i =0; i<imax; i++)
//      {
//        vtkCellLinks::Link &ilink = cellLinks->GetLink(i);
//        int ncells = ilink.ncells;
//        int *cells = ilink.cells;
//        MESSAGE("NODE " << i << " " << cellLinks << " " << cells << " " << ncells);
//        for (int j=0; j< ncells; j++)
//          MESSAGE("             " << j << " " << cells[j]);
//      }

      cellLinks->Resize(myID+SMDS_Mesh::chunkSize);

//      cellLinks = grid->GetCellLinks();
//      imax = cellLinks->Size;
//      for (int i =0; i<imax; i++)
//      {
//        vtkCellLinks::Link &ilink = cellLinks->GetLink(i);
//        int ncells = ilink.ncells;
//        int *cells = ilink.cells;
//        MESSAGE("NODE " << i << " " << cellLinks << " " << cells << " " << ncells);
//        for (int j=0; j< ncells; j++)
//          MESSAGE("             " << j << " " << cells[j]);
//      }

      mesh->myCellLinksSize = cellLinks->Size;
      //MESSAGE(" -------------------------------------- resize CellLinks " << myID << " --> " << mesh->myCellLinksSize);
  }
  //setXYZ(x, y, z);
}

//=======================================================================
//function : RemoveInverseElement
//purpose  : 
//=======================================================================

void SMDS_MeshNode::RemoveInverseElement(const SMDS_MeshElement * parent)
{
    MESSAGE("RemoveInverseElement " << myID << " " << parent->GetID());
    SMDS_Mesh::_meshList[myMeshId]->getGrid()->RemoveReferenceToCell(myID, parent->GetID()); // -PR- GetVtkID ?
}

//=======================================================================
//function : Print
//purpose  : 
//=======================================================================

void SMDS_MeshNode::Print(ostream & OS) const
{
	OS << "Node <" << myID << "> : X = " << X() << " Y = "
		<< Y() << " Z = " << Z() << endl;
}

//=======================================================================
//function : SetPosition
//purpose  : 
//=======================================================================

void SMDS_MeshNode::SetPosition(const SMDS_PositionPtr& aPos)
{
	myPosition = aPos;
}

//=======================================================================
//function : GetPosition
//purpose  : 
//=======================================================================

const SMDS_PositionPtr& SMDS_MeshNode::GetPosition() const
{
	return myPosition;
}

//=======================================================================
/*!
 * \brief Iterator on list of elements
 */
//=======================================================================

class SMDS_MeshNode_MyInvIterator:public SMDS_ElemIterator
{
private:
  SMDS_Mesh* myMesh;
  int* myCells;
  int  myNcells;
  SMDSAbs_ElementType                                 myType;
  int  iter;

 public:
  SMDS_MeshNode_MyInvIterator(SMDS_Mesh *mesh,
                              int* cells,
                              int ncells,
                              SMDSAbs_ElementType type):
    myMesh(mesh), myCells(cells), myNcells(ncells), myType(type), iter(0)
  {
     //MESSAGE("SMDS_MeshNode_MyInvIterator : ncells " << myNcells);
  }

  bool more()
  {
      return (iter< myNcells);
  }

  const SMDS_MeshElement* next()
  {
      int vtkId = myCells[iter];
      int smdsId = myMesh->fromVtkToSmds(vtkId);
      const SMDS_MeshElement* elem = myMesh->FindElement(smdsId);
      iter++;
      return elem;
  }	
};

SMDS_ElemIteratorPtr SMDS_MeshNode::
	GetInverseElementIterator(SMDSAbs_ElementType type) const
{
    vtkCellLinks::Link l = SMDS_Mesh::_meshList[myMeshId]->getGrid()->GetCellLinks()->GetLink(myID);
    //MESSAGE("ncells " << l.ncells);
    return SMDS_ElemIteratorPtr(new SMDS_MeshNode_MyInvIterator(SMDS_Mesh::_meshList[myMeshId], l.cells, l.ncells, type));
}

// Same as GetInverseElementIterator but the create iterator only return
// wanted type elements.
class SMDS_MeshNode_MyIterator:public SMDS_ElemIterator
{
private:
  SMDS_Mesh* myMesh;
  int* myCells;
  int  myNcells;
  SMDSAbs_ElementType                                 myType;
  int  iter;
  vector<SMDS_MeshElement*> myFiltCells;

 public:
  SMDS_MeshNode_MyIterator(SMDS_Mesh *mesh,
                           int* cells,
                           int ncells,
                           SMDSAbs_ElementType type):
    myMesh(mesh), myCells(cells), myNcells(ncells), myType(type), iter(0)
  {
        for (; iter<ncells; iter++)
        {
           int vtkId = myCells[iter];
           int smdsId = myMesh->fromVtkToSmds(vtkId);
           const SMDS_MeshElement* elem = myMesh->FindElement(smdsId);
           if (elem->GetType() == type)
               myFiltCells.push_back((SMDS_MeshElement*)elem);
        }
        myNcells = myFiltCells.size();
        iter = 0;
        //MESSAGE("SMDS_MeshNode_MyIterator (filter) " << ncells << " " << myNcells);
  }

  bool more()
  {
      return (iter< myNcells);
  }

  const SMDS_MeshElement* next()
  {
      const SMDS_MeshElement* elem = myFiltCells[iter];
      iter++;
      return elem;
  }
};

SMDS_ElemIteratorPtr SMDS_MeshNode::
	elementsIterator(SMDSAbs_ElementType type) const
{
  if(type==SMDSAbs_Node)
    return SMDS_MeshElement::elementsIterator(SMDSAbs_Node); 
  else
  {
    vtkCellLinks::Link l = SMDS_Mesh::_meshList[myMeshId]->getGrid()->GetCellLinks()->GetLink(myID);
    return SMDS_ElemIteratorPtr(new SMDS_MeshNode_MyIterator(SMDS_Mesh::_meshList[myMeshId], l.cells, l.ncells, type));
  }
}

int SMDS_MeshNode::NbNodes() const
{
	return 1;
}


double* SMDS_MeshNode::getCoord() const
{
  return SMDS_Mesh::_meshList[myMeshId]->getGrid()->GetPoint(myID);
}

double SMDS_MeshNode::X() const
{
  double *coord = getCoord();
  return coord[0];
}

double SMDS_MeshNode::Y() const
{
  double *coord = getCoord();
  return coord[1];
}

double SMDS_MeshNode::Z() const
{
  double *coord = getCoord();
  return coord[2];
}

//* resize the vtkPoints structure every SMDS_Mesh::chunkSize points
void SMDS_MeshNode::setXYZ(double x, double y, double z)
{
  vtkPoints *points = SMDS_Mesh::_meshList[myMeshId]->getGrid()->GetPoints();
  points->InsertPoint(myID, x, y, z);
}

SMDSAbs_ElementType SMDS_MeshNode::GetType() const
{
	return SMDSAbs_Node;
}

//=======================================================================
//function : AddInverseElement
//purpose  :
//=======================================================================
void SMDS_MeshNode::AddInverseElement(const SMDS_MeshElement* ME)
{
    const SMDS_MeshCell *cell = dynamic_cast<const SMDS_MeshCell*>(ME);
    assert(cell);
    SMDS_Mesh::_meshList[myMeshId]->getGrid()->AddReferenceToCell(myID, cell->getVtkId());
}

//=======================================================================
//function : ClearInverseElements
//purpose  :
//=======================================================================
void SMDS_MeshNode::ClearInverseElements()
{
  SMDS_Mesh::_meshList[myMeshId]->getGrid()->ResizeCellList(myID, 0);
}

bool SMDS_MeshNode::emptyInverseElements()
{
  vtkCellLinks::Link l = SMDS_Mesh::_meshList[myMeshId]->getGrid()->GetCellLinks()->GetLink(myID);
  return (l.ncells == 0);
}

//================================================================================
/*!
 * \brief Count inverse elements of given type
 */
//================================================================================

int SMDS_MeshNode::NbInverseElements(SMDSAbs_ElementType type) const
{
  vtkCellLinks::Link l = SMDS_Mesh::_meshList[myMeshId]->getGrid()->GetCellLinks()->GetLink(myID);

  if ( type == SMDSAbs_All )
    return l.ncells;

  int nb = 0;
  SMDS_Mesh *mesh = SMDS_Mesh::_meshList[myMeshId];
  for (int i=0; i<l.ncells; i++)
        {
           const SMDS_MeshElement* elem = mesh->FindNode(l.cells[i]);
           if (elem->GetType() == type)
                nb++;
        }
  return nb;
}

///////////////////////////////////////////////////////////////////////////////
/// To be used with STL set
///////////////////////////////////////////////////////////////////////////////
bool operator<(const SMDS_MeshNode& e1, const SMDS_MeshNode& e2)
{
	return e1.GetID()<e2.GetID();
	/*if(e1.myX<e2.myX) return true;
	else if(e1.myX==e2.myX)
	{
	 	if(e1.myY<e2.myY) return true;
		else if(e1.myY==e2.myY) return (e1.myZ<e2.myZ);
		else return false;
	}
	else return false;*/
}

