#ifndef PLOTTERSETTINGSDIALOGWIDGET_H
#define PLOTTERSETTINGSDIALOGWIDGET_H

#include <QDialog>
#include "ui_plottersettingsdialog.h"
#include <QValidator>
#include "plotter.h"
#include "logui.h"
#include <QSettings>
#include "dataproc.h"
#include <QStringList>

class PlotterSettingsDialogWidget : public QDialog, public Ui::PlotterSettingsDialog {
    Q_OBJECT
public:
    PlotterSettingsDialogWidget(QWidget *parent = 0);
    ~PlotterSettingsDialogWidget();
    QString readHomeNetworkConfig();

signals:
    void setHomeNetwork(int, int, int, int, int);
    void changePointSize(int);
    void setSourceNetwork(int, int, int, int, int);
    void setColourScheme(int);
    void setPortRange(int,int,int,int);
    void guessHomeNetwork();
    void setTransparentDecay(bool);
    void setPointSmoothing(bool);
    void setPointBulge(bool);
    void setBackgroundColour(int colCode);

public slots:
    void updateHomeNetworkAddress(int, int, int, int, int, QString);
    void updateInternetNetworkRange(int, int, int, int, int, QString);
    void applyHomeNetSettings();
    void applyInternetRangeSettings();
    void applyPortRange();
    void pointSizeChanged();
    void setColScheme(int);
    void updatePortRange(int, int);
    void setFullSrcNetRange();
    void guessHomeNetworkPressed();
    void transDecaySelected();
    void enableHomeNetRange(bool);
    void enablePortRange(bool);
    void smoothingEnabled(bool);
    void enablePointBulge(bool);
    void backgroundColourChange();
    void loadDefaultHomeNetwork();

private:
    QIntValidator *addressValidator;
    QIntValidator *slashNumValidator;
    QIntValidator *portNumValidator;
    QIntValidator *logPlotNumValidator;
    void init();
    void destroy();   
};

#endif // PLOTTERSETTINGSDIALOGWIDGET_H
