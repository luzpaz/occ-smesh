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
// File   : SMESHGUI_Helper.h
// Author : Oleg UVAROV
//
#ifndef SMESHGUI_HELPER_H
#define SMESHGUI_HELPER_H

// SMESH includes
#include "SMESH_SMESHGUI.hxx"

// Qt includes
#include <QList>

class SMESHGUI;
class SalomeApp_Notebook;
class QAbstractSpinBox;

//=================================================================================
// class    : SMESHGUI_Helper
// purpose  : Helper class for dialog box development, can be used as the second
//            base class for dialog box (or for corresponding operation, if exists).
//            Contains methods performing operations with SALOME Notebook parameters.
//=================================================================================
class SMESHGUI_EXPORT SMESHGUI_Helper
{ 
public:
  SMESHGUI_Helper( SMESHGUI* );
  virtual ~SMESHGUI_Helper();

protected:
  bool                checkParameters( bool, int, QAbstractSpinBox*, ... );
  bool                checkParameters( bool, const QList<QAbstractSpinBox*>& );

  SalomeApp_Notebook* getNotebook() const;

private:
  SMESHGUI*           mySMESHGUI;
  SalomeApp_Notebook* myNotebook;
};

#endif // SMESHGUI_HELPER_H
