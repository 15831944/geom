#include "geom/io.h"
#include "geom/ops.h"
#include <iostream>
#include <sstream>

using namespace geom;

int main()
{
    // points from CGAL example
    polygon_t poly;
    poly.points.push_back({391, 374});
    poly.points.push_back({240, 431});
    poly.points.push_back({252, 340});
    poly.points.push_back({374, 320});
    poly.points.push_back({289, 214});
    poly.points.push_back({134, 390});
    poly.points.push_back({ 68, 186});
    poly.points.push_back({154, 259});
    poly.points.push_back({161, 107});
    poly.points.push_back({435, 108});
    poly.points.push_back({208, 148});
    poly.points.push_back({295, 160});
    poly.points.push_back({421, 212});
    poly.points.push_back({441, 303});
    auto polygons = convex_partition(poly);

    auto output_path = [](const polygon_t& path) {
        std::ostringstream ss;
        bool rapid_to_first = true;
        for (auto& p : path.points) {
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
        path += output_path(polygon);
        std::cout << "   <path d=\"" << path << "\"></path>\n";
    }
    std::cout << "</svg>\n";
	return 0;
}
