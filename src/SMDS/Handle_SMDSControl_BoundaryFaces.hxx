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
//  File   : Handle_SMDSControl_BoundaryFaces.hxx
//  Module : SMESH

#ifndef _Handle_SMDSControl_BoundaryFaces_HeaderFile
#define _Handle_SMDSControl_BoundaryFaces_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_SMDSControl_MeshBoundary_HeaderFile
#include "Handle_SMDSControl_MeshBoundary.hxx"
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(SMDSControl_MeshBoundary);
class SMDSControl_BoundaryFaces;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SMDSControl_BoundaryFaces);

class Handle(SMDSControl_BoundaryFaces) : public Handle(SMDSControl_MeshBoundary) {
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
    Handle(SMDSControl_BoundaryFaces)():Handle(SMDSControl_MeshBoundary)() {} 
    Handle(SMDSControl_BoundaryFaces)(const Handle(SMDSControl_BoundaryFaces)& aHandle) : Handle(SMDSControl_MeshBoundary)(aHandle) 
     {
     }

    Handle(SMDSControl_BoundaryFaces)(const SMDSControl_BoundaryFaces* anItem) : Handle(SMDSControl_MeshBoundary)((SMDSControl_MeshBoundary *)anItem) 
     {
     }

    Handle(SMDSControl_BoundaryFaces)& operator=(const Handle(SMDSControl_BoundaryFaces)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SMDSControl_BoundaryFaces)& operator=(const SMDSControl_BoundaryFaces* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SMDSControl_BoundaryFaces* operator->() 
     {
      return (SMDSControl_BoundaryFaces *)ControlAccess();
     }

    SMDSControl_BoundaryFaces* operator->() const 
     {
      return (SMDSControl_BoundaryFaces *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SMDSControl_BoundaryFaces)();
 
   Standard_EXPORT static const Handle(SMDSControl_BoundaryFaces) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
