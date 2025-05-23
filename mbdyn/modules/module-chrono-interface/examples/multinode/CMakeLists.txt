#--------------------------------------------------------------
#
# Example of CMake configuration file to build an external
# project depending on Chrono and on optional Chrono modules.
#
# This minimal sample project can be used as a template for a
# user project.  Modify sections 1, 2, and 3 below as appropriate.
#
#--------------------------------------------------------------


cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------------
# === 1 ===
# Modify the project name if you want:
#--------------------------------------------------------------

project(CE_models)

#--------------------------------------------------------------
# === 2 ===
# Find the Chrono package and any REQUIRED or OPTIONAL modules
# by invoking the find_package function in CONFIG mode:
#    find_package(Chrono
#	              COMPONENTS req_module1 req_module1 ...
#	              OPTIONAL opt_module1 opt_module2 ...
#                 CONFIG)
# The following Chrono modules can be requested (their names
# are case insensitive): Cascade, Cosimulation, FEA, Irrlicht,
# Matlab, Parallel, Postprocess, Python, Vehicle.
#
# Note that you will have to set the variable Chrono_DIR to
# specify the location of the ChronoConfig.cmake script, if
# it is not in its default install location.
# Chrono_DIR can be either a Chrono build tree or a Chrono install tree.
#
# The following variables are set and can be used further down:
# CHRONO_FOUND
#   set to true if Chrono and all required components were found
# CHRONO_C_FLAGS
# CHRONO_CXX_FLAGS
#   C and C++ compilation flags
# CHRONO_INCLUDE_DIRS
#   additional paths for included headers
# CHRONO_LIBRARIES
#   list of required libraries (with full path)
# CHRONO_LINKER_FLAGS
#   additional linker flags
# CHRONO_DLLS
#   list of all DLL dependencies (with full path)
# CHRONO_DATA_DIR
#   path to the Chrono data make_directory
#
# In addition, for each requested component [COMPONENT], the
# following variable is set to true (ON) or false (OFF):
# CHRONO_[COMPONENT]_FOUND
#
# In this example, we only request the Irrlicht module (required)
#--------------------------------------------------------------

LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/../Chrono/lib")
find_package(Chrono
             COMPONENTS Parallel OpenGl
             CONFIG)

#--------------------------------------------------------------
# Return now if Chrono or a required component was not found.
#--------------------------------------------------------------

if (NOT Chrono_FOUND)
  message("Could not find Chrono or one of its required modules")
  return()
endif()

#--------------------------------------------------------------
# Enable creation of "application bundles" on MacOSX.
#--------------------------------------------------------------

# This is necessary for any Irrlicht-based project (like the example here).
# For OpenGL-based or non-graphics projects, this is optional and the block
# below can be removed (or else explcitly set CMAKE_MACOSX_BUNDLE to 'OFF').
#
# If creating application bundles, the build output will be named 'myexe.app'.
# Use the convenience script 'run_app.sh' available under 'contrib/appbundle-macosx/'
# to run:
#     start_demo.sh myexe.app

if(APPLE)
    set(CMAKE_MACOSX_BUNDLE ON)
endif()

#--------------------------------------------------------------
# Add path to Chrono headers and to headers of all dependencies
# of the requested modules.
#--------------------------------------------------------------
# in lunix system, it is needed for the usage of openmp.
set(Openmp_need "gomp")
# the directory for "mbdyn_ce.cc" and "mbdyn_ce.h"
set(MBDYN_CE_INTERFACE_HEADER /home/runsen/Downloads/GSoC2020/mbdyn-GSoC2020/modules/module-chrono-interface)

include_directories(${CHRONO_INCLUDE_DIRS} ${Openmp_need} ${MBDYN_CE_INTERFACE_HEADER})

#--------------------------------------------------------------
# Tweaks to disable some warnings with MSVC
#--------------------------------------------------------------
if(MSVC)
    add_definitions("-D_CRT_SECURE_NO_DEPRECATE")  # avoids deprecation warnings
    add_definitions("-D_SCL_SECURE_NO_DEPRECATE")  # avoids deprecation warnings
    add_definitions( "-DNOMINMAX" )                # do not use MSVC's min/max macros
    set(EXTRA_COMPILE_FLAGS "/wd4275")             # disable warnings triggered by Irrlicht
else()
    set(EXTRA_COMPILE_FLAGS "")
endif()

#--------------------------------------------------------------
# === 3 ===
# Add the executable from your project and specify all C++
# files in your project.
#--------------------------------------------------------------

#generate the correponding executable files
#add_executable(CE_models ce_models.cc mbdyn_ce.h)

# generate the corresponding shared/static libs
# SHARED means that generates shared libs.
# add all source files
 add_library(CE_models_multinode SHARED ce_models_multinode.cc ${MBDYN_CE_INTERFACE_HEADER}/mbdyn_ce.cc ${MBDYN_CE_INTERFACE_HEADER}/mbdyn_ce.h)
#--------------------------------------------------------------
# Set properties for your executable target
#
# Note that here we define a macro CHRONO_DATA_DIR which will
# contain the path to the Chrono data directory, either in its
# source tree (if using a build version of Chrono), or in its
# install tree (if using an installed version of Chrono).
#--------------------------------------------------------------

set_target_properties(CE_models_multinode PROPERTIES
	    COMPILE_FLAGS "${CHRONO_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}"
	    COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
	    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")
#--------------------------------------------------------------
# Link to Chrono libraries and dependency libraries
#--------------------------------------------------------------
target_link_libraries(CE_models_multinode ${CHRONO_LIBRARIES} ${Openmp_need})

#--------------------------------------------------------------
# === 4 (OPTIONAL) ===
#
# Optionally, add a custom command for copying all Chrono and
# dependency DLLs to the appropriate binary output folder.
# This function has effect only on Windows.
#
# Note that you must first set EXECUTABLE_OUTPUT_PATH
# (this can simply be ${CMAKE_BINARY_DIR}, like in this example)
#--------------------------------------------------------------

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARIES_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_DLL_copy_command("${CHRONO_DLLS}")
