###
# Build Library Name
#
# Arguments: name - undecorated library name
# Sets: LIBNAME - decorated library name
###
MACRO(DECORATE_NEL_LIB name)

  IF(WIN32)
    IF(NL_BUILD_MODE MATCHES "NL_RELEASE_DEBUG")
      SET(LIBNAME "${name}_rd")
    ELSE(NL_BUILD_MODE MATCHES "NL_RELEASE_DEBUG")
      IF(NL_BUILD_MODE MATCHES "NL_DEBUG")
        SET(LIBNAME "${name}_d")
      ELSE(NL_BUILD_MODE MATCHES "NL_DEBUG")
        SET(LIBNAME "${name}_r")
      ENDIF(NL_BUILD_MODE MATCHES "NL_DEBUG")
    ENDIF(NL_BUILD_MODE MATCHES "NL_RELEASE_DEBUG")
  ELSE(WIN32)
    SET(LIBNAME "${name}")
  ENDIF(WIN32)

ENDMACRO(DECORATE_NEL_LIB)
