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
//  File   : SMDS_VertexPosition.cxx
//  Author : Jean-Michel BOULCOURT
//  Module : SMESH

using namespace std;
#include "SMDS_VertexPosition.ixx"
#include "utilities.h"

//=======================================================================
//function : SMDS_VertexPosition
//purpose  : 
//=======================================================================

SMDS_VertexPosition::SMDS_VertexPosition()
  :SMDS_Position(0,SMDS_TOP_VERTEX)
{
}

//=======================================================================
//function : SMDS_VertexPosition
//purpose  : 
//=======================================================================

SMDS_VertexPosition::SMDS_VertexPosition(const Standard_Integer aVertexId)
  :SMDS_Position(aVertexId,SMDS_TOP_VERTEX)
{
}


//=======================================================================
//function : Coords
//purpose  : 
//=======================================================================

gp_Pnt SMDS_VertexPosition::Coords() const
{
  MESSAGE( "SMDS_VertexPosition::Coords not implemented" );
  return gp_Pnt(0,0,0);
}
