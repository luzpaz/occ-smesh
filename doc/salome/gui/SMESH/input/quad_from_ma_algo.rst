.. _quad_from_ma_algo_page:

***************************************************
Medial Axis Projection Quadrangle meshing algorithm
***************************************************

Medial Axis Projection algorithm can be used for meshing faces with
sinuous borders and a channel-like shape, for which it can be
difficult to define 1D hypotheses such that to obtain a good shape of
resulting quadrangles. The algorithm can be also applied to faces with ring
topology, which can be viewed as a closed 'channel'. In the latter
case radial discretization of a ring can be specified by
using *Number of Layers* or *Distribution of Layers*
hypothesis.

.. image:: ../images/quad_from_ma_mesh.png 
	:align: center

.. centered::
	A mesh of a river model (to the left) and of a ring-face (to the right)

The algorithm provides proper shape of quadrangles by constructing Medial
Axis between sinuous borders of the face and using it to
discretize the borders. (Shape of quadrangles can be not perfect at
locations where opposite sides of a 'channel' are far from being parallel.)

.. image:: ../images/quad_from_ma_medial_axis.png 
	:align: center

.. centered::
	Medial Axis between two blue sinuous borders

The Medial Axis is used in two ways:

#. If there is a sub-mesh on a sinuous border, then the nodes of this border are mapped to the opposite border via the Medial Axis.
#. If there are no sub-meshes on sinuous borders, then the part of the Medial Axis that can be mapped to both borders is discretized using a 1D hypothesis assigned to the face or its ancestor shapes, and the division points are mapped from the Medial Axis to both borders to find positions of nodes.

.. image:: ../images/quad_from_ma_ring_mesh.png 
	:align: center

.. centered::
	Mesh depends on defined sub-meshes: to the left - sub-meshes on both wires, to the right - a sub-mesh on internal wire only


**See Also** a sample TUI Script of a :ref:`tui_quad_ma_proj_algo`.

