#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Common" for configuration "Release"
set_property(TARGET Common APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Common PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/usr/local/lib/OpenMVS/libCommon.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Common )
list(APPEND _IMPORT_CHECK_FILES_FOR_Common "/usr/local/lib/OpenMVS/libCommon.a" )

# Import target "Math" for configuration "Release"
set_property(TARGET Math APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Math PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/usr/local/lib/OpenMVS/libMath.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Math )
list(APPEND _IMPORT_CHECK_FILES_FOR_Math "/usr/local/lib/OpenMVS/libMath.a" )

# Import target "IO" for configuration "Release"
set_property(TARGET IO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(IO PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/usr/local/lib/OpenMVS/libIO.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS IO )
list(APPEND _IMPORT_CHECK_FILES_FOR_IO "/usr/local/lib/OpenMVS/libIO.a" )

# Import target "MVS" for configuration "Release"
set_property(TARGET MVS APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MVS PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "/usr/local/lib/OpenMVS/libMVS.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS MVS )
list(APPEND _IMPORT_CHECK_FILES_FOR_MVS "/usr/local/lib/OpenMVS/libMVS.a" )

# Import target "InterfaceCOLMAP" for configuration "Release"
set_property(TARGET InterfaceCOLMAP APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(InterfaceCOLMAP PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/InterfaceCOLMAP"
  )

list(APPEND _IMPORT_CHECK_TARGETS InterfaceCOLMAP )
list(APPEND _IMPORT_CHECK_FILES_FOR_InterfaceCOLMAP "/usr/local/bin/OpenMVS/InterfaceCOLMAP" )

# Import target "InterfaceVisualSFM" for configuration "Release"
set_property(TARGET InterfaceVisualSFM APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(InterfaceVisualSFM PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/InterfaceVisualSFM"
  )

list(APPEND _IMPORT_CHECK_TARGETS InterfaceVisualSFM )
list(APPEND _IMPORT_CHECK_FILES_FOR_InterfaceVisualSFM "/usr/local/bin/OpenMVS/InterfaceVisualSFM" )

# Import target "DensifyPointCloud" for configuration "Release"
set_property(TARGET DensifyPointCloud APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(DensifyPointCloud PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/DensifyPointCloud"
  )

list(APPEND _IMPORT_CHECK_TARGETS DensifyPointCloud )
list(APPEND _IMPORT_CHECK_FILES_FOR_DensifyPointCloud "/usr/local/bin/OpenMVS/DensifyPointCloud" )

# Import target "ReconstructMesh" for configuration "Release"
set_property(TARGET ReconstructMesh APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ReconstructMesh PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/ReconstructMesh"
  )

list(APPEND _IMPORT_CHECK_TARGETS ReconstructMesh )
list(APPEND _IMPORT_CHECK_FILES_FOR_ReconstructMesh "/usr/local/bin/OpenMVS/ReconstructMesh" )

# Import target "RefineMesh" for configuration "Release"
set_property(TARGET RefineMesh APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(RefineMesh PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/RefineMesh"
  )

list(APPEND _IMPORT_CHECK_TARGETS RefineMesh )
list(APPEND _IMPORT_CHECK_FILES_FOR_RefineMesh "/usr/local/bin/OpenMVS/RefineMesh" )

# Import target "TextureMesh" for configuration "Release"
set_property(TARGET TextureMesh APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TextureMesh PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/TextureMesh"
  )

list(APPEND _IMPORT_CHECK_TARGETS TextureMesh )
list(APPEND _IMPORT_CHECK_FILES_FOR_TextureMesh "/usr/local/bin/OpenMVS/TextureMesh" )

# Import target "Viewer" for configuration "Release"
set_property(TARGET Viewer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Viewer PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/bin/OpenMVS/Viewer"
  )

list(APPEND _IMPORT_CHECK_TARGETS Viewer )
list(APPEND _IMPORT_CHECK_FILES_FOR_Viewer "/usr/local/bin/OpenMVS/Viewer" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
