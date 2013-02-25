# Uniting a Set of Triangles

import salome
import smesh

# create an empty mesh structure
mesh = smesh.Mesh() 

# create the following mesh:
# .----.----.----.
# |   /|   /|   /|
# |  / |  / |  / |
# | /  | /  | /  |
# |/   |/   |/   |
# .----.----.----.

bb = [0, 0, 0, 0]
tt = [0, 0, 0, 0]
ff = [0, 0, 0, 0, 0, 0]

bb[0] = mesh.AddNode( 0., 0., 0.)
bb[1] = mesh.AddNode(10., 0., 0.)
bb[2] = mesh.AddNode(20., 0., 0.)
bb[3] = mesh.AddNode(30., 0., 0.)

tt[0] = mesh.AddNode( 0., 15., 0.)
tt[1] = mesh.AddNode(10., 15., 0.)
tt[2] = mesh.AddNode(20., 15., 0.)
tt[3] = mesh.AddNode(30., 15., 0.)

ff[0] = mesh.AddFace([bb[0], bb[1], tt[1]])
ff[1] = mesh.AddFace([bb[0], tt[1], tt[0]])
ff[2] = mesh.AddFace([bb[1], bb[2], tt[2]])
ff[3] = mesh.AddFace([bb[1], tt[2], tt[1]])
ff[4] = mesh.AddFace([bb[2], bb[3], tt[3]])
ff[5] = mesh.AddFace([bb[2], tt[3], tt[2]])

# unite a set of triangles
print "\nUnite a set of triangles ... ",
res = mesh.TriToQuad([ff[2], ff[3], ff[4], ff[5]], smesh.FT_MinimumAngle, 60.)
if not res: print "failed!"
else:       print "done."

salome.sg.updateObjBrowser(1)