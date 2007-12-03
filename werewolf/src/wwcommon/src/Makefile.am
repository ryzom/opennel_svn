#
#

MAINTAINERCLEANFILES     = Makefile.in

lib_LTLIBRARIES          = libwwcommon.la

libwwcommon_la_SOURCES = CClientMotionHandler.cpp \
CControllerFactory.cpp \
CDRMotionInterpKey.cpp \
CGameEventServer.cpp \
CGameSpawnRequestEvent.cpp \
CGameUnspawnRequestEvent.cpp \
CGeneralInterpolator.cpp \
CGenericObjectPool.cpp \
CMotionController.cpp \
CMotionHandler.cpp \
CMotionInterpKey.cpp \
CMoveBackState.cpp \
CMoveFwdState.cpp \
CMoveIdleState.cpp \
CMovementState.cpp \
COrientIdleState.cpp \
COrientState.cpp \
COrientTurnLeftState.cpp \
COrientTurnRightState.cpp \
CPerformer.cpp \
CPolynomialMotionController.cpp \
CRequestHandler.cpp \
CRootSob.cpp \
CSobAddEvent.cpp \
CSobDREvent.cpp \
CSobFactory.cpp \
CSobHandlerFactory.cpp \
CSobManager.cpp \
CSobMoveEvent.cpp \
CSobOrientEvent.cpp \
CSobRemoveEvent.cpp \
CSobSpawnEvent.cpp \
CSobStateRequestEvent.cpp \
CSobStrafeEvent.cpp \
CSobUnspawnEvent.cpp \
CStateManager.cpp \
CStrafeIdleState.cpp \
CStrafeLeftState.cpp \
CStrafeRightState.cpp \
CStrafeState.cpp \
CTaskManager.cpp \
CYawMotionInterpKey.cpp \
IDirtyPropagator.cpp \
IGameEvent.cpp \
IProperty.cpp \
ISimulationObj.cpp \
ISimulationState.cpp \
ISobEvent.cpp \
ISobStateChangeEvent.cpp 

AM_CXXFLAGS         	 = -I/usr/include/python2.4

libwwcommon_la_LIBADD  = -lc -lpthread

libwwcommon_la_LDFLAGS = -version-info @LIBTOOL_VERSION@


# End of Makefile.am

