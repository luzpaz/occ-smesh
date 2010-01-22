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
	if (nodeIds.size() == 4)
		aType = VTK_QUAD;
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
	return NbNodes();
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
	return 0;
}

SMDSAbs_EntityType SMDS_VtkFace::GetEntityType() const
{
	int nbNodes = NbNodes();
	if (nbNodes == 3)
		return SMDSEntity_Triangle;
	else
		return SMDSEntity_Quadrangle;
}

vtkIdType SMDS_VtkFace::GetVtkType() const
{
	int nbNodes = NbNodes();
	if (nbNodes == 3)
		return VTK_TRIANGLE;
	else
		return VTK_QUAD;

}

SMDS_ElemIteratorPtr SMDS_VtkFace::elementsIterator(SMDSAbs_ElementType type) const
{
	switch (type)
	{
	case SMDSAbs_Node:
		return SMDS_ElemIteratorPtr(new SMDS_VtkCellIterator(SMDS_Mesh::_meshList[myMeshId], myVtkID, GetEntityType()));
	default:
		MESSAGE("ERROR : Iterator not implemented");
		return SMDS_ElemIteratorPtr((SMDS_ElemIterator*) NULL);
	}
}

