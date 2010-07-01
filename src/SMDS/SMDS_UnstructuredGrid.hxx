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
#include "chrono.hxx"

class SMDS_Downward;
class SMDS_Mesh;

class SMDS_UnstructuredGrid: public vtkUnstructuredGrid
{
public:
  void setSMDS_mesh(SMDS_Mesh *mesh);
  void compactGrid(std::vector<int>& idNodesOldToNew, int newNodeSize, std::vector<int>& idCellsOldToNew,
                   int newCellSize);

  virtual unsigned long GetMTime();
  virtual void Update();
  virtual void UpdateInformation();

  int CellIdToDownId(int vtkCellId);
  void setCellIdToDownId(int vtkCellId, int downId);
  void BuildDownwardConnectivity();
  vtkCellLinks* GetLinks()
  {
    return Links;
  }
  SMDS_Downward* getDownArray(unsigned char vtkType)
  {
    return _downArray[vtkType];
  }
  static SMDS_UnstructuredGrid* New();
protected:
  SMDS_UnstructuredGrid();
  ~SMDS_UnstructuredGrid();
  void copyNodes(vtkPoints *newPoints, std::vector<int>& idNodesOldToNew, int& alreadyCopied, int start, int end);
  void copyBloc(vtkUnsignedCharArray *newTypes, std::vector<int>& idCellsOldToNew, std::vector<int>& idNodesOldToNew,
                vtkCellArray* newConnectivity, vtkIdTypeArray* newLocations, vtkIdType* pointsCell, int& alreadyCopied,
                int start, int end);

  SMDS_Mesh *_mesh;
  std::vector<int> _cellIdToDownId; //!< convert vtk Id to downward[vtkType] id, initialized with -1
  std::vector<unsigned char> _downTypes;
  std::vector<SMDS_Downward*> _downArray;
  counters *_counters;
};

#endif	/* _SMDS_UNSTRUCTUREDGRID_HXX */

