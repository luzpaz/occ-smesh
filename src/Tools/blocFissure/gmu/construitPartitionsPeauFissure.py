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
from .checkDecoupePartition import checkDecoupePartition

  # -----------------------------------------------------------------------------
  # --- peau et face de fissure
  #
  # --- partition peau défaut - face de fissure prolongée - wire de fond de fissure prolongée
  #     il peut y avoir plusieurs faces externes, dont certaines sont découpées par la fissure
  #     liste de faces externes : facesDefaut
  #     liste de partitions face externe - fissure : partitionPeauFissFond (None quand pas d'intersection)

def construitPartitionsPeauFissure(facesDefaut, fissPipe):
  """
  partition peau défaut - face de fissure prolongée - wire de fond de fissure prolongée.
  Il peut y avoir plusieurs faces externes, dont certaines sont découpées par la fissure.
  @param facesDefaut liste de faces externes
  @param fissPipe    partition face de fissure etendue par pipe prolongé
  @return partitionsPeauFissFond : liste de partitions face externe - fissure (None quand pas d'intersection)
  """
  
  logging.info('start')
  partitionsPeauFissFond = []
  ipart = 0
  for filling in facesDefaut: 
    part = geompy.MakePartition([fissPipe, filling], [], [], [], geompy.ShapeType["FACE"], 0, [], 0)
    isPart = checkDecoupePartition([fissPipe, filling], part)
    if isPart: # on recrée la partition avec toutes les faces filling en outil pour avoir une face de fissure correcte
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