// Copyright (C) 2008  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// File      : SMESH_NoteBook.cxx
// Author    : Roman NIKOLAEV

#include "SMESH_2smeshpy.hxx"
#include "SMESH_NoteBook.hxx"
#include "SMESH_Gen_i.hxx"

#include <Resource_DataMapOfAsciiStringAsciiString.hxx>
#include <TColStd_SequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>

#include <vector>
#include <string>

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

using namespace std;



//================================================================================
/*!
 * \brief Constructor
 */
//================================================================================
ObjectStates::ObjectStates(TCollection_AsciiString theType)
{
  _type = theType;
  _dumpstate = 0;
}

//================================================================================
/*!
 * \brief Destructor
 */
//================================================================================
ObjectStates::~ObjectStates()
{
}

//================================================================================
/*!
 * \brief Add new object state 
 * \param theState - Object state (vector of notebook variable)
 */
//================================================================================
void ObjectStates::AddState(const TState &theState)
{
  _states.push_back(theState);
}

//================================================================================
/*!
 * \brief Return current object state
 * \\retval state - Object state (vector of notebook variable)
 */
//================================================================================
TState ObjectStates::GetCurrectState() const
{
  return _states[_dumpstate];
}


//================================================================================
/*!
 *
 */
//================================================================================
TAllStates ObjectStates::GetAllStates() const
{
  return _states;
}

//================================================================================
/*!
 *
 */
//================================================================================
void ObjectStates::IncrementState()
{
  _dumpstate++;
}

//================================================================================
/*!
 *
 */
//================================================================================
TCollection_AsciiString ObjectStates::GetObjectType() const{
  return _type;
}

//================================================================================
/*!
 * \brief Constructor
 */
//================================================================================
SMESH_NoteBook::SMESH_NoteBook()
{
  InitObjectMap();
}

//================================================================================
/*!
 * \brief Destructor
 */
//================================================================================
SMESH_NoteBook::~SMESH_NoteBook()
{
  TVariablesMap::const_iterator it = _objectMap.begin();
  for(;it!=_objectMap.end();it++) {
    if((*it).second)
      delete (*it).second;
  }
}

//================================================================================
/*!
 * \brief Replace parameters of the functions on the Salome NoteBook Variables
 * \param theString - Input string
 * \retval TCollection_AsciiString - Convertion result
 */
//================================================================================
TCollection_AsciiString SMESH_NoteBook::ReplaceVariables(const TCollection_AsciiString& theString) const
{
  _pyCommand aCmd( theString, -1);
  TCollection_AsciiString aMethod = aCmd.GetMethod();
  TCollection_AsciiString aObject = aCmd.GetObject();
  TVariablesMap::const_iterator it = _objectMap.find(aObject);
  if(!aMethod.IsEmpty() && it != _objectMap.end() ) {
    ObjectStates *aStates = (*it).second;
    bool modified = false;
    if(MYDEBUG)
      cout<<"SMESH_NoteBook::ReplaceVariables :Object Type : "<<aStates->GetObjectType()<<endl;
    if(aStates->GetObjectType().IsEqual("LocalLength")) {
      if(aMethod == "SetLength") {
        if(!aStates->GetCurrectState().at(0).IsEmpty() )
          aCmd.SetArg(1,aStates->GetCurrectState().at(0));
        aStates->IncrementState();
      }
      else if(aMethod == "SetPrecision") {
        if(!aStates->GetCurrectState().at(1).IsEmpty() )
          aCmd.SetArg(1,aStates->GetCurrectState().at(1));
        aStates->IncrementState();
      }
    }
    return aCmd.GetString();
  }
  
  return theString;
}
//================================================================================
/*!
 * \brief Private method
 */
//================================================================================
void SMESH_NoteBook::InitObjectMap()
{
  SMESH_Gen_i *aGen = SMESH_Gen_i::GetSMESHGen();
  if(!aGen)
    return;
  
  SALOMEDS::Study_ptr aStudy = aGen->GetCurrentStudy();
  if(aStudy->_is_nil())
    return;
  
  SALOMEDS::SObject_var aSO = aStudy->FindComponent(aGen->ComponentDataType());
  if(CORBA::is_nil(aSO))
    return;
  
  SALOMEDS::ChildIterator_var Itr = aStudy->NewChildIterator(aSO);
  char* aParameters;
  for(Itr->InitEx(true); Itr->More(); Itr->Next()) {
    SALOMEDS::SObject_var aSObject = Itr->Value();
    SALOMEDS::GenericAttribute_var anAttr;
    if ( aSObject->FindAttribute(anAttr, "AttributeString")) {
      aParameters = SALOMEDS::AttributeString::_narrow(anAttr)->Value();
      SALOMEDS::ListOfListOfStrings_var aSections = aStudy->ParseVariables(aParameters);
      if(MYDEBUG) {
        cout<<"Entry : "<< aSObject->GetID()<<endl;
        cout<<"aParameters : "<<aParameters<<endl;
      }      
      TCollection_AsciiString anObjType;
      CORBA::Object_var anObject = SMESH_Gen_i::SObjectToObject(aSObject);
      SMESH::SMESH_Hypothesis_var aHyp = SMESH::SMESH_Hypothesis::_narrow(anObject);
      if(!aHyp->_is_nil()) {
        anObjType = TCollection_AsciiString(aHyp->GetName());
      }
      else if(SMESH::SMESH_Mesh_var aMesh = SMESH::SMESH_Mesh::_narrow(anObject)) {
        anObjType = TCollection_AsciiString("Mesh");
      }
      if(MYDEBUG)
        cout<<"The object Type : "<<anObjType<<endl;
      
      ObjectStates *aState = new  ObjectStates(anObjType);
      for(int i = 0; i < aSections->length(); i++) {
        TState aVars;
        SALOMEDS::ListOfStrings aListOfVars = aSections[i];
        for(int j = 0;j<aListOfVars.length();j++) {
          TCollection_AsciiString aVar(aListOfVars[j].in());
          if(!aVar.IsEmpty() && aStudy->IsVariable(aVar.ToCString())) {
            aVar.InsertBefore(1,"\"");
            aVar.InsertAfter(aVar.Length(),"\"");
          }
          aVars.push_back(aVar);
          if(MYDEBUG) {
            cout<<"Variable: '"<<aVar<<"'"<<endl;
          }
        }
        aState->AddState(aVars);
      }
      _objectMap.insert(pair<TCollection_AsciiString,ObjectStates*>(TCollection_AsciiString(aSObject->GetID()),aState));
    }
  }
}
