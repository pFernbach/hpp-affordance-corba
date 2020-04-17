# Copyright (c) 2016, 2020, CNRS
# Authors: Anna Seppala, Guilhem Saurel
#
# This file is part of hpp-affordance-corba.
# hpp-affordance-corba is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-affordance-corba is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-affordance-corba.  If not, see
# <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT_NAME hpp-affordance-corba)
SET(PROJECT_DESCRIPTION "Corba server for Humanoid Path Planner Affordance applications")

SET(CUSTOM_HEADER_DIR hpp/corbaserver/affordance)
SET(PROJECT_USE_CMAKE_EXPORT TRUE)
SET(CXX_DISABLE_WERROR TRUE)

INCLUDE(cmake/hpp.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)

COMPUTE_PROJECT_ARGS(PROJECT_ARGS LANGUAGES CXX)
PROJECT(${PROJECT_NAME} ${PROJECT_ARGS})

LIST(APPEND PKG_CONFIG_ADDITIONAL_VARIABLES cmake_plugin)

# Dependencies
FINDPYTHON()
ADD_PROJECT_DEPENDENCY("hpp-core" REQUIRED)
ADD_PROJECT_DEPENDENCY("hpp-affordance" REQUIRED)
ADD_PROJECT_DEPENDENCY("hpp-corbaserver" REQUIRED)
ADD_PROJECT_DEPENDENCY("Boost" REQUIRED COMPONENTS thread date_time system REQUIRED)
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")

# Make sure that python submodule is installed in the same prefix as module.
IF (NOT ${HPP_CORBASERVER_PREFIX} STREQUAL ${CMAKE_INSTALL_PREFIX})
  MESSAGE(WARNING
    "  THIS PACKAGE SHOULD BE INSTALLED IN THE SAME PREFIX AS HPP-CORBASERVER,\n"
    "  SINCE IT INSTALLS A PYTHON SUBMODULE OF HPP.CORBASERVER.")
  MESSAGE("CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
  MESSAGE("HPP_CORBASERVER_PREFIX=${HPP_CORBASERVER_PREFIX}")
ENDIF()

SET(${PROJECT_NAME}_HEADERS
  include/${CUSTOM_HEADER_DIR}/fwd.hh
  include/${CUSTOM_HEADER_DIR}/server.hh
  )

SET(${PROJECT_NAME}_SOURCES
  src/${PROJECT_NAME}.cc
  )

# Standalone corba server
ADD_EXECUTABLE(hpp-affordance-server ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
TARGET_INCLUDE_DIRECTORIES(hpp-affordance-server PUBLIC $<INSTALL_INTERFACE:include>)
TARGET_LINK_LIBRARIES(hpp-affordance-server hpp-corbaserver::hpp-corbaserver hpp-affordance::hpp-affordance)
INSTALL(TARGETS hpp-affordance-server EXPORT ${TARGETS_EXPORT_NAME} DESTINATION bin)

ADD_SUBDIRECTORY(src)

CONFIG_FILES (include/${CUSTOM_HEADER_DIR}/doc.hh)

install(FILES
  data/package.xml
  DESTINATION share/${PROJECT_NAME})

install(FILES
  data/urdf/hyq/hyq_trunk_large.urdf
  data/urdf/darpa.urdf
  data/urdf/box.urdf
  DESTINATION share/${PROJECT_NAME}/urdf)

install(FILES
  data/srdf/hyq/hyq_trunk_large.srdf
  data/srdf/darpa.srdf
  data/srdf/box.srdf
  DESTINATION share/${PROJECT_NAME}/srdf)

install(FILES
  data/meshes/darpareduced.stl
  data/meshes/box.stl
  DESTINATION share/${PROJECT_NAME}/meshes)

install(FILES
  data/meshes/hyq/hyq_all.stl
  data/meshes/hyq/hyq_trunk_large.stl
  DESTINATION share/${PROJECT_NAME}/meshes/hyq)

PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})
