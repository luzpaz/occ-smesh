#include "SMDS_VtkEdge.hxx"
#include "SMDS_MeshNode.hxx"
#include "SMDS_Mesh.hxx"
#include "SMDS_VtkCellIterator.hxx"

#include "utilities.h"

#include <vector>

using namespace std;

SMDS_VtkEdge::SMDS_VtkEdge()
{
}

SMDS_VtkEdge::SMDS_VtkEdge(std::vector<vtkIdType> nodeIds, SMDS_Mesh* mesh)
{
	init(nodeIds, mesh);
}

SMDS_VtkEdge::~SMDS_VtkEdge()
{
}

void SMDS_VtkEdge::init(std::vector<vtkIdType> nodeIds, SMDS_Mesh* mesh)
{
	vtkUnstructuredGrid* grid = mesh->getGrid();
	myIdInShape = -1;
	myMeshId = mesh->getMeshId();
	vtkIdType aType = VTK_LINE;
	if (nodeIds.size() == 3)
      aType = VTK_QUADRATIC_EDGE;
	myVtkID = grid->InsertNextLinkedCell(aType, nodeIds.size(), &nodeIds[0]);
}

bool SMDS_VtkEdge::ChangeNodes(const SMDS_MeshNode * node1,
		const SMDS_MeshNode * node2)
{
	return true;
}

void SMDS_VtkEdge::Print(std::ostream & OS) const
{
	OS << "edge <" << GetID() << "> : ";
}

int SMDS_VtkEdge::NbNodes() const
{
	vtkUnstructuredGrid* grid =SMDS_Mesh::_meshList[myMeshId]->getGrid();
	int nbPoints = grid->GetCell(myVtkID)->GetNumberOfPoints();
	return nbPoints;
}

int SMDS_VtkEdge::NbEdges() const
{
	return 1;
}

SMDSAbs_EntityType SMDS_VtkEdge::GetEntityType() const
{
	if (NbNodes() == 2)
	  return SMDSEntity_Edge;
	else
	  return SMDSEntity_Quad_Edge;
}

vtkIdType SMDS_VtkEdge::GetVtkType() const
{
	if (NbNodes() == 2)
		return VTK_LINE;
	else
		return VTK_QUADRATIC_EDGE;

}

/*!
 * \brief Return node by its index
 * \param ind - node index
 * \retval const SMDS_MeshNode* - the node
 */
const SMDS_MeshNode*
SMDS_VtkEdge::GetNode(const int ind) const
{
  return SMDS_MeshElement::GetNode(ind); // --- a optimiser !
}

bool SMDS_VtkEdge::IsQuadratic() const
{
  if (this->NbNodes() > 2)
	return true;
  else
	return false;
}

SMDS_ElemIteratorPtr SMDS_VtkEdge::elementsIterator(SMDSAbs_ElementType type) const
{
	switch (type)
	{
	case SMDSAbs_Node:
		return SMDS_ElemIteratorPtr(new SMDS_VtkCellIterator(
				SMDS_Mesh::_meshList[myMeshId], myVtkID, GetEntityType()));
	default:
		MESSAGE("ERROR : Iterator not implemented");
		return SMDS_ElemIteratorPtr((SMDS_ElemIterator*) NULL);
	}
}
