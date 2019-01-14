# -*- coding: utf-8 -*-

import logging
from .geomsmesh import geompy

# -----------------------------------------------------------------------------
# --- prolongation des segments extremite des polylines, pour la decoupe

def prolongeVertices(vertices):
  """
  Prolongation des segments extremite d'une polyline definie par un vecteur de points.
  Chaque nouvelle extremite est obtenue par symetrie point du voisin de cette ancienne extremite
  (symetrie de centre l'ancienne extremite) : les segments extremes sont doubles.
  @param vertices : liste ordonnee des points (geomObject) de la polyline
  @return vertices : liste avec les deux extremites modifiees
  """
  logging.info("start")
  if len(vertices) < 2:
    return vertices
  v0 = vertices[0]
  v1 = vertices[1]
  m0 = geompy.MakeMirrorByPoint(v1, v0)
  ve = vertices[-1]
  vd = vertices[-2]
  m1 = geompy.MakeMirrorByPoint(vd, ve)
  vertices[0] = m0
  vertices[-1] = m1
  return vertices
