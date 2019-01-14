# -*- coding: utf-8 -*-

import logging
from .geomsmesh import geompy
from .geomsmesh import geomPublish
from .geomsmesh import geomPublishInFather
from . import initLog
from .checkDecoupePartition import checkDecoupePartition

  # -----------------------------------------------------------------------------
  # --- peau et face de fissure
  #
  # --- partition peau defaut - face de fissure prolongee - wire de fond de fissure prolongee
  #     il peut y avoir plusieurs faces externes, dont certaines sont decoupees par la fissure
  #     liste de faces externes : facesDefaut
  #     liste de partitions face externe - fissure : partitionPeauFissFond (None quand pas d'intersection)

def construitPartitionsPeauFissure(facesDefaut, fissPipe):
  """
  partition peau defaut - face de fissure prolongee - wire de fond de fissure prolongee.
  Il peut y avoir plusieurs faces externes, dont certaines sont decoupees par la fissure.
  @param facesDefaut liste de faces externes
  @param fissPipe    partition face de fissure etendue par pipe prolonge
  @return partitionsPeauFissFond : liste de partitions face externe - fissure (None quand pas d'intersection)
  """
  
  logging.info('start')
  partitionsPeauFissFond = []
  ipart = 0
  for filling in facesDefaut: 
    part = geompy.MakePartition([fissPipe, filling], [], [], [], geompy.ShapeType["FACE"], 0, [], 0)
    isPart = checkDecoupePartition([fissPipe, filling], part)
    if isPart: # on recree la partition avec toutes les faces filling en outil pour avoir une face de fissure correcte
      otherFD = [fd for fd in facesDefaut if fd != filling]
      if len(otherFD) > 0:
        fissPipePart = geompy.MakePartition([fissPipe], otherFD, [], [], geompy.ShapeType["FACE"], 0, [], 0)
      else:
        fissPipePart = fissPipe
      part = geompy.MakePartition([fissPipePart, filling], [], [], [], geompy.ShapeType["FACE"], 0, [], 0)
      partitionsPeauFissFond.append(part)
      geomPublish(initLog.debug, part, 'partitionPeauFissFond%d'%ipart )
    else:
      partitionsPeauFissFond.append(None)
    ipart = ipart +1

  return partitionsPeauFissFond
