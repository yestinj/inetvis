#ifndef HELPDOCUMENTATIONDIALOGWIDGET_H
#define HELPDOCUMENTATIONDIALOGWIDGET_H

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
#include "ui_helpdocumentationdialog.h"
#include <QDir>

class HelpDocumentationDialogWidget : public QDialog, public Ui::HelpDocumentationDialog {
    Q_OBJECT
public:
    HelpDocumentationDialogWidget(QWidget *parent = 0);
private:
    void init();
};
#endif // HELPDOCUMENTATIONDIALOGWIDGET_H
