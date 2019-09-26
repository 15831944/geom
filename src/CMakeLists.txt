# 1.52 Required for CGAL to play nice with C++11
FIND_PACKAGE(Boost 1.65.1 REQUIRED)
ADD_DEFINITIONS(-DBOOST_PARAMETER_MAX_ARITY=12)
FIND_PACKAGE(CGAL 4.14 REQUIRED)
INCLUDE(${CGAL_USE_FILE})

IF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frounding-math")
ENDIF()

add_definitions(-DCGAL_HAS_THREADS)

set(CMAKE_BUILD_TYPE Release)
INCLUDE_DIRECTORIES(${GEOM_INCLUDE_DIR}/geom ${Boost_INCLUDE_DIRS})
ADD_LIBRARY(geom STATIC polyhedron.cpp primitives.cpp ops.cpp ops_volume.cpp ops_projection.cpp ops_convex_partition.cpp CSGPrimitives3D.cpp Point.cpp io.cpp explode.cpp translate.cpp query.cpp aabb_tree.cpp)
TARGET_LINK_LIBRARIES(geom CGAL::CGAL ${Boost_LIBRARIES})
