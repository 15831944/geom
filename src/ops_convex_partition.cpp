/* cxxcam - C++ CAD/CAM driver library.
 * Copyright (C) 2019  Nicholas Gill
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
 * ops_convex_partition.cpp
 *
 *  Created on: 11/05/2019
 *	  Author: nicholas
 */
#include "ops.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/partition_2.h>
#include <vector>

// Convex partitioning
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Partition_traits_2<Kernel>                    Traits;
typedef Traits::Polygon_2                                   Polygon_2;
typedef Traits::Point_2                                     Point_2;

namespace geom
{

std::vector<polygon_t> convex_partition(const polygon_t& polygon) {
	Polygon_2 poly;
	for (auto& p : polygon.points)
		poly.push_back(Point_2(p.x, p.y));

	if (poly.is_convex())
		return { polygon };

	std::vector<Polygon_2> result;
	CGAL::optimal_convex_partition_2(poly.vertices_begin(), poly.vertices_end(), std::back_inserter(result));

    auto to_poly = [] (const Polygon_2& P) {
        polygon_t p;
        p.points.reserve(P.size());
        for(auto vi = P.vertices_begin(); vi != P.vertices_end(); ++vi)
            p.points.push_back({ vi->x(), vi->y() });
        return p;
    };

	std::vector<polygon_t> convex;
	for (auto& poly : result) {
		convex.push_back(to_poly(poly));
	}
	return convex;
}

}

