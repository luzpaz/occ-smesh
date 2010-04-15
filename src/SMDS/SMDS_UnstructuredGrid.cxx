

#include "SMDS_UnstructuredGrid.hxx"
#include "utilities.h"

#include <vtkCellArray.h>
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
	typedef enum {lookHoleStart, lookHoleEnd, lookBlocEnd} enumState;
	enumState compactState = lookHoleStart;

	// --- if newNodeSize, create a new compacted vtkPoints

	vtkPoints *newPoints = 0;
	if (newNodeSize)
	{
		MESSAGE("-------------- compactGrid, newNodeSize");
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
					startHole = i;
					compactState = lookHoleEnd;
				}
				break;
			case lookHoleEnd:
				if (idNodesOldToNew[i] >= 0)
				{
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
					MESSAGE("-------------- newNodeSize, endbloc");
					void *target = newPoints->GetVoidPointer(3*alreadyCopied);
					void *source = this->Points->GetVoidPointer(3*startBloc);
					int nbPoints = endBloc - startBloc;
					memcpy(target, source, 3*sizeof(float)*nbPoints);
					for (int j=startBloc; j<endBloc; j++)
						idNodesOldToNew[j] = alreadyCopied++;
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
			MESSAGE("------------- newNodeSize, shorter")
			void *target = newPoints->GetVoidPointer(0);
			void *source = this->Points->GetVoidPointer(0);
			int nbPoints = newNodeSize;
			memcpy(target, source, 3*sizeof(float)*nbPoints);
		}
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
				startHole = i;
				compactState = lookHoleEnd;
			}
			break;
		case lookHoleEnd:
			if (this->Types->GetValue(i) != VTK_EMPTY_CELL)
			{
				endHole = i;
				startBloc = i;
				compactState = lookBlocEnd;
			}
			break;
		case lookBlocEnd:
			if (this->Types->GetValue(i) == VTK_EMPTY_CELL) endBloc =i;
			else if (i == (oldCellSize-1)) endBloc = i+1;
			if (endBloc)
			{
		    	MESSAGE(" -------- newCellSize, endBloc");
				for (int j=startBloc; j<endBloc; j++)
				{
					newTypes->SetValue(alreadyCopied, this->Types->GetValue(j));
					idCellsOldToNew[j] = alreadyCopied;
					int oldLoc = this->Locations->GetValue(j);
					int nbpts;
					this->Connectivity->GetCell(oldLoc, nbpts, pointsCell);
					for (int l=0; l<nbpts; l++)
						pointsCell[l] = idNodesOldToNew[pointsCell[l]];
					int newcnt = newConnectivity->InsertNextCell(nbpts, pointsCell);
					int newLoc = newConnectivity->GetInsertLocation(nbpts);
					newLocations->SetValue(alreadyCopied, newLoc);
					alreadyCopied++;
				}
			}
			break;
		}
    }
    if (!alreadyCopied) // no hole, but shorter
    {
    	MESSAGE(" -------- newCellSize, shorter");
		for (int j=0; j<oldCellSize; j++)
		{
			newTypes->SetValue(alreadyCopied, this->Types->GetValue(j));
			idCellsOldToNew[j] = alreadyCopied;
			int oldLoc = this->Locations->GetValue(j);
			int nbpts;
			this->Connectivity->GetCell(oldLoc, nbpts, pointsCell);
			//MESSAGE(j << " " << alreadyCopied << " " << (int)this->Types->GetValue(j) << " " << oldLoc << " " << nbpts );
			for (int l=0; l<nbpts; l++)
			{
				int oldval = pointsCell[l];
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

    newConnectivity->Squeeze();
    //newTypes->Squeeze();
    //newLocations->Squeeze();

    if (newNodeSize)
    {
    	MESSAGE("------- newNodeSize, setPoints");
    	this->SetPoints(newPoints);
    }
    this->SetCells(newTypes, newLocations, newConnectivity);
    this->BuildLinks();
}
