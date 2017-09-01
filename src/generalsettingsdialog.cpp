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
    QString recordDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(recordDir);

    QString pcapSubdir = DataProcessor::getPcapsDir();
    ui->pcapsSubdirLineEdit->setText(pcapSubdir);

    QString framesSubdir = DataProcessor::getFramesDir();
    ui->framesSubdirLineEdit->setText(framesSubdir);

    QString snapshotsSubdir = DataProcessor::getSnapshotsDir();
    ui->snapshotsSubdirLineEdit->setText(snapshotsSubdir);

    QString liveSubdir = DataProcessor::getLiveSubdir();
    ui->liveSubdirLineEdit->setText(liveSubdir);

    QString replaySubdir = DataProcessor::getReplaySubdir();
    ui->replaySubdirLineEdit->setText(replaySubdir);

    QString defaultHomeNetwork = DataProcessor::getDefaultHomeNetwork();
    ui->defaultHomeNetworkLineEdit->setText(defaultHomeNetwork);

    bool showHomeNetNotSetWarning = DataProcessor::getShowHomeNetworkNotSetError();
    ui->homeNetNotSetWarnLineEdit->setText(QString::number(showHomeNetNotSetWarning));

    QString loggingDefaultDir = Log::getLogRootDir();
    ui->rootLogDirLineEdit->setText(loggingDefaultDir);

    QString stdoutFilename = Log::getStdoutFilename();
    ui->stdoutFilenameLineEdit->setText(stdoutFilename);

    QString stderrFilename = Log::getStderrFilename();
    ui->stderrFilenameLineEdit->setText(stderrFilename);

    QString screenshotFormat = DataProcessor::getScreenshotFormat();
    ui->snapshotFormatLineEdit->setText(screenshotFormat);

    int screenshotQuality = DataProcessor::getScreenshotQuality();
    ui->snapshotQualityLineEdit->setText(QString::number(screenshotQuality));

    QString screenshotExtension = DataProcessor::getScreenshotExtension();
    ui->snapshotExtensionLineEdit->setText(screenshotExtension);
}

void GeneralSettingsDialog::rootDirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo root directory button pressed");
    }
    // TODO: Look at de-duplicating this
    QString loggingDefaultDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::rootDirSaveAction() {
    if (LogUI::isEnabled()) {
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

void GeneralSettingsDialog::pcapSubdirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save pcap subdirectory button pressed");
    }
    // Write the new value
    QString newValue = ui->pcapsSubdirLineEdit->text();
    DataProcessor::setPcapsDir(newValue);

    // Read it out just to make sure it worked.
    QString pcapSubdir = DataProcessor::getPcapsDir();
    ui->pcapsSubdirLineEdit->setText(pcapSubdir);
}

void GeneralSettingsDialog::pcapSubdirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo pcap subdirectory button pressed");
    }

    QString pcapSubdir = DataProcessor::getPcapsDir();
    ui->pcapsSubdirLineEdit->setText(pcapSubdir);
}

void GeneralSettingsDialog::framesSubdirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save frames subdirectory button pressed");
    }
    // Write the new value
    QString newValue = ui->framesSubdirLineEdit->text();
    DataProcessor::setFramesDir(newValue);

    // Read it out just to make sure it worked.
    QString framesSubdir = DataProcessor::getFramesDir();
    ui->framesSubdirLineEdit->setText(framesSubdir);
}

void GeneralSettingsDialog::framesSubdirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo frames subdirectory button pressed");
    }

    QString framesSubdir = DataProcessor::getFramesDir();
    ui->framesSubdirLineEdit->setText(framesSubdir);
}

void GeneralSettingsDialog::snapshotsSubdirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save snapshots subdirectory button pressed");
    }
    // Write the new value
    QString newValue = ui->snapshotsSubdirLineEdit->text();
    DataProcessor::setSnapshotsDir(newValue);

    // Read it out just to make sure it worked.
    QString snapshotsSubdir = DataProcessor::getSnapshotsDir();
    ui->snapshotsSubdirLineEdit->setText(snapshotsSubdir);
}

void GeneralSettingsDialog::snapshotsSubdirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo snapshots subdirectory button pressed");
    }
    QString snapshotsSubdir = DataProcessor::getSnapshotsDir();
    ui->snapshotsSubdirLineEdit->setText(snapshotsSubdir);
}

void GeneralSettingsDialog::liveSubdirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save live subdirectory button pressed");
    }

    // Write the new value
    QString newValue = ui->liveSubdirLineEdit->text();
    DataProcessor::setLiveSubdir(newValue);

    // Read it out just to make sure it worked.
    QString liveSubdir = DataProcessor::getLiveSubdir();
    ui->liveSubdirLineEdit->setText(liveSubdir);
}

void GeneralSettingsDialog::liveSubdirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undolive subdirectory button pressed");
    }
    QString liveSubdir = DataProcessor::getLiveSubdir();
    ui->liveSubdirLineEdit->setText(liveSubdir);
}

void GeneralSettingsDialog::replaySubdirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save replay subdirectory button pressed");
    }
    // Write the new value
    QString newValue = ui->replaySubdirLineEdit->text();
    DataProcessor::setReplaySubdir(newValue);

    // Read it out just to make sure it worked.
    QString replaySubdir = DataProcessor::getReplaySubdir();
    ui->replaySubdirLineEdit->setText(replaySubdir);
}

void GeneralSettingsDialog::replaySubdirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo replay subdirectory button pressed");
    }
    QString replaySubdir = DataProcessor::getReplaySubdir();
    ui->replaySubdirLineEdit->setText(replaySubdir);
}

void GeneralSettingsDialog::defaultHomeNetworkSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save default home network button pressed");
    }
    // Write the new value
    QString newValue = ui->defaultHomeNetworkLineEdit->text();
    // TODO: Add some parsing here
    DataProcessor::setDefaultHomeNetwork(0, 0,0, 0, 0);

    QString defaultHomeNetwork = DataProcessor::getDefaultHomeNetwork();
    ui->defaultHomeNetworkLineEdit->setText(defaultHomeNetwork);
}

void GeneralSettingsDialog::defaultHomeNetworkUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo default home network button pressed");
    }
    QString defaultHomeNetwork = DataProcessor::getDefaultHomeNetwork();
    ui->defaultHomeNetworkLineEdit->setText(defaultHomeNetwork);
}

void GeneralSettingsDialog::showHomeNetNotSetWarningSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save show home network not set warning button pressed");
    }
    // Write the new value
    QString newValue = ui->homeNetNotSetWarnLineEdit->text();
    // Add validation and conversion
    DataProcessor::setShowHomeNetworkNotSetError(1);

    bool showHomeNetNotSetWarning = DataProcessor::getShowHomeNetworkNotSetError();
    ui->homeNetNotSetWarnLineEdit->setText(QString(showHomeNetNotSetWarning));
}

void GeneralSettingsDialog::showHomeNetNotSetWarningUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo show home network not set warning button pressed");
    }
    bool showHomeNetNotSetWarning = DataProcessor::getShowHomeNetworkNotSetError();
    ui->homeNetNotSetWarnLineEdit->setText(QString::number(showHomeNetNotSetWarning));
}

void GeneralSettingsDialog::logRootDirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save log root directory button pressed");
    }
    // Write the new value
    QString newValue = ui->rootLogDirLineEdit->text();
    Log::setLogRootDir(newValue);

    QString loggingDefaultDir = Log::getLogRootDir();
    ui->rootLogDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::logRootDirUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo log root directory button pressed");
    }
    QString loggingDefaultDir = Log::getLogRootDir();
    ui->rootLogDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::stdoutFilenameSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save stdout filename button pressed");
    }
    // Write the new value
    QString newValue = ui->stdoutFilenameLineEdit->text();
    Log::setStdoutFilename(newValue);

    QString stdoutFilename = Log::getStdoutFilename();
    ui->stdoutFilenameLineEdit->setText(stdoutFilename);
}

void GeneralSettingsDialog::stdoutFilenameUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo stdout filename button pressed");
    }
    QString stdoutFilename = Log::getStdoutFilename();
    ui->stdoutFilenameLineEdit->setText(stdoutFilename);
}

void GeneralSettingsDialog::stderrFilenameSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save stderr filename button pressed");
    }
    // Write the new value
    QString newValue = ui->stderrFilenameLineEdit->text();
    Log::setStderrFilename(newValue);

    QString stderrFilename = Log::getStderrFilename();
    ui->stderrFilenameLineEdit->setText(stderrFilename);
}

void GeneralSettingsDialog::stderrFilenameUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo stderr filename button pressed");
    }
    QString stderrFilename = Log::getStderrFilename();
    ui->stderrFilenameLineEdit->setText(stderrFilename);
}

void GeneralSettingsDialog::snapshotFormatSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save snapshot format button pressed");
    }

    // Write the new value
    QString newValue = ui->snapshotFormatLineEdit->text();
    DataProcessor::setScreenshotFormat(newValue);

    QString screenshotFormat = DataProcessor::getScreenshotFormat();
    ui->snapshotFormatLineEdit->setText(screenshotFormat);
}

void GeneralSettingsDialog::snapshotFormatUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo snapshot format button pressed");
    }
    QString screenshotFormat = DataProcessor::getScreenshotFormat();
    ui->snapshotFormatLineEdit->setText(screenshotFormat);
}

void GeneralSettingsDialog::snapshotQualitySaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save snapshot quality button pressed");
    }
    // Write the new value
    QString newValue = ui->snapshotQualityLineEdit->text();
    // TODO: Proper convrsion + validation
    DataProcessor::setScreenshotQuality(-1);

    int screenshotQuality = DataProcessor::getScreenshotQuality();
    ui->snapshotQualityLineEdit->setText(QString(screenshotQuality));
}

void GeneralSettingsDialog::snapshotQualityUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo snapshot quality button pressed");
    }
    int screenshotQuality = DataProcessor::getScreenshotQuality();
    ui->snapshotQualityLineEdit->setText(QString::number(screenshotQuality));
}

void GeneralSettingsDialog::snapshotExtensionSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save snapshot extension button pressed");
    }
    // Write the new value
    QString newValue = ui->snapshotExtensionLineEdit->text();
    DataProcessor::setScreenshotExtension(newValue);

    QString screenshotExtension = DataProcessor::getScreenshotExtension();
    ui->snapshotExtensionLineEdit->setText(screenshotExtension);
}

void GeneralSettingsDialog::snapshotExtensionUndoAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Undo snapshot extension button pressed");
    }
    QString screenshotExtension = DataProcessor::getScreenshotExtension();
    ui->snapshotExtensionLineEdit->setText(screenshotExtension);
}

