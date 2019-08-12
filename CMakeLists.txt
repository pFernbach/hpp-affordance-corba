# Copyright (c) 2016 CNRS
# Author: Anna Seppala
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

# Requires at least CMake 2.6 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CXX_DISABLE_WERROR true)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/hpp.cmake)
INCLUDE(cmake/hpp/python.cmake)

SET(PROJECT_NAME hpp-affordance-corba)
SET(PROJECT_DESCRIPTION "Corba server for Humanoid Path Planner Affordance applications")

SET(CUSTOM_HEADER_DIR hpp/corbaserver/affordance)

SETUP_HPP_PROJECT ()

LIST(APPEND PKG_CONFIG_ADDITIONAL_VARIABLES cmake_plugin)

# Dependencies
HPP_FINDPYTHON()
ADD_REQUIRED_DEPENDENCY("hpp-core >= 3")
ADD_REQUIRED_DEPENDENCY("hpp-pinocchio >= 3")
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")
ADD_REQUIRED_DEPENDENCY("hpp-constraints >= 3")
ADD_REQUIRED_DEPENDENCY("hpp-affordance")
ADD_REQUIRED_DEPENDENCY("hpp-template-corba")
ADD_REQUIRED_DEPENDENCY("hpp-util")
# This dependency ensures us that the intermediate __init__.py files are
# already installed.
ADD_REQUIRED_DEPENDENCY("hpp-corbaserver >= 3")
# Make sure that python submodule is installed in the same prefix as module.
IF (NOT ${HPP_CORBASERVER_PREFIX} STREQUAL ${CMAKE_INSTALL_PREFIX})
  MESSAGE(WARNING
    "  THIS PACKAGE SHOULD BE INSTALLED IN THE SAME PREFIX AS HPP-CORBASERVER,\n"
    "  SINCE IT INSTALLS A PYTHON SUBMODULE OF HPP.CORBASERVER.")
  MESSAGE("CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
  MESSAGE("HPP_CORBASERVER_PREFIX=${HPP_CORBASERVER_PREFIX}")
ENDIF()

PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

ADD_SUBDIRECTORY(include/${CUSTOM_HEADER_DIR})
ADD_SUBDIRECTORY(src)

CONFIG_FILES (include/hpp/corbaserver/affordance/doc.hh)

SET(CATKIN_PACKAGE_SHARE_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

install(FILES
  data/package.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  )

install(FILES
  data/urdf/hyq/hyq_trunk_large.urdf
  data/urdf/darpa.urdf
  data/urdf/box.urdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/urdf
  )

install(FILES
  data/srdf/hyq/hyq_trunk_large.srdf
  data/srdf/darpa.srdf
  data/srdf/box.srdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/srdf
  )

install(FILES
  data/meshes/darpareduced.stl
  data/meshes/box.stl
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/meshes
  )

install(FILES
  data/meshes/hyq/hyq_all.stl
  data/meshes/hyq/hyq_trunk_large.stl
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/meshes/hyq
  )

SETUP_PROJECT_FINALIZE()
