# -*- coding: utf-8 -*-

import logging
from .geomsmesh import geompy

# -----------------------------------------------------------------------------
# --- teste si l'operation de partition a produit une modification

def checkDecoupePartition(shapes, part):
  """
  Teste si l'operation de partition a produit une decoupe
  (plus de shapes dans la partition).
  Resultat non garanti si recouvrement des shapes d'origine.
  @param shapes : liste des shapes d'origine
  @param part : resultat de la partition
  @return True si la partition a decoupe les shapes d'origine
  """
  logging.info('start')
  # TODO: ShapeInfo donne des resultats faux (deux faces au lieu de une)
  
  isPart = False
  orig = {}
  for shape in shapes:
    info = geompy.ShapeInfo(shape)
    logging.debug("shape info %s", info)
    for k in ['VERTEX', 'EDGE', 'FACE', 'SOLID']:
      if k in list(orig.keys()):
        orig[k] += info[k]
      else:
        orig[k] = info[k]
  logging.debug("original shapes info %s", orig)
  info = geompy.ShapeInfo(part)
  logging.debug("partition info %s", info)
  for k in ['VERTEX', 'EDGE', 'FACE', 'SOLID']:
    if orig[k] < info[k]:
      isPart = True
      break
  logging.debug("partition modifie l'original %s", isPart)

  return isPart

