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
        controlpanelwidget.h \
        aboutdialogwidget.h \
        helpdocumentationdialogwidget.h \
        plottersettingsdialogwidget.h \
        referenceframesettingsdialogwidget.h \
        visdisplaywidget.h

SOURCES	+= graphicelement.cpp \
	packetheaders.cpp \
	plotter.cpp \
	timeutil.cpp \
	main.cpp \
	glviswidget.cpp \
	dataproc.cpp \
	packetevent.cpp \
	logui.cpp \
        controlpanelwidget.cpp \
        aboutdialogwidget.cpp \
        helpdocumentationdialogwidget.cpp \
        plottersettingsdialogwidget.cpp \
        referenceframesettingsdialogwidget.cpp \
        visdisplaywidget.cpp

FORMS = controlpanel.ui \
	visdisplay.ui \
	plottersettingsdialog.ui \
	referenceframesettingsdialog.ui \
	aboutdialog.ui \
	helpdocumentationdialog.ui

RESOURCES += icons.qrc	

## CUSTOM SETTINGS ----------------------------------------

## Build Mode
CONFIG   += debug warn_on
#CONFIG	  += debug
#CONFIG   += release
### uncomment one or the other above

# Turn off C++ warnings while building
CONFIG += warn_off

## Linking
CONFIG   += create_prl link_prl static

## Optimization flags
QMAKE_CFLAGS_RELEASE = -O3 -march=native -mfpmath=sse -mmmx -msse -msse2 -mno-sse3 -momit-leaf-frame-pointer
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE

QT +=  opengl

## end of CUSTOM SETTINGS ---------------------------------

## platform specific settings -----------------------------
macx {
    DEFINES += MAC
    LIBS += -lpcap
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj

} else:unix {

    DEFINES += LINUX
    LIBS += -lpcap
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj

} else:win32 {

    LIBS += .\lib\win\wpcap.lib c:\MinGW\lib\libws2_32.a
    DEFINES += WIN
    INCLUDEPATH += include\win\winpcap4
}
## end platform specific settings
