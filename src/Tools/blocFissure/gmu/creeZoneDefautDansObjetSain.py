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
from .geomsmesh import smesh
import SMESH
import SALOMEDS

from .creeZoneDefautMaillage import creeZoneDefautMaillage
from .peauInterne import peauInterne
from .quadranglesToShapeNoCorner import quadranglesToShapeNoCorner
from .creeZoneDefautFilling import creeZoneDefautFilling
from .creeZoneDefautGeom import creeZoneDefautGeom
from .getCentreFondFiss import getCentreFondFiss

# -----------------------------------------------------------------------------
# ---

def creeZoneDefautDansObjetSain(geometriesSaines, maillagesSains, shapesFissure,
                                shapeFissureParams, maillageFissureParams):
  """
  #TODO: a compléter
  """
  logging.info('start')

  geometrieSaine      = geometriesSaines[0]
  maillageSain        = maillagesSains[0]
  isHexa              = maillagesSains[1]
  shapeDefaut         = shapesFissure[0]
  tailleDefaut        = shapesFissure[2]
  coordsNoeudsFissure = shapesFissure[3]

  isElliptique = False
  if 'elliptique' in shapeFissureParams:
    isElliptique      = shapeFissureParams['elliptique']
  if isElliptique:
    if 'demiGrandAxe' in shapeFissureParams:
      demiGrandAxe    = shapeFissureParams['demiGrandAxe']
    else:
      demiGrandAxe    = shapeFissureParams['longueur']
    lgExtrusion = 2.0*demiGrandAxe
  else:
    lgExtrusion = 50.

  nomRep              = maillageFissureParams['nomRep']
  nomFicSain          = maillageFissureParams['nomFicSain']

  fichierMaillageSain    = nomRep + '/' + nomFicSain + '.med'
  
  # --- centre de fond de fissure et tangente
  
  edgeFondExt, centreFondFiss, tgtCentre = getCentreFondFiss(shapesFissure)
  

  # --- zone de défaut
  nomZones = "zoneDefaut"

  [origShapes, verticesShapes, dmoyen] = \
    creeZoneDefautMaillage(maillagesSains, shapeDefaut, tailleDefaut, nomZones, coordsNoeudsFissure)

  maillageSain.ExportMED(fichierMaillageSain)
  logging.debug("fichier maillage sain %s", fichierMaillageSain)
  [maillageSain, internalBoundary, zoneDefaut, zoneDefaut_skin, zoneDefaut_internalFaces, zoneDefaut_internalEdges] = \
    peauInterne(fichierMaillageSain, shapeDefaut, nomZones)

  facesDefaut = []
  centresDefaut = []
  normalsDefaut =[]
  extrusionsDefaut = []
  isPlane = False
  if isHexa and not isPlane:
    meshQuad =  smesh.CopyMesh( zoneDefaut_skin, 'meshQuad', 0, 0)
    
    fillings, noeuds_bords, bordsPartages, fillconts, idFilToCont = quadranglesToShapeNoCorner(meshQuad, shapeFissureParams, centreFondFiss)
    
    for filling in fillings:
      [faceDefaut, centreDefaut, normalDefaut, extrusionDefaut] = \
        creeZoneDefautFilling(filling, shapeDefaut, lgExtrusion)
      facesDefaut.append(faceDefaut)
      centresDefaut.append(centreDefaut)
      normalsDefaut.append(normalDefaut)
      extrusionsDefaut.append(extrusionDefaut)
  else:
    [facesDefaut, centreDefaut, normalDefaut, extrusionDefaut] = \
      creeZoneDefautGeom( geometrieSaine, shapeDefaut, origShapes, verticesShapes, dmoyen, lgExtrusion)
    bordsPartages = []
    for face in facesDefaut:
      bordsPartages.append([None,None]) # TODO : traitement des arêtes vives ?
    fillconts = facesDefaut
    idFilToCont = list(range(len(facesDefaut)))

  return [facesDefaut, centresDefaut, normalsDefaut, extrusionsDefaut, dmoyen, bordsPartages, fillconts, idFilToCont,
          maillageSain, internalBoundary, zoneDefaut, zoneDefaut_skin, zoneDefaut_internalFaces, zoneDefaut_internalEdges,
          edgeFondExt, centreFondFiss, tgtCentre]

