#include "visdisplaywidget.h"
#include <QThread>

VisDisplayWidget::VisDisplayWidget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    readSettings();
    setFocusProxy(displayWidget);
}

void VisDisplayWidget::updateGLVisWidget() {
    displayWidget->update();
}

GLVisWidget* VisDisplayWidget::getPtrToVisPane() {
    return displayWidget;
}

void VisDisplayWidget::showEvent(QShowEvent *ev) {
    QWidget::showEvent(ev);
    // This is a hack more than anything. Delay so that control panel
    // won't overlap the vis display widget
    QThread::sleep(1);
    emit window_loaded();
}

void VisDisplayWidget::writeSettings() {
    QSettings settings;
    settings.beginGroup("glviswidget");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void VisDisplayWidget::readSettings() {
    QSettings settings;
    settings.beginGroup("glviswidget");
    resize(settings.value("size", QSize(704, 576)).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    settings.endGroup();
}

void VisDisplayWidget::closeEvent(QCloseEvent *event) {
    writeSettings();
    event->accept();
}
