#ifndef _SMDS_MESHCELL_HXX_
#define _SMDS_MESHCELL_HXX_

#include "SMDS_MeshElement.hxx"

/*!
 * \brief Base class for all cells
 */

class SMDS_EXPORT SMDS_MeshCell: public SMDS_MeshElement
{
public:
  SMDS_MeshCell();
  virtual ~SMDS_MeshCell();
  inline void setVtkId(int vtkId)
  {
    myVtkID = vtkId;
  }

  inline int getVtkId() const
  {
    return myVtkID;
  }

  static int nbCells;
protected:
  int myVtkID;
};

#endif
