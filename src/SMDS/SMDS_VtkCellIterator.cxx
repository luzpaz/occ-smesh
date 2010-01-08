
#include "SMDS_VtkCellIterator.hxx"

SMDS_VtkCellIterator::SMDS_VtkCellIterator(SMDS_Mesh* mesh, int vtkCellId):
  _mesh(mesh), _cellId(vtkCellId), _index(0)
{    
  vtkUnstructuredGrid* grid = _mesh->getGrid();
  _vtkIdList = grid->GetCell(_cellId)->GetPointIds();
  _nbNodes = _vtkIdList->GetNumberOfIds();
}

bool SMDS_VtkCellIterator::more()
{
  return (_index < _nbNodes);
}

const SMDS_MeshElement* SMDS_VtkCellIterator::next()
{
  vtkIdType id = _vtkIdList->GetId(_index++);
  return _mesh->FindNode(id);
}
