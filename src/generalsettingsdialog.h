#ifndef GENERALSETTINGSDIALOG_H
#define GENERALSETTINGSDIALOG_H

#include <QDialog>
#include "dataproc.h"
#include "logui.h"

namespace Ui {
class GeneralSettingsDialog;
}

class GeneralSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralSettingsDialog(QWidget *parent = 0);
    ~GeneralSettingsDialog();

public slots:
    void rootDirUndoAction();
    void rootDirSaveAction();

private:
    Ui::GeneralSettingsDialog *ui;
    void initializeSettingsDialog();
};

#endif // GENERALSETTINGSDIALOG_H
