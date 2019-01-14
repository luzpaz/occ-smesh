# -*- coding: utf-8 -*-

import logging
from .geomsmesh import geompy
from .geomsmesh import geomPublish
from .geomsmesh import geomPublishInFather
from . import initLog

# -----------------------------------------------------------------------------
# --- identification des faces tore et fissure dans le solide hors tore du bloc partitionne

def facesToreInBloc(blocp, facefissoutore, facetore1, facetore2):
  """
  identification des faces tore et fissure dans le bloc partitionne : sous shapes du bloc
  @param blocp : bloc partitionne
  @param facefissoutore : la face de fissure externe au tore
  @param facetore1 : face du tore selon la generatrice
  @param facetore2 : face du tore selon la generatrice
  @return (blocFaceFiss, blocFaceTore1, blocFaceTore2) sous shapes reperees
  """
  logging.info('start')

  blocFaceFiss = geompy.GetInPlaceByHistory(blocp, facefissoutore)
  blocFaceTore1 = geompy.GetInPlaceByHistory(blocp, facetore1)
  blocFaceTore2 = geompy.GetInPlaceByHistory(blocp, facetore2)

  geomPublishInFather(initLog.debug, blocp, blocFaceFiss,'blocFaceFiss')
  geomPublishInFather(initLog.debug, blocp, blocFaceTore1,'blocFaceTore1')
  geomPublishInFather(initLog.debug, blocp, blocFaceTore2,'blocFaceTore2')

  return blocFaceFiss, blocFaceTore1, blocFaceTore2

