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


#include "QAD_Desktop.h"

#include "SMESHGUI_GEOMGenUtils.h"
#include "SMESHGUI_Utils.h"

namespace SMESH{

  GEOM::GEOM_Gen_var GetGEOMGen()
  {
    static GEOM::GEOM_Gen_var aGEOMGen;
    if(CORBA::is_nil(aGEOMGen)){
      if(QAD_Desktop* aDesktop = QAD_Application::getDesktop()){
	Engines::Component_var aComponent = aDesktop->getEngine("FactoryServer","GEOM");
	aGEOMGen = GEOM::GEOM_Gen::_narrow(aComponent);
      }
    }
    return aGEOMGen;
  }


  GEOM::GEOM_Object_var GetShapeOnMeshOrSubMesh(SALOMEDS::SObject_ptr theSObject)
  {
    if(!theSObject->_is_nil()) {
      using namespace SALOMEDS;
      SObject_var aSubSObject;
      static int Tag_RefOnShape = 1;
      if(theSObject->FindSubObject(Tag_RefOnShape,aSubSObject)){
	SObject_var aGeomSObject;
	if(aSubSObject->ReferencedObject(aGeomSObject)){
	  return SObjectToInterface<GEOM::GEOM_Object>(aGeomSObject);
	}
      }
    }
    return GEOM::GEOM_Object::_nil();
  }
    
}
