.. _filters_page: 

*************
About filters
*************

**Filters** allow picking only the mesh elements satisfying to a specific condition or a set of conditions. Filters can be used to create or edit mesh groups, remove elements from the mesh, control mesh quality by different parameters, etc.

Several criteria can be combined together by using logical operators *AND* and *OR*. In addition, a filter criterion can be reverted using logical operator *NOT*.

Some filtering criteria use the functionality of :ref:`mesh quality controls <quality_page>` to filter mesh nodes / elements by specific characteristic (Area, Length, etc).

The functionality of mesh filters is available in both GUI and TUI modes:

* In GUI, filters are available in some dialog boxes via "Set Filters" button, clicking on which opens the :ref:`dialog box <filtering_elements>` allowing to specify the list of filter criteria to be applied to the current selection. See :ref:`selection_filter_library_page` page to learn more about selection filters and their usage in GUI.

* In Python scripts, filters can be used to choose only some mesh nodes or elements for the operations, which require the list of entities as input parameter (create/modify group, remove nodes/elements, etc) and for the operations, which accept objects (groups, sub-meshes) as input parameter. The page :ref:`tui_filters_page` provides examples of the filters usage in Python scripts.

.. toctree::
   :maxdepth: 2
   :hidden:

   selection_filter_library.rst
