#include "visdisplaywidget.h"
#include <QThread>

VisDisplayWidget::VisDisplayWidget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
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
