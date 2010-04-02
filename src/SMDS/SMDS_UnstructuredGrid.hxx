/* 
 * File:   SMDS_UnstructuredGrid.hxx
 * Author: prascle
 *
 * Created on September 16, 2009, 10:28 PM
 */

#ifndef _SMDS_UNSTRUCTUREDGRID_HXX
#define	_SMDS_UNSTRUCTUREDGRID_HXX

#include <vtkUnstructuredGrid.h>
//#include <vtkCellLinks.h>

//class SMDS_CellLinks: public vtkCellLinks
//{
//public:
//    Link *AdjustSize(vtkIdType sz);
//
//    //virtual void Delete();
//    static SMDS_CellLinks* New();
//protected:
//    SMDS_CellLinks();
//    ~SMDS_CellLinks();
//};

class SMDS_UnstructuredGrid: public vtkUnstructuredGrid
{
public:
//    void BuildLinks(); // initialise un SMDS_CellLinks;
//    SMDS_CellLinks* GetCellLinks();
//
//    vtkIdType GetCellArraySize() { return (this->Connectivity ? this->Connectivity->GetSize() : 0); };

	virtual unsigned long GetMTime();
	virtual void UpdateInformation();

    //virtual void Delete();
    static SMDS_UnstructuredGrid* New();
protected:
    SMDS_UnstructuredGrid();
    ~SMDS_UnstructuredGrid();
};


#endif	/* _SMDS_UNSTRUCTUREDGRID_HXX */

