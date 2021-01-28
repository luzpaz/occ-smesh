# -*- coding: utf-8 -*-
# Copyright (C) 2014-2020  EDF R&D
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

"""Cas-test de blocFissure sur un tube"""
__revision__ = "V02.05"

import logging
import os

from blocFissure import gmu

import salome
from SketchAPI import *
from salome.shaper import model
import SHAPERSTUDY
import SMESH
from salome.smesh import smeshBuilder

#=============== Options ====================
# 1. NOM_OBJET = nom de l'objet
NOM_OBJET = "Tube"
# 2. CAO
# 2.1. Le tube : rayon intérieur, rayon extérieur, hauteur
RAYON_INT =  9.84
RAYON_EXT = 11.11
HAUTEUR = 50.
# 2.2. La fissure :
F_L_1 = 80.
F_THETA_1 = 3.
F_C_1 = 1.04
F_L_2 = 20.
# 3. Maillage
# Nombre de segments du tube
NB_S_E = 4
NB_S_H = 60
NB_S_P = 30
#============================================

salome.salome_init()

###
### SHAPER component
###

model.begin()
partSet = model.moduleDocument()

### Create Part
Part_1 = model.addPart(partSet)
Part_1_doc = Part_1.document()
model.addParameter(Part_1_doc, "R_I", "%f" % RAYON_INT)
model.addParameter(Part_1_doc, "R_E", "%f" % RAYON_EXT)
model.addParameter(Part_1_doc, "H", "%f" % HAUTEUR)
model.addParameter(Part_1_doc, "F_L_1", "{}".format(F_L_1))
model.addParameter(Part_1_doc, "F_THETA_1", "{}".format(F_THETA_1))
model.addParameter(Part_1_doc, "F_R_1", "{}*{}".format(F_C_1,RAYON_INT))
model.addParameter(Part_1_doc, "F_L_2", "{}".format(F_L_2))

### Create Sketch
Sketch_1 = model.addSketch(Part_1_doc, model.defaultPlane("XOZ"))

### Create SketchLine
SketchLine_1 = Sketch_1.addLine(11.11, -22.82842712474619, 9.84, -22.82842712474619)

### Create SketchLine
SketchLine_2 = Sketch_1.addLine(9.84, -22.82842712474619, 9.84, 27.17157287525381)

### Create SketchLine
SketchLine_3 = Sketch_1.addLine(9.84, 27.17157287525381, 11.11, 27.17157287525381)

### Create SketchLine
SketchLine_4 = Sketch_1.addLine(11.11, 27.17157287525381, 11.11, -22.82842712474619)
Sketch_1.setCoincident(SketchLine_4.endPoint(), SketchLine_1.startPoint())
Sketch_1.setCoincident(SketchLine_1.endPoint(), SketchLine_2.startPoint())
Sketch_1.setCoincident(SketchLine_2.endPoint(), SketchLine_3.startPoint())
Sketch_1.setCoincident(SketchLine_3.endPoint(), SketchLine_4.startPoint())
Sketch_1.setHorizontal(SketchLine_1.result())
Sketch_1.setVertical(SketchLine_2.result())
Sketch_1.setHorizontal(SketchLine_3.result())
Sketch_1.setVertical(SketchLine_4.result())
Sketch_1.setLength(SketchLine_4.result(), "H")

### Create SketchProjection
SketchProjection_1 = Sketch_1.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_1 = SketchProjection_1.createdFeature()
Sketch_1.setHorizontalDistance(SketchAPI_Point(SketchPoint_1).coordinates(), SketchLine_1.endPoint(), "R_I")
Sketch_1.setHorizontalDistance(SketchAPI_Point(SketchPoint_1).coordinates(), SketchLine_1.startPoint(), "R_E")
model.do()

### Create Revolution
Revolution_1 = model.addRevolution(Part_1_doc, [model.selection("FACE", "Sketch_1/Face-SketchLine_4r-SketchLine_3r-SketchLine_2r-SketchLine_1r")], model.selection("EDGE", "PartSet/OZ"), 180, 0)
Revolution_1.result().setName("Tube")
Revolution_1.result().setTransparency(0.9399999999999999)

### Create Sketch
Sketch_2 = model.addSketch(Part_1_doc, model.standardPlane("YOZ"))

### Create SketchArc
SketchArc_1 = Sketch_2.addArc(-80, 0, 10.10993798722564, 4.722461741385164, 10.10993798722563, -4.722461741385561, True)

### Create SketchProjection
SketchProjection_2 = Sketch_2.addProjection(model.selection("EDGE", "PartSet/OY"), False)
SketchLine_5 = SketchProjection_2.createdFeature()
Sketch_2.setCoincident(SketchArc_1.center(), SketchLine_5.result())

### Create SketchProjection
SketchProjection_3 = Sketch_2.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_2 = SketchProjection_3.createdFeature()
Sketch_2.setHorizontalDistance(SketchArc_1.center(), SketchAPI_Point(SketchPoint_2).coordinates(), "F_L_1")

### Create SketchPoint
SketchPoint_3 = Sketch_2.addPoint(10.2336, 0)
SketchPoint_3.setAuxiliary(True)
Sketch_2.setCoincident(SketchPoint_3.coordinates(), SketchArc_1.results()[1])
Sketch_2.setMiddlePoint(SketchPoint_3.coordinates(), SketchArc_1.results()[1])
Sketch_2.setCoincident(SketchPoint_3.coordinates(), SketchLine_5.result())

### Create SketchProjection
SketchProjection_4 = Sketch_2.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_4 = SketchProjection_4.createdFeature()
Sketch_2.setHorizontalDistance(SketchPoint_3.coordinates(), SketchAPI_Point(SketchPoint_4).coordinates(), "F_R_1")

### Create SketchLine
SketchLine_6 = Sketch_2.addLine(-80, 0, 10.10993798722564, 4.722461741385164)
SketchLine_6.setAuxiliary(True)
Sketch_2.setCoincident(SketchArc_1.center(), SketchLine_6.startPoint())
Sketch_2.setCoincident(SketchArc_1.startPoint(), SketchLine_6.endPoint())

### Create SketchConstraintAngle
Sketch_2.setAngle(SketchLine_5.result(), SketchLine_6.result(), "F_THETA_1", type = "Direct")

### Create SketchProjection
SketchProjection_5 = Sketch_2.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_5 = SketchProjection_5.createdFeature()

### Create SketchEllipticArc
SketchEllipticArc_1 = Sketch_2.addEllipticArc(-20, 8.956370781951521e-27, -10.21629725685072, 9.783702743149284, 10.10993798723031, 4.722461741243296, 9.440921421952831, 9.565935245237778, False)
[SketchPoint_6, SketchPoint_7, SketchPoint_8, SketchPoint_9, SketchPoint_10, SketchPoint_11, SketchPoint_12, SketchLine_7, SketchLine_8] = \
  SketchEllipticArc_1.construction(center = "aux", firstFocus = "aux", secondFocus = "aux", majorAxisStart = "aux", majorAxisEnd = "aux", minorAxisStart = "aux", minorAxisEnd = "aux", majorAxis = "aux", minorAxis = "aux")
Sketch_2.setCoincident(SketchEllipticArc_1.startPoint(), SketchLine_6.endPoint())
Sketch_2.setCoincident(SketchAPI_Point(SketchPoint_6).coordinates(), SketchLine_5.result())
Sketch_2.setTangent(SketchEllipticArc_1.result(), SketchArc_1.results()[1])

### Create SketchProjection
SketchProjection_6 = Sketch_2.addProjection(model.selection("VERTEX", "PartSet/Origin"), False)
SketchPoint_13 = SketchProjection_6.createdFeature()
Sketch_2.setHorizontalDistance(SketchAPI_Point(SketchPoint_6).coordinates(), SketchAPI_Point(SketchPoint_13).coordinates(), "F_L_2")

### Create SketchLine
SketchLine_9 = Sketch_2.addLine(-20, 8.956370781951521e-27, 9.440921421952831, 9.565935245237778)
SketchLine_9.setAuxiliary(True)
Sketch_2.setCoincident(SketchEllipticArc_1.center(), SketchLine_9.startPoint())
Sketch_2.setCoincident(SketchEllipticArc_1.endPoint(), SketchLine_9.endPoint())

### Create SketchConstraintAngle
Sketch_2.setAngle(SketchLine_9.result(), SketchLine_5.result(), "6.*F_THETA_1", type = "Direct")

### Create SketchConstraintAngle
Sketch_2.setAngle(SketchLine_5.result(), SketchLine_7.result(), 45, type = "Supplementary")

### Create SketchConstraintMirror
SketchConstraintMirror_1 = Sketch_2.addMirror(SketchLine_5.result(), [SketchEllipticArc_1.result()])
[SketchEllipticArc_2] = SketchConstraintMirror_1.mirrored()

### Create SketchLine
SketchLine_10 = Sketch_2.addLine(9.440921421952831, 9.565935245237778, 9.440921421952837, -9.565935245237783)
Sketch_2.setCoincident(SketchEllipticArc_1.endPoint(), SketchLine_10.startPoint())
Sketch_2.setCoincident(SketchAPI_EllipticArc(SketchEllipticArc_2).endPoint(), SketchLine_10.endPoint())
model.do()

### Create Face
Face_1 = model.addFace(Part_1_doc, [model.selection("FACE", "Sketch_2/Face-SketchArc_1_2r-SketchEllipticArc_1f-SketchLine_10f-SketchEllipticArc_2f")])
Face_1.result().setName("Fissure")

### Create Group
Group_1 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "Fissure/Modified_Edge&Sketch_2/SketchEllipticArc_1")])

### Create Group
Group_2 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "Fissure/Modified_Edge&Sketch_2/SketchArc_1_2")])

### Create Group
Group_3 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "Fissure/Modified_Edge&Sketch_2/SketchEllipticArc_2")])

### Create Group
Group_4 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "Fissure/Modified_Edge&Sketch_2/SketchLine_10")])

### Create Group
Group_5 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "[Tube/Generated_Face&Sketch_1/SketchLine_1][Tube/From_Face]")])
Group_5.setName("Epaisseur")
Group_5.result().setName("Epaisseur")

### Create Group
Group_6 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "[Tube/Generated_Face&Sketch_1/SketchLine_4][Tube/From_Face]")])
Group_6.setName("Hauteur")
Group_6.result().setName("Hauteur")

### Create Group
Group_7 = model.addGroup(Part_1_doc, "Edges", [model.selection("EDGE", "[Tube/Generated_Face&Sketch_1/SketchLine_4][Tube/Generated_Face&Sketch_1/SketchLine_1]")])
Group_7.setName("Peripherie")
Group_7.result().setName("Peripherie")

### Create Export
ficxao = os.path.join(gmu.pathBloc, "materielCasTests", "{}Fiss.xao".format(NOM_OBJET))
text = ".. Exportation de la géométrie de la fissure dans le fichier '{}'".format(ficxao)
logging.info(text)
_ = model.exportToXAO(Part_1_doc, ficxao, model.selection("FACE", "Fissure"), 'XAO')

model.end()

###
### SHAPERSTUDY component
###

model.publishToShaperStudy()

l_aux = SHAPERSTUDY.shape(model.featureStringId(Revolution_1))
objet = l_aux[0]
l_groups = l_aux[1:]

###
### SMESH component
###

smesh = smeshBuilder.New()
Maillage_tube = smesh.Mesh(objet)
smesh.SetName(Maillage_tube, NOM_OBJET)

for groupe in l_groups:
  groupe_nom = groupe.GetName()
  if ( groupe_nom[:1] == "E" ):
    group_e = groupe
  elif ( groupe_nom[:1] == "H" ):
    group_h = groupe
  elif ( groupe_nom[:1] == "P" ):
    group_p = groupe
  _ = Maillage_tube.GroupOnGeom(groupe,groupe_nom,SMESH.EDGE)

Regular_1D = Maillage_tube.Segment()
Nb_Segments_1 = Regular_1D.NumberOfSegments(NB_S_P)
Nb_Segments_1.SetDistrType( 0 )
Quadrangle_2D = Maillage_tube.Quadrangle(algo=smeshBuilder.QUADRANGLE)
Hexa_3D = Maillage_tube.Hexahedron(algo=smeshBuilder.Hexa)

Regular_1D_1 = Maillage_tube.Segment(geom=group_e)
Number_of_Segments_1 = Regular_1D_1.NumberOfSegments(NB_S_E)
Propagation_of_1D_Hyp = Regular_1D_1.Propagation()

Regular_1D_2 = Maillage_tube.Segment(geom=group_h)
Number_of_Segments_2 = Regular_1D_2.NumberOfSegments(NB_S_H)
Propagation_of_1D_Hyp_1 = Regular_1D_2.Propagation()

is_done = Maillage_tube.Compute()
text = "Maillage_tube.Compute"
if is_done:
  logging.info(text+" OK")
else:
  text = "Erreur au calcul du maillage.\n" + text
  logging.info(text)
  raise Exception(text)

ficmed = os.path.join(gmu.pathBloc, "materielCasTests","{}.med".format(NOM_OBJET))
text = ".. Archivage du maillage dans le fichier '{}'".format(ficmed)
logging.info(text)
Maillage_tube.ExportMED(ficmed)

## set object names
smesh.SetName(Maillage_tube.GetMesh(), 'Maillage_tube')
smesh.SetName(Regular_1D.GetAlgorithm(), 'Regular_1D')
smesh.SetName(Nb_Segments_1, 'Nb. Segments_1')
smesh.SetName(Quadrangle_2D.GetAlgorithm(), 'Quadrangle_2D')
smesh.SetName(Hexa_3D.GetAlgorithm(), 'Hexa_3D')

if salome.sg.hasDesktop():
  salome.sg.updateObjBrowser()
