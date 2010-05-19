

#include "SMDS_UnstructuredGrid.hxx"
#include "utilities.h"

#include <vtkCellArray.h>
#include <vtkCellLinks.h>
#include <vtkIdTypeArray.h>
#include <vtkUnsignedCharArray.h>

#include <list>

using namespace std;

SMDS_UnstructuredGrid* SMDS_UnstructuredGrid::New()
{
	MESSAGE("SMDS_UnstructuredGrid::New");
	return new SMDS_UnstructuredGrid();
}

SMDS_UnstructuredGrid::SMDS_UnstructuredGrid() : vtkUnstructuredGrid()
{
}

SMDS_UnstructuredGrid::~SMDS_UnstructuredGrid()
{
}


unsigned long SMDS_UnstructuredGrid::GetMTime()
{
	unsigned long mtime = vtkUnstructuredGrid::GetMTime();
	MESSAGE("vtkUnstructuredGrid::GetMTime: " << mtime);
	return mtime;
}

void SMDS_UnstructuredGrid::Update()
{
	MESSAGE("SMDS_UnstructuredGrid::Update");
	return vtkUnstructuredGrid::Update();
}

void SMDS_UnstructuredGrid::UpdateInformation()
{
	MESSAGE("SMDS_UnstructuredGrid::UpdateInformation");
	return vtkUnstructuredGrid::UpdateInformation();
}

void SMDS_UnstructuredGrid::compactGrid(std::vector<int>& idNodesOldToNew, int newNodeSize,
										std::vector<int>& idCellsOldToNew, int newCellSize)
{
	MESSAGE("------------------------- SMDS_UnstructuredGrid::compactGrid " << newNodeSize << " " << newCellSize);

	int startHole = 0;
	int endHole = 0;
	int startBloc = 0;
	int endBloc = 0;
	int alreadyCopied = 0;
	int holes = 0;

	typedef enum {lookHoleStart, lookHoleEnd, lookBlocEnd} enumState;
	enumState compactState = lookHoleStart;

//	if (this->Links)
//	{
//		this->Links->UnRegister(this);
//		this->Links = 0;
//	}

	// --- if newNodeSize, create a new compacted vtkPoints

	vtkPoints *newPoints = 0;
	if (newNodeSize)
	{
		MESSAGE("-------------- compactGrid, newNodeSize " << newNodeSize);
		newPoints = vtkPoints::New();
		newPoints->Initialize();
		newPoints->Allocate(newNodeSize);
		newPoints->SetNumberOfPoints(newNodeSize);
		int oldNodeSize = idNodesOldToNew.size();

		for (int i=0; i< oldNodeSize; i++)
		{
			switch(compactState)
			{
			case lookHoleStart:
				if (idNodesOldToNew[i] < 0)
				{
					MESSAGE("-------------- newNodeSize, startHole " << i <<  " " << oldNodeSize);
					startHole = i;
					if (!alreadyCopied) // copy the first bloc
					{
						MESSAGE("--------- copy first nodes before hole " << i << " " << oldNodeSize);
						copyNodes(newPoints, idNodesOldToNew, alreadyCopied, 0, startHole);
					}
					compactState = lookHoleEnd;
				}
				break;
			case lookHoleEnd:
				if (idNodesOldToNew[i] >= 0)
				{
					MESSAGE("-------------- newNodeSize, endHole " << i <<  " " << oldNodeSize);
					endHole = i;
					startBloc = i;
					compactState = lookBlocEnd;
				}
				break;
			case lookBlocEnd:
				if (idNodesOldToNew[i] < 0) endBloc = i; // see nbPoints below
				else if (i == (oldNodeSize-1)) endBloc = i+1;
				if (endBloc)
				{
					MESSAGE("-------------- newNodeSize, endbloc " << endBloc <<  " " << oldNodeSize);
					copyNodes(newPoints, idNodesOldToNew, alreadyCopied, startBloc, endBloc);
					compactState = lookHoleStart;
					startHole = i;
					endHole = 0;
					startBloc = 0;
					endBloc = 0;
				}
				break;
			}
		}
		if (!alreadyCopied) // no hole, but shorter, no need to modify idNodesOldToNew
		{
			MESSAGE("------------- newNodeSize, shorter " << oldNodeSize);
			copyNodes(newPoints, idNodesOldToNew, alreadyCopied, 0, newNodeSize);
		}
		newPoints->Squeeze();
	}

	// --- create new compacted Connectivity, Locations and Types

    int oldCellSize = this->Types->GetNumberOfTuples();

	vtkCellArray *newConnectivity = vtkCellArray::New();
	newConnectivity->Initialize();
	int oldCellDataSize = this->Connectivity->GetData()->GetSize();
	newConnectivity->Allocate(oldCellDataSize);
	MESSAGE("oldCellSize="<< oldCellSize << " oldCellDataSize=" << oldCellDataSize);

	vtkUnsignedCharArray *newTypes = vtkUnsignedCharArray::New();
	newTypes->Initialize();
	//newTypes->Allocate(oldCellSize);
	newTypes->SetNumberOfValues(newCellSize);

	vtkIdTypeArray *newLocations = vtkIdTypeArray::New();
	newLocations->Initialize();
	//newLocations->Allocate(oldCellSize);
	newLocations->SetNumberOfValues(newCellSize);

	startHole = 0;
	endHole = 0;
	startBloc = 0;
	endBloc = 0;
	alreadyCopied = 0;
	holes = 0;
	compactState = lookHoleStart;

	vtkIdType tmpid[50];
	vtkIdType *pointsCell =&tmpid[0]; // --- points id to fill a new cell

    for (int i=0; i<oldCellSize; i++)
    {
		switch(compactState)
		{
		case lookHoleStart:
			if (this->Types->GetValue(i) == VTK_EMPTY_CELL)
			{
		    MESSAGE(" -------- newCellSize, startHole " << i << " " << oldCellSize);
				startHole = i;
				compactState = lookHoleEnd;
				if (!alreadyCopied) // copy the first bloc
				{
					MESSAGE("--------- copy first bloc before hole " << i << " " << oldCellSize);
					copyBloc(newTypes, idCellsOldToNew, idNodesOldToNew, newConnectivity, newLocations, pointsCell, alreadyCopied, 0, startHole);
				}
			}
			break;
		case lookHoleEnd:
			if (this->Types->GetValue(i) != VTK_EMPTY_CELL)
			{
		    MESSAGE(" -------- newCellSize, EndHole " << i << " " << oldCellSize);
				endHole = i;
				startBloc = i;
				compactState = lookBlocEnd;
				holes += endHole - startHole;
				//alreadyCopied = startBloc -holes;
			}
			break;
		case lookBlocEnd:
			endBloc =0;
			if (this->Types->GetValue(i) == VTK_EMPTY_CELL) endBloc =i;
			else if (i == (oldCellSize-1)) endBloc = i+1;
			if (endBloc)
			{
		  	MESSAGE(" -------- newCellSize, endBloc " << endBloc << " " << oldCellSize);
		   	copyBloc(newTypes, idCellsOldToNew, idNodesOldToNew, newConnectivity, newLocations, pointsCell, alreadyCopied, startBloc, endBloc);
  			compactState = lookHoleStart;
			}
			break;
		}
    }
    if (!alreadyCopied) // no hole, but shorter
    {
    	MESSAGE(" -------- newCellSize, shorter " << oldCellSize);
    	copyBloc(newTypes, idCellsOldToNew, idNodesOldToNew, newConnectivity, newLocations, pointsCell, alreadyCopied, 0, oldCellSize);
    }

    newConnectivity->Squeeze();
    //newTypes->Squeeze();
    //newLocations->Squeeze();

    if (newNodeSize)
    {
    	MESSAGE("------- newNodeSize, setPoints");
    	this->SetPoints(newPoints);
    	MESSAGE("NumberOfPoints: " << this->GetNumberOfPoints());
    }
    this->SetCells(newTypes, newLocations, newConnectivity);
    this->BuildLinks();
}

void SMDS_UnstructuredGrid::copyNodes(vtkPoints *newPoints,
		std::vector<int>& idNodesOldToNew,
		int& alreadyCopied,
		int start,
		int end)
{
	MESSAGE("copyNodes " << alreadyCopied << " " << start << " " << end << " size: " << end - start << " total: " << alreadyCopied + end - start);
	void *target = newPoints->GetVoidPointer(3*alreadyCopied);
	void *source = this->Points->GetVoidPointer(3*start);
	int nbPoints = end - start;
	if (nbPoints >0)
	{
		memcpy(target, source, 3*sizeof(float)*nbPoints);
		for (int j=start; j<end; j++)
			idNodesOldToNew[j] = alreadyCopied++;
	}
}

void SMDS_UnstructuredGrid::copyBloc(vtkUnsignedCharArray *newTypes,
		std::vector<int>& idCellsOldToNew,
		std::vector<int>& idNodesOldToNew,
		vtkCellArray* newConnectivity,
		vtkIdTypeArray* newLocations,
		vtkIdType* pointsCell,
		int& alreadyCopied,
		int start,
		int end)
{
	MESSAGE("copyBloc " << alreadyCopied << " " << start << " " << end << " size: " << end - start << " total: " << alreadyCopied + end - start);
	for (int j=start; j<end; j++)
	{
		newTypes->SetValue(alreadyCopied, this->Types->GetValue(j));
		idCellsOldToNew[j] = alreadyCopied;
		vtkIdType oldLoc = this->Locations->GetValue(j);
		vtkIdType nbpts;
		vtkIdType *oldPtsCell = 0;
		this->Connectivity->GetCell(oldLoc, nbpts, oldPtsCell);
		//MESSAGE(j << " " << alreadyCopied << " " << (int)this->Types->GetValue(j) << " " << oldLoc << " " << nbpts );
		for (int l=0; l<nbpts; l++)
		{
			int oldval = oldPtsCell[l];
			pointsCell[l] = idNodesOldToNew[oldval];
			//MESSAGE("   " << oldval << " " << pointsCell[l]);
		}
		int newcnt = newConnectivity->InsertNextCell(nbpts, pointsCell);
		int newLoc = newConnectivity->GetInsertLocation(nbpts);
		//MESSAGE(newcnt << " " << newLoc);
		newLocations->SetValue(alreadyCopied, newLoc);
		alreadyCopied++;
	}
}
