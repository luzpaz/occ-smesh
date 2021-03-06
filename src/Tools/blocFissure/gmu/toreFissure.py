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

import logging
from .geomsmesh import geompy
from .geomsmesh import geomPublish
from .geomsmesh import geomPublishInFather
from . import initLog
import math
from .triedreBase import triedreBase

O, OX, OY, OZ = triedreBase()

# -----------------------------------------------------------------------------
# --- tore et plan de fissure

def toreFissure(minRad,allonge,rayTore):
  """
  Construction de la geometrie du tore elliptique autour du front de fissure.
  L'ellipse est construite dans le plan xoy, axe oy.
  @param minRad :petit rayon
  @param allonge :rapport grand rayon / petit rayon
  @param rayTore :rayon du tore construit autour de la generatrice de l'ellipse
  @return (generatrice, FaceGenFiss, Pipe_1, FaceFissure, Plane_1, Pipe1Part) : ellipse, section du tore,
  tore plein, face plane de le fissure, plan de la fissure, tore partitioné par le plan de fissure.
  """
  logging.info("start ", minRad, allonge, rayTore)
  
  Vertex_1 = geompy.MakeVertex( minRad, 0, 0)
  Vertex_2 = geompy.MakeVertex(-minRad, 0, 0)
  Vertex_3 = geompy.MakeRotation(Vertex_1, OZ,  45*math.pi/180.0)
  Arc_1 = geompy.MakeArc(Vertex_1, Vertex_2, Vertex_3)
  generatrice = geompy.MakeScaleAlongAxes(Arc_1, O, 1, allonge, 1)

  #geomPublish(initLog.debug,  Vertex_1, 'Vertex_1' )
  #geomPublish(initLog.debug,  Vertex_2, 'Vertex_2' )
  #geomPublish(initLog.debug,  Vertex_3, 'Vertex_3' )
  #geomPublish(initLog.debug,  Arc_1, 'Arc_1' )
  #geomPublish(initLog.debug,  generatrice, 'generatrice' )

  # --- face circulaire sur la generatrice, pour extrusion

  Circle_1 = geompy.MakeCircle(O, OY, rayTore)
  Rotation_1 = geompy.MakeRotation(Circle_1, OY, -90*math.pi/180.0)
  Translation_1 = geompy.MakeTranslation(Rotation_1, minRad, 0, 0)
  FaceGenFiss = geompy.MakeFaceWires([Translation_1], 1)

  #geomPublish(initLog.debug,  Circle_1, 'Circle_1' )
  #geomPublish(initLog.debug,  Rotation_1, 'Rotation_1' )
  #geomPublish(initLog.debug,  Translation_1, 'Translation_1' )
  #geomPublish(initLog.debug,  FaceGenFiss, 'FaceGenFiss' )

  # --- tore extrude

  Pipe_1 = geompy.MakePipe(FaceGenFiss, generatrice)

  # --- plan fissure, delimite par la generatrice

  Scale_1_vertex_3 = geompy.GetSubShape(generatrice, [3])
  Line_1 = geompy.MakeLineTwoPnt(Vertex_1, Scale_1_vertex_3)
  FaceFissure = geompy.MakeFaceWires([generatrice, Line_1], 1)

  #geomPublishInFather(initLog.debug, generatrice, Scale_1_vertex_3, 'Scale_1:vertex_3' )
  #geomPublish(initLog.debug,  Line_1, 'Line_1' )
  #geomPublish(initLog.debug,  FaceFissure, 'FaceFissure' )

  # --- tore coupe en 2 demi tore de section 1/2 disque

  Plane_1 = geompy.MakePlane(O, OZ, 2000)
  Pipe1Part = geompy.MakePartition([Pipe_1], [Plane_1], [], [], geompy.ShapeType["SOLID"], 0, [], 1)
  geomPublish(initLog.debug, Pipe1Part , 'Pipe1Part' )

  return generatrice, FaceGenFiss, Pipe_1, FaceFissure, Plane_1, Pipe1Part
