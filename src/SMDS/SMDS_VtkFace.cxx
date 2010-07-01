#include "SMDS_VtkFace.hxx"
#include "SMDS_MeshNode.hxx"
#include "SMDS_Mesh.hxx"
#include "SMDS_VtkCellIterator.hxx"

#include "utilities.h"

#include <vector>

using namespace std;

SMDS_VtkFace::SMDS_VtkFace()
{
}

SMDS_VtkFace::SMDS_VtkFace(std::vector<vtkIdType> nodeIds, SMDS_Mesh* mesh)
{
  init(nodeIds, mesh);
}

SMDS_VtkFace::~SMDS_VtkFace()
{
}

void SMDS_VtkFace::init(std::vector<vtkIdType> nodeIds, SMDS_Mesh* mesh)
{
  vtkUnstructuredGrid* grid = mesh->getGrid();
  myIdInShape = -1;
  myMeshId = mesh->getMeshId();
  vtkIdType aType = VTK_TRIANGLE;
  switch (nodeIds.size())
  {
    case 3:
      aType = VTK_TRIANGLE;
      break;
    case 4:
      aType = VTK_QUAD;
      break;
    case 6:
      aType = VTK_QUADRATIC_TRIANGLE;
      break;
    case 8:
      aType = VTK_QUADRATIC_QUAD;
      break;
    default:
      aType = VTK_TRIANGLE;
      break;
  }
  myVtkID = grid->InsertNextLinkedCell(aType, nodeIds.size(), &nodeIds[0]);
}

bool SMDS_VtkFace::ChangeNodes(const SMDS_MeshNode* nodes[], const int nbNodes)
{
  return true;
}

void SMDS_VtkFace::Print(std::ostream & OS) const
{
  OS << "edge <" << GetID() << "> : ";
}

int SMDS_VtkFace::NbEdges() const
{
  switch (NbNodes())
  {
    case 3:
    case 6:
      return 3;
    case 4:
    case 8:
      return 4;
    default:
      MESSAGE("invalid number of nodes")
      ;
  }
  return 0;
}

int SMDS_VtkFace::NbFaces() const
{
  return 1;
}

int SMDS_VtkFace::NbNodes() const
{
  vtkUnstructuredGrid* grid = SMDS_Mesh::_meshList[myMeshId]->getGrid();
  int nbPoints = grid->GetCell(myVtkID)->GetNumberOfPoints();
  return nbPoints;
}

/*!
 * \brief Return node by its index
 * \param ind - node index
 * \retval const SMDS_MeshNode* - the node
 */
const SMDS_MeshNode*
SMDS_VtkFace::GetNode(const int ind) const
{
  return SMDS_MeshElement::GetNode(ind); // --- a optimiser !
}

bool SMDS_VtkFace::IsQuadratic() const
{
  if (this->NbNodes() > 5)
    return true;
  else
    return false;
}

SMDSAbs_EntityType SMDS_VtkFace::GetEntityType() const
{
  SMDSAbs_EntityType aType = SMDSEntity_Triangle;
  switch (NbNodes())
  {
    case 3:
    case 6:
      aType = SMDSEntity_Triangle;
      break;
    case 4:
    case 8:
      aType = SMDSEntity_Quadrangle;
      break;
  }
  return aType;
}

vtkIdType SMDS_VtkFace::GetVtkType() const
{
  switch (NbNodes())
  {
    case 3:
      return VTK_TRIANGLE;
    case 6:
      return VTK_QUADRATIC_TRIANGLE;
    case 4:
      return VTK_QUAD;
    case 8:
      return VTK_QUADRATIC_QUAD;
  }
}

SMDS_ElemIteratorPtr SMDS_VtkFace::elementsIterator(SMDSAbs_ElementType type) const
{
  switch (type)
  {
    case SMDSAbs_Node:
      return SMDS_ElemIteratorPtr(
                                  new SMDS_VtkCellIterator(
                                                           SMDS_Mesh::_meshList[myMeshId],
                                                           myVtkID,
                                                           GetEntityType()));
    default:
      MESSAGE("ERROR : Iterator not implemented")
      ;
      return SMDS_ElemIteratorPtr((SMDS_ElemIterator*) NULL);
  }
}

