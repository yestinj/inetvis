#include "helpdocumentationdialogwidget.h"
#include <iostream>

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

HelpDocumentationDialogWidget::HelpDocumentationDialogWidget(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    init();
}

void HelpDocumentationDialogWidget::init() {

    QString documentationFileRef = QDir::currentPath() + "/../doc/inetvisdoc.html";

    // TODO: This needs to be more robust and support different OSes.
    if (!QFile::exists(documentationFileRef)) {
        documentationFileRef = "/opt/inetvis/doc/inetvisdoc.html";
    }

    if (QFile::exists(documentationFileRef)) {
        helpTextBrowser->setSource(documentationFileRef);
    }
    else {

        QString altDocumentationText =
                "<h3>Documentation & Help pane</h3><p>if you see this message, there was an error reading the InetVis html manual providing documentation and help for the program. It should be installed in a 'doc/' directory relative to the inetvis binary.</p>";

        helpTextBrowser->setText(altDocumentationText);
    }
}
