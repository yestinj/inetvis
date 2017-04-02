#include "visdisplaywidget.h"

VisDisplayWidget::VisDisplayWidget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
}

void VisDisplayWidget::init() {
    //init widget flags
    setFocusProxy(displayWidget);
    //displayWidget can get keyboard events
}

void VisDisplayWidget::updateGLVisWidget() {
    displayWidget->update();
}

GLVisWidget* VisDisplayWidget::getPtrToVisPane() {
    return displayWidget;
}
