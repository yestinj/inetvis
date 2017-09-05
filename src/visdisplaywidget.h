#ifndef VISDISPLAYWIDGET_H
#define VISDISPLAYWIDGET_H

#include <QWidget>
#include "ui_visdisplay.h"
#include "logui.h"
#include <QCloseEvent>

class VisDisplayWidget : public QWidget, public Ui::VisDisplay {
    Q_OBJECT
public:
    VisDisplayWidget(QWidget *parent = 0);

protected:
    void showEvent(QShowEvent *ev);

signals:
    void window_loaded();

public slots:
    void updateGLVisWidget();

private:
    void init();
    GLVisWidget* getPtrToVisPane();
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent *event);

};
#endif // VISDISPLAYWIDGET_H
