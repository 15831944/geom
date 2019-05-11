
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

MACRO(CXXCAM_TEST testname)
	ADD_EXECUTABLE(${testname} EXCLUDE_FROM_ALL "${testname}.cpp")
	TARGET_LINK_LIBRARIES(${testname} geom)
	SET_TARGET_PROPERTIES(${testname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
	ADD_TEST(${testname} "${MAKE_CURRENT_BINARY_DIR}/${testname}")
	ADD_DEPENDENCIES(check ${testname})
ENDMACRO()

FOREACH(test 
geom_basic 
geom_tools 
geom_cube 
geom_cube2 
geom_sphere 
geom_cube_io 
geom_sphere_io 
geom_cylinder 
geom_translate 
geom_explode 
geom_nonmanifold 
geom_validate_rotations 
geom_validate_boolean 
geom_project
geom_convex
)
	CXXCAM_TEST(${test})
ENDFOREACH()

FIND_PACKAGE(Boost 1.52.0 REQUIRED)
FIND_PACKAGE(CGAL REQUIRED)
INCLUDE(${CGAL_USE_FILE})
add_definitions(-DCGAL_HAS_THREADS)
ADD_EXECUTABLE(geom_tet_mesh EXCLUDE_FROM_ALL "geom_tet_mesh.cpp")
TARGET_LINK_LIBRARIES(geom_tet_mesh ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(geom_tet_mesh PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
ADD_TEST(geom_tet_mesh "${MAKE_CURRENT_BINARY_DIR}/geom_tet_mesh")
ADD_DEPENDENCIES(check geom_tet_mesh)

