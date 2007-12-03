#
#

MAINTAINERCLEANFILES  = Makefile.in

bin_PROGRAMS		= client

client_LDADD	=	../wwcommon/src/libwwcommon.la @CEGUI_LIBS@

client_SOURCES	=	main.cpp		\
CClientEventManager.cpp \
Models/CBBClusterModel.cpp \
Models/CLeafClusterModel.cpp \
main.cpp \
CCameraThird.cpp \
tasks/CBackgroundTask.cpp \
tasks/C3DSwapTask.cpp \
tasks/CAnimateTask.cpp \
tasks/CSkyTask.cpp \
tasks/CLandscapeTask.cpp \
tasks/CNetworkTask.cpp \
tasks/CCameraTask.cpp \
tasks/CResourceTask.cpp \
tasks/CGameTask.cpp \
tasks/CTimeTask.cpp \
tasks/CPreGameTask.cpp \
tasks/CPacsTask.cpp \
tasks/CGuiTask.cpp \
tasks/CConfigTask.cpp \
tasks/CIntroTask.cpp \
tasks/CInputTask.cpp \
tasks/C3DTask.cpp \
controllers/CInteractiveSobController.cpp \
controllers/CPlayerController.cpp \
controllers/CInputController.cpp \
views/CPerformer3DView.cpp \
CClientSimulation.cpp \
CWWTask.cpp \
CActorProxy.cpp \
entities/CAnimLayer.cpp \
entities/CAnimGroup.cpp \
entities/IControllableMedia.cpp \
entities/CMeshGroup.cpp \
entities/CSkeleton.cpp \
entities/CEntityMedia.cpp \
CLocalObjectView.cpp 

AM_CXXFLAGS              = -I/usr/include/python2.4 @CEGUI_CFLAGS@

# End of Makefile.am
