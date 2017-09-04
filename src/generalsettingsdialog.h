#ifndef GENERALSETTINGSDIALOG_H
#define GENERALSETTINGSDIALOG_H

#include <QDialog>
#include <QStandardPaths>

#include "dataproc.h"
#include "logui.h"
#include "QErrorMessage"

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
    void rootDirLoadAction();
    void rootDirSaveAction();
    void rootDirResetAction();
    void pcapSubdirSaveAction();
    void pcapSubdirLoadAction();
    void pcapSubdirResetAction();
    void framesSubdirSaveAction();
    void framesSubdirLoadAction();
    void framesSubdirResetAction();
    void snapshotsSubdirSaveAction();
    void snapshotsSubdirLoadAction();
    void snapshotsSubdirResetAction();
    void liveSubdirSaveAction();
    void liveSubdirLoadAction();
    void liveSubdirResetAction();
    void replaySubdirSaveAction();
    void replaySubdirLoadAction();
    void replaySubdirResetAction();
    void defaultHomeNetworkSaveAction();
    void defaultHomeNetworkLoadAction();
    void defaultHomeNetworkResetAction();
    void showHomeNetNotSetWarningSaveAction();
    void showHomeNetNotSetWarningLoadAction();
    void showHomeNetNotSetWarningResetAction();
    void logRootDirSaveAction();
    void logRootDirLoadAction();
    void logRootDirResetAction();
    void stdoutFilenameSaveAction();
    void stdoutFilenameLoadAction();
    void stdoutFilenameResetAction();
    void stderrFilenameSaveAction();
    void stderrFilenameLoadAction();
    void stderrFilenameResetAction();
    void snapshotFormatSaveAction();
    void snapshotFormatLoadAction();
    void snapshotFormatResetAction();
    void snapshotQualitySaveAction();
    void snapshotQualityLoadAction();
    void snapshotQualityResetAction();
    void snapshotExtensionSaveAction();
    void snapshotExtensionLoadAction();
    void snapshotExtensionResetAction();

    void resetAllSettingsAction();
    void loadAllSettingsAction();

private:
    Ui::GeneralSettingsDialog *ui;
    void initializeSettingsDialog();
    QErrorMessage *qErrMsg;
};

#endif // GENERALSETTINGSDIALOG_H
