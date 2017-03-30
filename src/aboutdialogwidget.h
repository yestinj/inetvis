#ifndef ABOUTDIALOGWIDGET_H
#define ABOUTDIALOGWIDGET_H

#include <QWidget>
#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialogWidget : public QDialog, public Ui::ControlPanel {
    Q_OBJECT

    public:
        AboutDialogWidget(QWidget *parent = 0);
};
#endif // ABOUTDIALOGWIDGET_H
