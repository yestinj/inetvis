#ifndef CONTROLPANELWIDGET_H
#define CONTROLPANELWIDGET_H

#include "ui_controlpanel.h"
#include <QMainWindow>
#include <QDateTime>
#include <QStatusBar>
#include <QValidator>
#include <QFileDialog>
#include <QErrorMessage>
#include <QDoubleValidator>
#include "aboutdialogwidget.h"
#include "helpdocumentationdialogwidget.h"
#include "logui.h"
#include "generalsettingsdialog.h"
#include <iostream>
#include <math.h>
#include "timeutil.h"

#define DISABLE_RECORDING_LIVE_CAPTURE true

class ControlPanelWidget : public QMainWindow, public Ui::ControlPanel {
    Q_OBJECT

public:
    ControlPanelWidget(QMainWindow *parent = 0);
    ~ControlPanelWidget();
    void updateReplayTimeSliderPosition(const QDateTime&);
    void updateTimeWindowSlider(double);

public slots:
    void applyFilter();
    void applyReplayPosition();
    void applyTimeWindow();
    void fileOpen();
    void helpAbout();
    void monitorLocalHostSelected();
    void pauseMenuAction();
    void playMenuAction();
    void playPauseToolButtonClicked();
    void takeSnapshot();
    void captureFramesToFile(bool);
    void record(bool);
    void replayFileModeSelected();
    void replayPositionSliderReleased();
    void replayPositionSliderChanged(int);
    void setNormalReplaySpeed();
    void timeScaleLineEditChanged();
    void timeScaleSliderChanged(int);
    void timeSliderScaleRealeased();
    void timeWindowChange();
    void timeWindowSliderChanged(int);
    void timeWindowSliderReleased();
    void viewVisDisplayPanel();
    void viewPlotterSettings();
    void viewGeneralSettings();
    void viewReferenceFrameSettings();
    void helpDoc();
    void showOnTop();
    void updateReplayTimeDisplay(const QDateTime&, const QDateTime&);
    void playing();
    void paused();
    void statusBarMessage(QString);
    void updateReplayRate(double);
    void updateTimeWindowDisplay(double);
    void setTimeRange(const QDateTime, const QDateTime);
    void setRecordButton(bool);
    void reportErrorMessage(QString);
    void replayPositionDateTimeChanged();    

signals:
    void play();
    void pause();
    void showVisDisplayPanel();
    void togglePlayPause();
    void setReplayRate(double rate);
    void setReplayPosition(const QDateTime newRepPos);
    void setTimeWindow(double seconds);
    void selectReplayFile(const QString file);
    void setFilter(const QString filterExpr);
    void setHomeNetwork(int a, int b, int c, int d, int slash);
    void showHomeNetworkDialog();
    void selectNetworkInterface(const QString);
    void showPlotterSettings();
    void showReferenceFrameSettings();
    void showGeneralSettings();
    void captureFrames(bool);
    void captureSingleFrame();
    void recordToFile(bool);

private:
    double startTime;
    bool timeWinSetByUser;
    bool replayPosSetByUser;
    float timeScale;
    QIcon *playIcon;
    QIcon *pauseIcon;
    QLabel *packetEventsLabel;
    QDoubleValidator *timeScaleValidator;
    double timeSpan;
    bool replayPosSetBySlider;
    bool timeWindowSetBySlider;
    bool replayPositionSliderChangeStarted;
    bool recordToPcapFileSetByUser;
    bool captureFramesSetByUser;
    QErrorMessage *qErrMsg;
    AboutDialogWidget *aboutDialog;
    HelpDocumentationDialogWidget *helpDialog;
    QString loguiFileName;
    void init();
    void destroy();
    QString strGetRepPos();

};
#endif // CONTROLPANELWIDGET_H
