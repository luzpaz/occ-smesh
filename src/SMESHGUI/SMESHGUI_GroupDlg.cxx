//  SMESH SMESHGUI : GUI for SMESH component
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
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SMESHGUI_GroupDlg.cxx
//  Author : Natalia KOPNOVA
//  Module : SMESH
//  $Header$

#include "SMESHGUI_GroupDlg.h"
#include "SMESHGUI_FilterDlg.h"
#include "SMESHGUI_Filter.h"

#include "SMESHGUI.h"
#include "SMESHGUI_Utils.h"
#include "SMESHGUI_VTKUtils.h"
#include "SMESHGUI_GroupUtils.h"
#include "SMESHGUI_FilterUtils.h"
#include "SMESHGUI_GEOMGenUtils.h"

#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"
#include "VTKViewer_ViewFrame.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_MessageBox.h"
#include "QAD_RightFrame.h"
#include "utilities.h"

#include "SMESH_Actor.h"

#include "GEOMBase.h"

// QT Includes
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmemarray.h>

// STL includes
#include <vector>
#include <algorithm>

using namespace std;

//=================================================================================
// class    : SMESHGUI_GroupDlg()
// purpose  : 
//=================================================================================
SMESHGUI_GroupDlg::SMESHGUI_GroupDlg( QWidget* parent, const char* name, SALOME_Selection* theSel,
				      SMESH::SMESH_Mesh_ptr theMesh, bool modal, WFlags fl )
  : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu | WDestructiveClose )
{
  if ( !name ) setName( "SMESHGUI_GroupDlg" );
  initDialog(theSel, true);
  init(theMesh);

  /* Move widget on the botton right corner of main widget */
  int x, y ;
  mySMESHGUI->DefineDlgPosition(this, x, y);
  this->move(x, y);
}

SMESHGUI_GroupDlg::SMESHGUI_GroupDlg( QWidget* parent, const char* name, SALOME_Selection* theSel,
				      SMESH::SMESH_Group_ptr theGroup, bool modal, WFlags fl )
  : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu | WDestructiveClose )
{
  if ( !name ) setName( "SMESHGUI_GroupDlg" );
  initDialog(theSel, false);
  init(theGroup);

  /* Move widget on the botton right corner of main widget */
  int x, y ;
  mySMESHGUI->DefineDlgPosition(this, x, y);
  this->move(x, y);
}

void SMESHGUI_GroupDlg::initDialog(SALOME_Selection* theSel, bool create)
{
  myFilterDlg = 0;
  
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap( "SMESH",tr("ICON_SELECT")));

  if (create)
    setCaption( tr( "SMESH_CREATE_GROUP_TITLE"  ) );
  else 
    setCaption( tr( "SMESH_EDIT_GROUP_TITLE"  ) );
  setSizeGripEnabled( TRUE );

  QVBoxLayout* aMainLayout = new QVBoxLayout(this, 11, 6);
  
  /***************************************************************/
  myTypeGroup = new QButtonGroup(1, Qt::Vertical, this, "Group types");
  myTypeGroup->setTitle(tr("SMESH_ELEMENTS_TYPE"));
  myTypeGroup->setExclusive(true);

  QStringList types;
  types.append(tr("MESH_NODE"));
  types.append(tr("SMESH_EDGE"));
  types.append(tr("SMESH_FACE"));
  types.append(tr("SMESH_VOLUME"));
  QRadioButton* rb;
  for (int i = 0; i < types.count(); i++) {
    rb = new QRadioButton(types[i], myTypeGroup);
  }
  myTypeGroup->setEnabled(create);
  myTypeId = -1;
    
  /***************************************************************/
  QHBox* aNameBox = new QHBox(this, "name box");
  QLabel* aName = new QLabel(aNameBox, "name label");
  aName->setText(tr("SMESH_NAME"));
  aName->setMinimumSize(50,0);
  myName = new QLineEdit(aNameBox, "name");
    
  /***************************************************************/
  QGroupBox* aContentBox = new QGroupBox(1, Qt::Horizontal, this, "content box");
  aContentBox->setTitle(tr("SMESH_CONTENT"));
  QFrame* aContent = new QFrame(aContentBox, "content");
  QGridLayout* aLayout = new QGridLayout(aContent, 7, 4);
  aLayout->setSpacing(6);
  aLayout->setAutoAdd(false);

  QLabel* aLabel = new QLabel(aContent, "elements label");
  aLabel->setText(tr("SMESH_ID_ELEMENTS"));
  myElements = new QListBox(aContent, "elements list");
  myElements->setSelectionMode(QListBox::Extended);
  //  myElements->setMinimumHeight(150);

  myFilter = new QPushButton(aContent, "filter");
  myFilter->setText(tr("SMESH_BUT_FILTER"));
  QPushButton* aAddBtn = new QPushButton(aContent, "add");
  aAddBtn->setText(tr("SMESH_BUT_ADD"));
  QPushButton* aRemoveBtn = new QPushButton(aContent, "remove");
  aRemoveBtn->setText(tr("SMESH_BUT_REMOVE"));
  QPushButton* aSortBtn = new QPushButton(aContent, "sort");
  aSortBtn->setText(tr("SMESH_BUT_SORT"));

  aLayout->addWidget(aLabel, 0, 0);
  aLayout->addMultiCellWidget(myElements, 1, 6, 0, 0);
  aLayout->addWidget(myFilter, 1, 2);
  aLayout->addWidget(aAddBtn, 3, 2);
  aLayout->addWidget(aRemoveBtn, 4, 2);
  aLayout->addWidget(aSortBtn, 6, 2);

  aLayout->setColStretch(0, 1);
  aLayout->addColSpacing(1, 20);
  aLayout->addColSpacing(3, 20);
  aLayout->setRowStretch(2, 1);
  aLayout->setRowStretch(5, 1);

  aContentBox->setMinimumHeight(aContent->sizeHint().height() + 
				aContentBox->sizeHint().height());

  /***************************************************************/
  QGroupBox* aSelectBox = new QGroupBox(3, Qt::Horizontal, this, "select box");
  aSelectBox->setTitle(tr("SMESH_SELECT_FROM"));
  
  mySelectSubMesh = new QCheckBox(aSelectBox, "submesh checkbox");
  mySelectSubMesh->setText(tr("SMESH_SUBMESH"));
  mySelectSubMesh->setMinimumSize(50, 0);
  mySubMeshBtn = new QPushButton(aSelectBox, "submesh button");
  mySubMeshBtn->setText("");
  mySubMeshBtn->setPixmap(image0);
  mySubMeshLine = new QLineEdit(aSelectBox, "submesh line");
  mySubMeshLine->setReadOnly(true);
  onSelectSubMesh(false);
  
  mySelectGroup = new QCheckBox(aSelectBox, "group checkbox");
  mySelectGroup->setText(tr("SMESH_GROUP"));
  mySelectGroup->setMinimumSize(50, 0);
  myGroupBtn = new QPushButton(aSelectBox, "group button");
  myGroupBtn->setText("");
  myGroupBtn->setPixmap(image0);
  myGroupLine = new QLineEdit(aSelectBox, "group line");
  myGroupLine->setReadOnly(true);
  onSelectGroup(false);
  
  mySelectGeomGroup = new QCheckBox(aSelectBox, "geometry group checkbox");
  mySelectGeomGroup->setText(tr("SMESH_GEOM_GROUP"));
  mySelectGeomGroup->setMinimumSize(50, 0);
  mySelectGeomGroup->setEnabled(create);
  myGeomGroupBtn = new QPushButton(aSelectBox, "geometry group button");
  myGeomGroupBtn->setText("");
  myGeomGroupBtn->setPixmap(image0);
  myGeomGroupLine = new QLineEdit(aSelectBox, "geometry group line");
  myGeomGroupLine->setReadOnly(true);
  onSelectGeomGroup(false);
  
  aSelectBox->setMinimumHeight(137);
  aSelectBox->setMinimumWidth(305);

  /***************************************************************/
  QFrame* aButtons = new QFrame(this, "button box");
  aButtons->setFrameStyle(QFrame::Box | QFrame::Sunken);
  QHBoxLayout* aBtnLayout = new QHBoxLayout(aButtons, 11, 6);
  aBtnLayout->setAutoAdd(false);

  QPushButton* aOKBtn = new QPushButton(aButtons, "ok");
  aOKBtn->setText(tr("SMESH_BUT_OK"));
  aOKBtn->setAutoDefault(true);
  aOKBtn->setDefault(true);
  QPushButton* aApplyBtn = new QPushButton(aButtons, "apply");
  aApplyBtn->setText(tr("SMESH_BUT_APPLY"));
  aApplyBtn->setAutoDefault(true);
  QPushButton* aCloseBtn = new QPushButton(aButtons, "close");
  aCloseBtn->setText(tr("SMESH_BUT_CLOSE"));
  aCloseBtn->setAutoDefault(true);

  aBtnLayout->addWidget(aOKBtn);
  aBtnLayout->addWidget(aApplyBtn);
  aBtnLayout->addStretch();
  aBtnLayout->addWidget(aCloseBtn);

  /***************************************************************/
  aMainLayout->addWidget(myTypeGroup);
  aMainLayout->addWidget(aNameBox);
  aMainLayout->addWidget(aContentBox);
  aMainLayout->addWidget(aSelectBox);
  aMainLayout->addWidget(aButtons);

  /* signals and slots connections */
  connect(myTypeGroup, SIGNAL(clicked(int)), this, SLOT(onTypeChanged(int)));

  connect(myName, SIGNAL(textChanged(const QString&)), this, SLOT(onNameChanged(const QString&)));
  connect(myElements, SIGNAL(selectionChanged()), this, SLOT(onListSelectionChanged()));

  connect(myFilter, SIGNAL(clicked()), this, SLOT(setFilters()));
  connect(aAddBtn, SIGNAL(clicked()), this, SLOT(onAdd()));
  connect(aRemoveBtn, SIGNAL(clicked()), this, SLOT(onRemove()));
  connect(aSortBtn, SIGNAL(clicked()), this, SLOT(onSort()));

  connect(mySelectSubMesh, SIGNAL(toggled(bool)), this, SLOT(onSelectSubMesh(bool)));
  connect(mySelectGroup, SIGNAL(toggled(bool)), this, SLOT(onSelectGroup(bool)));
  connect(mySelectGeomGroup, SIGNAL(toggled(bool)), this, SLOT(onSelectGeomGroup(bool)));
  connect(mySubMeshBtn, SIGNAL(clicked()), this, SLOT(setCurrentSelection()));
  connect(myGroupBtn, SIGNAL(clicked()), this, SLOT(setCurrentSelection()));
  connect(myGeomGroupBtn, SIGNAL(clicked()), this, SLOT(setCurrentSelection()));


  connect(aOKBtn, SIGNAL(clicked()), this, SLOT(onOK()));
  connect(aApplyBtn, SIGNAL(clicked()), this, SLOT(onApply()));
  connect(aCloseBtn, SIGNAL(clicked()), this, SLOT(onClose()));

  /* Init selection */
  mySelection = theSel;  
  mySMESHGUI = SMESHGUI::GetSMESHGUI();
  mySMESHGUI->SetActiveDialogBox(this);
  mySMESHGUI->SetState(800);

  mySelectionMode = -1;
  mySubMeshFilter = new SMESH_TypeFilter(SUBMESH);
  myGroupFilter = new SMESH_TypeFilter(GROUP);

  connect(mySMESHGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(onDeactivate()));
  connect(mySMESHGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(onClose()));
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(onObjectSelectionChanged()));

  updateButtons();
}

//=================================================================================
// function : ~SMESHGUI_GroupDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
SMESHGUI_GroupDlg::~SMESHGUI_GroupDlg()
{
    // no need to delete child widgets, Qt does it all for us
  if ( myFilterDlg != 0 )
  {
    myFilterDlg->reparent( 0, QPoint() );
    delete myFilterDlg;
  }
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::init(SMESH::SMESH_Mesh_ptr theMesh)
{
  /* init data from current selection */
  myMesh = SMESH::SMESH_Mesh::_duplicate(theMesh);
  myGroup = SMESH::SMESH_Group::_nil();

  myActor = SMESH::FindActorByObject(myMesh);
  SMESH::SetPickable(myActor);

  myTypeGroup->setButton(0);
  onTypeChanged(0);
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::init(SMESH::SMESH_Group_ptr theGroup)
{
  myMesh = theGroup->GetMesh();
  myGroup = SMESH::SMESH_Group::_duplicate(theGroup);
  
  myActor = SMESH::FindActorByObject(myMesh);
  if ( !myActor )
    myActor = SMESH::FindActorByObject(myGroup);
  SMESH::SetPickable(myActor);

  int aType = 0;
  switch(theGroup->GetType()) {
  case SMESH::NODE: aType= 0; break;
  case SMESH::EDGE: aType = 1; break;
  case SMESH::FACE: aType = 2; break;
  case SMESH::VOLUME: aType = 3; break;
  } 
  myTypeGroup->setButton(aType);
  onTypeChanged(aType);

  myName->setText(myGroup->GetName());
  myName->home(false);

  if (!theGroup->IsEmpty()) {
    SMESH::long_array_var anElements = myGroup->GetListOfID();
    int k = anElements->length();
    for (int i = 0; i < k; i++) {
      myIdList.append(anElements[i]);
      myElements->insertItem(QString::number(anElements[i]));
    }
    myElements->selectAll(true);
  }
}


//=================================================================================
// function : updateButtons()
// purpose  : 
//=================================================================================
void SMESHGUI_GroupDlg::updateButtons()
{
  bool enable = !myName->text().stripWhiteSpace().isEmpty() && myElements->count() > 0;
  QPushButton* aBtn;
  aBtn = (QPushButton*) child("ok", "QPushButton");
  if (aBtn) aBtn->setEnabled(enable);
  aBtn = (QPushButton*) child("apply", "QPushButton");
  if (aBtn) aBtn->setEnabled(enable);
}

//=================================================================================
// function : onNameChanged()
// purpose  : 
//=================================================================================
void SMESHGUI_GroupDlg::onNameChanged(const QString& text)
{
  updateButtons();
}

//=================================================================================
// function : onTypeChanged()
// purpose  : Radio button management
//=================================================================================
void SMESHGUI_GroupDlg::onTypeChanged(int id)
{
  if (myTypeId != id) {
    myElements->clear();
    if (myCurrentLineEdit == 0)
      setSelectionMode(id);
  }
  myTypeId = id;
}

//=================================================================================
// function : setSelectionMode()
// purpose  : Radio button management
//=================================================================================
void SMESHGUI_GroupDlg::setSelectionMode(int theMode)
{
  if (mySelectionMode != theMode) {
    mySelection->ClearIObjects();
    mySelection->ClearFilters();
    SMESH::SetPointRepresentation(false);
    if (theMode < 4) {
      switch(theMode){
      case 0:
        if ( myActor )
          myActor->SetPointRepresentation(true);
        else
          SMESH::SetPointRepresentation(true);
	QAD_Application::getDesktop()->SetSelectionMode(NodeSelection, true);
	break;
      case 1:
	QAD_Application::getDesktop()->SetSelectionMode(EdgeSelection, true);
	break;
      case 2:
	QAD_Application::getDesktop()->SetSelectionMode(FaceSelection, true);
	break;
      default:
	QAD_Application::getDesktop()->SetSelectionMode(VolumeSelection, true);
      }
    }
    else {
      QAD_Application::getDesktop()->SetSelectionMode(ActorSelection, true);
      if (theMode == 4)
	mySelection->AddFilter(mySubMeshFilter);
      else if (theMode == 5)
	mySelection->AddFilter(myGroupFilter);
    }
    mySelectionMode = theMode;
  }
} 

//=================================================================================
// function : onApply()
// purpose  :
//=================================================================================
bool SMESHGUI_GroupDlg::onApply()
{
  if (mySMESHGUI->ActiveStudyLocked())
    return false;
  if (!myName->text().stripWhiteSpace().isEmpty() && myElements->count() > 0) {
    mySelection->ClearIObjects();
    if (myGroup->_is_nil()) {
      SMESH::ElementType aType = SMESH::ALL;
      switch(myTypeId) {
      case 0: aType = SMESH::NODE; break;
      case 1: aType = SMESH::EDGE; break;
      case 2: aType = SMESH::FACE; break;
      case 3: aType = SMESH::VOLUME; break;
      }
      SMESH::long_array_var anIdList = new SMESH::long_array;
      int i, k = myElements->count();
      anIdList->length(k);
      QListBoxItem* anItem;
      for (i = 0, anItem = myElements->firstItem(); anItem != 0; i++, anItem = anItem->next()) {
	anIdList[i] = anItem->text().toInt();
      }

      myGroup = SMESH::AddGroup(myMesh, aType, myName->text());
      myGroup->Add(anIdList.inout());

      //Add reference to geometry group if it is neccessary
      if (!CORBA::is_nil( myGeomGroup ))
	{
	  SALOMEDS::Study_var aStudy = SMESH::GetActiveStudyDocument();
	  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
	  SALOMEDS::SObject_var aGeomGroupSO = aStudy->FindObjectIOR( aStudy->ConvertObjectToIOR(myGeomGroup) );
	  SALOMEDS::SObject_var aMeshGroupSO = aStudy->FindObjectIOR( aStudy->ConvertObjectToIOR(myGroup) );
	  SALOMEDS::SObject_var aReference = aStudyBuilder->NewObject(aMeshGroupSO);
	  aStudyBuilder->Addreference(aReference, aGeomGroupSO);
	}
      
      /* init for next operation */
      myName->setText("");
      myElements->clear();
      myGroup = SMESH::SMESH_Group::_nil();
    }
    else {
      myGroup->SetName(myName->text());

      QValueList<int> aAddList;
      QValueList<int>::iterator anIt;
      QListBoxItem* anItem;
      for (anItem = myElements->firstItem(); anItem != 0; anItem = anItem->next()) {
	int anId = anItem->text().toInt();
	if ((anIt = myIdList.find(anId)) == myIdList.end())
	  aAddList.append(anId);
	else
	  myIdList.remove(anIt);
      }
      if (!aAddList.empty()) {
	SMESH::long_array_var anIdList = new SMESH::long_array;
	anIdList->length(aAddList.count());
        int i;
	for (i = 0, anIt = aAddList.begin(); anIt != aAddList.end(); anIt++, i++)
	  anIdList[i] = *anIt;
	myGroup->Add(anIdList.inout());
      }
      if (!myIdList.empty()) {
	SMESH::long_array_var anIdList = new SMESH::long_array;
	anIdList->length(myIdList.count());
        int i;
	for (i = 0, anIt = myIdList.begin(); anIt != myIdList.end(); anIt++, i++)
	  anIdList[i] = *anIt;
	myGroup->Remove(anIdList.inout());
      }
      /* init for next operation */
      myIdList.clear();
      for (anItem = myElements->firstItem(); anItem != 0; anItem = anItem->next())
	myIdList.append(anItem->text().toInt());
    }

    mySMESHGUI->GetActiveStudy()->updateObjBrowser(true);
    SMESH::UpdateView(); // asv: fix of BUG PAL5515
    mySelection->ClearIObjects();
    return true;
  }
  return false;
}

//=================================================================================
// function : onOK()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::onOK()
{
  if ( onApply() )
    onClose();
}

static bool busy = false;
//=================================================================================
// function : onListSelectionChanged()
// purpose  : Called when selection in element list is changed
//=================================================================================
void SMESHGUI_GroupDlg::onListSelectionChanged()
{
  //  MESSAGE("SMESHGUI_GroupDlg::onListSelectionChanged(); myActor = " << myActor);
  if (busy || !myActor) return;
  busy = true;

  if (myCurrentLineEdit == 0) {
    mySelection->ClearIObjects();
    TColStd_MapOfInteger aIndexes;
    QListBoxItem* anItem;
    for (anItem = myElements->firstItem(); anItem != 0; anItem = anItem->next()) {
      if (anItem->isSelected()) {
	int anId = anItem->text().toInt();
	aIndexes.Add(anId);
      }
    }
    mySelection->AddOrRemoveIndex(myActor->getIO(), aIndexes, false, false);
    mySelection->AddIObject(myActor->getIO());
  }
  busy = false;
}

//=================================================================================
// function : onObjectSelectionChanged()
// purpose  : Called when selection in 3D view or ObjectBrowser is changed
//=================================================================================
void SMESHGUI_GroupDlg::onObjectSelectionChanged()
{
  if (busy || !isEnabled()) return;
  busy = true;

  int aNbSel = mySelection->IObjectCount();
  myElements->clearSelection();
 
  if (myCurrentLineEdit) {
    myCurrentLineEdit->setText("") ;
    QString aString = "";
    
    if (myCurrentLineEdit == myGeomGroupLine)
      {
	if(aNbSel != 1)
	  {
	    myGeomGroup = GEOM::GEOM_Object::_nil();
	    busy = false;
	    return;
	  }
	Standard_Boolean testResult = Standard_False;
	myGeomGroup = GEOMBase::ConvertIOinGEOMObject(mySelection->firstIObject(), testResult );
	
	// Check if the object is a geometry group
	if(!testResult || CORBA::is_nil( myGeomGroup ) || myGeomGroup->GetType() != 37)
	  {
	    myGeomGroup = GEOM::GEOM_Object::_nil();
	    busy = false;
	    return;
	  }
	// Check if group constructed on the same shape as a mesh or on its child
	SALOMEDS::Study_var aStudy = SMESH::GetActiveStudyDocument();
	GEOM::GEOM_IGroupOperations_var anOp = SMESH::GetGEOMGen()->GetIGroupOperations(aStudy->StudyId());
	// The main shape of the group 
	GEOM::GEOM_Object_var aGroupMainShape = anOp->GetMainShape( myGeomGroup );
	SALOMEDS::SObject_var aGroupMainShapeSO = aStudy->FindObjectIOR( aStudy->ConvertObjectToIOR(aGroupMainShape) );
	// The mesh SObject
	SALOMEDS::SObject_var aMeshSO = aStudy->FindObjectIOR( aStudy->ConvertObjectToIOR(myMesh) );
	
	SALOMEDS::SObject_var anObj, aRef;
	bool isRefOrSubShape = false;
	
	if ( aMeshSO->FindSubObject( 1, anObj ) &&  anObj->ReferencedObject( aRef )) {
	  if ( strcmp( aRef->GetID(), aGroupMainShapeSO->GetID() ) == 0 )
	    isRefOrSubShape = true;
	  else
	    {
	      SALOMEDS::SObject_var aFather = aGroupMainShapeSO->GetFather();
	      SALOMEDS::SComponent_var aComponent = aGroupMainShapeSO->GetFatherComponent();
	      while ( !isRefOrSubShape && strcmp( aFather->GetID(), aComponent->GetID() ) != 0 )
		{
		  if (strcmp( aRef->GetID(), aFather->GetID() ) == 0)
		    isRefOrSubShape = true;
		  else
		    aFather = aFather->GetFather();
		}
	    }
	  if ( !isRefOrSubShape ) 
	    {
	      myGeomGroup = GEOM::GEOM_Object::_nil();
	      busy = false;
	      return;
	    }
	}
      }
    
    if (aNbSel >= 1) {
      if (aNbSel > 1) {
	if (myCurrentLineEdit == mySubMeshLine)
	  aString = tr("SMESH_SUBMESH_SELECTED").arg(aNbSel);
	else if (myCurrentLineEdit == myGroupLine || myCurrentLineEdit == myGeomGroupLine)
	  aString = tr("SMESH_GROUP_SELECTED").arg(aNbSel);
      }
      else {
	aString = mySelection->firstIObject()->getName();
      }
    }
    
    myCurrentLineEdit->setText(aString) ;
    myCurrentLineEdit->home( false );
  }
  else {
    if (aNbSel == 1) {
      QString aListStr = "";
      int aNbItems = 0;
      if (myTypeId == 0) {
	aNbItems = SMESH::GetNameOfSelectedNodes(mySelection, aListStr);
      }
      else {
	aNbItems = SMESH::GetNameOfSelectedElements(mySelection, aListStr);
      }
      if (aNbItems > 0) {
	QStringList anElements = QStringList::split(" ", aListStr);
	QListBoxItem* anItem = 0;
	for (QStringList::iterator it = anElements.begin(); it != anElements.end(); ++it) {
	  anItem = myElements->findItem(*it, Qt::ExactMatch);
	  if (anItem) myElements->setSelected(anItem, true);
	}
      }
    }
  }
  
  if ( !myActor ) {
    if ( !myGroup->_is_nil() )
      myActor = SMESH::FindActorByObject(myGroup);
    else
      myActor = SMESH::FindActorByObject(myMesh);
  }
  
  busy = false;
}


//=================================================================================
// function : onSelectSubMesh()
// purpose  : Called when selection in 3D view or ObjectBrowser is changed
//=================================================================================
void SMESHGUI_GroupDlg::onSelectSubMesh(bool on)
{
  if (on) {
    if (mySelectGroup->isChecked()) {
      mySelectGroup->setChecked(false);
    } 
    else if (mySelectGeomGroup->isChecked()) {
      mySelectGeomGroup->setChecked(false);
    }
    myCurrentLineEdit = mySubMeshLine;
    setSelectionMode(4);
  }
  else {
    mySubMeshLine->setText("");
    myCurrentLineEdit = 0;
    if (myTypeId != -1)
      setSelectionMode(myTypeId);
  }
  mySubMeshBtn->setEnabled(on);
  mySubMeshLine->setEnabled(on);
}


//=================================================================================
// function : (onSelectGroup)
// purpose  : Called when selection in 3D view or ObjectBrowser is changed
//=================================================================================
void SMESHGUI_GroupDlg::onSelectGroup(bool on)
{
  if (on) {
    if (mySelectSubMesh->isChecked()) {
      mySelectSubMesh->setChecked(false);
    }
    else if (mySelectGeomGroup->isChecked()) {
      mySelectGeomGroup->setChecked(false);
    }
    myCurrentLineEdit = myGroupLine;
    setSelectionMode(5);
  }
  else {
    myGroupLine->setText("");
    myCurrentLineEdit = 0;
    if (myTypeId != -1)
      setSelectionMode(myTypeId);
  }
  myGroupBtn->setEnabled(on);
  myGroupLine->setEnabled(on);
}


//=================================================================================
// function : (onSelectGeomGroup)
// purpose  : Called when selection in 3D view or ObjectBrowser is changed
//=================================================================================
void SMESHGUI_GroupDlg::onSelectGeomGroup(bool on)
{
  if (on) {
    if (mySelectSubMesh->isChecked()) {
      mySelectSubMesh->setChecked(false);
    }
    else if (mySelectGroup->isChecked()) {
      mySelectGroup->setChecked(false);
    }
    myCurrentLineEdit = myGeomGroupLine;
    setSelectionMode(6);
  }
  else {
    myGeomGroupLine->setText("");
    myCurrentLineEdit = 0;
    if (myTypeId != -1)
      setSelectionMode(myTypeId);
  }
  myGeomGroupBtn->setEnabled(on);
  myGeomGroupLine->setEnabled(on);
}


//=================================================================================
// function : setCurrentSelection()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::setCurrentSelection()
{
  QPushButton* send = (QPushButton*)sender();
  myCurrentLineEdit = 0;
  if (send == mySubMeshBtn) {
    myCurrentLineEdit = mySubMeshLine;
    onObjectSelectionChanged();
  }
  else if (send == myGroupBtn) {
    myCurrentLineEdit = myGroupLine;
    onObjectSelectionChanged();
  }
  else if (send == myGeomGroupBtn) {
    myCurrentLineEdit = myGeomGroupLine;
    onObjectSelectionChanged();
  }
}


//=================================================================================
// function : setFilters()
// purpose  : SLOT. Called when "Filter" button pressed. 
//=================================================================================
void SMESHGUI_GroupDlg::setFilters()
{
  SMESH::ElementType aType = SMESH::ALL;
  switch ( myTypeId )
  {
    case 0 : aType = SMESH::NODE; break;
    case 1 : aType = SMESH::EDGE; break;
    case 2 : aType = SMESH::FACE; break;
    case 3 : aType = SMESH::VOLUME; break;
    default: return;
  }

  if ( myFilterDlg == 0 )
  {
    myFilterDlg = new SMESHGUI_FilterDlg( (QWidget*)parent(), aType );
    connect( myFilterDlg, SIGNAL( Accepted() ), SLOT( onFilterAccepted() ) );
  }
  else
    myFilterDlg->Init( aType );

  myFilterDlg->SetSelection( mySelection );
  myFilterDlg->SetMesh( myMesh );
  myFilterDlg->SetSourceWg( myElements );

  myFilterDlg->show();
}

//=================================================================================
// function : onFilterAccepted()
// purpose  : SLOT. Called when Filter dlg closed with OK button.
//            Uncheck "Select submesh" and "Select group" checkboxes
//=================================================================================
void SMESHGUI_GroupDlg::onFilterAccepted()
{
  if ( mySelectSubMesh->isChecked() || mySelectGroup->isChecked() )
  {
    mySelectionMode = myTypeId;
    mySelectSubMesh->setChecked( false );
    mySelectGroup->setChecked( false );
  }
}

//=================================================================================
// function : onAdd()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::onAdd()
{
  int aNbSel = mySelection->IObjectCount();
  if (aNbSel == 0 || !myActor) return;

  busy = true;

  SMESH::ElementType aType = SMESH::ALL;
  switch(myTypeId) {
  case 0: aType = SMESH::NODE; break;
  case 1: aType = SMESH::EDGE; break;
  case 2: aType = SMESH::FACE; break;
  case 3: aType = SMESH::VOLUME; break;
  }

  if (myCurrentLineEdit == 0) {
    //if (aNbSel != 1) { busy = false; return; }
    QString aListStr = "";
    int aNbItems = 0;
    if (myTypeId == 0) {
      aNbItems = SMESH::GetNameOfSelectedNodes(mySelection, myActor->getIO(), aListStr);
    }
    else {
      aNbItems = SMESH::GetNameOfSelectedElements(mySelection, myActor->getIO(), aListStr);
    }
    if (aNbItems > 0) {
      QStringList anElements = QStringList::split(" ", aListStr);
      QListBoxItem* anItem = 0;
      for (QStringList::iterator it = anElements.begin(); it != anElements.end(); ++it) {
	anItem = myElements->findItem(*it, Qt::ExactMatch);
	if (!anItem) {
	  anItem = new QListBoxText(*it);
	  myElements->insertItem(anItem);
	}
	myElements->setSelected(anItem, true);
      }
    }
  }
  else if (myCurrentLineEdit == mySubMeshLine) {
    SALOME_ListIteratorOfListIO anIt(mySelection->StoredIObjects());
    for (; anIt.More(); anIt.Next()) {
      SMESH::SMESH_subMesh_var aSubMesh = SMESH::IObjectToInterface<SMESH::SMESH_subMesh>(anIt.Value());
      if (!aSubMesh->_is_nil()) {
	// check if mesh is the same
	if (aSubMesh->GetFather()->GetId() == myMesh->GetId()) {
          try {
            SMESH::long_array_var anElements = aSubMesh->GetElementsByType ( aType );
            int k = anElements->length();
            QListBoxItem* anItem = 0;
            for (int i = 0; i < k; i++) {
              QString aText = QString::number(anElements[i]);
              anItem = myElements->findItem(aText, Qt::ExactMatch);
              if (!anItem) {
                anItem = new QListBoxText(aText);
                myElements->insertItem(anItem);
              }
              myElements->setSelected(anItem, true);
            }
          }
          catch (const SALOME::SALOME_Exception& ex) {
            QtCatchCorbaException(ex);
          }
        }
      }
    }
    mySelectSubMesh->setChecked(false);
    busy = false;
    onListSelectionChanged();
  }
  else if (myCurrentLineEdit == myGroupLine) {
    SALOME_ListIteratorOfListIO anIt(mySelection->StoredIObjects());
    for (; anIt.More(); anIt.Next()) {
      SMESH::SMESH_Group_var aGroup = SMESH::IObjectToInterface<SMESH::SMESH_Group>(anIt.Value());
      if (!aGroup->_is_nil()) {
	// check if mesh is the same
	if (aGroup->GetType() == aType && aGroup->GetMesh()->GetId() == myMesh->GetId()) {
	  SMESH::long_array_var anElements = aGroup->GetListOfID();
	  int k = anElements->length();
	  QListBoxItem* anItem = 0;
	  for (int i = 0; i < k; i++) {
	    QString aText = QString::number(anElements[i]);
	    anItem = myElements->findItem(aText, Qt::ExactMatch);
	    if (!anItem) {
	      anItem = new QListBoxText(aText);
	      myElements->insertItem(anItem);
	    }
	    myElements->setSelected(anItem, true);
	  }
	}
      }
    }
    mySelectGroup->setChecked(false);
    busy = false;
    onListSelectionChanged();
  }
  else if (myCurrentLineEdit == myGeomGroupLine && !CORBA::is_nil(myGeomGroup)) {
    
    SALOMEDS::Study_var aStudy = SMESH::GetActiveStudyDocument();
    GEOM::GEOM_IGroupOperations_var aGroupOp = SMESH::GetGEOMGen()->GetIGroupOperations(aStudy->StudyId());
    
    SMESH::ElementType aGroupType = SMESH::ALL;
    switch(aGroupOp->GetType(myGeomGroup)) {
    case 7: aGroupType = SMESH::NODE; break;
    case 6: aGroupType = SMESH::EDGE; break;
    case 4: aGroupType = SMESH::FACE; break;
    case 2: aGroupType = SMESH::VOLUME; break;
    default: return;
    }
    
    if (aGroupType == aType) {
      SALOMEDS::SObject_var aGroupSO = aStudy->FindObjectIOR( aStudy->ConvertObjectToIOR(myGeomGroup) );
      // Construct filter
      SMESH::FilterManager_var aFilterMgr = SMESH::GetFilterManager();
      SMESH::Filter_var aFilter = aFilterMgr->CreateFilter();
      SMESH::BelongToGeom_var aBelongToGeom = aFilterMgr->CreateBelongToGeom();;
      aBelongToGeom->SetGeom(myGeomGroup);
      aBelongToGeom->SetShapeName(aGroupSO->GetName());
      aBelongToGeom->SetElementType(aType);
      aFilter->SetPredicate( aBelongToGeom );
      
      SMESH::long_array_var anElements = aFilter->GetElementsId( myMesh );
      
      int k = anElements->length();
      QListBoxItem* anItem = 0;
      for (int i = 0; i < k; i++) {
	QString aText = QString::number(anElements[i]);
	anItem = myElements->findItem(aText, Qt::ExactMatch);
	if (!anItem) {
	  anItem = new QListBoxText(aText);
	  myElements->insertItem(anItem);
	}
	myElements->setSelected(anItem, true);
      }
    }
    
    mySelectGeomGroup->setChecked(false);
    busy = false;
    onListSelectionChanged();
  }
  busy = false;
  //  mySelection->ClearIObjects();
  updateButtons();
}

//=================================================================================
// function : onRemove()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::onRemove()
{
  busy = true;
  if (myCurrentLineEdit == 0) {
    for (int i = myElements->count(); i > 0; i--) {
      if (myElements->isSelected(i-1)) {
	myElements->removeItem(i-1);
      }
    }
  }
  else {
    int aNbSel = mySelection->IObjectCount();
    if (aNbSel == 0) { busy = false; return; }
    
    SMESH::ElementType aType = SMESH::ALL;
    switch(myTypeId) {
    case 0: aType = SMESH::NODE; break;
    case 1: aType = SMESH::EDGE; break;
    case 2: aType = SMESH::FACE; break;
    case 3: aType = SMESH::VOLUME; break;
    }

    if (myCurrentLineEdit == mySubMeshLine) {
      SALOME_ListIteratorOfListIO anIt(mySelection->StoredIObjects());
      for (; anIt.More(); anIt.Next()) {
	SMESH::SMESH_subMesh_var aSubMesh = SMESH::IObjectToInterface<SMESH::SMESH_subMesh>(anIt.Value());
	if (!aSubMesh->_is_nil()) {
	  // check if mesh is the same
	  if (aSubMesh->GetFather()->GetId() == myMesh->GetId()) {
	    if (aType == SMESH::NODE) {
	      try {
		SMESH::long_array_var anElements = aSubMesh->GetNodesId();
		int k = anElements->length();
		QListBoxItem* anItem = 0;
		for (int i = 0; i < k; i++) {
		  anItem = myElements->findItem(QString::number(anElements[i]), Qt::ExactMatch);
		  if (anItem) delete anItem;
		}
	      }
	      catch (const SALOME::SALOME_Exception& ex) {
		QtCatchCorbaException(ex);
	      }
	    }
	    else {
	      try {
		SMESH::long_array_var anElements = aSubMesh->GetElementsId();
		int k = anElements->length();
		QListBoxItem* anItem = 0;
		for (int i = 0; i < k; i++) {
		  anItem = myElements->findItem(QString::number(anElements[i]), Qt::ExactMatch);
		  if (anItem) delete anItem;
		}
	      }
	      catch (const SALOME::SALOME_Exception& ex) {
		QtCatchCorbaException(ex);
	      }
	    }
	  }
	}
      }
    }
    else if (myCurrentLineEdit == myGroupLine) {
      Standard_Boolean aRes;
      SALOME_ListIteratorOfListIO anIt(mySelection->StoredIObjects());
      for (; anIt.More(); anIt.Next()) {
	SMESH::SMESH_Group_var aGroup = SMESH::IObjectToInterface<SMESH::SMESH_Group>(anIt.Value());
	if (aRes && !aGroup->_is_nil()) {
	  // check if mesh is the same
	  if (aGroup->GetType() == aType && aGroup->GetMesh()->GetId() == myMesh->GetId()) {
	    SMESH::long_array_var anElements = aGroup->GetListOfID();
	    int k = anElements->length();
	    QListBoxItem* anItem = 0;
	    for (int i = 0; i < k; i++) {
	      anItem = myElements->findItem(QString::number(anElements[i]), Qt::ExactMatch);
	      if (anItem) delete anItem;
	    }
	  }
	}
      }
    }
  }
  busy = false;
  updateButtons();
}

//=================================================================================
// function : onSort()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::onSort()
{
  // PAL5412: sorts items in ascending by "string" value
  // myElements->sort(true);
  // myElements->update();
  int i, k = myElements->count();
  if (k > 0) {
    busy = true;
    QStringList aSelected;
    std::vector<int> anArray(k);
    //    QMemArray<int> anArray(k);
    QListBoxItem* anItem;
    // fill the array
    for (anItem = myElements->firstItem(), i = 0; anItem != 0; anItem = anItem->next(), i++) {
      anArray[i] = anItem->text().toInt();
      if (anItem->isSelected()) 
	aSelected.append(anItem->text());
    }
    // sort & update list
    std::sort(anArray.begin(), anArray.end());
    //    anArray.sort();
    myElements->clear();
    for (i = 0; i < k; i++) {
      myElements->insertItem(QString::number(anArray[i]));
    }
    for (QStringList::iterator it = aSelected.begin(); it != aSelected.end(); ++it) {
      anItem = myElements->findItem(*it, Qt::ExactMatch);
      if (anItem) myElements->setSelected(anItem, true);
    }
    busy = false;
  }
}

//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void SMESHGUI_GroupDlg::closeEvent( QCloseEvent* e )
{
  onClose();
}

//=======================================================================
// name    : SMESHGUI_GroupDlg::onClose
// Purpose : SLOT called when "Close" button pressed. Close dialog
//=======================================================================
void SMESHGUI_GroupDlg::onClose()
{
  QAD_StudyFrame* aStudyFrame = mySMESHGUI->GetActiveStudy()->getActiveStudyFrame();
  if (aStudyFrame->getTypeView() == VIEW_VTK) {
    SMESH::SetPointRepresentation(false);
    SMESH::SetPickable();

    // remove filters from viewer
    if(VTKViewer_InteractorStyleSALOME* aStyle = SMESH::GetInteractorStyle()){
      SMESH::RemoveFilter(SMESHGUI_EdgeFilter,aStyle);
      SMESH::RemoveFilter(SMESHGUI_FaceFilter,aStyle);
    }
  }
  
  mySelection->ClearIObjects();
  QAD_Application::getDesktop()->SetSelectionMode(ActorSelection);
  mySelection->ClearFilters();
  mySMESHGUI->ResetState();

  reject();
}

//=======================================================================
// name    : SMESHGUI_GroupDlg::onDeactivate
// Purpose : SLOT called when dialog must be deativated
//=======================================================================
void SMESHGUI_GroupDlg::onDeactivate()
{
  setEnabled( false );
}

//=======================================================================
// name    : SMESHGUI_GroupDlg::enterEvent
// Purpose : Event filter
//=======================================================================
void SMESHGUI_GroupDlg::enterEvent( QEvent* )
{
  if ( !isEnabled() ) {
    SMESHGUI::GetSMESHGUI()->EmitSignalDeactivateDialog();
    setEnabled( true );
    mySelectionMode = -1;
    setSelectionMode( myTypeId );
  }
}

//=======================================================================
//function : hideEvent
//purpose  : caused by ESC key
//=======================================================================

void SMESHGUI_GroupDlg::hideEvent ( QHideEvent * e )
{
  if ( !isMinimized() )
    onClose();
}
