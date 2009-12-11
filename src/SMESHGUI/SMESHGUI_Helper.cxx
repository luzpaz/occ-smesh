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
// SMESH SMESHGUI : GUI for SMESH component
// File   : SMESHGUI_Helper.cxx
// Author : Oleg UVAROV
// SMESH includes
//
#include "SMESHGUI_Helper.h"
#include "SMESHGUI.h"

// SALOME GUI includes
#include <SUIT_Desktop.h>
#include <SUIT_MessageBox.h>
#include <SUIT_Session.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_DoubleSpinBox.h>
#include <SalomeApp_IntSpinBox.h>
#include <SalomeApp_Notebook.h>

//=================================================================================
// name    : SMESHGUI_Helper::SMESHGUI_Helper
// Purpose :
//=================================================================================
SMESHGUI_Helper::SMESHGUI_Helper( SMESHGUI* theModule ) :
  mySMESHGUI( theModule )
{
  myNotebook = new SalomeApp_Notebook( mySMESHGUI->activeStudy() );
}

//=======================================================================
// name    : SMESHGUI_Helper::~SMESHGUI_Helper
// Purpose :
//=======================================================================
SMESHGUI_Helper::~SMESHGUI_Helper()
{
  if( myNotebook )
  {
    delete myNotebook;
    myNotebook = 0;
  }
}

//================================================================
// Function : checkParameters
// Purpose  :
//================================================================
bool SMESHGUI_Helper::checkParameters( bool theMess, int theCount, QAbstractSpinBox* theFirstSpinBox, ... )
{
  va_list aSpins;
  va_start( aSpins, theFirstSpinBox );

  int aCounter = 0;
  QList<QAbstractSpinBox*> aSpinBoxList;
  QAbstractSpinBox* aSpinBox = theFirstSpinBox;
  while( aSpinBox && aCounter < theCount )
  {
    aSpinBoxList.append( aSpinBox );
    aSpinBox = va_arg( aSpins, QAbstractSpinBox* );
    aCounter++;
  }
  return checkParameters( theMess, aSpinBoxList );
}

//================================================================
// Function : checkParameters
// Purpose  :
//================================================================
bool SMESHGUI_Helper::checkParameters( bool theMess, const QList<QAbstractSpinBox*>& theSpinBoxList )
{
  SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>( SUIT_Session::session()->activeApplication() );
  if ( !app )
    return false;

  QString msg;
  QStringList absentParams;

  bool ok = true;
  QListIterator<QAbstractSpinBox*> anIter( theSpinBoxList );
  while( anIter.hasNext() )
  {
    QAbstractSpinBox* aSpinBox = anIter.next();
    if( SalomeApp_DoubleSpinBox* aDoubleSpinBox = dynamic_cast<SalomeApp_DoubleSpinBox*>( aSpinBox ) )
      ok = aDoubleSpinBox->isValid( msg, absentParams, theMess ) && ok;
    else if( SalomeApp_IntSpinBox* anIntSpinBox = dynamic_cast<SalomeApp_IntSpinBox*>( aSpinBox ) )
      ok = anIntSpinBox->isValid( msg, absentParams, theMess ) && ok;
  }

  if( !ok && theMess )
  {
    if( !absentParams.isEmpty() )
      app->defineAbsentParameters( absentParams );
    else
    {
      QString str( QObject::tr( "SMESH_INCORRECT_INPUT" ) );
      if( !msg.isEmpty() )
        str += "\n" + msg;
      SUIT_MessageBox::critical( mySMESHGUI->desktop(), QObject::tr( "SMESH_ERROR" ), str );
    }
  }
  return ok;
}

//================================================================
// Function : getNotebook
// Purpose  :
//================================================================
SalomeApp_Notebook* SMESHGUI_Helper::getNotebook() const
{
  return myNotebook;
}
