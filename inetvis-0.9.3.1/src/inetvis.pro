# InetVis 0.9.3-qt3, 2007-11-08

TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt opengl stl

LIBS	+= -lGL -lpcap

HEADERS	+= dataproc.h \
	graphicelement.h \
	packetheaders.h \
	timeutil.h \
	glviswidget.h \
	plotter.h \
	packetevent.h \
	logui.h

SOURCES	+= graphicelement.cpp \
	packetheaders.cpp \
	plotter.cpp \
	timeutil.cpp \
	main.cpp \
	glviswidget.cpp \
	dataproc.cpp \
	packetevent.cpp \
	logui.cpp

FORMS	= controlpanel.ui \
	visdisplay.ui \
	plottersettingsdialog.ui \
	referenceframesettingsdialog.ui \
	aboutdialog.ui \
	helpdocumentationdialog.ui

IMAGES	= images/emblem-pictures.png \
	images/fileopen \
	images/filesave \
	images/stock_insert-video-plugin.png \
	images/stock_media-fwd.png \
	images/stock_media-pause.png \
	images/stock_media-play.png \
	images/stock_media-rec.png \
	images/stock_media-rew.png \
	images/stock_calc-accept.png

## CUSTOM SETTINGS ----------------------------------------

## Build Mode
#CONFIG   += debug warn_on
CONFIG   += release
### uncomment one or the other above

## Linking
CONFIG   += create_prl link_prl static
QMAKE_LFLAGS_RELEASE = -m32

## Optimization flags
QMAKE_CFLAGS_RELEASE = -O3 -march=pentium4 -m32 -mfpmath=sse -mmmx -msse -msse2 -mno-sse3 -momit-leaf-frame-pointer -maccumulate-outgoing-args
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE

## end of CUSTOM SETTINGS ---------------------------------

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



