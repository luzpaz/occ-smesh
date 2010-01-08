#ifndef _SMDS_VTKCELLITERATOR_HXX_
#define _SMDS_VTKCELLITERATOR_HXX_

#include "SMDS_ElemIterator.hxx"
#include "SMDS_Mesh.hxx"

#include <vtkCell.h>
#include <vtkIdList.h>

class SMDS_VtkCellIterator : public SMDS_ElemIterator
{
public:
  SMDS_VtkCellIterator(SMDS_Mesh* mesh, int vtkCellId);
  virtual bool more();
  virtual const SMDS_MeshElement* next();
protected:
  SMDS_Mesh* _mesh;
  int _cellId;
  int _index;
  int _nbNodes;
  vtkIdList* _vtkIdList;
};

#endif
