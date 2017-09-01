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
    void pcapSubdirSaveAction();
    void pcapSubdirUndoAction();
    void framesSubdirSaveAction();
    void framesSubdirUndoAction();
    void snapshotsSubdirSaveAction();
    void snapshotsSubdirUndoAction();
    void liveSubdirSaveAction();
    void liveSubdirUndoAction();
    void replaySubdirSaveAction();
    void replaySubdirUndoAction();
    void defaultHomeNetworkSaveAction();
    void defaultHomeNetworkUndoAction();
    void showHomeNetNotSetWarningSaveAction();
    void showHomeNetNotSetWarningUndoAction();
    void logRootDirSaveAction();
    void logRootDirUndoAction();
    void stdoutFilenameSaveAction();
    void stdoutFilenameUndoAction();
    void stderrFilenameSaveAction();
    void stderrFilenameUndoAction();
    void snapshotFormatSaveAction();
    void snapshotFormatUndoAction();
    void snapshotQualitySaveAction();
    void snapshotQualityUndoAction();
    void snapshotExtensionSaveAction();
    void snapshotExtensionUndoAction();


private:
    Ui::GeneralSettingsDialog *ui;
    void initializeSettingsDialog();
};

#endif // GENERALSETTINGSDIALOG_H
