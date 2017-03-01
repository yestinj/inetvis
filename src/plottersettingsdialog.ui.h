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


void PlotterSettingsDialog::init()
{
   //setup validators for input
   addressValidator = new QIntValidator(0, 255, this, "address range validator");
   slashNumValidator = new QIntValidator(0, 31, this, "slash notation number validator");
   portNumValidator = new QIntValidator(0, 65535, this, "port number validator");
   logPlotNumValidator = new QIntValidator(2, 100000, this, "port number validator");

   //apply validaters for home net range settings
   homeNetALineEdit->setValidator(addressValidator);
   homeNetBLineEdit->setValidator(addressValidator);
   homeNetCLineEdit->setValidator(addressValidator);
   homeNetDLineEdit->setValidator(addressValidator);
   homeNetSlashNumLineEdit->setValidator(slashNumValidator);

   //apply validators for ineternet range settings
   internetALineEdit->setValidator(addressValidator);
   internetBLineEdit->setValidator(addressValidator);
   internetCLineEdit->setValidator(addressValidator);
   internetDLineEdit->setValidator(addressValidator);
   internetSlashNumLineEdit->setValidator(slashNumValidator);

   //apply validators for port number settings
   startPortLineEdit->setValidator(portNumValidator);
   endPortLineEdit->setValidator(portNumValidator);

   //apply validator for log number setting
   logPlotLineEdit->setValidator(logPlotNumValidator);

   //colour scheme choices
   colourMappingSchemeComboBox->insertItem(STR_COL_MAP_DST_PORT,
         COL_MAP_DST_PORT);
   colourMappingSchemeComboBox->insertItem(STR_COL_LOG_MAP_DST_PORT,
         COL_LOG_MAP_DST_PORT);
   colourMappingSchemeComboBox->insertItem(STR_COL_MAP_SRC_PORT,
         COL_MAP_SRC_PORT);
   colourMappingSchemeComboBox->insertItem(STR_COL_MAP_SRC_ADR,
         COL_MAP_SRC_ADR);
   colourMappingSchemeComboBox->insertItem(STR_COL_MAP_PROTO,
         COL_MAP_PROTO);
   colourMappingSchemeComboBox->insertItem(STR_COL_MAP_PKT_SIZE,
         COL_MAP_PKT_SIZE);
   colourMappingSchemeComboBox->insertItem(STR_COL_LOG_MAP_PKT_SIZE,
         COL_LOG_MAP_PKT_SIZE);
      //Note these options should corrospond to those in the Plotter class

}


void PlotterSettingsDialog::destroy()
{
   delete addressValidator;
   delete slashNumValidator;
   delete portNumValidator;
   delete logPlotNumValidator;

}


void PlotterSettingsDialog::updateHomeNetworkAddress(int a, int b, int c,
      int d, int slashMask, QString range)
{
   //update slash dot line edit
   homeNetALineEdit->setText(QString::number(a));
   homeNetBLineEdit->setText(QString::number(b));
   homeNetCLineEdit->setText(QString::number(c));
   homeNetDLineEdit->setText(QString::number(d));
   homeNetSlashNumLineEdit->setText(QString::number(slashMask));

   //update range
   destinationRangeLineEdit->setText(range);

}


void PlotterSettingsDialog::updateInternetNetworkRange(int a, int b, int c,
      int d, int slashMask, QString range)
{
   //update slash dot line edit
   internetALineEdit->setText(QString::number(a));
   internetBLineEdit->setText(QString::number(b));
   internetCLineEdit->setText(QString::number(c));
   internetDLineEdit->setText(QString::number(d));
   internetSlashNumLineEdit->setText(QString::number(slashMask));

   //update range
   sourceRangeLineEdit->setText(range);

}


void PlotterSettingsDialog::applyHomeNetSettings()
{
   //get values
   int octA = homeNetALineEdit->text().toInt();
   int octB = homeNetBLineEdit->text().toInt();
   int octC = homeNetCLineEdit->text().toInt();
   int octD = homeNetDLineEdit->text().toInt();
   int slashMask = homeNetSlashNumLineEdit->text().toInt();

   emit setHomeNetwork(octA, octB, octC, octD, slashMask);

   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[PS] home network set to: " 
            + QString("%1.%2.%3.%4/%5").arg(octA).arg(octB).arg(octC).arg(octD).arg(slashMask));

}


void PlotterSettingsDialog::applyInternetRangeSettings()
{

   //get values
   int octA = internetALineEdit->text().toInt();
   int octB = internetBLineEdit->text().toInt();
   int octC = internetCLineEdit->text().toInt();
   int octD = internetDLineEdit->text().toInt();
   int slashMask = internetSlashNumLineEdit->text().toInt();

   emit setSourceNetwork(octA, octB, octC, octD, slashMask);

   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[PS] home network set to: " 
            + QString("%1.%2.%3.%4/%5").arg(octA).arg(octB).arg(octC).arg(octD).arg(slashMask));

}


void PlotterSettingsDialog::applyPortRange()
{

   //port range choice
   int startPort = startPortLineEdit->text().toInt();
   int endPort = endPortLineEdit->text().toInt();
   int logParam = logPlotLineEdit->text().toInt();

   //assert the start range is larger than the end range
   if (startPort > endPort)
   {   startPortLineEdit->setText(endPortLineEdit->text());

      //update UI Log
      if(LogUI::isEnabled())
         LogUI::logEvent("[PS] user attempted to set the start port higher than the end port of the range");
   }

   //asset the log plot range base number is larger than 2
   if (logParam < 2)
      logPlotLineEdit->setText("2");

   if (linearPlotRadioButton->isChecked())
   {  emit setPortRange(startPort, endPort, (int)Y_PLOT_LINEAR_DST_PORT,
            logParam);
            //Y_PLOT_LINEAR_DST_PORT defined in plotter.h
   
      //update UI Log
      if(LogUI::isEnabled())
         LogUI::logEvent("[PS] linear port range set to: " 
               + QString("%1 - %2").arg(startPort).arg(endPort));
   }
   else
   {
      emit setPortRange(startPort, endPort, (int)Y_PLOT_LOGARITHMIC_DST_PORT,
            logParam);
            //Y_PLOT_LOGARITHMIC_DST_PORT defined in plotter.h

      //update UI Log
      if(LogUI::isEnabled())
         LogUI::logEvent("[PS] log port range set to: " 
               + QString("%1 - %2").arg(startPort).arg(endPort) 
               + ", with log parameter: " + QString::number(logParam));
   }

}


void PlotterSettingsDialog::pointSizeChanged()
{
   emit changePointSize(pointSizeSpinBox->value());

   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[PS] point size set to: " 
            + QString::number(pointSizeSpinBox->value()));

}


void PlotterSettingsDialog::setColScheme(int scheme)
{
   //Note these options should corrospond to those in the Plotter class

   emit setColourScheme(scheme);

   //update UI Log
   if(LogUI::isEnabled())
   {  //string description
      QString colSchemeName = "";

      //use the text description of the colour scheme
      switch (scheme)
      {  case COL_MAP_DST_PORT:
            colSchemeName = STR_COL_MAP_DST_PORT;
            break;
         case COL_LOG_MAP_DST_PORT:
            colSchemeName = STR_COL_LOG_MAP_DST_PORT;
            break;
         case COL_MAP_SRC_PORT:
            colSchemeName = STR_COL_MAP_SRC_PORT;
            break;
         case COL_MAP_SRC_ADR:
            colSchemeName = STR_COL_MAP_SRC_ADR;
            break;
         case COL_MAP_PROTO:
            colSchemeName = STR_COL_MAP_PROTO;
            break;
         case COL_MAP_PKT_SIZE:
            colSchemeName = STR_COL_MAP_PKT_SIZE;
            break;
         case COL_LOG_MAP_PKT_SIZE:
            colSchemeName = STR_COL_LOG_MAP_PKT_SIZE;
            break;
      }
      
      LogUI::logEvent("[PS] colour scheme set to: \"" + colSchemeName + '\"');
   }

}


void PlotterSettingsDialog::updatePortRange(int start, int end)
{
   startPortLineEdit->setText(QString::number(start));
   endPortLineEdit->setText(QString::number(end));

}


void PlotterSettingsDialog::setFullSrcNetRange()
{
   internetALineEdit->setText(QString::number(0));
   internetBLineEdit->setText(QString::number(0));
   internetCLineEdit->setText(QString::number(0));
   internetDLineEdit->setText(QString::number(0));
   internetSlashNumLineEdit->setText(QString::number(0));

   //update UI Log
   if(LogUI::isEnabled())
   {  LogUI::logEvent("[PS] reset to full internet range");
   }

   applyInternetRangeSettings();

}


void PlotterSettingsDialog::guessHomeNetworkPressed()
{
   //update UI Log
   if(LogUI::isEnabled())
   {  LogUI::logEvent("[PS] guess home network pressed");
   }

   emit guessHomeNetwork();

}


void PlotterSettingsDialog::transDecaySelected()
{
    emit setTransparentDecay(transparentDecayCheckBox->isChecked());

   //update UI Log
   if(LogUI::isEnabled())
   {  if (transparentDecayCheckBox->isChecked())
         LogUI::logEvent("[PS] transparent decay enabled");
      else
         LogUI::logEvent("[PS] transparent decay disabled");
   }

}


void PlotterSettingsDialog::enableHomeNetRange(bool enable)
{
    if (DestinationGroupBox->isEnabled() == enable)
       return; //same state
    //TODO
}


void PlotterSettingsDialog::enablePortRange(bool enable)
{
    if (SourceButtonGroup->isEnabled() == enable)
       return; //same state
    //TODO
}


void PlotterSettingsDialog::smoothingEnabled(bool on)
{
    emit setPointSmoothing(on);

   //update UI Log
   if(LogUI::isEnabled())
   {  if (on)
         LogUI::logEvent("[PS] point smoothing enabled");
      else
         LogUI::logEvent("[PS] point smoothing disabled");
   }

}



void PlotterSettingsDialog::enablePointBulge(bool enable)
{
    setPointBulge(enable);

   //update UI Log
   if(LogUI::isEnabled())
   {  if (enable)
         LogUI::logEvent("[PS] point bulge enabled");
      else
         LogUI::logEvent("[PS] point bulge disabled");
   }

}


void PlotterSettingsDialog::backgroundColourChange()
{
  //define background colour codes
  #ifndef BLACK_BACKGROUND
     #define BLACK_BACKGROUND 0
  #endif
  #ifndef WHITE_BACKGROUND
     #define WHITE_BACKGROUND 1
  #endif

   if (whiteRadioButton->isOn())
   {  emit setBackgroundColour(WHITE_BACKGROUND);

      //update UI Log
      if(LogUI::isEnabled())
      {  LogUI::logEvent("[PS] background set to white");
      }

   }
   else //assume black button toggled
   {  emit setBackgroundColour(BLACK_BACKGROUND);

      //update UI Log
      if(LogUI::isEnabled())
      {  LogUI::logEvent("[PS] background set to black");
      }

   }

}


