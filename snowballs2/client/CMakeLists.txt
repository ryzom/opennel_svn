ADD_SUBDIRECTORY(src)

SET(database "${CMAKE_INSTALL_PREFIX}/${PKGDIR}")
CONFIGURE_FILE(snowballs_client.cfg.in snowballs_client.cfg)

INSTALL(FILES ${CMAKE_BINARY_DIR}/snowballs_client.cfg DESTINATION ${PKGDIR})
INSTALL(FILES ${CMAKE_BINARY_DIR}/snowballs_client_default.cfg DESTINATION ${PKGDIR})
INSTALL(DIRECTORY data/
        DESTINATION ${PKGDIR}/data/
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN "Makefile*" EXCLUDE)
