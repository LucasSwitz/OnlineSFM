# Install script for directory: /home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Array"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Cholesky"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/CholmodSupport"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Core"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Dense"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Eigen"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Eigen2Support"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Eigenvalues"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Geometry"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Householder"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/IterativeLinearSolvers"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Jacobi"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/LU"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/LeastSquares"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/MetisSupport"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/OrderingMethods"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/PaStiXSupport"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/PardisoSupport"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/QR"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/QtAlignedMalloc"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SPQRSupport"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SVD"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/Sparse"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SparseCholesky"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SparseCore"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SparseLU"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SparseQR"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/StdDeque"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/StdList"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/StdVector"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/SuperLUSupport"
    "/home/gumbo00/OnlineSFM/scripts/install/eigen/Eigen/UmfPackSupport"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/gumbo00/OnlineSFM/scripts/install/eigen_build/Eigen/src/cmake_install.cmake")

endif()

