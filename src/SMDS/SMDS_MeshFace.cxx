//  SMESH SMDS : implementaion of Salome mesh data structure
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SMDS_MeshFace.cxx
//  Author : Jean-Michel BOULCOURT
//  Module : SMESH

using namespace std;
#include "SMDS_MeshFace.ixx"


//=======================================================================
//function : SMDS_MeshFace
//purpose  : 
//=======================================================================

SMDS_MeshFace::SMDS_MeshFace(const Standard_Integer ID, const Standard_Integer nb) 
:SMDS_MeshElement(ID,nb,SMDSAbs_Face)
{
}


//=======================================================================
//function : NbEdges
//purpose  : 
//=======================================================================

Standard_Integer SMDS_MeshFace::NbEdges() const
{
  return myNbNodes;
}

//=======================================================================
//function : Print
//purpose  : 
//=======================================================================

void SMDS_MeshFace::Print(Standard_OStream& OS) const
{
  OS << "face <" << myID <<" > : ";
  for (Standard_Integer i=1; i<myNbNodes; ++i)
    OS << GetConnection(i) << ",";
  OS << GetConnection(myNbNodes) << ") " << endl;
}


