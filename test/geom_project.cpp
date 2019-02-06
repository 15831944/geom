#include "geom/polyhedron.h"
#include "geom/io.h"
#include "geom/ops.h"
#include "geom/primitives.h"
#include <iostream>
#include <sstream>

using namespace geom;

int main()
{
	auto model = make_cone({0, 0, 20}, {0, 0, 0}, 3, 3, 8);
    auto polygons = projection_xy(model);

    auto output_path = [](const polygon_t::polygon& path) {
        std::ostringstream ss;
        bool rapid_to_first = true;
        for (auto& p : path) {
            if (rapid_to_first) {
                ss << std::fixed << "M" << p.x << " " << p.y;
                rapid_to_first = false;
            }
            ss << std::fixed << "L" << p.x << " " << p.y;
        }
        return ss.str();
    };

    std::cout << "<svg xmlns=\"http://www.w3.org/2000/svg\">\n";
    for (auto& polygon : polygons)
    {
        std::string path;
        for (auto& p : polygon.polygons)
            path += output_path(p);
        std::cout << "   <path d=\"" << path << "\"></path>\n";
    }
    std::cout << "</svg>\n";
	return 0;
}
