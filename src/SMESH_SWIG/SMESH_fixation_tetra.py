#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

# Tetrahedrization of the geometry generated by the Python script
# SMESH_fixation.py
# Hypothesis and algorithms for the mesh generation are global
#
import salome
from . import SMESH_fixation

import SMESH, SALOMEDS
from salome.smesh import smeshBuilder
smesh =  smeshBuilder.New(salome.myStudy)

compshell = SMESH_fixation.compshell
idcomp = SMESH_fixation.idcomp
geompy = SMESH_fixation.geompy
salome = SMESH_fixation.salome

print("Analysis of the geometry to be meshed :")
subShellList = geompy.SubShapeAll(compshell, geompy.ShapeType["SHELL"])
subFaceList  = geompy.SubShapeAll(compshell, geompy.ShapeType["FACE"])
subEdgeList  = geompy.SubShapeAll(compshell, geompy.ShapeType["EDGE"])

print("number of Shells in compshell : ", len(subShellList))
print("number of Faces  in compshell : ", len(subFaceList))
print("number of Edges  in compshell : ", len(subEdgeList))

status = geompy.CheckShape(compshell)
print(" check status ", status)

### ---------------------------- SMESH --------------------------------------
smesh.SetCurrentStudy(salome.myStudy)

# ---- init a Mesh with the compshell

mesh = smesh.Mesh(compshell, "MeshcompShell")


# ---- set Hypothesis and Algorithm

print("-------------------------- NumberOfSegments")

numberOfSegments = 5

regular1D = mesh.Segment()
regular1D.SetName("Wire Discretisation")
hypNbSeg = regular1D.NumberOfSegments(numberOfSegments)
print(hypNbSeg.GetName())
print(hypNbSeg.GetId())
print(hypNbSeg.GetNumberOfSegments())
smesh.SetName(hypNbSeg, "NumberOfSegments_" + str(numberOfSegments))

## print "-------------------------- MaxElementArea"

## maxElementArea = 80

## mefisto2D = mesh.Triangle()
## mefisto2D.SetName("MEFISTO_2D")
## hypArea = mefisto2D.MaxElementArea(maxElementArea)
## print hypArea.GetName()
## print hypArea.GetId()
## print hypArea.GetMaxElementArea()
## smesh.SetName(hypArea, "MaxElementArea_" + str(maxElementArea))

print("-------------------------- LengthFromEdges")

mefisto2D = mesh.Triangle()
mefisto2D.SetName("MEFISTO_2D")
hypLengthFromEdges = mefisto2D.LengthFromEdges()
print(hypLengthFromEdges.GetName())
print(hypLengthFromEdges.GetId())
smesh.SetName(hypLengthFromEdges, "LengthFromEdges")


print("-------------------------- MaxElementVolume")

maxElementVolume = 1000

netgen3D = mesh.Tetrahedron(smeshBuilder.NETGEN)
netgen3D.SetName("NETGEN_3D")
hypVolume = netgen3D.MaxElementVolume(maxElementVolume)
print(hypVolume.GetName())
print(hypVolume.GetId())
print(hypVolume.GetMaxElementVolume())
smesh.SetName(hypVolume, "MaxElementVolume_" + str(maxElementVolume))

print("-------------------------- compute compshell")
ret = mesh.Compute(mesh)
print(ret)
if ret != 0:
    log = mesh.GetLog(0) # no erase trace
    for linelog in log:
        print(linelog)
    print("Information about the MeshcompShel:")
    print("Number of nodes        : ", mesh.NbNodes())
    print("Number of edges        : ", mesh.NbEdges())
    print("Number of faces        : ", mesh.NbFaces())
    print("Number of triangles    : ", mesh.NbTriangles())
    print("Number of volumes      : ", mesh.NbVolumes())
    print("Number of tetrahedrons : ", mesh.NbTetras())
    
else:
    print("problem when computing the mesh")

salome.sg.updateObjBrowser(True)
