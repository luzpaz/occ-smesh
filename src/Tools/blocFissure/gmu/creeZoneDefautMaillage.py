# -*- coding: utf-8 -*-

import logging
from .geomsmesh import geompy
import math
from .distance2 import distance2
import traceback
from .fissError import fissError

# -----------------------------------------------------------------------------
# --- zone de defaut extraite du maillage

def creeZoneDefautMaillage(maillagesSains, shapeDefaut, tailleDefaut,
                           nomZones, coordsNoeudsFissure):
  """
  Identification de la zone a remailler, operations sur le maillage
  de l'objet sain.
  La zone a remailler est definie a partir d'un objet geometrique
  ou a partir d'un jeu de points et d'une distance d'influence.
  @param maillagesSains : (le maillage de l'objet initial, booleen isHexa)
  @param shapeDefaut : objet geometrique representant la fissure
  (selon les cas, un point central, ou une shape plus complexe,
  dont on ne garde que les vertices)
  @param tailleDefaut : distance d'influence definissant la zone a remailler:
  tous les elements du maillage initial qui penetrent dans cette zone
  sont detectes
  @param nomZones : prefixe des noms de groupes crees dans le maillage initial. S'il y a un groupe de noeuds
  @coordsNoeudsFissure : jeu de points donne par une liste (x1,y1,z1, x2,y2,z2, ...)
  @return (origShapes, verticesShapes, dmoyen) liste id subShapes,
  listes noeuds de bord, longueur arete moyenne bord
  """
  logging.info("start")
  
  maillageSain = maillagesSains[0]
  isHexa = maillagesSains[1]
  lists = maillageSain.CreateHoleSkin(tailleDefaut, shapeDefaut, nomZones, coordsNoeudsFissure)

  logging.debug("lists=%s", lists)

  trace = True
  origShapes = []
  verticesShapes = []

  cumul = 0 # somme des distances carrees entre point ordonnes (taille des aretes)
  nb = 0    # nombre d'aretes evaluees

  for aList in lists:
    aShape = aList[0]
    origShapes.append(aShape)
    logging.debug("  shapeId %s", aShape)
    vertices = []
    xyz0 = None
    for inode in range(1, len(aList)):
      xyz = maillageSain.GetNodeXYZ(aList[inode])
      if xyz0 is not None:
        cumul += distance2(xyz, xyz0)
        nb += 1
      xyz0 = xyz
      #logging.debug("    node %s %s", aList[inode], xyz)
      vertices.append(geompy.MakeVertex(xyz[0], xyz[1], xyz[2]))
      pass
    verticesShapes.append(vertices)
    pass

  if (nb == 0) :
    texte = "La zone a remailler n'est pas detectee correctement.<br>"
    texte += "Cause possible :<ul>"
    texte += "<li>La distance d'influence est trop petite. "
    texte += "L'ordre de grandeur minimal correspond a la taille des mailles du maillage sain dans la zone a remailler.</li></ul>"
    raise fissError(traceback.extract_stack(),texte)

  dmoyen = math.sqrt(cumul/nb) # ~ taille de l'arete moyenne du maillage global
  return origShapes, verticesShapes, dmoyen
