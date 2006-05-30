//  SMESH StdMeshers : implementaion of SMESH idl descriptions
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
//  File   : StdMeshers_Penta_3D.hxx
//  Module : SMESH

#ifndef StdMeshers_Penta_3D_HeaderFile
#define StdMeshers_Penta_3D_HeaderFile

#include <map>

////////////////////////////////////////////////////////////////////////
//
//  class StdMeshers_SMESHBlock
//
////////////////////////////////////////////////////////////////////////
#include <gp_Pnt.hxx>
#include <gp_XYZ.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Shell.hxx>
#include <TopTools_IndexedMapOfOrientedShape.hxx>
#include <TColStd_MapOfInteger.hxx>

#include "SMESH_Block.hxx"

#include "SMESH_MesherHelper.hxx"

typedef std::map< double, std::vector<const SMDS_MeshNode*> > StdMeshers_IJNodeMap;

class StdMeshers_SMESHBlock {
 
public:
  //
  StdMeshers_SMESHBlock();
  
  void Load (const TopoDS_Shell& theShell);
  
  void Load (const TopoDS_Shell& theShell,
	     const TopoDS_Vertex& theV000,
	     const TopoDS_Vertex& theV001);
  
  void ComputeParameters(const gp_Pnt& thePnt, 
			 gp_XYZ& theXYZ);
  
  void ComputeParameters(const gp_Pnt& thePnt,
			 const TopoDS_Shape& theShape,
			 gp_XYZ& theXYZ);
  
  void ComputeParameters(const double& theU,
			 const TopoDS_Shape& theShape,
			 gp_XYZ& theXYZ);
  
  void Point(const gp_XYZ& theParams, 
	     gp_Pnt& thePnt);
  
  void Point(const gp_XYZ& theParams,
	     const TopoDS_Shape& theShape, 
	     gp_Pnt& thePnt);
  
  int ShapeID(const TopoDS_Shape& theShape); 
  
  const TopoDS_Shape& Shape(const int theID);

  SMESH_Block & Block() { return myTBlock; }

  bool IsForwadEdge(const int theEdgeID);

  int  ErrorStatus() const;


protected:
  TopoDS_Shell                       myShell;
  TopTools_IndexedMapOfOrientedShape myShapeIDMap;
  SMESH_Block                        myTBlock;
  TopoDS_Shape                       myEmptyShape;
  vector<int>                        myIsEdgeForward;
  //
  int myErrorStatus;
};

////////////////////////////////////////////////////////////////////////
//
//  class StdMeshers_TNode
//
////////////////////////////////////////////////////////////////////////
#include "SMDS_MeshNode.hxx"

class StdMeshers_TNode {

public:
  
  StdMeshers_TNode(){
    myNode=NULL;
    myXYZ.SetCoord(99., 99., 99.);
    myShapeSupportID=-1;
    myBaseNodeID=-1;
  }
  
  void SetNode(const SMDS_MeshNode* theNode) {
    myNode=(SMDS_MeshNode*) theNode;
  }
  
  const SMDS_MeshNode* Node()const {
    return myNode;
  }

  void SetShapeSupportID (const int theID) {
    myShapeSupportID=theID;
  }
  
  int ShapeSupportID()const {
    return myShapeSupportID;
  }
  
  void SetNormCoord (const gp_XYZ& theXYZ) {
    myXYZ=theXYZ;
  }

  const gp_XYZ& NormCoord ()const{
    return myXYZ;
  }
  
  void SetBaseNodeID (const int theID) {
    myBaseNodeID=theID;
  }
  
  int BaseNodeID ()const{
    return myBaseNodeID;
  }

private:
  SMDS_MeshNode* myNode;
  int  myShapeSupportID;
  gp_XYZ         myXYZ;
  int            myBaseNodeID;
};

////////////////////////////////////////////////////////////////////////
//
//  class StdMeshers_Penta_3D
//
////////////////////////////////////////////////////////////////////////
#include "SMESH_Mesh.hxx"
#include <TopoDS_Shape.hxx>
//
class StdMeshers_Penta_3D {
//
  public: // methods
    StdMeshers_Penta_3D();
    
    ~StdMeshers_Penta_3D();
    
    bool Compute(SMESH_Mesh& , const TopoDS_Shape& );
    
    int ErrorStatus() const {
      return myErrorStatus;
    }
   
    void SetTolerance(const double theTol3D) {
      myTol3D=theTol3D;
    }
    
    double Tolerance() const {
      return myTol3D;
    }

    bool LoadIJNodes(StdMeshers_IJNodeMap & theIJNodes,
                     const TopoDS_Face&     theFace,
                     const TopoDS_Edge&     theBaseEdge,
                     SMESHDS_Mesh*          theMesh);
    // Load nodes bound to theFace into column (vectors) and rows
    // of theIJNodes.
    // The value of theIJNodes map is a vector of ordered nodes so
    // that the 0-the one lies on theBaseEdge.
    // The key of theIJNodes map is a normalized parameter of each
    // 0-the node on theBaseEdge.


  protected: // methods
    
    void CheckData();
    
    void MakeBlock();

    void MakeNodes();

    double SetHorizEdgeXYZ(const gp_XYZ&                  aBNXYZ,
                           const int                      aFaceID,
                           vector<const SMDS_MeshNode*>*& aCol1,
                           vector<const SMDS_MeshNode*>*& aCol2);

    void ShapeSupportID(const bool theIsUpperLayer,
			const SMESH_Block::TShapeID theBNSSID,
			SMESH_Block::TShapeID& theSSID);

    void FindNodeOnShape(const TopoDS_Shape& aS,
			 const gp_XYZ& aParams,
                         const int z,
			 StdMeshers_TNode& aTN);

    void CreateNode(const bool theIsUpperLayer,
		    const gp_XYZ& aParams,
		    StdMeshers_TNode& aTN);

    void ClearMeshOnFxy1();

    void MakeMeshOnFxy1();

    void MakeConnectingMap();

    int GetIndexOnLayer(const int aID);

    void MakeVolumeMesh();
  
    void SetMesh(SMESH_Mesh& theMesh) {
      myMesh=(void *)&theMesh;
    }
    
    SMESH_Mesh* GetMesh()const {
      return (SMESH_Mesh*)myMesh;
    }
    
  protected: // fields
    TopoDS_Shape              myShape;
    StdMeshers_SMESHBlock     myBlock;
    void *                    myMesh;
    int                       myErrorStatus;
    //
    vector <StdMeshers_TNode> myTNodes;
    int                       myISize;
    int                       myJSize;
    double                    myTol3D;        // Tolerance value     
    std::map < int, int >     myConnectingMap;
    //
    vector<StdMeshers_IJNodeMap> myWallNodesMaps; // nodes on a face
    vector<gp_XYZ>            myShapeXYZ; // point on each sub-shape

    bool myCreateQuadratic;
    SMESH_MesherHelper* myTool; // toll for working with quadratic elements
};

#endif
