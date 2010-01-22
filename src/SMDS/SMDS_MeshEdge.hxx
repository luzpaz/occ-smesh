#ifndef _SMDS_MESHEDGE_HXX_
#define _SMDS_MESHEDGE_HXX_

#include "SMESH_SMDS.hxx"

#include "SMDS_MeshCell.hxx"

class SMDS_EXPORT SMDS_MeshEdge:public SMDS_MeshCell
{
	
  public:
	SMDSAbs_ElementType GetType() const;
  virtual vtkIdType GetVtkType() const;
};
#endif
