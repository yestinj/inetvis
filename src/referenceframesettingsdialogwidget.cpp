/*******************************************************************
InetVis - Internet Visualisation
Version: 2.1.0
release date: 2017/09/21

Original Authors: Jean-Pierre van Riel, Barry Irwin
Initvis 2.x Authors: Yestin Johnson, Barry Irwin
Rhodes University
Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7
Computer Science Masters Project - 2017
author: Jean-Pierre van Riel
supervisor: Barry Irwin

InetVis - Internet Visualisation for network traffic.
Copyright (C) 2006-2017, Jean-Pierre van Riel, Barry Irwin, Yestin Johnson

*******************************************************************/

#include "referenceframesettingsdialogwidget.h"
#include <iostream>

ReferenceFrameSettingsDialogWidget::ReferenceFrameSettingsDialogWidget(QWidget *parent) :
    QDialog(parent) {
    setupUi(this);
}

void ReferenceFrameSettingsDialogWidget::applyReferenceFrame() {
    emit setReferenceFrame(primaryAxesCheckBox->isChecked(),
                           boundingAxesCheckBox->isChecked(),
                           cubeCheckBox->isChecked(),
                           planeCheckBox->isChecked(),
                           markersCheckBox->isChecked());
}


void ReferenceFrameSettingsDialogWidget::applyGrid() {
    emit setGridLines(frontCheckBox->isChecked(),
                      backCheckBox->isChecked(),
                      leftCheckBox->isChecked(),
                      rightCheckBox->isChecked(),
                      topCheckBox->isChecked(),
                      bottomCheckBox->isChecked(),
                      planeGridCheckBox->isChecked(),
                      (opacitySpinBox->value() / 100.0));
    //the tranparency value is accepted as a fractiuon, and is in fact
    //the opacity value i.e. 100 would be fully opague, 0 fully transparent
}


void ReferenceFrameSettingsDialogWidget::applyPartitions() {
    emit setPartitions(xAxisSpinBox->value(), yAxisSpinBox->value(), zAxisSpinBox->value());
}


void ReferenceFrameSettingsDialogWidget::axisLabelsStateChange(bool axesOn) {
    emit displayAxisLabels(axesOn);
}


void ReferenceFrameSettingsDialogWidget::dateTimeLabelStateChange(bool timeOn) {
    emit displayDateTime(timeOn);
}


void ReferenceFrameSettingsDialogWidget::framerateLabelStateChange(bool fpsOn) {
    emit displayFramerate(fpsOn);
}


void ReferenceFrameSettingsDialogWidget::projectionModeChanged() {
    if (perspectiveRadioButton->isChecked())
        emit setPerspectiveProjection();
    else
        emit setOrthographicProjection();
}
