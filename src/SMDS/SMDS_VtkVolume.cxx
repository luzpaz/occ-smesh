
#include "SMDS_VtkVolume.hxx"
#include "SMDS_MeshNode.hxx"
#include "SMDS_Mesh.hxx"
#include "SMDS_VtkCellIterator.hxx"

#include "utilities.h"

#include <vector>


SMDS_VtkVolume::SMDS_VtkVolume()
{
}

SMDS_VtkVolume::SMDS_VtkVolume(std::vector<vtkIdType> nodeIds, SMDS_Mesh* mesh)
{
  init(nodeIds, mesh);
}
/*!
 * typed used are vtk types (@see vtkCellType.h)
 * see GetEntityType() for conversion in SMDS type (@see SMDSAbs_ElementType.hxx)
 */
void SMDS_VtkVolume::init(std::vector<vtkIdType> nodeIds, SMDS_Mesh* mesh)
{
  vtkUnstructuredGrid* grid = mesh->getGrid();
  myIdInShape = -1;
  myMeshId = mesh->getMeshId();
  vtkIdType aType = VTK_TETRA;
  switch(nodeIds.size())
    {
    case  4: aType = VTK_TETRA;                break;
    case  5: aType = VTK_PYRAMID;              break;
    case  6: aType = VTK_WEDGE;                break;
    case  8: aType = VTK_HEXAHEDRON;           break;
    case 10: aType = VTK_QUADRATIC_TETRA;      break;
    case 13: aType = VTK_QUADRATIC_PYRAMID;    break;
    case 15: aType = VTK_QUADRATIC_WEDGE;      break;
    case 20: aType = VTK_QUADRATIC_HEXAHEDRON; break;
    default: aType = VTK_HEXAHEDRON; break;
    }
  myVtkID = grid->InsertNextLinkedCell(aType, nodeIds.size(), &nodeIds[0]);
}

bool SMDS_VtkVolume::ChangeNodes(const SMDS_MeshNode* nodes[],
                                 const int            nbNodes)
{
  // utilise dans SMDS_Mesh
  return true;
}

SMDS_VtkVolume::~SMDS_VtkVolume()
{
}

void SMDS_VtkVolume::Print(ostream & OS) const
{
  OS << "volume <" << GetID() << "> : ";
}

int SMDS_VtkVolume::NbFaces() const
{
  switch(NbNodes())
    {
    case  4:
    case 10: return 4;
    case  5:
    case 13: return 5;
    case  6:
    case 15: return 5;
    case  8:
    case 20: return 6;
    default: MESSAGE("invalid number of nodes");
    }
  return 0;
}

int SMDS_VtkVolume::NbNodes() const
{
  vtkUnstructuredGrid* grid =SMDS_Mesh::_meshList[myMeshId]->getGrid();
  int nbPoints = grid->GetCell(myVtkID)->GetNumberOfPoints();
  return nbPoints;
}

int SMDS_VtkVolume::NbEdges() const
{
  switch(NbNodes())
    {
    case  4:
    case 10: return 6;
    case  5:
    case 13: return 8;
    case  6:
    case 15: return 9;
    case  8:
    case 20: return 12;
    default: MESSAGE("invalid number of nodes");
    }
  return 0;
}

SMDS_ElemIteratorPtr SMDS_VtkVolume::elementsIterator(SMDSAbs_ElementType type) const
{
  switch(type)
    {
    case SMDSAbs_Node:
      return SMDS_ElemIteratorPtr(new SMDS_VtkCellIterator(SMDS_Mesh::_meshList[myMeshId], myVtkID, GetEntityType()));
    default:
      MESSAGE("ERROR : Iterator not implemented");
      return SMDS_ElemIteratorPtr((SMDS_ElemIterator*)NULL);
    }
}

SMDSAbs_ElementType SMDS_VtkVolume::GetType() const
{
  return SMDSAbs_Volume;
}

/*!
 * \brief Return node by its index
 * \param ind - node index
 * \retval const SMDS_MeshNode* - the node
 */
const SMDS_MeshNode* SMDS_VtkVolume::GetNode(const int ind) const
{
  return SMDS_MeshElement::GetNode(ind); // --- a optimiser !
}

bool SMDS_VtkVolume::IsQuadratic() const
{
  if (this->NbNodes() > 9)
	return true;
  else
	return false;
}

SMDSAbs_EntityType SMDS_VtkVolume::GetEntityType() const
{
  SMDSAbs_EntityType aType = SMDSEntity_Tetra;
  switch(NbNodes())
    {
    case  4: aType = SMDSEntity_Tetra;        break;
    case  5: aType = SMDSEntity_Pyramid;      break;
    case  6: aType = SMDSEntity_Penta;        break;
    case  8: aType = SMDSEntity_Hexa;         break;
    case 10: aType = SMDSEntity_Quad_Tetra;   break;
    case 13: aType = SMDSEntity_Quad_Pyramid; break;
    case 15: aType = SMDSEntity_Quad_Penta;   break;
    case 20: aType = SMDSEntity_Quad_Hexa;    break;
    default: aType = SMDSEntity_Hexa;         break;
    }
  return aType;
}

vtkIdType SMDS_VtkVolume::GetVtkType() const
{
  vtkIdType aType = VTK_TETRA;
  switch(NbNodes())
    {
    case  4: aType = VTK_TETRA;                break;
    case  5: aType = VTK_PYRAMID;              break;
    case  6: aType = VTK_WEDGE;                break;
    case  8: aType = VTK_HEXAHEDRON;           break;
    case 10: aType = VTK_QUADRATIC_TETRA;      break;
    case 13: aType = VTK_QUADRATIC_PYRAMID;    break;
    case 15: aType = VTK_QUADRATIC_WEDGE;      break;
    case 20: aType = VTK_QUADRATIC_HEXAHEDRON; break;
    default: aType = VTK_HEXAHEDRON;           break;
    }
 return aType;
}
