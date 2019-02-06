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
 * ops_projection.cpp
 *
 *  Created on: 05/02/2019
 *      Author: nicholas
 */
#include "ops.h"
#include "polyhedron.h"
#include "private.h"
#include "copy_to.h"
#include <utility>
#include <cassert>
#include <stdexcept>
#include <algorithm>

#include "cgal.h"

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Boolean_set_operations_2.h>

// Project
typedef CGAL::Polygon_2<Nef_Kernel> Polygon_2;
typedef CGAL::Polygon_with_holes_2<Nef_Kernel> Polygon_with_holes_2;

namespace geom
{

std::vector<polygon_t> projection_xy(const polyhedron_t& polyhedron) {
	auto P = to_Polyhedron_3(polyhedron);

    // Based on https://stackoverflow.com/questions/50467907/polyhedron-projection-to-xy-plane-with-cgal
    std::vector<Polygon_2> ii;
	for(auto fi = P.facets_begin(); fi != P.facets_end(); ++fi) {
		auto hc = fi->facet_begin();
		auto hc_end = hc;

        Polygon_2 polygon;
        do {
            auto p1 = hc->vertex()->point();
            polygon.insert(polygon.vertices_end(), { p1.x(), p1.y() });
            ++hc;
        } while (hc != hc_end);

        if (!polygon.is_simple())
            continue;

        if (polygon.orientation() == CGAL::NEGATIVE)
            polygon.reverse_orientation();
        ii.push_back(polygon);
    }

    std::vector<Polygon_with_holes_2> oi;
    CGAL::join(ii.begin(), ii.end(), std::back_inserter(oi));

    auto to_poly = [] (const Polygon_2& P) {
        polygon_t::polygon p;
        p.reserve(P.size());
        for(auto vi = P.vertices_begin(); vi != P.vertices_end(); ++vi)
            p.push_back({ to_double(vi->x()), to_double(vi->y()) });
        return p;
    };

    std::vector<polygon_t> projection;
    for (auto& o : oi) {
        projection.emplace_back();
        auto& p = projection.back();

        p.polygons.push_back(to_poly(o.outer_boundary()));
        for(auto hi = o.holes_begin(); hi != o.holes_end(); ++hi)
            p.polygons.push_back(to_poly(*hi));
    }

    return projection;
}

}

