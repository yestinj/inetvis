#include "generalsettingsdialog.h"
#include "ui_generalsettingsdialog.h"

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingsDialog) {
    ui->setupUi(this);

    initializeSettingsDialog();

    qErrMsg = new QErrorMessage((QWidget*)this);
    qErrMsg->resize(QSize(400,240)); //set custom size to report error msg
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

void GeneralSettingsDialog::rootDirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load root directory button pressed");
    }

    QString loggingDefaultDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::rootDirSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save root directory button pressed");
    }

    // Write the new value
    QString newValue = ui->rootDirLineEdit->text();
    DataProcessor::setRecordDir(newValue);

    // Read it out just to make sure it worked.
    QString loggingDefaultDir = DataProcessor::getRecordDir();
    ui->rootDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::rootDirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset root directory button pressed");
    }

    DataProcessor::setRecordDir(RECORD_DEFAULT_DIR_DEFAULT);

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

void GeneralSettingsDialog::pcapSubdirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load pcap subdirectory button pressed");
    }

    QString pcapSubdir = DataProcessor::getPcapsDir();
    ui->pcapsSubdirLineEdit->setText(pcapSubdir);
}

void GeneralSettingsDialog::pcapSubdirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset pcap subdirectory button pressed");
    }

    DataProcessor::setPcapsDir(RECORD_PCAPS_SUBDIR_DEFAULT);

    // Read it out just to make sure it worked.
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

void GeneralSettingsDialog::framesSubdirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load frames subdirectory button pressed");
    }

    QString framesSubdir = DataProcessor::getFramesDir();
    ui->framesSubdirLineEdit->setText(framesSubdir);
}

void GeneralSettingsDialog::framesSubdirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset frames subdirectory button pressed");
    }

    DataProcessor::setFramesDir(RECORD_FRAMES_SUBDIR_DEFAULT);

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

void GeneralSettingsDialog::snapshotsSubdirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load snapshots subdirectory button pressed");
    }
    QString snapshotsSubdir = DataProcessor::getSnapshotsDir();
    ui->snapshotsSubdirLineEdit->setText(snapshotsSubdir);
}

void GeneralSettingsDialog::snapshotsSubdirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset snapshots subdirectory button pressed");
    }

    DataProcessor::setSnapshotsDir(RECORD_SNAPSHOTS_SUBDIR_DEFAULT);

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

void GeneralSettingsDialog::liveSubdirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load live subdirectory button pressed");
    }
    QString liveSubdir = DataProcessor::getLiveSubdir();
    ui->liveSubdirLineEdit->setText(liveSubdir);
}

void GeneralSettingsDialog::liveSubdirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Rest live subdirectory button pressed");
    }

    DataProcessor::setLiveSubdir(RECORD_LIVE_SUBDIR_DEFAULT);

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

void GeneralSettingsDialog::replaySubdirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load replay subdirectory button pressed");
    }
    QString replaySubdir = DataProcessor::getReplaySubdir();
    ui->replaySubdirLineEdit->setText(replaySubdir);
}

void GeneralSettingsDialog::replaySubdirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset replay subdirectory button pressed");
    }

    DataProcessor::setReplaySubdir(RECORD_REPLAY_SUBDIR_DEFAULT);

    QString replaySubdir = DataProcessor::getReplaySubdir();
    ui->replaySubdirLineEdit->setText(replaySubdir);
}

void GeneralSettingsDialog::defaultHomeNetworkSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save default home network button pressed");
    }
    // Write the new value
    QString newValue = ui->defaultHomeNetworkLineEdit->text();    

    bool success = DataProcessor::setDefaultHomeNetwork(newValue);

    // If the update failed, show an error.
    if (!success) {
        qErrMsg->showMessage("ERROR: Invalid IP address or subnet mask set. Please try again.");
    }

    // This will update to the last valid setting value.
    QString defaultHomeNetwork = DataProcessor::getDefaultHomeNetwork();
    ui->defaultHomeNetworkLineEdit->setText(defaultHomeNetwork);
}

void GeneralSettingsDialog::defaultHomeNetworkLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load default home network button pressed");
    }
    QString defaultHomeNetwork = DataProcessor::getDefaultHomeNetwork();
    ui->defaultHomeNetworkLineEdit->setText(defaultHomeNetwork);
}

void GeneralSettingsDialog::defaultHomeNetworkResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset default home network button pressed");
    }
    DataProcessor::setDefaultHomeNetwork(DEFAULT_HOME_NETWORK_DEFAULT);

    QString defaultHomeNetwork = DataProcessor::getDefaultHomeNetwork();
    ui->defaultHomeNetworkLineEdit->setText(defaultHomeNetwork);
}

void GeneralSettingsDialog::showHomeNetNotSetWarningSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save show home network not set warning button pressed");
    }
    // Write the new value
    QString newValue = ui->homeNetNotSetWarnLineEdit->text();    
    DataProcessor::setShowHomeNetworkNotSetError(newValue.toInt());

    bool showHomeNetNotSetWarning = DataProcessor::getShowHomeNetworkNotSetError();
    ui->homeNetNotSetWarnLineEdit->setText(QString::number(showHomeNetNotSetWarning));
}

void GeneralSettingsDialog::showHomeNetNotSetWarningLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load show home network not set warning button pressed");
    }
    bool showHomeNetNotSetWarning = DataProcessor::getShowHomeNetworkNotSetError();
    ui->homeNetNotSetWarnLineEdit->setText(QString::number(showHomeNetNotSetWarning));
}

void GeneralSettingsDialog::showHomeNetNotSetWarningResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset show home network not set warning button pressed");
    }

    DataProcessor::setShowHomeNetworkNotSetError(SHOW_HOME_NETWORK_NOT_SET_ERROR_DEFAULT);

    bool showHomeNetNotSetWarning = DataProcessor::getShowHomeNetworkNotSetError();
    ui->homeNetNotSetWarnLineEdit->setText(QString::number(showHomeNetNotSetWarning));
}

void GeneralSettingsDialog::defaultMonitoringInterfaceSaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save default monitor interface button pressed");
    }
    // Write the new value
    QString newValue = ui->defaultMonitorInterfaceLineEdit->text();
    DataProcessor::setDefaultMonitorInterface(newValue);

    QString defaultMonitorInterface = DataProcessor::getDefaultMonitorInterface();
    ui->defaultMonitorInterfaceLineEdit->setText(defaultMonitorInterface);
}

void GeneralSettingsDialog::defaultMonitoringInterfaceLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load default monitor interface button pressed");
    }
    QString defaultMonitorInterface = DataProcessor::getDefaultMonitorInterface();
    ui->defaultMonitorInterfaceLineEdit->setText(defaultMonitorInterface);
}

void GeneralSettingsDialog::defaultMonitoringInterfaceResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset default monitor interface button pressed");
    }
    DataProcessor::setDefaultMonitorInterface(DEFAULT_MONITOR_INTERFACE_DEFAULT);

    QString defaultMonitorInterface = DataProcessor::getDefaultMonitorInterface();
    ui->defaultMonitorInterfaceLineEdit->setText(defaultMonitorInterface);
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

void GeneralSettingsDialog::logRootDirLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load log root directory button pressed");
    }
    QString loggingDefaultDir = Log::getLogRootDir();
    ui->rootLogDirLineEdit->setText(loggingDefaultDir);
}

void GeneralSettingsDialog::logRootDirResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset log root directory button pressed");
    }

    Log::setLogRootDir(LOG_ROOT_DIR_DEFAULT);

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

void GeneralSettingsDialog::stdoutFilenameLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load stdout filename button pressed");
    }
    QString stdoutFilename = Log::getStdoutFilename();
    ui->stdoutFilenameLineEdit->setText(stdoutFilename);
}

void GeneralSettingsDialog::stdoutFilenameResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset stdout filename button pressed");
    }

    Log::setStdoutFilename(STDOUT_FILENAME_DEFAULT);

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

void GeneralSettingsDialog::stderrFilenameLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load stderr filename button pressed");
    }
    QString stderrFilename = Log::getStderrFilename();
    ui->stderrFilenameLineEdit->setText(stderrFilename);
}

void GeneralSettingsDialog::stderrFilenameResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset stderr filename button pressed");
    }
    Log::setStderrFilename(STDERR_FILENAME_DEFAULT);

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

void GeneralSettingsDialog::snapshotFormatLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load snapshot format button pressed");
    }
    QString screenshotFormat = DataProcessor::getScreenshotFormat();
    ui->snapshotFormatLineEdit->setText(screenshotFormat);
}

void GeneralSettingsDialog::snapshotFormatResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset snapshot format button pressed");
    }
    DataProcessor::setScreenshotFormat(SCREENSHOT_FORMAT_DEFAULT);

    QString screenshotFormat = DataProcessor::getScreenshotFormat();
    ui->snapshotFormatLineEdit->setText(screenshotFormat);
}

void GeneralSettingsDialog::snapshotQualitySaveAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Save snapshot quality button pressed");
    }
    // Write the new value
    QString newValue = ui->snapshotQualityLineEdit->text();
    DataProcessor::setScreenshotQuality(newValue.toInt());

    int screenshotQuality = DataProcessor::getScreenshotQuality();
    ui->snapshotQualityLineEdit->setText(QString::number(screenshotQuality));
}

void GeneralSettingsDialog::snapshotQualityLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load snapshot quality button pressed");
    }
    int screenshotQuality = DataProcessor::getScreenshotQuality();
    ui->snapshotQualityLineEdit->setText(QString::number(screenshotQuality));
}

void GeneralSettingsDialog::snapshotQualityResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset snapshot quality button pressed");
    }
    DataProcessor::setScreenshotQuality(SCREENSHOT_QUALITY_DEFAULT);

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

void GeneralSettingsDialog::snapshotExtensionLoadAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load snapshot extension button pressed");
    }
    QString screenshotExtension = DataProcessor::getScreenshotExtension();
    ui->snapshotExtensionLineEdit->setText(screenshotExtension);
}

void GeneralSettingsDialog::snapshotExtensionResetAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset snapshot extension button pressed");
    }
    DataProcessor::setScreenshotExtension(SCREENSHOT_EXTENSION_DEFAULT);

    QString screenshotExtension = DataProcessor::getScreenshotExtension();
    ui->snapshotExtensionLineEdit->setText(screenshotExtension);
}

void GeneralSettingsDialog::resetAllSettingsAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Reset all settings button pressed");
    }

    // TODO: See if there's a better way to do this.. Perhaps a list/map?
    rootDirResetAction();
    pcapSubdirResetAction();
    framesSubdirResetAction();
    snapshotsSubdirResetAction();
    liveSubdirResetAction();
    replaySubdirResetAction();
    defaultHomeNetworkResetAction();
    showHomeNetNotSetWarningResetAction();
    logRootDirResetAction();
    stdoutFilenameResetAction();
    stderrFilenameResetAction();
    snapshotFormatResetAction();
    snapshotQualityResetAction();
    snapshotExtensionResetAction();  
}

void GeneralSettingsDialog::loadAllSettingsAction() {
    if (LogUI::isEnabled()) {
        LogUI::logEvent("[GS] Load all settings from disk button pressed");
    }

    rootDirLoadAction();
    pcapSubdirLoadAction();
    framesSubdirLoadAction();
    snapshotsSubdirLoadAction();
    liveSubdirLoadAction();
    replaySubdirLoadAction();
    defaultHomeNetworkLoadAction();
    showHomeNetNotSetWarningLoadAction();
    logRootDirLoadAction();
    stdoutFilenameLoadAction();
    stderrFilenameLoadAction();
    snapshotFormatLoadAction();
    snapshotQualityLoadAction();
    snapshotExtensionLoadAction();
}
