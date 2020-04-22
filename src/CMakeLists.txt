# Copyright (c) 2016, 2020, CNRS
# Author: Anna Seppala, Guilhem Saurel
#
# This file is part of hpp-corbaserver.
# hpp-corbaserver is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-corbaserver is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

SET(IDL_SOURCES
  affordance
  )

INCLUDE(${HPP_CORBASERVER_CMAKE_PLUGIN})
OMNIIDL_INCLUDE_DIRECTORIES(
		${HPP_CORBASERVER_DATAROOTDIR}/idl
		${CMAKE_SOURCE_DIR}/idl
		)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/affordance)
FOREACH(IDL ${IDL_SOURCES})
  GENERATE_IDL_CPP(${IDL} ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/affordance
    HEADER_SUFFIX -idl.hh)
  GENERATE_IDL_PYTHON(${IDL} ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/affordance
    ENABLE_DOCSTRING
    STUBS hpp_stubs.affordance
    ARGUMENTS
    -Wbmodules=hpp_idl
    -Wbextern=common:hpp_stubs
    )
  INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${IDL}-idl.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hpp/corbaserver/affordance/)
ENDFOREACH()

INSTALL(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hpp_idl/hpp/corbaserver/affordance
  DESTINATION ${PYTHON_SITELIB}/hpp_idl/hpp/corbaserver)
INSTALL(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hpp_stubs/affordance
  DESTINATION ${PYTHON_SITELIB}/hpp_stubs)

ADD_CUSTOM_TARGET(generate_idl_cpp DEPENDS ${ALL_IDL_CPP_STUBS})
ADD_CUSTOM_TARGET(generate_idl_python DEPENDS ${ALL_IDL_PYTHON_STUBS})

HPP_ADD_SERVER_PLUGIN(affordance-corba
  SOURCES
  ${ALL_IDL_CPP_STUBS}
  affordance.impl.hh
  affordance.impl.cc
  server.cc
  LINK_DEPENDENCIES hpp-corbaserver::hpp-corbaserver hpp-affordance::hpp-affordance
  PKG_CONFIG_DEPENDENCIES omniORB4
  )

ADD_DEPENDENCIES(affordance-corba generate_idl_cpp)
ADD_DEPENDENCIES(affordance-corba generate_idl_python)

PYTHON_INSTALL_ON_SITE(hpp/corbaserver/affordance __init__.py)
PYTHON_INSTALL_ON_SITE(hpp/corbaserver/affordance affordance.py)
PYTHON_INSTALL_ON_SITE(hpp/corbaserver/affordance client.py)
PYTHON_INSTALL_ON_SITE(hpp/corbaserver/affordance corba.py)
PYTHON_INSTALL_ON_SITE(hpp/corbaserver/affordance robot.py)

INSTALL(
  FILES
  ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/affordance/affordance.idl
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idl/hpp/corbaserver/affordance
)

# Generate and install python code for each dependency idl file
FOREACH(IDL ${HPP_CORBASERVER_IDL_SOURCES})
  ADD_CUSTOM_COMMAND(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/affordance/${IDL}_idl.py
    COMMAND ${OMNIIDL}
    ARGS -bpython -Wbpackage=""
    -I${HPP_CORBASERVER_DATAROOTDIR}/idl
    ${HPP_CORBASERVER_DATAROOTDIR}/idl/hpp/corbaserver/${IDL}.idl
    MAIN_DEPENDENCY
    ${HPP_CORBASERVER_DATAROOTDIR}/idl/hpp/corbaserver/${IDL}.idl
    )
  SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL}_idl.py
    GENERATED=ON
    )
  SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/affordance/__init__.py
    GENERATED=ON
    )
  INSTALL(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL}_idl.py
    ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/affordance/__init__.py
    DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/affordance
    )
ENDFOREACH()
