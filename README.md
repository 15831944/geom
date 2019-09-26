geom
====

CGAL wrapper and utilities.

Wrapper primary purpose is to increase compilation speed by reducing the number of template instances that need to be
instanciated.

Utilities include volume calculation, primitive construction (imported from DOLFIN), and query functions.

Building
--------

Install CGAL 4.14, set `CGAL_DIR` to path containing CGAL when building.
