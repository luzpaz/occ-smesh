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
//  File   : SMDS_ListNodeOfListOfMeshElement_0.cxx
//  Module : SMESH

using namespace std;
#include "SMDS_ListNodeOfListOfMeshElement.hxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

#ifndef _SMDS_MeshElement_HeaderFile
#include "SMDS_MeshElement.hxx"
#endif
#ifndef _SMDS_ListOfMeshElement_HeaderFile
#include "SMDS_ListOfMeshElement.hxx"
#endif
#ifndef _SMDS_ListIteratorOfListOfMeshElement_HeaderFile
#include "SMDS_ListIteratorOfListOfMeshElement.hxx"
#endif
SMDS_ListNodeOfListOfMeshElement::~SMDS_ListNodeOfListOfMeshElement() {}
 


Standard_EXPORT Handle_Standard_Type& SMDS_ListNodeOfListOfMeshElement_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TCollection_MapNode);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TCollection_MapNode);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SMDS_ListNodeOfListOfMeshElement",
			                                 sizeof(SMDS_ListNodeOfListOfMeshElement),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SMDS_ListNodeOfListOfMeshElement) Handle(SMDS_ListNodeOfListOfMeshElement)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SMDS_ListNodeOfListOfMeshElement) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SMDS_ListNodeOfListOfMeshElement))) {
       _anOtherObject = Handle(SMDS_ListNodeOfListOfMeshElement)((Handle(SMDS_ListNodeOfListOfMeshElement)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SMDS_ListNodeOfListOfMeshElement::DynamicType() const 
{ 
  return STANDARD_TYPE(SMDS_ListNodeOfListOfMeshElement) ; 
}
Standard_Boolean SMDS_ListNodeOfListOfMeshElement::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SMDS_ListNodeOfListOfMeshElement) == AType || TCollection_MapNode::IsKind(AType)); 
}
Handle_SMDS_ListNodeOfListOfMeshElement::~Handle_SMDS_ListNodeOfListOfMeshElement() {}
#define Item Handle_SMDS_MeshElement
#define Item_hxx <SMDS_MeshElement.hxx>
#define TCollection_ListNode SMDS_ListNodeOfListOfMeshElement
#define TCollection_ListNode_hxx <SMDS_ListNodeOfListOfMeshElement.hxx>
#define TCollection_ListIterator SMDS_ListIteratorOfListOfMeshElement
#define TCollection_ListIterator_hxx <SMDS_ListIteratorOfListOfMeshElement.hxx>
#define Handle_TCollection_ListNode Handle_SMDS_ListNodeOfListOfMeshElement
#define TCollection_ListNode_Type_() SMDS_ListNodeOfListOfMeshElement_Type_()
#define TCollection_List SMDS_ListOfMeshElement
#define TCollection_List_hxx <SMDS_ListOfMeshElement.hxx>
#include <TCollection_ListNode.gxx>

