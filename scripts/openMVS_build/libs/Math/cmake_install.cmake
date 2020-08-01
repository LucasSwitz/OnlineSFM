# Install script for directory: /home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/OpenMVS/Math/Common.h;/usr/local/include/OpenMVS/Math/LBP.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/OpenMVS/Math" TYPE FILE FILES
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/Common.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/LBP.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/OpenMVS/Math/IBFS/IBFS.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/OpenMVS/Math/IBFS" TYPE FILE FILES "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/IBFS/IBFS.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/OpenMVS/Math/LMFit/lmmin.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/OpenMVS/Math/LMFit" TYPE FILE FILES "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/LMFit/lmmin.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/OpenMVS/Math/TRWS/MRFEnergy.h;/usr/local/include/OpenMVS/Math/TRWS/MRFEnergy.inl;/usr/local/include/OpenMVS/Math/TRWS/instances.h;/usr/local/include/OpenMVS/Math/TRWS/minimize.inl;/usr/local/include/OpenMVS/Math/TRWS/ordering.inl;/usr/local/include/OpenMVS/Math/TRWS/treeProbabilities.inl;/usr/local/include/OpenMVS/Math/TRWS/typeBinary.h;/usr/local/include/OpenMVS/Math/TRWS/typeBinaryFast.h;/usr/local/include/OpenMVS/Math/TRWS/typeGeneral.h;/usr/local/include/OpenMVS/Math/TRWS/typePotts.h;/usr/local/include/OpenMVS/Math/TRWS/typeTruncatedLinear.h;/usr/local/include/OpenMVS/Math/TRWS/typeTruncatedLinear2D.h;/usr/local/include/OpenMVS/Math/TRWS/typeTruncatedQuadratic.h;/usr/local/include/OpenMVS/Math/TRWS/typeTruncatedQuadratic2D.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/OpenMVS/Math/TRWS" TYPE FILE FILES
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/MRFEnergy.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/MRFEnergy.inl"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/instances.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/minimize.inl"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/ordering.inl"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/treeProbabilities.inl"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeBinary.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeBinaryFast.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeGeneral.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typePotts.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeTruncatedLinear.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeTruncatedLinear2D.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeTruncatedQuadratic.h"
    "/home/gumbo00/OnlineSFM/scripts/install/openMVS/libs/Math/TRWS/typeTruncatedQuadratic2D.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/OpenMVS/libMath.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/OpenMVS" TYPE STATIC_LIBRARY FILES "/home/gumbo00/OnlineSFM/scripts/install/openMVS_build/lib/libMath.a")
endif()

