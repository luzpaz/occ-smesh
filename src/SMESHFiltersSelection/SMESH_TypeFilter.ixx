// File generated by CPPExt (Transient)
//                     Copyright (C) 1991,1995 by
//  
//                      MATRA DATAVISION, FRANCE
//  
// This software is furnished in accordance with the terms and conditions
// of the contract and with the inclusion of the above copyright notice.
// This software or any other copy thereof may not be provided or otherwise
// be made available to any other person. No title to an ownership of the
// software is hereby transferred.
//  
// At the termination of the contract, the software and all copies of this
// software must be deleted.
//
#include "SMESH_TypeFilter.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SMESH_TypeFilter::~SMESH_TypeFilter() {}
 


Standard_EXPORT Handle_Standard_Type& SMESH_TypeFilter_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(SALOME_Filter);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(SALOME_Filter);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SMESH_TypeFilter",
			                                 sizeof(SMESH_TypeFilter),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SMESH_TypeFilter) Handle(SMESH_TypeFilter)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SMESH_TypeFilter) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SMESH_TypeFilter))) {
       _anOtherObject = Handle(SMESH_TypeFilter)((Handle(SMESH_TypeFilter)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SMESH_TypeFilter::DynamicType() const 
{ 
  return STANDARD_TYPE(SMESH_TypeFilter) ; 
}
Standard_Boolean SMESH_TypeFilter::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SMESH_TypeFilter) == AType || SALOME_Filter::IsKind(AType)); 
}
Handle_SMESH_TypeFilter::~Handle_SMESH_TypeFilter() {}

