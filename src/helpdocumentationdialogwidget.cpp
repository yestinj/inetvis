#include "helpdocumentationdialogwidget.h"

HelpDocumentationDialogWidget::HelpDocumentationDialogWidget(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    init();
}

void HelpDocumentationDialogWidget::init() {
    // TODO: Improve this
    QString documentationFileRef = QDir::currentPath() + "/../doc/inetvisdoc.html";
    QString altDocumentationText =
            "<h3>Documentation & Help pane</h3><p>if you see this message, there was an error reading the InetVis html manual providing documentation and help for the program. It should be installed in a 'doc/' directory relative to the inetvis binary.</p>";

    if (QFile::exists(documentationFileRef))
        helpTextBrowser->setSource(documentationFileRef);
    else
        helpTextBrowser->setText(altDocumentationText);
}
