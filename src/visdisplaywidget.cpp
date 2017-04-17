#include "visdisplaywidget.h"

VisDisplayWidget::VisDisplayWidget(QWidget *parent) :
    QWidget(parent) {
    setupUi(this);
    // TODO: Check if this is needed
    init();
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
