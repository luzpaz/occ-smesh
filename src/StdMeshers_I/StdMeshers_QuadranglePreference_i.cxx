//  SMESH SMESH_I : idl implementation based on 'SMESH' unit's calsses
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : StdMeshers_QuadranglePreference_i.cxx
//           Moved here from SMESH_LocalLength_i.cxx
//  Author : Paul RASCLE, EDF
//  Module : SMESH
//  $Header$

#include "StdMeshers_QuadranglePreference_i.hxx"
#include "SMESH_Gen_i.hxx"
#include "SMESH_Gen.hxx"

#include "Utils_CorbaException.hxx"
#include "utilities.h"

#include <TCollection_AsciiString.hxx>

using namespace std;

//=============================================================================
/*!
 *  StdMeshers_QuadranglePreference_i::StdMeshers_QuadranglePreference_i
 *
 *  Constructor
 */
//=============================================================================

StdMeshers_QuadranglePreference_i::StdMeshers_QuadranglePreference_i
( PortableServer::POA_ptr thePOA,
  int                     theStudyId,
  ::SMESH_Gen*            theGenImpl ): SALOME::GenericObj_i( thePOA ), 
                                        SMESH_Hypothesis_i( thePOA )
{
  myBaseImpl = new ::StdMeshers_QuadranglePreference( theGenImpl->GetANewId(),
                                                      theStudyId,
                                                      theGenImpl );
}

//=============================================================================
/*!
 *  StdMeshers_QuadranglePreference_i::~StdMeshers_QuadranglePreference_i
 *
 *  Destructor
 */
//=============================================================================

StdMeshers_QuadranglePreference_i::~StdMeshers_QuadranglePreference_i()
{
}

//=============================================================================
/*!
 *  StdMeshers_QuadranglePreference_i::GetImpl
 *
 *  Get implementation
 */
//=============================================================================

::StdMeshers_QuadranglePreference* StdMeshers_QuadranglePreference_i::GetImpl()
{
  return ( ::StdMeshers_QuadranglePreference* )myBaseImpl;
}

//================================================================================
/*!
 * \brief Verify whether hypothesis supports given entity type 
  * \param type - dimension (see SMESH::Dimension enumeration)
  * \retval CORBA::Boolean - TRUE if dimension is supported, FALSE otherwise
 * 
 * Verify whether hypothesis supports given entity type (see SMESH::Dimension enumeration)
 */
//================================================================================  

CORBA::Boolean StdMeshers_QuadranglePreference_i::IsDimSupported( SMESH::Dimension type )
{
  return type == SMESH::DIM_2D;
}

