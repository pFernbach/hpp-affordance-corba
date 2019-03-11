# Copyright (c) 2016 CNRS
# Author: Anna Seppala
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

SET(LIBRARY_NAME ${PROJECT_NAME})
SET(IDL_SOURCES
  affordance
  )

SET(HPP_CORBASERVER_IDL_SOURCES
  common
)

# search for python
FINDPYTHON()

OMNIIDL_INCLUDE_DIRECTORIES(
		${HPP_CORBASERVER_DATAROOTDIR}/idl
		${CMAKE_SOURCE_DIR}/idl
		)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/affordance)
FOREACH(IDL ${IDL_SOURCES})
  GENERATE_IDL_CPP (${IDL} ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/affordance)
  GENERATE_IDL_PYTHON (${IDL} ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/affordance)
  INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${IDL}.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hpp/corbaserver/affordance/)
    INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${IDL}_idl.py
    DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/affordance/
    )
ENDFOREACH()
FOREACH(IDL ${HPP_CORBASERVER_IDL_SOURCES})
  GENERATE_IDL_CPP (${IDL}
    ${HPP_CORBASERVER_DATAROOTDIR}/idl/hpp/corbaserver
		)
	GENERATE_IDL_PYTHON (${IDL}
	    ${HPP_CORBASERVER_DATAROOTDIR}/idl/hpp/corbaserver
		)
ENDFOREACH()
ADD_CUSTOM_TARGET(generate_idl_cpp DEPENDS ${ALL_IDL_CPP_STUBS})
ADD_CUSTOM_TARGET(generate_idl_python DEPENDS ${ALL_IDL_PYTHON_STUBS})

ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  common.hh
  commonSK.cc
  affordance.hh
  affordanceSK.cc
  affordance.impl.hh
  affordance.impl.cc
  affordance_idl.py
  server.cc
  common_idl.py
)

ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_cpp)
ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_python)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} hpp-affordance)

PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-corbaserver)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-affordance)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-template-corba)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-util)

INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})

PYTHON_INSTALL_ON_SITE(
  hpp/corbaserver/affordance corba.py ${PYTHON_SITELIB}
  )
PYTHON_INSTALL_ON_SITE(
  hpp/corbaserver/affordance __init__.py ${PYTHON_SITELIB}
  )
PYTHON_INSTALL_ON_SITE(
  hpp/corbaserver/affordance client.py ${PYTHON_SITELIB}
  )
PYTHON_INSTALL_ON_SITE(
  hpp/corbaserver/affordance affordance.py ${PYTHON_SITELIB}
  )
PYTHON_INSTALL_ON_SITE (
	hpp/corbaserver/affordance robot.py ${PYTHON_SITELIB}
	)

INSTALL(
  FILES
  ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/affordance/affordance.idl
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idl/hpp/corbaserver/affordance
)

# Generate and install python code for each dependency idl file
FOREACH (IDL ${HPP_CORBASERVER_IDL_SOURCES})
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
  INSTALL(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL}_idl.py
    DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/affordance
    )
ENDFOREACH()


# Standalone corba server
ADD_EXECUTABLE (hpp-affordance-server hpp-affordance-corba.cc)
TARGET_LINK_LIBRARIES (hpp-affordance-server ${LIBRARY_NAME} hpp-affordance)

PKG_CONFIG_USE_DEPENDENCY(hpp-affordance-server hpp-corbaserver)
PKG_CONFIG_USE_DEPENDENCY(hpp-affordance-server hpp-template-corba)
PKG_CONFIG_USE_DEPENDENCY(hpp-affordance-server hpp-util)
PKG_CONFIG_USE_DEPENDENCY(hpp-affordance-server hpp-affordance)

INSTALL (TARGETS hpp-affordance-server DESTINATION ${CMAKE_INSTALL_BINDIR})
