# Using SALOME NoteBook

import geompy
import smesh
import salome_notebook

# set variables
notebook = salome_notebook.notebook
notebook.set("Length", 100)
notebook.set("Width", 200)
notebook.set("Offset", 50)

notebook.set("NbSegments", 7)
notebook.set("MaxElementArea", 800)
notebook.set("MaxElementVolume", 900)

# create a box
box = geompy.MakeBoxDXDYDZ("Length", "Width", 300)
idbox = geompy.addToStudy(box, "Box")

# create a mesh
tetra = smesh.Mesh(box, "MeshBox")

algo1D = tetra.Segment()
algo1D.NumberOfSegments("NbSegments")

algo2D = tetra.Triangle()
algo2D.MaxElementArea("MaxElementArea")

algo3D = tetra.Tetrahedron()
algo3D.MaxElementVolume("MaxElementVolume")

# compute the mesh
ret = tetra.Compute()

# translate the mesh
point = smesh.PointStruct("Offset", 0., 0.)
vector = smesh.DirStruct(point) 
tetra.TranslateObject(tetra, vector, 0)
