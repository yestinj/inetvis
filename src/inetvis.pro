TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt opengl stl

HEADERS	+= dataproc.h \
	graphicelement.h \
	packetheaders.h \
	timeutil.h \
	glviswidget.h \
	plotter.h \
	packetevent.h \
	logui.h \
    controlpanelwidget.h

SOURCES	+= graphicelement.cpp \
	packetheaders.cpp \
	plotter.cpp \
	timeutil.cpp \
	main.cpp \
	glviswidget.cpp \
	dataproc.cpp \
	packetevent.cpp \
	logui.cpp \
    controlpanelwidget.cpp

FORMS = controlpanel.ui \
	visdisplay.ui \
	plottersettingsdialog.ui \
	referenceframesettingsdialog.ui \
	aboutdialog.ui \
	helpdocumentationdialog.ui

# Replaced by RESOURCES. Leaving around for now..
#IMAGES	= images/emblem-pictures.png \	
#	images/fileopen \
#	images/filesave \
#	images/stock_insert-video-plugin.png \
#	images/stock_media-fwd.png \
#	images/stock_media-pause.png \
#	images/stock_media-play.png \
#	images/stock_media-rec.png \
#	images/stock_media-rew.png \
#	images/stock_calc-accept.png

RESOURCES += icons.qrc	

## CUSTOM SETTINGS ----------------------------------------

## Build Mode
#CONFIG   += debug warn_on
CONFIG	  += debug
#CONFIG   += release
### uncomment one or the other above

## Linking
CONFIG   += create_prl link_prl static
QMAKE_LFLAGS_RELEASE = -m32

## Optimization flags
QMAKE_CFLAGS_RELEASE = -O3 -march=pentium4 -m32 -mfpmath=sse -mmmx -msse -msse2 -mno-sse3 -momit-leaf-frame-pointer -maccumulate-outgoing-args
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE

## end of CUSTOM SETTINGS ---------------------------------


## platform specific settings -----------------------------

unix {

	DEFINES += LINUX
	LIBS	+= -lGL -lpcap

  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

win32 {

	LIBS	+= .\lib\win\wpcap.lib c:\MinGW\lib\libws2_32.a
	DEFINES += WIN
	INCLUDEPATH += include\win\winpcap4
}


## qt3 support settings -------------------------------------

#QT += qt3support

#DEFINES  += QT3_SUPPORT_WARNINGS

#The following line was inserted by qt3to4
QT +=  opengl
#The following line was inserted by qt3to4
#CONFIG += uic3

# Turn off C++ warnings while building
CONFIG += warn_off

