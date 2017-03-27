#include "controlpanelwidget.h"
#include <iostream>

ControlPanelWidget::ControlPanelWidget(QMainWindow *parent) : QMainWindow(parent) {
    setupUi(this);
}

void ControlPanelWidget::applyFilter() {
    std::cout << "HELLO WORLD!!!";
}

void ControlPanelWidget::applyReplayPostion() {

}

void ControlPanelWidget::applyTimeWindow() {

}

void ControlPanelWidget::fileOpen() {

}

void ControlPanelWidget::helpAbout() {

}

void ControlPanelWidget::monitorLocalHostSelected() {

}

void ControlPanelWidget::pauseMenuAction() {

}

void ControlPanelWidget::playMenuAction() {

}

void ControlPanelWidget::playPauseToolButtonClicked() {

}

void ControlPanelWidget::takeSnapshot() {

}

void ControlPanelWidget::captureFramesToFile(bool) {

}

void ControlPanelWidget::record(bool) {

}

void ControlPanelWidget::replayFileModeSelected() {

}

void ControlPanelWidget::replayPositionSliderReleased() {

}

void ControlPanelWidget::replayPositionSliderChanged(int) {

}

void ControlPanelWidget::setNormalReplaySpeed() {

}

void ControlPanelWidget::timeScaleLineEditChanged() {

}

void ControlPanelWidget::timeScaleSliderChanged(int) {

}

void ControlPanelWidget::timeSliderScaleRealeased() {

}

void ControlPanelWidget::timeWindowChange() {

}

void ControlPanelWidget::timeWindowSliderChanged(int) {

}

void ControlPanelWidget::timeWindowSliderReleased() {

}

void ControlPanelWidget::viewVisDisplayPanel() {

}

void ControlPanelWidget::viewPlotterSettings() {

}

void ControlPanelWidget::viewReferenceFrameSettings() {

}

void ControlPanelWidget::helpDoc() {

}

void ControlPanelWidget::showOnTop() {

}

void ControlPanelWidget::updateReplayTimeDisplay() {

}

void ControlPanelWidget::playing() {

}

void ControlPanelWidget::paused() {

}

void ControlPanelWidget::statusBarMessage(QString) {

}

void ControlPanelWidget::updateReplayRate(double) {

}

void ControlPanelWidget::updateTimeWindowDisplay(double) {

}

void ControlPanelWidget::setTimeRange(const QDateTime, const QDateTime) {

}

void ControlPanelWidget::setRecordButton(bool) {

}

void ControlPanelWidget::reportErrorMessage(QString) {

}
