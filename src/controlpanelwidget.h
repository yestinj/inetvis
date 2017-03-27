#ifndef CONTROLPANELWIDGET_H
#define CONTROLPANELWIDGET_H
#include "ui_controlpanel.h"
#include <QMainWindow>

class ControlPanelWidget : public QMainWindow, public Ui::ControlPanel {
    Q_OBJECT

    public:
        ControlPanelWidget(QMainWindow *parent = 0);

    private slots:
        void applyFilter();
        void applyReplayPostion();
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
        void viewReferenceFrameSettings();
        void helpDoc();
        void showOnTop();
        void updateReplayTimeDisplay();
        void playing();
        void paused();
        void statusBarMessage(QString);
        void updateReplayRate(double);
        void updateTimeWindowDisplay(double);
        void setTimeRange(const QDateTime, const QDateTime);
        void setRecordButton(bool);
        void reportErrorMessage(QString);


};

#endif // CONTROLPANELWIDGET_H
