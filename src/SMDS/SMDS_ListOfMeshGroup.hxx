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
//  File   : SMDS_ListOfMeshGroup.hxx
//  Module : SMESH

#ifndef _SMDS_ListOfMeshGroup_HeaderFile
#define _SMDS_ListOfMeshGroup_HeaderFile

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Handle_SMDS_MeshGroup_HeaderFile
#include "Handle_SMDS_MeshGroup.hxx"
#endif
#ifndef _Handle_SMDS_ListNodeOfListOfMeshGroup_HeaderFile
#include "Handle_SMDS_ListNodeOfListOfMeshGroup.hxx"
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoSuchObject;
class SMDS_ListIteratorOfListOfMeshGroup;
class SMDS_MeshGroup;
class SMDS_ListNodeOfListOfMeshGroup;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SMDS_ListOfMeshGroup  {

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
Standard_EXPORT SMDS_ListOfMeshGroup();
Standard_EXPORT   void Assign(const SMDS_ListOfMeshGroup& Other) ;
  void operator=(const SMDS_ListOfMeshGroup& Other) 
{
  Assign(Other);
}

Standard_EXPORT   Standard_Integer Extent() const;
Standard_EXPORT   void Clear() ;
~SMDS_ListOfMeshGroup()
{
  Clear();
}

Standard_EXPORT inline   Standard_Boolean IsEmpty() const;
Standard_EXPORT   void Prepend(const Handle(SMDS_MeshGroup)& I) ;
Standard_EXPORT   void Prepend(SMDS_ListOfMeshGroup& Other) ;
Standard_EXPORT   void Append(const Handle(SMDS_MeshGroup)& I) ;
Standard_EXPORT   void Append(SMDS_ListOfMeshGroup& Other) ;
Standard_EXPORT   Handle_SMDS_MeshGroup& First() const;
Standard_EXPORT   Handle_SMDS_MeshGroup& Last() const;
Standard_EXPORT   void RemoveFirst() ;
Standard_EXPORT   void Remove(SMDS_ListIteratorOfListOfMeshGroup& It) ;
Standard_EXPORT   void InsertBefore(const Handle(SMDS_MeshGroup)& I,SMDS_ListIteratorOfListOfMeshGroup& It) ;
Standard_EXPORT   void InsertBefore(SMDS_ListOfMeshGroup& Other,SMDS_ListIteratorOfListOfMeshGroup& It) ;
Standard_EXPORT   void InsertAfter(const Handle(SMDS_MeshGroup)& I,SMDS_ListIteratorOfListOfMeshGroup& It) ;
Standard_EXPORT   void InsertAfter(SMDS_ListOfMeshGroup& Other,SMDS_ListIteratorOfListOfMeshGroup& It) ;


friend class SMDS_ListIteratorOfListOfMeshGroup;



protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 
Standard_EXPORT SMDS_ListOfMeshGroup(const SMDS_ListOfMeshGroup& Other);


 // Fields PRIVATE
 //
Standard_Address myFirst;
Standard_Address myLast;


};

#define Item Handle_SMDS_MeshGroup
#define Item_hxx "SMDS_MeshGroup.hxx"
#define TCollection_ListNode SMDS_ListNodeOfListOfMeshGroup
#define TCollection_ListNode_hxx "SMDS_ListNodeOfListOfMeshGroup.hxx"
#define TCollection_ListIterator SMDS_ListIteratorOfListOfMeshGroup
#define TCollection_ListIterator_hxx "SMDS_ListIteratorOfListOfMeshGroup.hxx"
#define Handle_TCollection_ListNode Handle_SMDS_ListNodeOfListOfMeshGroup
#define TCollection_ListNode_Type_() SMDS_ListNodeOfListOfMeshGroup_Type_()
#define TCollection_List SMDS_ListOfMeshGroup
#define TCollection_List_hxx "SMDS_ListOfMeshGroup.hxx"

#include <TCollection_List.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx


// other inline functions and methods (like "C++: function call" methods)
//


#endif
