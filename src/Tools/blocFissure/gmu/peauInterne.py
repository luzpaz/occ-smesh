# -*- coding: utf-8 -*-

import logging
from .geomsmesh import smesh
import SMESH
import traceback
from .fissError import fissError

from .listOfExtraFunctions import lookForCorner
from .fusionMaillageAttributionDefaut import fusionMaillageDefaut

# -----------------------------------------------------------------------------
# --- peau interne du defaut dans le maillage sain

def peauInterne(fichierMaillage, shapeDefaut, nomZones):
  """
  Retrouve les groupes de defaut dans le maillage sain modifie par CreateHoleSkin (CreeZoneDefautMaillage)
  On recupere le volume et la peau de la zone de defaut, les eventuelles faces et aretes internes de cette zone.
  """
  logging.info("start")
  ([maillageSain], status) = smesh.CreateMeshesFromMED(fichierMaillage)

  groups = maillageSain.GetGroups()
  zoneDefaut = None
  for grp in groups:
    logging.debug("groupe %s",grp.GetName())
    if grp.GetName() == nomZones + "_vol":
      zoneDefaut = grp
      break
  zoneDefaut_skin = None
  for grp in groups:
    if grp.GetName() == nomZones + "_skin":
      zoneDefaut_skin = grp
      break
  zoneDefaut_internalFaces = None
  for grp in groups:
    if grp.GetName() == nomZones + "_internalFaces":
      zoneDefaut_internalFaces = grp
      break
  zoneDefaut_internalEdges = None
  for grp in groups:
    if grp.GetName() == nomZones + "_internalEdges":
      zoneDefaut_internalEdges = grp
      break
    
  # --- Le groupe ZoneDefaut ne doit contenir que des Hexaedres"
  
  info=maillageSain.GetMeshInfo(zoneDefaut)
  keys = list(info.keys()); keys.sort()
  nbelem=0
  nbhexa=0
  for i in keys:
    #print "  %s  :  %d" % ( i, info[i] )
    nbelem+=info[i]
    if "Entity_Hexa" in str(i):
      nbhexa+=info[i]
  if (nbelem == 0) or (nbhexa < nbelem) :
    texte = "La zone a remailler est incorrecte.<br>"
    texte += "Causes possibles :<ul>"
    texte += "<li>Les mailles a enlever dans le maillage sain n'ont pas ete detectees.</li>"
    texte += "<li>Certaines faces du maillage sain sont a l'envers : les normales aux faces en paroi de volume doivent etre sortantes.</li>"
    texte += "<li>Il n'y a pas que des Hexaedres regles lineaires dans la zone a remailler (notamment mailles quadratiques, tetraedres non traites)</li></ul>"
    raise fissError(traceback.extract_stack(),texte)

  nbAdded, maillageSain, DefautBoundary = maillageSain.MakeBoundaryElements( SMESH.BND_2DFROM3D, 'DefBound', '', 0, [ zoneDefaut ])
  internal = maillageSain.GetMesh().CutListOfGroups( [ DefautBoundary ], [ zoneDefaut_skin ], 'internal' )
  internalBoundary = smesh.CopyMesh( internal, 'internalBoundary', 0, 0)
  
  maillageDefautCible = smesh.CopyMesh(zoneDefaut_skin, 'maillageCible', 0, 0)
  listOfCorner = lookForCorner(maillageDefautCible)
  logging.debug("listOfCorner = %s", listOfCorner)
  if len(listOfCorner) > 0:
      logging.info("presence de coins a la surface externe de la zone a reconstruire")
      zoneDefaut_skin, internalBoundary = fusionMaillageDefaut(maillageSain, maillageDefautCible, internalBoundary, zoneDefaut_skin, shapeDefaut, listOfCorner)

  return maillageSain, internalBoundary, zoneDefaut, zoneDefaut_skin, zoneDefaut_internalFaces, zoneDefaut_internalEdges

