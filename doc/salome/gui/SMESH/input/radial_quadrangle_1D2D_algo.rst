.. _radial_quadrangle_1D2D_algo_page:

***********************
Radial Quadrangle 1D-2D
***********************

This algorithm applies to the meshing of 2D shapes under the
following conditions: the face must be a full ellipse or a part of ellipse
(i.e. the number of edges is less or equal to 3 and one of them is an ellipse curve).
The resulting mesh consists of triangles (near the center point) and
quadrangles.

This algorithm is optionally parametrized by the hypothesis indicating
the number of mesh layers along the radius. The distribution of layers
can be set with any 1D Hypothesis. If the face boundary includes
radial edges, this distribution is applied to the longest radial
edge. If the face boundary does not include radial edges, this
distribution is applied to the longest virtual radial edge. The
distribution is applied to the longest radial edge starting from its
end lying on the elliptic curve.


If no own hypothesis of the algorithm is assigned, any local or global
hypothesis is used by the algorithm to discretize edges.

If no 1D hypothesis is assigned to an edge, 
:ref:`Default Number of Segments <nb_segments_pref>` preferences
parameter is used to discretize the edge.

.. image:: ../images/hypo_radquad_dlg.png
	:align: center

.. image:: ../images/mesh_radquad_01.png 
	:align: center

.. centered::
	Radial Quadrangle 2D mesh on the top and the bottom faces of a cylinder

.. image:: ../images/mesh_radquad_02.png 
	:align: center

.. centered::
	Radial Quadrangle 2D mesh on a part of circle

**See also** a sample :ref:`TUI Script <tui_radial_quadrangle>`.


