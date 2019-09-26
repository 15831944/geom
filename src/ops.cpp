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
 * ops.cpp
 *
 *  Created on: 06/05/2013
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

// glide
#include <CGAL/minkowski_sum_3.h>

// merge
#include <CGAL/Nef_nary_union_3.h>

// subdivide
#include <CGAL/subdivision_method_3.h>

namespace geom
{

polyhedron_t glide(const polyhedron_t& polyhedron, const polyline_t& path)
{
	typedef std::vector<Point_3>::const_iterator point_iterator;
	typedef std::pair<point_iterator, point_iterator>  point_range;
	typedef std::vector<point_range> polyline;

	std::vector<Point_3> point_line;
	for(auto& point : path.line)
		point_line.emplace_back(point.x, point.y, point.z);

	polyline poly = { point_range(begin(point_line), end(point_line)) };
	Nef_polyhedron_3 N1(poly.begin(), poly.end(), Nef_polyhedron_3::Polylines_tag());

	// Need to make a copy because minkowski_sum_3 can modify its arguments
	auto nef = get_priv(polyhedron)->nef;
	auto priv = std::make_shared<polyhedron_t::private_t>(CGAL::minkowski_sum_3(nef, N1));

	if (priv->nef.is_simple())
		return make_polyhedron( std::move(priv) );

	throw std::runtime_error("glide result is not 2-manifold");
}

polyhedron_t merge(const std::vector<polyhedron_t>& polyhedra)
{
	CGAL::Nef_nary_union_3<Nef_polyhedron_3> op;
	for(auto& polyhedron : polyhedra)
		op.add_polyhedron(get_priv(polyhedron)->nef);

	auto priv = std::make_shared<polyhedron_t::private_t>(op.get_union());
	return make_polyhedron( std::move(priv) );
}

polyhedron_t subdivide(const polyhedron_t& polyhedron, unsigned step) {
	auto P = to_Polyhedron_3(polyhedron);
    namespace params = CGAL::parameters;

    CGAL::Subdivision_method_3::CatmullClark_subdivision(P, params::number_of_iterations(step));
    auto priv = std::make_shared<polyhedron_t::private_t>( P );
    return make_polyhedron( std::move(priv) );
}

}

