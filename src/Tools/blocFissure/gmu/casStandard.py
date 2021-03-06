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

import os
from .geomsmesh import geompy, smesh
from .geomsmesh import geomPublish
from .geomsmesh import geomPublishInFather
from . import initLog

import math
import GEOM
import SALOMEDS
import SMESH
#import StdMeshers
#import GHS3DPlugin
#import NETGENPlugin
import logging

from .fissureGenerique import fissureGenerique

from .initEtude import initEtude
from .triedreBase import triedreBase
from .genereMeshCalculZoneDefaut import genereMeshCalculZoneDefaut
from .creeZoneDefautDansObjetSain import creeZoneDefautDansObjetSain
from .construitFissureGenerale import construitFissureGenerale

O, OX, OY, OZ = triedreBase()

class casStandard(fissureGenerique):
  """
  problème de fissure standard, défini par :
  - un maillage sain (hexaèdres),
  - une face géométrique de fissure, qui doit légèrement dépasser hors du volume maillé
  - les numéros d'arêtes (edges géométriques) correspondant au fond de fissure
  - les paramètres de maillage de la fissure
  """

  # ---------------------------------------------------------------------------
  def __init__ (self, dicoParams, references = None, numeroCas = 0):
    initEtude()
    self.references = references
    self.dicoParams = dicoParams
    if 'nomCas' in self.dicoParams:
      self.nomCas = self.dicoParams['nomCas']
    elif 'nomres' in self.dicoParams:
      self.nomCas = os.path.splitext(os.path.split(self.dicoParams['nomres'])[1])[0]
    else:
      self.nomCas = 'casStandard'
    if 'reptrav' in self.dicoParams:
      self.reptrav = self.dicoParams['reptrav']
    else:
      self.reptrav = '.'  
    self.numeroCas = numeroCas
    if self.numeroCas != 0:
      self.nomCas = self.nomProbleme +"_%d"%(self.numeroCas)
    else:
      self.nomProbleme = self.nomCas
    if 'lenSegPipe' in self.dicoParams:
      self.lenSegPipe = self.dicoParams['lenSegPipe']
    else:
      self.lenSegPipe =self.dicoParams['rayonPipe']
    if 'step' in self.dicoParams:
      step = self.dicoParams['step']
    else:
      step = -1 # exécuter toutes les étapes
    if 'aretesVives' not in self.dicoParams:
      self.dicoParams['aretesVives'] = 0
    if self.numeroCas == 0: # valeur par défaut : exécution immédiate, sinon execution différée dans le cas d'une liste de problèmes
      self.executeProbleme(step)
    
  # ---------------------------------------------------------------------------
  def genereMaillageSain(self, geometriesSaines, meshParams):
    logging.info("genereMaillageSain %s", self.nomCas)

    ([objetSain], status) = smesh.CreateMeshesFromMED(self.dicoParams['maillageSain'])
    smesh.SetName(objetSain.GetMesh(), 'objetSain')

    return [objetSain, True] # True : maillage hexa

  # ---------------------------------------------------------------------------
  def setParamShapeFissure(self):
    """
    paramètres de la fissure pour méthode insereFissureGenerale
    lgInfluence : distance autour de la shape de fissure a remailler (A ajuster selon le maillage)
    rayonPipe   : le rayon du pile maillé en hexa autour du fond de fissure
    convexe     : True : la face est convexe (vue de l'exterieur) sert si on ne donne pas de point interne
    pointIn_x   : optionnel : coordonnée x d'un point dans le solide sain (pour orienter la face - idem avec y,z)
    """
    logging.info("setParamShapeFissure %s", self.nomCas)
    if 'pointInterieur' in self.dicoParams:
      self.shapeFissureParams = dict(lgInfluence = self.dicoParams['lgInfluence'],
                                     rayonPipe   = self.dicoParams['rayonPipe'],
                                     lenSegPipe  = self.lenSegPipe,
                                     pointIn_x   = self.dicoParams['pointInterieur'][0],
                                     pointIn_y   = self.dicoParams['pointInterieur'][1],
                                     pointIn_z   = self.dicoParams['pointInterieur'][2])
    else:
      self.shapeFissureParams = dict(lgInfluence = self.dicoParams['lgInfluence'],
                                     rayonPipe   = self.dicoParams['rayonPipe'],
                                     lenSegPipe  = self.lenSegPipe)

  # ---------------------------------------------------------------------------
  def genereShapeFissure( self, geometriesSaines, geomParams, shapeFissureParams):
    logging.info("genereShapeFissure %s", self.nomCas)

    lgInfluence = shapeFissureParams['lgInfluence']

    shellFiss = geompy.ImportFile( self.dicoParams['brepFaceFissure'], "BREP")
    fondFiss = geompy.CreateGroup(shellFiss, geompy.ShapeType["EDGE"])
    geompy.UnionIDs(fondFiss, self.dicoParams['edgeFissIds'] )
    geomPublish(initLog.debug, shellFiss, 'shellFiss' )
    geomPublishInFather(initLog.debug, shellFiss, fondFiss, 'fondFiss' )


    coordsNoeudsFissure = genereMeshCalculZoneDefaut(shellFiss, self.dicoParams['meshBrep'][0] ,self.dicoParams['meshBrep'][1])

    centre = None
    return [shellFiss, centre, lgInfluence, coordsNoeudsFissure, fondFiss]

  # ---------------------------------------------------------------------------
  def setParamMaillageFissure(self):
    self.maillageFissureParams = dict(nomRep           = self.reptrav,
                                      nomFicSain       = self.nomCas +'_sain',
                                      nomFicFissure    = self.nomCas,
                                      nbsegRad         = self.dicoParams['nbSegRad'],
                                      nbsegCercle      = self.dicoParams['nbSegCercle'],
                                      areteFaceFissure = self.dicoParams['areteFaceFissure'],
                                      aretesVives      = self.dicoParams['aretesVives'])

  # ---------------------------------------------------------------------------
  def genereZoneDefaut(self, geometriesSaines, maillagesSains, shapesFissure, shapeFissureParams, maillageFissureParams):
    elementsDefaut = creeZoneDefautDansObjetSain(geometriesSaines, maillagesSains, shapesFissure, shapeFissureParams, maillageFissureParams)
    return elementsDefaut

  # ---------------------------------------------------------------------------
  def genereMaillageFissure(self, geometriesSaines, maillagesSains,
                            shapesFissure, shapeFissureParams,
                            maillageFissureParams, elementsDefaut, step):
    maillageFissure = construitFissureGenerale(maillagesSains,
                                              shapesFissure, shapeFissureParams,
                                              maillageFissureParams, elementsDefaut, step)
    return maillageFissure

  # ---------------------------------------------------------------------------
  def setReferencesMaillageFissure(self):
    if self.references is not None:
      self.referencesMaillageFissure = self.references
    else:
      self.referencesMaillageFissure = dict(Entity_Quad_Pyramid    = 0,
                                            Entity_Quad_Triangle   = 0,
                                            Entity_Quad_Edge       = 0,
                                            Entity_Quad_Penta      = 0,
                                            Entity_Quad_Hexa       = 0,
                                            Entity_Node            = 0,
                                            Entity_Quad_Tetra      = 0,
                                            Entity_Quad_Quadrangle = 0)

