/* 
 * File:   SMDS_UnstructuredGrid.hxx
 * Author: prascle
 *
 * Created on September 16, 2009, 10:28 PM
 */

#ifndef _SMDS_UNSTRUCTUREDGRID_HXX
#define	_SMDS_UNSTRUCTUREDGRID_HXX

#include <vector>

#include <vtkUnstructuredGrid.h>

class SMDS_UnstructuredGrid: public vtkUnstructuredGrid
{
public:
	void compactGrid(std::vector<int>& idNodesOldToNew, int newNodeSize,
					 std::vector<int>& idCellsOldToNew, int newCellSize);

	virtual unsigned long GetMTime();
	virtual void Update();
	virtual void UpdateInformation();

    static SMDS_UnstructuredGrid* New();
protected:
    SMDS_UnstructuredGrid();
    ~SMDS_UnstructuredGrid();
    void copyNodes(vtkPoints *newPoints,
    		std::vector<int>& idNodesOldToNew,
    		int& alreadyCopied,
    		int start,
    		int end);
  	void copyBloc(vtkUnsignedCharArray *newTypes,
			std::vector<int>& idCellsOldToNew,
			std::vector<int>& idNodesOldToNew,
  			vtkCellArray* newConnectivity,
  			vtkIdTypeArray* newLocations,
  			vtkIdType* pointsCell,
  			int& alreadyCopied,
  			int start,
  			int end);

};

#endif	/* _SMDS_UNSTRUCTUREDGRID_HXX */

