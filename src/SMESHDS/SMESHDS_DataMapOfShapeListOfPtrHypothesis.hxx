//  SMESH SMESHDS : management of mesh data and SMESH document
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
//  File   : SMESHDS_DataMapOfShapeListOfPtrHypothesis.hxx
//  Module : SMESH

#ifndef _SMESHDS_DataMapOfShapeListOfPtrHypothesis_HeaderFile
#define _SMESHDS_DataMapOfShapeListOfPtrHypothesis_HeaderFile

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Handle_SMESHDS_DataMapNodeOfDataMapOfShapeListOfPtrHypothesis_HeaderFile
#include "Handle_SMESHDS_DataMapNodeOfDataMapOfShapeListOfPtrHypothesis.hxx"
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_DomainError;
class Standard_NoSuchObject;
class TopoDS_Shape;
class SMESHDS_ListOfPtrHypothesis;
class TopTools_ShapeMapHasher;
class SMESHDS_DataMapNodeOfDataMapOfShapeListOfPtrHypothesis;
class SMESHDS_DataMapIteratorOfDataMapOfShapeListOfPtrHypothesis;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SMESHDS_DataMapOfShapeListOfPtrHypothesis  : public TCollection_BasicMap {

public:

    inline void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    inline void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    inline void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
//    inline void  operator delete(void *anAddress, size_t size) 
//      { 
//        if (anAddress) Standard::Free((Standard_Address&)anAddress,size); 
//      }
 // Methods PUBLIC
 // 
Standard_EXPORT SMESHDS_DataMapOfShapeListOfPtrHypothesis(const Standard_Integer NbBuckets = 1);
Standard_EXPORT   SMESHDS_DataMapOfShapeListOfPtrHypothesis& Assign(const SMESHDS_DataMapOfShapeListOfPtrHypothesis& Other) ;
  SMESHDS_DataMapOfShapeListOfPtrHypothesis& operator =(const SMESHDS_DataMapOfShapeListOfPtrHypothesis& Other) 
{
  return Assign(Other);
}

Standard_EXPORT   void ReSize(const Standard_Integer NbBuckets) ;
Standard_EXPORT   void Clear() ;
~SMESHDS_DataMapOfShapeListOfPtrHypothesis()
{
  Clear();
}

Standard_EXPORT   Standard_Boolean Bind(const TopoDS_Shape& K,const SMESHDS_ListOfPtrHypothesis& I) ;
Standard_EXPORT   Standard_Boolean IsBound(const TopoDS_Shape& K) const;
Standard_EXPORT   Standard_Boolean UnBind(const TopoDS_Shape& K) ;
Standard_EXPORT  const SMESHDS_ListOfPtrHypothesis& Find(const TopoDS_Shape& K) const;
 const SMESHDS_ListOfPtrHypothesis& operator()(const TopoDS_Shape& K) const
{
  return Find(K);
}

Standard_EXPORT   SMESHDS_ListOfPtrHypothesis& ChangeFind(const TopoDS_Shape& K) ;
  SMESHDS_ListOfPtrHypothesis& operator()(const TopoDS_Shape& K) 
{
  return ChangeFind(K);
}






protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 
Standard_EXPORT SMESHDS_DataMapOfShapeListOfPtrHypothesis(const SMESHDS_DataMapOfShapeListOfPtrHypothesis& Other);


 // Fields PRIVATE
 //


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
