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
SMESH_NoteBook::SMESH_NoteBook()
{
  InitObjectMap();
}

//================================================================================
/*!
 * \brief Constructor
 */
//================================================================================
SMESH_NoteBook::~SMESH_NoteBook()
{
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
    
    if(aMethod == "SetLength" && !(*it).second.at(0).IsEmpty() ) {
      aCmd.SetArg(1,(*it).second.at(0));
    }
    else if(aMethod == "SetPrecision" && !(*it).second.at(1).IsEmpty() ){
      aCmd.SetArg(1,(*it).second.at(1));
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
  TCollection_AsciiString aParameters;
  for(Itr->InitEx(true); Itr->More(); Itr->Next()) {
    SALOMEDS::SObject_var aSObject = Itr->Value();
    SALOMEDS::GenericAttribute_var anAttr;
    if ( aSObject->FindAttribute(anAttr, "AttributeString")) {
      aParameters = TCollection_AsciiString(SALOMEDS::AttributeString::_narrow(anAttr)->Value());
      vector<string> vect = ParseVariables(aParameters.ToCString(),':');
      if(MYDEBUG) {
        cout<<"Entry : "<< aSObject->GetID()<<endl;
        cout<<"aParameters : "<<aParameters<<endl;
      }
      vector<TCollection_AsciiString> aVars;
      for(int i = 0;i<vect.size();i++) {
        TCollection_AsciiString aVar(vect[i].c_str());
        if(!aVar.IsEmpty() && aStudy->IsVariable(vect[i].c_str())) {
          aVar.InsertBefore(1,"\"");
          aVar.InsertAfter(aVar.Length(),"\"");
        }
        aVars.push_back(aVar);
        if(MYDEBUG) {
          cout<<"Variable: "<<aVar<<endl;
        }
      }
      _objectMap.insert(pair<TCollection_AsciiString,vector<TCollection_AsciiString> >(TCollection_AsciiString(aSObject->GetID()),aVars));
    }
  }
}

//================================================================================
/*!
 * \brief Private method
 */
//================================================================================
vector<string> SMESH_NoteBook::ParseVariables(const string& theVariables, const char sep) const
{
  vector<string> aResult;
  if(theVariables[0] == sep ) aResult.push_back(string());
  int pos = theVariables.find(sep);
  if(pos < 0) {
    aResult.push_back(theVariables);
    return aResult;
  }

  string s = theVariables;
  if(s[0] == sep) s = s.substr(1, s.size());
  while((pos = s.find(sep)) >= 0) {
    aResult.push_back(s.substr(0, pos));
    s = s.substr(pos+1, s.size());
  }

  if(!s.empty() && s[0] != sep) aResult.push_back(s);
  if(theVariables[theVariables.size()-1] == sep) aResult.push_back(string());

  return aResult;
}
