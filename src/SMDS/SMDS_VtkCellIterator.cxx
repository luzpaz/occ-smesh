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
      tempid = _vtkIdList->GetId(1);
      _vtkIdList->SetId(1, _vtkIdList->GetId(2));
      _vtkIdList->SetId(2, tempid);
      break;
    }
    case SMDSEntity_Pyramid:
    {
      tempid = _vtkIdList->GetId(1);
      _vtkIdList->SetId(1, _vtkIdList->GetId(3));
      _vtkIdList->SetId(3, tempid);
      break;
    }
    case SMDSEntity_Penta:
    {
      tempid = _vtkIdList->GetId(1);
      _vtkIdList->SetId(1, _vtkIdList->GetId(2));
      _vtkIdList->SetId(2, tempid);
      tempid = _vtkIdList->GetId(4);
      _vtkIdList->SetId(4, _vtkIdList->GetId(5));
      _vtkIdList->SetId(5, tempid);
     break;
    }
    case SMDSEntity_Hexa:
    {
      tempid = _vtkIdList->GetId(1);
      _vtkIdList->SetId(1, _vtkIdList->GetId(3));
      _vtkIdList->SetId(3, tempid);
      tempid = _vtkIdList->GetId(5);
      _vtkIdList->SetId(5, _vtkIdList->GetId(7));
      _vtkIdList->SetId(7, tempid);
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
