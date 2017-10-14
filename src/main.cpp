/*******************************************************************
InetVis - Internet Visualisation
Version: 2.1.0
release date: 2017/09/21

Original Authors: Jean-Pierre van Riel, Barry Irwin
Initvis 2.x Authors: Yestin Johnson, Barry Irwin
Rhodes University
Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7
Computer Science Masters Project - 2017
author: Jean-Pierre van Riel
supervisor: Barry Irwin

InetVis - Internet Visualisation for network traffic.
Copyright (C) 2006-2017, Jean-Pierre van Riel, Barry Irwin, Yestin Johnson

*******************************************************************/

#include <QtWidgets>
#include <QApplication>
#include <QWidget>
#include <qgl.h>
#include <QMainWindow>
#include "logui.h"
#include "controlpanelwidget.h"
#include "visdisplaywidget.h"
#include "plottersettingsdialogwidget.h"
#include "dataproc.h"
#include "referenceframesettingsdialogwidget.h"
#include "log.h"
#include "generalsettingsdialog.h"


void initialiseQtSettings() {

    QCoreApplication::setOrganizationName("Rhodes University");
    QCoreApplication::setOrganizationDomain("ru.ac.za");
    QCoreApplication::setApplicationName("InetVis");

    //QSettings settings;
    //settings.clear();

    //QString recordPath = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).first() + "/" + "inetvis-recorded";

    if (!DataProcessor::isRecordDirSet()) {
        DataProcessor::setRecordDir(RECORD_DEFAULT_DIR_DEFAULT);
    }
    if (!DataProcessor::isPcapsDirSet()) {
        DataProcessor::setPcapsDir(RECORD_PCAPS_SUBDIR_DEFAULT);
    }
    if (!DataProcessor::isFramesDirSet()) {
        DataProcessor::setFramesDir(RECORD_FRAMES_SUBDIR_DEFAULT);
    }
    if (!DataProcessor::isSnapshotDirSet()) {
        DataProcessor::setSnapshotsDir(RECORD_SNAPSHOTS_SUBDIR_DEFAULT);
    }
    if (!DataProcessor::isDefaultHomeNetworkSet()) {
        DataProcessor::setLiveSubdir(RECORD_LIVE_SUBDIR_DEFAULT);
    }
    if (!DataProcessor::isShowHomeNetworkNotSetError()) {
        DataProcessor::setReplaySubdir(RECORD_REPLAY_SUBDIR_DEFAULT);
    }
    // Home network settings    
    if (!DataProcessor::isDefaultHomeNetworkSet()) {
        DataProcessor::setDefaultHomeNetwork(DEFAULT_HOME_NETWORK_DEFAULT);
    }
    if (!DataProcessor::isShowHomeNetworkNotSetError()) {
        DataProcessor::setShowHomeNetworkNotSetError(SHOW_HOME_NETWORK_NOT_SET_ERROR_DEFAULT);
    }
    if (!DataProcessor::isDefaultMonitorInterfaceSet()) {
        DataProcessor::setDefaultMonitorInterface(DEFAULT_MONITOR_INTERFACE_DEFAULT);
    }
    // Log file settings
    if (!Log::isLogRootDirSet()) {
        Log::setLogRootDir(LOG_ROOT_DIR_DEFAULT);
    }
    if (!Log::isStdoutFilenameSet()) {
        Log::setStdoutFilename(STDOUT_FILENAME_DEFAULT);
    }
    if (!Log::isStderrFilenameSet()) {
        Log::setStderrFilename(STDERR_FILENAME_DEFAULT);
    }
    // Frame snapshot settings
    if (!DataProcessor::isScreenshotFormatSet()) {
        DataProcessor::setScreenshotFormat(SCREENSHOT_FORMAT_DEFAULT);
    }
    if (!DataProcessor::isScreenshotQualitySet()) {
        DataProcessor::setScreenshotQuality(SCREENSHOT_QUALITY_DEFAULT);
    }
    if (!DataProcessor::isScreenshotExtensionSet()) {
        DataProcessor::setScreenshotExtension(SCREENSHOT_EXTENSION_DEFAULT);
    }
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(); //since both c++ streams and c printf are used for
    //debugging code output

    initialiseQtSettings();

    QApplication app(argc, argv);

    //Test and ensure that system has OpenGL support
    if (!QGLFormat::hasOpenGL()) {
        qWarning( "This system has no OpenGL support. Exiting.");
        return -1;
    }

    Log::enable();

    //uncomment the two lines below to enable user interface logging
    // facilitates logging user interaction
    //if(!LogUI::enable())   {
       //qWarning("Unable to open file for user interface logging - feature left disabled.");
    //}

    //declare support objects
    DataProcessor dp;  //provides backend for reading and parsing capture files
    LogUIQuit luiq; //small helper object to ensure UI logging closes gracefully
    LogQuit lq; //small helper object to ensure logging closes gracefully

    //declare GUI forms/windows
    ControlPanelWidget cp;
    VisDisplayWidget vd; //visaulization window

    GLVisWidget* vdw = vd.displayWidget;

    PlotterSettingsDialogWidget ps; //to set plotting features
    ReferenceFrameSettingsDialogWidget rfs; //to set reference frame features
    GeneralSettingsDialog gsd; //to set general settings

    //setup object reference links
    vdw->setDataProcLink(dp.getDataProcessorPtr());

    //connect signals and slots for control panel
    QObject::connect(vdw, SIGNAL(showControlPanel()), &cp, SLOT(showOnTop()));
    QObject::connect(&dp, SIGNAL(updateReplayTimeDisplay(const QDateTime &,
                                                         const QDateTime &)), &cp, SLOT(updateReplayTimeDisplay(
                                                                                            const QDateTime &, const QDateTime &)));
    QObject::connect(&dp, SIGNAL(playing()), &cp, SLOT(playing()));
    QObject::connect(&dp, SIGNAL(paused()), &cp, SLOT(paused()));
    QObject::connect(&dp, SIGNAL(setStatusBarMessage(QString)), &cp,
                     SLOT(statusBarMessage(QString)));
    QObject::connect(&dp, SIGNAL(updateReplayRateDisplay(double)), &cp,
                     SLOT(updateReplayRate(double)));
    QObject::connect(&dp, SIGNAL(updateTimeWindowDisplay(double)),
                     &cp, SLOT(updateTimeWindowDisplay(double)));
    QObject::connect(&dp, SIGNAL(setTimeRange(const QDateTime, const QDateTime)),
                     &cp, SLOT(setTimeRange(const QDateTime, const QDateTime)));
    QObject::connect(&dp, SIGNAL(setRecordButton(bool)),
                     &cp, SLOT(setRecordButton(bool)));
    QObject::connect(&dp, SIGNAL(sendErrMsg(QString)), &cp, SLOT(reportErrorMessage(QString)));

    //connect signals and slots for visualization display window and the visualization display widget
    QObject::connect(&cp, SIGNAL(showVisDisplayPanel()), &vd, SLOT(show()));
    QObject::connect(&dp, SIGNAL(updateGLVisWidget()), &vd, SLOT(updateGLVisWidget()));

    QObject::connect(vdw, SIGNAL(resetGLVisWidget()), &dp, SLOT(resetVisualisationPlane()));

    QObject::connect(&rfs, SIGNAL(setPerspectiveProjection()), vdw,
                     SLOT(setPerspectiveProjection()));
    QObject::connect(&rfs, SIGNAL(setOrthographicProjection()), vdw,
                     SLOT(setOrthographicProjection()));
    QObject::connect(&rfs, SIGNAL(setReferenceFrame(bool, bool, bool, bool, bool)),
                     vdw, SLOT(setReferenceFrame(bool, bool, bool, bool, bool)));
    QObject::connect(&rfs,
                     SIGNAL(setGridLines(bool, bool, bool, bool, bool, bool, bool, float)),
                     vdw,
                     SLOT(setGridLines(bool, bool, bool, bool, bool, bool, bool, float)));
    QObject::connect(&rfs, SIGNAL(setPartitions(int, int, int)), vdw,
                     SLOT(setPartitions(int, int, int)));
    QObject::connect(&ps, SIGNAL(changePointSize(int)), vdw,
                     SLOT(setPointSize(int)));
    QObject::connect(&cp, SIGNAL(captureFrames(bool)), vdw,
                     SLOT(captureFrames(bool)));
    QObject::connect(&cp, SIGNAL(captureSingleFrame()), vdw,
                     SLOT(captureCurrentFrame()));
    QObject::connect(&dp, SIGNAL(setXAxisLabels(const QString, const QString)), vdw,
                     SLOT(setXAxisLabels(const QString, const QString)));
    QObject::connect(&dp, SIGNAL(setYAxisLabels(const QString, const QString)), vdw,
                     SLOT(setYAxisLabels(const QString, const QString)));
    QObject::connect(&dp, SIGNAL(setZAxisLabels(const QString, const QString)), vdw,
                     SLOT(setZAxisLabels(const QString, const QString)));
    QObject::connect(&rfs, SIGNAL(displayAxisLabels(bool)), vdw,
                     SLOT(displayAxisLabels(bool)));
    QObject::connect(&rfs, SIGNAL(displayDateTime(bool)), vdw,
                     SLOT(displayDateTime(bool)));
    QObject::connect(&rfs, SIGNAL(displayFramerate(bool)), vdw,
                     SLOT(displayFramerate(bool)));
    QObject::connect(&dp, SIGNAL(setDateTimeReference(QDateTime*)), vdw,
                     SLOT(setDateTimeReference(QDateTime*)));
    QObject::connect(&ps, SIGNAL(setTransparentDecay(bool)), vdw,
                     SLOT(enableTransDecay(bool)));
    QObject::connect(&ps, SIGNAL(setPointSmoothing(bool)), vdw,
                     SLOT(enablePointSmoothing(bool)));
    QObject::connect(&ps, SIGNAL(setBackgroundColour(int)), vdw,
                     SLOT(setBackgroundCol(int)));

    //connect signals and slots for the plotting settings dialog
    QObject::connect(&cp, SIGNAL(showPlotterSettings()), &ps, SLOT(show()));
    QObject::connect(vdw, SIGNAL(showPlotterSettings()), &ps, SLOT(show()));
    QObject::connect(&dp, SIGNAL(updateHomeNetworkDisplay(int, int, int, int, int,
                                                          QString)), &ps, SLOT(updateHomeNetworkAddress(int, int, int, int,
                                                                                                        int, QString)));
    QObject::connect(&dp, SIGNAL(updateSourceNetworkDisplay(int, int, int, int, int,
                                                            QString)), &ps, SLOT(updateInternetNetworkRange(int, int, int, int,
                                                                                                            int, QString)));
    QObject::connect(&dp, SIGNAL(updatePortRangeDisplay(int, int)), &ps,
                     SLOT(updatePortRange(int, int)));

    //connect signals and slots for the reference frame dialog
    QObject::connect(&cp, SIGNAL(showReferenceFrameSettings()), &rfs, SLOT(show()));
    QObject::connect(vdw, SIGNAL(showReferenceFrameSettings()), &rfs, SLOT(show()));

    //connect signals and slots for the general settings dialog
    QObject::connect(&cp, SIGNAL(showGeneralSettings()), &gsd, SLOT(show()));
    QObject::connect(vdw, SIGNAL(showGeneralSettings()), &gsd, SLOT(show()));

    //connect signals and slots for data processor
    //dp.connect(&cp, SIGNAL(selectMode(int)), &dp, SLOT(setMode(int)));
    //from control panel
    QObject::connect(&cp, SIGNAL(play()), &dp, SLOT(play()));
    QObject::connect(&cp, SIGNAL(pause()), &dp, SLOT(pause()));
    QObject::connect(&cp, SIGNAL(togglePlayPause()), &dp, SLOT(togglePlayPause()));
    QObject::connect(vdw, SIGNAL(togglePlayPause()), &dp, SLOT(togglePlayPause()));
    QObject::connect(&cp, SIGNAL(setReplayRate(double)), &dp,
                     SLOT(setReplayRate(double)));
    QObject::connect(&cp, SIGNAL(setReplayPosition(const QDateTime)), &dp,
                     SLOT(setReplayPosition(const QDateTime)));
    QObject::connect(&cp, SIGNAL(setTimeWindow(double)), &dp,
                     SLOT(setTimeWindow(double)));
    QObject::connect(&cp, SIGNAL(selectReplayFile(const QString)), &dp,
                     SLOT(selectReplayFile(const QString)));
    QObject::connect(&cp, SIGNAL(setFilter(const QString)), &dp,
                     SLOT(setFilter(const QString)));
    QObject::connect(&cp, SIGNAL(setHomeNetwork(int, int, int, int, int)), &dp,
                     SLOT(setHomeNetwork(int, int, int, int, int)));
    QObject::connect(&cp, SIGNAL(selectNetworkInterface(const QString)), &dp,
                     SLOT(selectNetworkInterface(const QString)));
    QObject::connect(&cp, SIGNAL(recordToFile(bool)), &dp, SLOT(record(bool)));
    //from plotter settings
    //dp.connect(&ps, SIGNAL(), &dp, SLOT());
    QObject::connect(&ps, SIGNAL(setHomeNetwork(int, int, int, int, int)),
                     &dp, SLOT(setHomeNetwork(int, int, int, int, int)));
    QObject::connect(&ps, SIGNAL(setSourceNetwork(int, int, int, int, int)),
                     &dp, SLOT(setSourceNetwork(int, int, int, int, int)));
    QObject::connect(&ps, SIGNAL(setColourScheme(int)), &dp,
                     SLOT(setColourScheme(int)));
    QObject::connect(&ps, SIGNAL(setPortRange(int, int, int, int)), &dp,
                     SLOT(setPortRange(int, int, int, int)));
    QObject::connect(&ps, SIGNAL(guessHomeNetwork()), &dp, SLOT(guessHomeNetwork()));
    QObject::connect(&ps, SIGNAL(setTransparentDecay(bool)), &dp,
                     SLOT(setTransparentDecay(bool)));
    QObject::connect(&ps, SIGNAL(setPointBulge(bool)), &dp,
                     SLOT(enablePointBulge(bool)));
    QObject::connect(&ps, SIGNAL(setBackgroundColour(int)), &dp,
                     SLOT(changeBackgroundColour(int)));

    QObject::connect(vdw, SIGNAL(closeAllWindows()), &ps, SLOT(reject()));
    QObject::connect(vdw, SIGNAL(closeAllWindows()), &rfs, SLOT(reject()));
    QObject::connect(vdw, SIGNAL(closeAllWindows()), &gsd, SLOT(reject()));
    QObject::connect(vdw, SIGNAL(closeAllWindows()), &cp, SLOT(close()));

    //connect slot for reciving quit signal
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &luiq, SLOT(close()));
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &lq, SLOT(close()));
    QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    dp.init();

    // Only show the control panel once the main display widget is on screen.
    QObject::connect(&vd, SIGNAL(window_loaded()), &cp, SLOT(show()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    vd.show();

    return app.exec();
}
