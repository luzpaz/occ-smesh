# Free borders

# initialize SALOME and modules
import salome, SMESH
salome.salome_init()
from salome.geom import geomBuilder
geompy = geomBuilder.New(salome.myStudy)
from salome.smesh import smeshBuilder
smesh =  smeshBuilder.New(salome.myStudy)

# create mesh
face = geompy.MakeFaceHW(100, 100, 1, theName="quadrangle")
mesh = smesh.Mesh(face)
mesh.Segment().NumberOfSegments(10)
mesh.Triangle().MaxElementArea(25)
mesh.Compute()

# get all free borders
filter = smesh.GetFilter(SMESH.EDGE, SMESH.FT_FreeBorders)
ids = mesh.GetIdsFromFilter(filter)
print("Number of edges on free borders:", len(ids))
