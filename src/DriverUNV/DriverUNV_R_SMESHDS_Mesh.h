//  SMESH DriverUNV : driver to read and write 'unv' files
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
//  File   : DriverUNV_R_SMESHDS_Mesh.h
//  Module : SMESH

#ifndef _INCLUDE_DRIVERUNV_R_SMESHDS_MESH
#define _INCLUDE_DRIVERUNV_R_SMESHDS_MESH

#include <stdio.h>

#include "SMESHDS_Mesh.hxx"
#include "Mesh_Reader.h"

class DriverUNV_R_SMESHDS_Mesh : public Mesh_Reader {

  public :
    DriverUNV_R_SMESHDS_Mesh();
    ~DriverUNV_R_SMESHDS_Mesh();

    void Add();
    void Read();
    void SetMesh(Handle(SMDS_Mesh)& aMesh);
    void SetFile(string);

    void SetFileId(FILE*);
    void SetMeshId(int);

private :
    Handle_SMDS_Mesh myMesh;
    string myFile; 
    FILE* myFileId;
    int myMeshId;

};
#endif
