#include "helpdocumentationdialogwidget.h"
#include <iostream>

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
