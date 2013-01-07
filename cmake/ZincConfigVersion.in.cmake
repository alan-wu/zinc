
SET(PACKAGE_VERSION "@ZINC_VERSION@")
 
# Check whether the requested PACKAGE_FIND_VERSION is compatible
IF("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
  SET(PACKAGE_VERSION_COMPATIBLE FALSE)
ELSE()
  SET(PACKAGE_VERSION_COMPATIBLE TRUE)
  IF ("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
    SET(PACKAGE_VERSION_EXACT TRUE)
  ENDIF()
ENDIF()