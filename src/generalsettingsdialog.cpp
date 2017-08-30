#include "generalsettingsdialog.h"
#include "ui_generalsettingsdialog.h"

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingsDialog) {
    ui->setupUi(this);

    initializeSettingsDialog();
}

GeneralSettingsDialog::~GeneralSettingsDialog() {
    delete ui;
}

void GeneralSettingsDialog::initializeSettingsDialog() {
    QString loggingDefaultDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::rootDirUndoAction() {
    if(LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo root directory button pressed");
    }
    // TODO: Look at de-duplicating this
    QString loggingDefaultDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::rootDirSaveAction() {
    if(LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save root directory button pressed");
    }

    // TODO: Add validation!!!

    // Write the new value
    QString newValue = ui->rootDirLineEdit->text();
    DataProcessor::setRecordDir(newValue);

    // Read it out just to make sure it worked.
    QString loggingDefaultDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(loggingDefaultDir);
}
