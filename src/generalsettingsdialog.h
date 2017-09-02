#ifndef GENERALSETTINGSDIALOG_H
#define GENERALSETTINGSDIALOG_H

#include <QDialog>
#include <QStandardPaths>

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
    void rootDirResetAction();
    void pcapSubdirSaveAction();
    void pcapSubdirUndoAction();
    void pcapSubdirResetAction();
    void framesSubdirSaveAction();
    void framesSubdirUndoAction();
    void framesSubdirResetAction();
    void snapshotsSubdirSaveAction();
    void snapshotsSubdirUndoAction();
    void snapshotsSubdirResetAction();
    void liveSubdirSaveAction();
    void liveSubdirUndoAction();
    void liveSubdirResetAction();
    void replaySubdirSaveAction();
    void replaySubdirUndoAction();
    void replaySubdirResetAction();
    void defaultHomeNetworkSaveAction();
    void defaultHomeNetworkUndoAction();
    void defaultHomeNetworkResetAction();
    void showHomeNetNotSetWarningSaveAction();
    void showHomeNetNotSetWarningUndoAction();
    void showHomeNetNotSetWarningResetAction();
    void logRootDirSaveAction();
    void logRootDirUndoAction();
    void logRootDirResetAction();
    void stdoutFilenameSaveAction();
    void stdoutFilenameUndoAction();
    void stdoutFilenameResetAction();
    void stderrFilenameSaveAction();
    void stderrFilenameUndoAction();
    void stderrFilenameResetAction();
    void snapshotFormatSaveAction();
    void snapshotFormatUndoAction();
    void snapshotFormatResetAction();
    void snapshotQualitySaveAction();
    void snapshotQualityUndoAction();
    void snapshotQualityResetAction();
    void snapshotExtensionSaveAction();
    void snapshotExtensionUndoAction();
    void snapshotExtensionResetAction();

    void resetAllSettingsAction();
    void loadAllSettingsAction();

private:
    Ui::GeneralSettingsDialog *ui;
    void initializeSettingsDialog();
};

#endif // GENERALSETTINGSDIALOG_H
