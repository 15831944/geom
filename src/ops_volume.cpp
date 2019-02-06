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
 * ops_volume.cpp
 *
 *  Created on: 06/02/2019
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

// Meshing (for volume)
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Polyhedral_mesh_domain_3.h>
#include <CGAL/Polyhedral_mesh_domain_with_features_3.h>
#include <CGAL/make_mesh_3.h>
#include <CGAL/refine_mesh_3.h>

// Meshing
typedef CGAL::Exact_predicates_inexact_constructions_kernel Inexact_Kernel;
typedef CGAL::Mesh_polyhedron_3<Inexact_Kernel>::type Mesh_polyhedron_3;

typedef CGAL::Polyhedral_mesh_domain_with_features_3<Inexact_Kernel, Mesh_polyhedron_3> Mesh_domain;
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

namespace geom
{

double volume(const polyhedron_t& polyhedron)
{
	Mesh_polyhedron_3 PK;
	{
		typedef CGAL::Mesh_polyhedron_3<Nef_Kernel>::type Nef_Mesh_Polyhedron_3;
	
		Nef_Mesh_Polyhedron_3 EP;
		get_priv(polyhedron)->nef.convert_to_polyhedron(EP);
		copy_to(EP, PK);
	}
	assert(PK.is_valid());

	Mesh_domain domain(PK);
	domain.detect_features();

	using CGAL::parameters::facet_angle;
	using CGAL::parameters::facet_size;
	using CGAL::parameters::facet_distance;
	using CGAL::parameters::cell_radius_edge_ratio;
	// facet_size = 0.15, facet_distance = 0.008
	// TODO these parameters need to be tweaked.
	Mesh_criteria criteria(facet_angle = 25, facet_size = 1, facet_distance = 0.8, cell_radius_edge_ratio = 3);

	using CGAL::parameters::no_perturb;
	using CGAL::parameters::no_exude;
	auto c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, no_perturb(), no_exude());

	auto triangulation = c3t3.triangulation();

	std::vector<double> volumes;
	volumes.reserve(triangulation.number_of_finite_cells());
	for(auto it = triangulation.finite_cells_begin(); it != triangulation.finite_cells_end(); ++it)
	{
		auto tetr = triangulation.tetrahedron(it);
		volumes.push_back(tetr.volume());
	}
	std::sort(volumes.begin(), volumes.end());
	return std::accumulate(volumes.begin(), volumes.end(), 0.0);
}

}

