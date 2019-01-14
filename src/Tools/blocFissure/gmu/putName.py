# -*- coding: utf-8 -*-

from .geomsmesh import smesh

# -----------------------------------------------------------------------------
# --- nommage des objets mesh (algorithme, hypothese, subMesh)

def putName(objmesh,name, i=-1):
  if i >= 0:
    suffix = "_%d"%i
    name += suffix
  smesh.SetName(objmesh, name)

