

#include "SMDS_UnstructuredGrid.hxx"

using namespace std;

vtkCellLinks::Link* SMDS_CellLinks::AdjustSize(vtkIdType sz)
{
  vtkIdType i;
  vtkCellLinks::Link *newArray;
  vtkIdType newSize = sz;
  vtkCellLinks::Link linkInit = {0,NULL};

  newArray = new vtkCellLinks::Link[newSize];

  for (i=0; i<sz && i<this->Size; i++)
    {
    newArray[i] = this->Array[i];
    }

  for (i=this->Size; i < newSize ; i++)
    {
    newArray[i] = linkInit;
    }

  this->Size = newSize;
  delete [] this->Array;
  this->Array = newArray;

  return this->Array;
}

SMDS_CellLinks* SMDS_CellLinks::New()
{
  return new SMDS_CellLinks();
}

SMDS_CellLinks::SMDS_CellLinks() : vtkCellLinks()
{
}

SMDS_CellLinks::~SMDS_CellLinks()
{
}

/*! initialize an SMDS_CellLinks instance instead of a vtkCellLinks instance
 *
 */
void SMDS_UnstructuredGrid::BuildLinks()
{
  // Remove the old links if they are already built
  if (this->Links)
    {
    this->Links->UnRegister(this);
    }

  this->Links = SMDS_CellLinks::New();
  this->Links->Allocate(this->GetNumberOfPoints());
  this->Links->Register(this);
  this->Links->BuildLinks(this, this->Connectivity);
  this->Links->Delete();
}

SMDS_CellLinks* SMDS_UnstructuredGrid::GetCellLinks()
{
  return static_cast<SMDS_CellLinks*>(this->Links);
}

SMDS_UnstructuredGrid* SMDS_UnstructuredGrid::New()
{
  return new SMDS_UnstructuredGrid();
}

SMDS_UnstructuredGrid::SMDS_UnstructuredGrid() : vtkUnstructuredGrid()
{
}

SMDS_UnstructuredGrid::~SMDS_UnstructuredGrid()
{
}


