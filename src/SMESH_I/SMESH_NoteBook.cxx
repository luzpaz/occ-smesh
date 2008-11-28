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
#include "SMESH_PythonDump.hxx"

#include <Resource_DataMapOfAsciiStringAsciiString.hxx>
#include <TColStd_SequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>

#include <vector>
#include <string>

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 1;
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
LayerDistributionStates::LayerDistributionStates():
  ObjectStates("LayerDistribution")
{
}
//================================================================================
/*!
 * \brief Destructor
 */
//================================================================================
LayerDistributionStates::~LayerDistributionStates()
{
}


//================================================================================
/*!
 * \brief AddDistribution
 */
//================================================================================
void LayerDistributionStates::AddDistribution(const TCollection_AsciiString& theDistribution)
{
  _distributions.insert(pair<TCollection_AsciiString,TCollection_AsciiString>(theDistribution,""));
}

//================================================================================
/*!
 * \brief HasDistribution
 */
//================================================================================
bool LayerDistributionStates::HasDistribution(const TCollection_AsciiString& theDistribution) const
{
  return _distributions.find(theDistribution) != _distributions.end();
}

//================================================================================
/*!
 * \brief SetDistributionType
 */
//================================================================================
bool LayerDistributionStates::SetDistributionType(const TCollection_AsciiString& theDistribution,
                                                  const TCollection_AsciiString& theType)
{
  TDistributionMap::iterator it = _distributions.find(theDistribution);
  if(it == _distributions.end())
    return false;
  (*it).second = theType;
  return true;
}

//================================================================================
/*!
 * \brief GetDistributionType
 */
//================================================================================
TCollection_AsciiString LayerDistributionStates::
GetDistributionType(const TCollection_AsciiString& theDistribution) const
{
  TDistributionMap::const_iterator it = _distributions.find(theDistribution);
  return (it == _distributions.end()) ? TCollection_AsciiString() : (*it).second;
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
void SMESH_NoteBook::ReplaceVariables()
{

  for(int i=0;i<_commands.size();i++) {
    Handle(_pyCommand) aCmd = _commands[i];
    TCollection_AsciiString aMethod = aCmd->GetMethod();
    TCollection_AsciiString aObject = aCmd->GetObject();
    TCollection_AsciiString aResultValue = aCmd->GetResultValue();
    if(MYDEBUG) {
      cout<<"Command before : "<< aCmd->GetString()<<endl;
      cout<<"Method : "<< aMethod<<endl;
      cout<<"Object : "<< aObject<<endl;
      cout<<"Result : "<< aResultValue<<endl;
    }
    
    // check if method modifies the object itself
    TVariablesMap::const_iterator it = _objectMap.find(aObject);
    if(it == _objectMap.end()) // check if method returns a new object
      it = _objectMap.find(aResultValue);
    
    if(it == _objectMap.end()) { // check if method modifies a mesh using mesh editor
      TMeshEditorMap::const_iterator meIt = myMeshEditors.find(aObject);
      if(meIt != myMeshEditors.end()) {
        TCollection_AsciiString aMesh = (*meIt).second;
	it = _objectMap.find(aMesh);
      }
    }
    
    if(it != _objectMap.end() && !aMethod.IsEmpty()) {
      ObjectStates *aStates = (*it).second;
      // Case for LocalLength hypothesis
      if(aStates->GetObjectType().IsEqual("LocalLength")) {
        if(aMethod.IsEqual("SetLength")) {
          if(!aStates->GetCurrectState().at(0).IsEmpty() )
            aCmd->SetArg(1,aStates->GetCurrectState().at(0));
          aStates->IncrementState();
        }
        else if(aMethod.IsEqual("SetPrecision")) {
          if(!aStates->GetCurrectState().at(1).IsEmpty() )
            aCmd->SetArg(1,aStates->GetCurrectState().at(1));
          aStates->IncrementState();
        }
      }
      
      // Case for SegmentLengthAroundVertex hypothesis
      else if(aStates->GetObjectType().IsEqual("SegmentLengthAroundVertex")) {
        if(aMethod == "SetLength") {
          if(!aStates->GetCurrectState().at(0).IsEmpty() )
            aCmd->SetArg(1,aStates->GetCurrectState().at(0));
          aStates->IncrementState();
        }
      }
      // Case for LayerDistribution hypothesis (not finished yet)
      else if(aStates->GetObjectType() == "LayerDistribution") {
        if(aMethod == "SetLayerDistribution"){
          LayerDistributionStates* aLDStates = (LayerDistributionStates*)(aStates);
          aLDStates->AddDistribution(aCmd->GetArg(1));
        }
      }

      else if(aStates->GetObjectType().IsEqual("Mesh")) {
        if(aMethod.IsEqual("Translate") ||
           aMethod.IsEqual("TranslateMakeGroups") ||
           aMethod.IsEqual("TranslateMakeMesh")) {
          bool isVariableFound = false;
          int anArgIndex = 0;
          for(int i = 1, n = aCmd->GetNbArgs(); i <= n; i++) {
            if(aCmd->GetArg(i).IsEqual("SMESH.PointStruct")) {
              anArgIndex = i+1;
              break;
            }
          }
          if(anArgIndex > 0) {
            for(int j = 0; j <= 2; j++) {
              if(!aStates->GetCurrectState().at(j).IsEmpty()) {
                isVariableFound = true;
                aCmd->SetArg(anArgIndex+j, aStates->GetCurrectState().at(j));
              }
            }
          }
          if(isVariableFound) {
            aCmd->SetArg(anArgIndex - 1, TCollection_AsciiString(SMESH_2smeshpy::SmeshpyName())+".PointStructStr");
            aCmd->SetArg(anArgIndex - 2, TCollection_AsciiString(SMESH_2smeshpy::SmeshpyName())+".DirStructStr");
          }
          aStates->IncrementState();
        }
      }
    }
    if(MYDEBUG) {
      cout<<"Command after: "<< aCmd->GetString()<<endl;
    }
  }
  //  ProcessLayerDistribution();
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
      ObjectStates *aState = NULL;
      if(anObjType == "LayerDistribution")
        aState = new LayerDistributionStates();
      else
        aState = new  ObjectStates(anObjType);
      
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

//================================================================================
/*!
 * 
 */
//================================================================================
void SMESH_NoteBook::AddCommand(const TCollection_AsciiString& theString)
{
  Handle(_pyCommand) aCommand = new _pyCommand( theString, -1);
  _commands.push_back(aCommand);

  if ( aCommand->GetMethod() == "GetMeshEditor" ) { // MeshEditor creation
    myMeshEditors.insert( make_pair( aCommand->GetResultValue(),
				     aCommand->GetObject() ) );
  }
}

//================================================================================
/*!
 * 
 */
//================================================================================
void SMESH_NoteBook::ProcessLayerDistribution()
{
  // 1) Find all LayerDistribution states
  vector<LayerDistributionStates*> aLDS;
  TVariablesMap::const_iterator it = _objectMap.begin();
  for(;it != _objectMap.end();it++)
    if(LayerDistributionStates* aLDStates = (LayerDistributionStates*)((*it).second)) {
      aLDS.push_back(aLDStates);
    }
  
  // 2) Initialize all type of 1D Distribution hypothesis
  for(int i=0;i<_commands.size();i++){
    for(int j =0;j < aLDS.size();j++){
      TCollection_AsciiString aResultValue = _commands[i]->GetResultValue();
      if(_commands[i]->GetMethod() == "CreateHypothesis" &&
         aLDS[j]->HasDistribution(aResultValue)){
        TCollection_AsciiString aType = _commands[i]->GetArg(1);
        aType.RemoveAll('\'');
        aLDS[j]->SetDistributionType(aResultValue,aType);
      }
    }
  }
  // 3) ... and replase variables ...

  for(int i=0;i<_commands.size();i++){
    for(int j =0;j < aLDS.size();j++){
      TCollection_AsciiString anObject = _commands[i]->GetObject();

      if(aLDS[j]->HasDistribution(anObject)) {
        TCollection_AsciiString aType = aLDS[j]->GetDistributionType(anObject);
        TCollection_AsciiString aMethod = _commands[i]->GetMethod();
        if(aType == "LocalLength") {
          if(aMethod == "SetLength") {
            if(!aLDS[j]->GetCurrectState().at(0).IsEmpty() )
              _commands[i]->SetArg(1,aLDS[j]->GetCurrectState().at(0));
            aLDS[j]->IncrementState();
          }
          else if(aMethod == "SetPrecision") {
            if(!aLDS[j]->GetCurrectState().at(1).IsEmpty() )
              _commands[i]->SetArg(1,aLDS[j]->GetCurrectState().at(1));
            aLDS[j]->IncrementState();
          }
        }
      }
    }
  }
}
//================================================================================
/*!
 *  \brief Return result script
 */
//================================================================================
TCollection_AsciiString SMESH_NoteBook::GetResultScript() const
{
  TCollection_AsciiString aResult;
  for(int i=0;i<_commands.size();i++)
    aResult+=_commands[i]->GetString()+"\n";
  return aResult;
}
