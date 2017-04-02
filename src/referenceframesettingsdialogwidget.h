#ifndef REFERENCEFRAMESETTINGSDIALOGWIDGET_H
#define REFERENCEFRAMESETTINGSDIALOGWIDGET_H

#include <QDialog>
#include "ui_referenceframesettingsdialog.h"
#include "logui.h"

class ReferenceFrameSettingsDialogWidget : public QDialog, public Ui::ReferenceFrameSettingsDialog {
    Q_OBJECT
public:
    ReferenceFrameSettingsDialogWidget(QWidget *parent = 0);

signals:
    void setGridLines(bool, bool, bool, bool, bool, bool, bool, float);
    void setReferenceFrame(bool, bool, bool, bool, bool);
    void setPartitions(int, int, int);
    void setPerspectiveProjection();
    void setOrthographicProjection();
    void displayAxisLabels(bool);
    void displayDateTime(bool);
    void displayFramerate(bool);

public slots:
    void applyReferenceFrame();
    void applyGrid();
    void applyPartitions();
    void axisLabelsStateChange(bool);
    void dateTimeLabelStateChange(bool);
    void framerateLabelStateChange(bool);
    void projectionModeChanged();

private:

};
#endif // REFERENCEFRAMESETTINGSDIALOGWIDGET_H
