#ifndef REFERENCEFRAMESETTINGSDIALOGWIDGET_H
#define REFERENCEFRAMESETTINGSDIALOGWIDGET_H

#include <QDialog>
#include "ui_referenceframesettingsdialog.h"

class ReferenceFrameSettingsDialogWidget : public QDialog, public Ui::ReferenceFrameSettingsDialog {
    Q_OBJECT
public:
    ReferenceFrameSettingsDialogWidget(QWidget *parent = 0);

signals:

public slots:

private:

};
#endif // REFERENCEFRAMESETTINGSDIALOGWIDGET_H
