#ifndef HELPDOCUMENTATIONDIALOGWIDGET_H
#define HELPDOCUMENTATIONDIALOGWIDGET_H

#include <QWidget>
#include <QDialog>
#include "ui_helpdocumentationdialog.h"

class HelpDocumentationDialogWidget : public QDialog, public Ui::HelpDocumentationDialog {
    Q_OBJECT
public:
    HelpDocumentationDialogWidget(QWidget *parent = 0);

signals:

public slots:

};

#endif // HELPDOCUMENTATIONDIALOGWIDGET_H
