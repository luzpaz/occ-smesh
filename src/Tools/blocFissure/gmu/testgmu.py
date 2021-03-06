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

from blocFissure import gmu
from blocFissure.gmu.initEtude import initEtude
initEtude()
from blocFissure.gmu.triedreBase import triedreBase
O, OX, OY, OZ = triedreBase()

from blocFissure.gmu.distance2 import distance2
a=[10, 20, 30]
b=[5, 7, 3]
c=distance2(a,b)

import unittest
from blocFissure.gmu import initLog
initLog.setUnitTests()

from blocFissure.gmu import distance2

suite = unittest.TestLoader().loadTestsFromTestCase(distance2.Test_distance2)
unittest.TextTestRunner(verbosity=2).run(suite)


