//  SMESH SMDS : implementaion of Salome mesh data structure
//
//  Copyright (C) 2003  OPEN CASCADE
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
//  See http://www.opencascade.org or email : webmaster@opencascade.org 
//
//
//
//  File   : SMDSControl_BoundaryFaces.ixx
//  Module : SMESH

#include "SMDSControl_BoundaryFaces.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SMDSControl_BoundaryFaces::~SMDSControl_BoundaryFaces() {}
 


Standard_EXPORT Handle_Standard_Type& SMDSControl_BoundaryFaces_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(SMDSControl_MeshBoundary);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(SMDSControl_MeshBoundary);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SMDSControl_BoundaryFaces",
			                                 sizeof(SMDSControl_BoundaryFaces),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SMDSControl_BoundaryFaces) Handle(SMDSControl_BoundaryFaces)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SMDSControl_BoundaryFaces) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SMDSControl_BoundaryFaces))) {
       _anOtherObject = Handle(SMDSControl_BoundaryFaces)((Handle(SMDSControl_BoundaryFaces)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SMDSControl_BoundaryFaces::DynamicType() const 
{ 
  return STANDARD_TYPE(SMDSControl_BoundaryFaces) ; 
}
Standard_Boolean SMDSControl_BoundaryFaces::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SMDSControl_BoundaryFaces) == AType || SMDSControl_MeshBoundary::IsKind(AType)); 
}
Handle_SMDSControl_BoundaryFaces::~Handle_SMDSControl_BoundaryFaces() {}

