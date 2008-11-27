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
// File      : SMESH_NoteBook.hxx
// Author    : Roman NIKOLAEV ()


#ifndef SMESH_NoteBook_HeaderFile
#define SMESH_NoteBook_HeaderFile

#include <TCollection_AsciiString.hxx>
#include <Resource_DataMapOfAsciiStringAsciiString.hxx>

#include <vector>
#include <string>

typedef std::vector<TCollection_AsciiString>  TState;
typedef std::vector<TState>                   TAllStates;

class ObjectStates{
  
public:
  
  ObjectStates(TCollection_AsciiString theType);
  ~ObjectStates();

  void AddState(const TState &theState);

  TState GetCurrectState() const;
  TAllStates GetAllStates() const;
  void IncrementState();
  TCollection_AsciiString GetObjectType() const;

  

private:
  TCollection_AsciiString                   _type;
  TAllStates                                _states;
  int                                       _dumpstate;
};

class SMESH_NoteBook
{
public:
  SMESH_NoteBook(Handle(_pyGen) theGen);
  ~SMESH_NoteBook();
  TCollection_AsciiString ReplaceVariables(const TCollection_AsciiString& theString) const;

  typedef std::map<TCollection_AsciiString,ObjectStates*> TVariablesMap;

private:
  void InitObjectMap();
  
private:
  TVariablesMap _objectMap;
  Handle(_pyGen) myGen;
};

#endif //SMESH_NoteBook_HeaderFile
