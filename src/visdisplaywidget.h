#ifndef VISDISPLAYWIDGET_H
#define VISDISPLAYWIDGET_H

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
