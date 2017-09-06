#ifndef ABOUTDIALOGWIDGET_H
#define ABOUTDIALOGWIDGET_H

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
#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialogWidget : public QDialog, public Ui::AboutDialog {
    Q_OBJECT

public:
    AboutDialogWidget(QWidget *parent = 0);
};
#endif // ABOUTDIALOGWIDGET_H
