#include "SMDS_VtkCellIterator.hxx"

SMDS_VtkCellIterator::SMDS_VtkCellIterator(SMDS_Mesh* mesh, int vtkCellId, SMDSAbs_EntityType aType) :
  _mesh(mesh), _cellId(vtkCellId), _index(0), _type(aType)
{
  vtkUnstructuredGrid* grid = _mesh->getGrid();
  _vtkIdList = vtkIdList::New();
  grid->GetCellPoints(_cellId, _vtkIdList);
  _nbNodes = _vtkIdList->GetNumberOfIds();
  vtkIdType tempid;
  switch (_type)
  {
    case SMDSEntity_Tetra:
    {
      this->exchange(1, 2);
      break;
    }
    case SMDSEntity_Pyramid:
    {
      this->exchange(1, 3);
      break;
    }
    case SMDSEntity_Penta:
    {
      //this->exchange(1, 2);
      //this->exchange(4, 5);
      break;
    }
    case SMDSEntity_Hexa:
    {
      this->exchange(1, 3);
      this->exchange(5, 7);
      break;
    }
    case SMDSEntity_Quad_Tetra:
    {
      this->exchange(1, 2);
      this->exchange(4, 6);
      this->exchange(8, 9);
      break;
    }
    case SMDSEntity_Quad_Pyramid:
    {
      this->exchange(1, 3);
      this->exchange(5, 8);
      this->exchange(6, 7);
      this->exchange(10, 12);
      break;
    }
    case SMDSEntity_Quad_Penta:
    {
      //this->exchange(1, 2);
      //this->exchange(4, 5);
      //this->exchange(6, 8);
      //this->exchange(9, 11);
      //this->exchange(13, 14);
      break;
    }
    case SMDSEntity_Quad_Hexa:
    {
      this->exchange(1, 3);
      this->exchange(5, 7);
      this->exchange(8, 11);
      this->exchange(9, 10);
      this->exchange(12, 15);
      this->exchange(13, 14);
      this->exchange(17, 19);
      break;
    }
    default:
      break;
  }
}

SMDS_VtkCellIterator::~SMDS_VtkCellIterator()
{
  _vtkIdList->Delete();
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
