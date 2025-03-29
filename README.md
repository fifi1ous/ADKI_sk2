# 155ADKI Algorithms of Digital Cartography and GIS

## Description
This repository contains projects for the course **[155ADKI Algorithms of Digital Cartography and GIS](https://geo.fsv.cvut.cz/gwiki/155ADKI_Algoritmy_digitální_kartografie_a_GIS)**.

## Authors
#### Group 2
- **[Michal Kovář](https://github.com/kovarmi9)**
- **[Filip Roučka](https://github.com/fifi1ous)**

## Repository Contents

### [U1 - Geometric Point Search](https://github.com/fifi1ous/ADKI_sk2/tree/main/U1)
#### [Assignment](https://web.natur.cuni.cz/~bayertom/images/courses/Adk/adkcv1.pdf)
This folder contains the implementation of algorithms to solve the **Point Location Problem**. using the following methods:
- **Ray Crossing Algorithm**: Determines point location by counting how many times a ray from the point intersects the polygon's edges. Odd intersections mean the point is inside, even intersections mean outside.
- **Winding Number Algorithm**: Determines point location by calculating the total angle formed by the polygon edges around the point. A non-zero angle means inside, zero means outside.
- **Handling Special Cases**: Handles special cases when a point is on an edge or at a vertex.
- **Optimized Polygon Search**: Uses bounding box filtering for faster identification of relevant polygons.
- **Support for Polygons with Holes**: Handles polygons with internal holes.
- **Loading shpefiles**: Uses **[Shapelib](https://shapelib.maptools.org/)** to process shapefiles.
- **Graphical User Interface (GUI)**: Built with **[Qt Framework](https://qt.io/)** for interactive visualization.

### [U2 - Building Simplification](https://github.com/fifi1ous/ADKI_sk2/tree/main/U2)
#### [Assignment](https://web.natur.cuni.cz/~bayertom/images/courses/Adk/adkcv2.pdf)
This folder contains the implementation of algorithms to solve the **Building Simplification**. using the following methods:
