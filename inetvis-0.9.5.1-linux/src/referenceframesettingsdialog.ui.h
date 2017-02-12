/*******************************************************************************

InetVis - Internet Visualisation

version: 0.9.5
release date: 2007/11/21

Computer Science Honours Project - 2005
Computer Science Masters Project - 2006/7

author: Jean-Pierre van Riel
supervisor: Barry Irwin

License
-------

InetVis - Internet Visaulisation for network traffic.
Copyright (C) 2006, Jean-Pierre van Riel

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA.

*******************************************************************************/


/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <iostream>

void ReferenceFrameSettingsDialog::applyReferenceFrame()
{
   emit setReferenceFrame(primaryAxesCheckBox->isChecked(),
                                        boundingAxesCheckBox->isChecked(),
                                        cubeCheckBox->isChecked(),
                                        planeCheckBox->isChecked(),
                                        markersCheckBox->isChecked());
}


void ReferenceFrameSettingsDialog::applyGrid()
{
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


void ReferenceFrameSettingsDialog::applyPartitions()
{
   emit setPartitions(xAxisSpinBox->value(), yAxisSpinBox->value(), zAxisSpinBox->value());

}




void ReferenceFrameSettingsDialog::axisLabelsStateChange(bool axesOn)
{
   emit displayAxisLabels(axesOn);

}


void ReferenceFrameSettingsDialog::dateTimeLabelStateChange(bool timeOn)
{
   emit displayDateTime(timeOn);

}


void ReferenceFrameSettingsDialog::framerateLabelStateChange(bool fpsOn)
{
   emit displayFramerate(fpsOn);

}


void ReferenceFrameSettingsDialog::projectionModeChanged()
{
   if (perspectiveRadioButton->isChecked())
      emit setPerspectiveProjection();
   else
      emit setOrthographicProjection();
}
