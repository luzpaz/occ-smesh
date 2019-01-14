# -*- coding: utf-8 -*-

import logging
from .geomsmesh import geompy
from .geomsmesh import geomPublish
from .geomsmesh import geomPublishInFather
from . import initLog
import bisect

publie = False

def getCentreFondFiss(shapesFissure):
  """
  identification du centre de fond de fissure,
  transformation fond de fissure en edge unique (seulement pour la procedure construitFissureGenerale).
  On distingue le cas d'utilisation de la procedure insereFissureLongue par le nombre d'elements de shapesFissure.
  """
  global publie
  logging.debug("start")
  
  fondFiss          = shapesFissure[4] # groupe d'edges de fond de fissure
  if len(shapesFissure) == 6:          # procedure construitFissureGenerale, et edge fond de fissure fournie explicitement
    edgeFondExt     = shapesFissure[5]
  else:
    edgeFondExt     = None
  
  if len(shapesFissure) > 6:           # procedure insereFissureLongue (fissure plane, plusieurs edges sur le fond de fissure)
    centreFondFiss = shapesFissure[1]
    tgtCentre = None
  else:                                # procedure construitFissureGenerale, determination edge unique et milieu de l'edge
    if geompy.NumberOfEdges(fondFiss) > 1:
      if geompy.NbShapes(fondFiss, geompy.ShapeType["WIRE"]) > 0: # wire
        aWire = fondFiss
      else:                                                       # compound
        edges = geompy.ExtractShapes(fondFiss, geompy.ShapeType["EDGE"], False)
        aWire = geompy.MakeWire(edges, 1e-07)
    else:
      edgeFondExt = fondFiss
      aWire = geompy.MakeWire([fondFiss], 1e-07)
    if not publie:
      geomPublish(initLog.debug, aWire, "wireFondFissExt")
        
    lgWire = geompy.BasicProperties(aWire)[0]
    edges = geompy.ExtractShapes(aWire, geompy.ShapeType["EDGE"], True)
    lgEdges = [geompy.BasicProperties(ed)[0] for ed in edges]
    lgSumEd =[sum(lgEdges[0:i+1]) for i in range(len(lgEdges))]
    iedr = bisect.bisect_left(lgSumEd, lgWire/2.0)
    iedg = iedr -1
    if iedg < 0:
      lgOnEdge = lgWire/2.0
    else:
      lgOnEdge = lgWire/2.0 - lgSumEd[iedg]
    logging.debug("lgsumEdges %s", lgSumEd)
    logging.debug("id edge: %s, lgOnEdge: %s, lgEdge: %s",iedr, lgOnEdge, lgEdges[iedr])
    if iedr > 0: # il y a une edge avant celle du milieu
      if geompy.MinDistance(edges[iedr-1], geompy.MakeVertexOnCurve(edges[iedr], 0.0 )) < 1.e-3: # edge orientee croissante 
        centreFondFiss = geompy.MakeVertexOnCurve(edges[iedr], lgOnEdge/lgEdges[iedr])
      else:
        centreFondFiss = geompy.MakeVertexOnCurve(edges[iedr], 1.0 - lgOnEdge/lgEdges[iedr])
    elif iedr < len(edges)-1: # il y a une edge apres celle du milieu
      if geompy.MinDistance(edges[iedr+1], geompy.MakeVertexOnCurve(edges[iedr], 1.0 )) < 1.e-3: # edge orientee croissante
        centreFondFiss = geompy.MakeVertexOnCurve(edges[iedr], lgOnEdge/lgEdges[iedr])
      else:
        centreFondFiss = geompy.MakeVertexOnCurve(edges[iedr], 1.0 - lgOnEdge/lgEdges[iedr])
    else: # on ne sait pas comment est orientee l'edge unique, mais ça n'a pas d'importance
      centreFondFiss = geompy.MakeVertexOnCurve(edges[iedr], lgOnEdge/lgEdges[iedr])
    geomPublishInFather(initLog.debug,aWire, centreFondFiss, "centreFondFiss")
    tgtCentre = geompy.MakeTangentOnCurve(edges[iedr], lgOnEdge/ lgEdges[iedr])
    
    if edgeFondExt is None: # fond de fissure non fourni explicitement sous forme d'edge
      try:
        edgeFondExt = geompy.MakeEdgeWire(aWire, 0.0005, 1e-07)
      except:
        logging.debug("erreur MakeEdgeWire sur fond de fissure, on fait sans")
        edgeFondExt = None
    if not publie and edgeFondExt is not None:
      geomPublish(initLog.debug, edgeFondExt, "edgeFondExt")
  
  publie = True
  return edgeFondExt, centreFondFiss, tgtCentre
