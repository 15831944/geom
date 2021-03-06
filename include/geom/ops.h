/* cxxcam - C++ CAD/CAM driver library.
 * Copyright (C) 2013  Nicholas Gill
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * ops.h
 *
 *  Created on: 06/05/2013
 *      Author: nicholas
 */

#ifndef GEOM_OPS_H_
#define GEOM_OPS_H_
#include <vector>

namespace geom
{

class polyhedron_t;

struct polyline_t
{
	struct point
	{
		double x;
		double y;
		double z;
	};
	std::vector<point> line;
};

struct polygon_t
{
	struct point
	{
		double x;
		double y;
	};
    std::vector<point> points;
};
struct polygon_tree_t
{
    std::vector<polygon_t> polygons;
};

polyhedron_t glide(const polyhedron_t& polyhedron, const polyline_t& path);
double volume(const polyhedron_t& polyhedron);
polyhedron_t merge(const std::vector<polyhedron_t>& polyhedra);
polyhedron_t subdivide(const polyhedron_t& polyhedron, unsigned step);
std::vector<polygon_tree_t> projection_xy(const polyhedron_t& polyhedron);
std::vector<polygon_t> convex_partition(const polygon_t& polygon);

}

#endif /* GEOM_OPS_H_ */
