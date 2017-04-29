#ifndef VISDISPLAYWIDGET_H
#define VISDISPLAYWIDGET_H

#include <QWidget>
#include "ui_visdisplay.h"
#include "logui.h"

class VisDisplayWidget : public QWidget, public Ui::VisDisplay {
    Q_OBJECT
public:
    VisDisplayWidget(QWidget *parent = 0);

public slots:
    void updateGLVisWidget();

private:
    void init();
    GLVisWidget* getPtrToVisPane();

};
#endif // VISDISPLAYWIDGET_H