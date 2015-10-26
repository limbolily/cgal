# Top level CMakeLists.txt for CGAL-branchbuild

include("yong_injector/injector.cmake")

message( "== CMake setup ==" )
#project(CGAL CXX C)
yong_project(CGAL CXX)

# Minimal version of CMake:
if(WIN32)
  cmake_minimum_required(VERSION 2.8.6)
else()
  cmake_minimum_required(VERSION 2.6.2)
endif()

# option for branch build

#option( CGAL_BRANCH_BUILD "Create CGAL from branch" ON)
set(CGAL_BRANCH_BUILD ON)
mark_as_advanced( CGAL_BRANCH_BUILD )

include(${CMAKE_SOURCE_DIR}/Installation/cmake/modules/CGAL_SCM.cmake)

# add option for duplicate file detection
#option( CGAL_REPORT_DUPLICATE_FILES "Switch on to start (naive) detection of duplicate source- and headerfiles in packages" OFF)
set(CGAL_REPORT_DUPLICATE_FILES OFF)

message( "== CMake setup (DONE) ==\n" )

set(RUNNING_CGAL_AUTO_TEST FALSE)
set(CGAL_AUTO_LINK_ENABLED FALSE)
set(WITH_CGAL_ImageIO FALSE)
set(WITH_CGAL_Qt3 FALSE)
set(WITH_CGAL_Qt4 FALSE)
add_definitions(-DBOOST_ALL_NO_LIB -DCGAL_NO_AUTOLINK)

# and finally start actual build
add_subdirectory( Installation )

# for releases, where dont_submit is honored
#if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Documentation")
#  option(BUILD_DOC "Build the doxygen documentation" OFF)
#  if (BUILD_DOC)
#    add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/Documentation" )
#  endif(BUILD_DOC)
#endif()
yong_project_end()
