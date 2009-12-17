//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  SMESH SMESH_I : idl implementation based on 'SMESH' unit's calsses
//  File   : SMESH_Hypothesis_i.cxx
//  Author : Paul RASCLE, EDF
//  Module : SMESH
//  $Header$
//
#include <iostream>
#include <sstream>
#include "SMESH_Hypothesis_i.hxx"
#include "SMESH_Gen_i.hxx"
#include "utilities.h"

using namespace std;

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::SMESH_Hypothesis_i
 * 
 *  Constructor
 */
//=============================================================================

SMESH_Hypothesis_i::SMESH_Hypothesis_i( PortableServer::POA_ptr thePOA )
     : SALOME::GenericObj_i( thePOA )
{
  MESSAGE( "SMESH_Hypothesis_i::SMESH_Hypothesis_i / Début" );
  myBaseImpl = 0;
  
  MESSAGE( "SMESH_Hypothesis_i::SMESH_Hypothesis_i / Fin" );
};

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::~SMESH_Hypothesis_i
 *
 *  Destructor
 */
//=============================================================================

SMESH_Hypothesis_i::~SMESH_Hypothesis_i()
{
  MESSAGE( "SMESH_Hypothesis_i::~SMESH_Hypothesis_i" );
  if ( myBaseImpl )
    delete myBaseImpl;
};

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetName
 *
 *  Get type name of hypothesis
 */
//=============================================================================

char* SMESH_Hypothesis_i::GetName()
{
  //MESSAGE( "SMESH_Hypothesis_i::GetName" );
  return CORBA::string_dup( myBaseImpl->GetName() );
};

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetLibName
 *
 *  Get plugin library name of hypothesis (required by persistency mechanism)
 */
//=============================================================================

char* SMESH_Hypothesis_i::GetLibName()
{
  MESSAGE( "SMESH_Hypothesis_i::GetLibName" );
  return CORBA::string_dup( myBaseImpl->GetLibName() );
};

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::SetLibName
 *
 *  Set plugin library name of hypothesis (required by persistency mechanism)
 */
//=============================================================================

void SMESH_Hypothesis_i::SetLibName(const char* theLibName)
{
  MESSAGE( "SMESH_Hypothesis_i::SetLibName" );
  myBaseImpl->SetLibName( theLibName );
};

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetId
 *
 *  Get unique id of hypothesis
 */
//=============================================================================

CORBA::Long SMESH_Hypothesis_i::GetId()
{
  MESSAGE( "SMESH_Hypothesis_i::GetId" );
  return myBaseImpl->GetID();
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::IsPublished()
 *
 */
//=============================================================================
bool SMESH_Hypothesis_i::IsPublished(){
  bool res = false;
  SMESH_Gen_i *gen = SMESH_Gen_i::GetSMESHGen();
  if(gen){
    SALOMEDS::SObject_var SO = 
      SMESH_Gen_i::ObjectToSObject(gen->GetCurrentStudy() , SMESH::SMESH_Hypothesis::_narrow(_this()));
    res = !SO->_is_nil();
  }
  return res;
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetEntry()
 */
//=============================================================================
char* SMESH_Hypothesis_i::GetEntry()
{
  char aBuf[100];
  sprintf( aBuf, "%i", GetId() );
  return CORBA::string_dup( aBuf );
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetComponent()
 */
//=============================================================================
char* SMESH_Hypothesis_i::GetComponent()
{
  return CORBA::string_dup( "SMESH" );
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::IsValid()
 */
//=============================================================================
CORBA::Boolean SMESH_Hypothesis_i::IsValid()
{
  return true;
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::SetParameters()
 *
 */
//=============================================================================
void SMESH_Hypothesis_i::SetParameters( SALOME::Notebook_ptr theNotebook, const SALOME::StringArray& theParameters )
{
  theNotebook->ClearDependencies( _this(), SALOME::Parameters );
  std::list<std::string> aParams;
  int n = theParameters.length();
  for( int i=0; i<n; i++ )
  {
    std::string aParam = CORBA::string_dup( theParameters[i] );
    aParams.push_back( aParam );
    
    SALOME::Parameter_ptr aParamPtr = theNotebook->GetParameter( aParam.c_str() );
    if( !CORBA::is_nil( aParamPtr ) )
      theNotebook->AddDependency( _this(), aParamPtr );
  }
  myBaseImpl->SetParameters( aParams );

  UpdateStringAttribute();
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetParameters()
 *
 */
//=============================================================================
SALOME::StringArray* SMESH_Hypothesis_i::GetParameters()
{
  std::list<std::string> aParams = myBaseImpl->GetParameters();
  SALOME::StringArray_var aRes = new SALOME::StringArray();
  aRes->length( aParams.size() );
  std::list<std::string>::const_iterator it = aParams.begin(), last = aParams.end();
  for( int i=0; it!=last; it++, i++ )
    aRes[i] = CORBA::string_dup( it->c_str() );
  return aRes._retn();
}

//=============================================================================
/*!
 * SMESH_Hypothesis_i::Update()
 *
 */
//=============================================================================
void SMESH_Hypothesis_i::Update( SALOME::Notebook_ptr theNotebook )
{
}

//=============================================================================
/*!
 * SMESH_Hypothesis_i::UpdateStringAttribute()
 *
 */
//=============================================================================
void SMESH_Hypothesis_i::UpdateStringAttribute()
{
  // implementation of the method has been temporarily changed
  // previous implementation can be found in revision 1.12.14.8
  SMESH_Gen_i* aSMESHGen = SMESH_Gen_i::GetSMESHGen();

  SALOMEDS::Study_ptr aStudy = aSMESHGen->GetCurrentStudy();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::SObject_var aSObject = SMESH_Gen_i::ObjectToSObject( aStudy, SMESH::SMESH_Hypothesis::_narrow( _this() ) );
  if( CORBA::is_nil( aSObject ) )
    return;

  SALOME::StringArray* aParameters = GetParameters();

  SALOMEDS::GenericAttribute_var anAttr = aStudyBuilder->FindOrCreateAttribute( aSObject, "AttributeString" );
  SALOMEDS::AttributeString_var aStringAttrib = SALOMEDS::AttributeString::_narrow( anAttr );

  std::string aString;
  for( int i = 0, n = aParameters->length(); i < n; i++ ) {
    std::string aParameter = aParameters->operator[](i).in();
    aString += aParameter;
    if( i != n-1 )
      aString += ":";
  }

  aStringAttrib->SetValue( aString.c_str() );
  aStringAttrib->Destroy();
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::GetImpl
 *
 *  Get implementation
 */
//=============================================================================

::SMESH_Hypothesis* SMESH_Hypothesis_i::GetImpl()
{
  //MESSAGE( "SMESH_Hypothesis_i::GetImpl" );
  return myBaseImpl;
}

//=============================================================================
/*!
 *  SMESH_Hypothesis_i::SaveTo
 *
 *  Persistence: Dumps parameters to the string stream
 */
//=============================================================================

char* SMESH_Hypothesis_i::SaveTo()
{
  MESSAGE( "SMESH_Hypothesis_i::SaveTo" );
  std::ostringstream os;
  myBaseImpl->SaveTo( os );
  return CORBA::string_dup( os.str().c_str() );
}

//=============================================================================
/*!
*  SMESH_Hypothesis_i::LoadFrom
*
*  Persistence: Restores parameters from string
*/
//=============================================================================

void SMESH_Hypothesis_i::LoadFrom( const char* theStream )
{
  MESSAGE( "SMESH_Hypothesis_i::LoadFrom" );
  std::istringstream is( theStream );
  myBaseImpl->LoadFrom( is );
}
