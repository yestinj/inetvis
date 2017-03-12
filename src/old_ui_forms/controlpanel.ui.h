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
#include <iostream> //for testing and debug purposes
#include <math.h>
#include "timeutil.h"
//Added by qt3to4:
#include <q3mimefactory.h>
//Added by qt3to4:
#include <q3mimefactory.h>


// DEGBUG DIRECTIVES -----------------------------------------------------------
//* //un-commnet to compile with debug info
//#define DEBUG_REPLAY_POSITION
//*/


void ControlPanel::init()
{
    //init widget flags
    //setWFlags(Qt::WType_TopLevel);
    //setWFlags(Qt::WX11BypassWM);
    //setWFlags(Qt::WStyle_StaysOnTop);
    //setWFlags(Qt::WStyle_ContextHelp);

   //init replay conditions
   replayPosSetByUser = true;
   replayPosSetBySlider = true;
   replayPositionSliderChangeStarted = true;
   timeWinSetByUser = true;
   timeWindowSetBySlider = true;
   captureFramesSetByUser = true;
   recordToPcapFileSetByUser = true;

   //init icons for custom playPauseToolButton control
   playIcon = new QIcon(qPixmapFromMimeSource("stock_media-play.png"));
   pauseIcon = new QIcon(qPixmapFromMimeSource("stock_media-pause.png"));

   //setup validators for input
   timeScaleValidator = new QDoubleValidator(this);
   timeScaleLineEdit->setValidator(timeScaleValidator);

   //setup toggle actions (for menu)
   recordCapture_Frames_to_FileAction->setToggleAction(true);

   //init GUI controls - disable some options
   replayPositionGroupBox->setEnabled(false);
   recordToolButton->setEnabled(false);

   //init about dialog
   aboutDialog = new AboutDialog((QWidget*)this);
   helpDialog = new HelpDocumentationDialog((QWidget*)this);
   //aboutDialog->setShown(false);
   //helpDialog->setShown(false);

   //init error message dialog
   qErrMsg = new QErrorMessage((QWidget*)this);
   qErrMsg->resize(QSize(400,240)); //set custom size to report error msg

}


void ControlPanel::destroy()
{
    //delete dialogs
    delete qErrMsg;
    delete helpDialog;
    delete aboutDialog;

   //delete icons used for playPauseToolButton
   delete playIcon;
   delete pauseIcon;
   delete timeScaleValidator;

}


void ControlPanel::timeSliderScaleRealeased()
{
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] time scale slider used");

   //set the value in the textEditLine, which will do the rest and emit a
   //signal to the data processor...
   timeScaleLineEditChanged();

}


void ControlPanel::fileOpen()
{

   QString fileName = Q3FileDialog::getOpenFileName("",
         "Capture Files (*.cap *.dump)", this, "Open Replay File",
         "Select a replay file for playback");
   if (!fileName.isNull())
   {  //file selected
      replayCaptureFileAction->setOn(true);
      //notfiy data processor
      emit selectReplayFile(fileName);
      //update UI Log
      if(LogUI::isEnabled())
         LogUI::logEvent("[CP] capture file sucessfully opened: \"" + fileName 
               + "\"");
      //enable editing the replay position
      replayPositionGroupBox->setEnabled(true);
      ReplaySpeedGroupBox->setEnabled(true);
      recordToolButton->setEnabled(true);
   }
   else  
   {  //canceled

      //update UI Log
      if(LogUI::isEnabled())
         LogUI::logEvent("[CP] capture file failed to open: \"" + fileName + "\"");

      replayCaptureFileAction->setOn(false);
      //disable editing the replay position
      replayPositionGroupBox->setEnabled(false);
      ReplaySpeedGroupBox->setEnabled(false);
      

   }

}


void ControlPanel::playPauseToolButtonClicked()
{
   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] play/pause toggled at replay time: " + strGetRepPos());

   emit togglePlayPause();

}


void ControlPanel::pauseMenuAction()
{
   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] menu 'Replay->Pause' selected at timestamp: " 
            + strGetRepPos());

   emit pause();

}


void ControlPanel::playMenuAction()
{
   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] menu 'Replay->Play' selected at timestamp: "
            + strGetRepPos());

   emit play();

}


void ControlPanel::replayFileModeSelected()
{
   this->fileOpen();

}


void ControlPanel::monitorLocalHostSelected()
{
   //disable editing the replay position and time scale
   replayPositionGroupBox->setEnabled(false);
   ReplaySpeedGroupBox->setEnabled(false);
   recordToolButton->setEnabled(true);
   //TODO: In future may create temp file so that seeking back is possible
   //while monitoring live

   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] menu 'Mode->Monitor Local Host' selected");

   //notify the data processor of mode change
   emit selectNetworkInterface(""); //will open default interface


}


void ControlPanel::viewVisDisplayPanel()
{
   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] 'Display Window' opened");

   emit showVisDisplayPanel();

}


void ControlPanel::playing()
{
   //update button icon to show next appropriate action
   playPauseToolButton->setIconSet(*pauseIcon);

}


void ControlPanel::paused()
{
   //update button icon to show next appropriate action
   playPauseToolButton->setIconSet(*playIcon);

}


void ControlPanel::timeScaleLineEditChanged()
{
   //update the time scale slider bar
   bool convertSuccess;
   double newRate = timeScaleLineEdit->text().toDouble(&convertSuccess);
   if (convertSuccess)
   {  emit setReplayRate(newRate);

      //update UI log
      if(LogUI::isEnabled())
         LogUI::logEvent(QString("[CP] time scale set to new rate: %1").arg(newRate));
   }
   else //reapply the last valid value
   {  timeScaleLineEdit->setText(QString::number(timeScale));

      //update UI log
      if(LogUI::isEnabled())
         LogUI::logEvent(QString("[CP] time scale input not successfuly converted and reset to rate: %1").arg(newRate));
   }

}


void ControlPanel::statusBarMessage(QString message)
{
   statusBar()->message(message);

}


//time scale (replay speed) GUI fucntions


void ControlPanel::updateReplayRate(double value)
{
   //keep a copy of the time scale string in case a attempt to change it from
   //another method fails
   timeScale = value;
   timeScaleLineEdit->setText(QString::number(timeScale));

   //custom updating of slider position - the inverse of the operations
   //performed in timeScaleSliderChanged
   if (timeScale < 0.01) //millsec adjustement
   {  timeScaleSlider->setValue((int)(timeScale * 1000));
   }
   else if (timeScale < 0.1)  //10s of millsec adjustement
   {  timeScaleSlider->setValue((int)(timeScale  * 100 + 10));
   }
   else if (timeScale < 1.0) //100s of millsec adjustement
   {  timeScaleSlider->setValue((int)(timeScale  * 10 + 20));
   }
   else if (timeScale < 10) //1 to 10x adjustement
   {  timeScaleSlider->setValue((int)(timeScale + 30));
   }
   else if (timeScale < 3600) //multiples of minutes adjustment
   {  timeScaleSlider->setValue((int)(timeScale / 60 + 40));
   }
   else if (timeScale <= 86000) //multiples of hours adjustment
   {  timeScaleSlider->setValue((int)(timeScale / 3600 + 100));
   }

}


void ControlPanel::timeScaleSliderChanged(int value)
{
   //custom increasing adjustment by slider - begins with fractions of a
   //second, then multiples of time, then minutes, the hours, up to a rate
   //of a day in a second
   if (value <= 10) //millsec adjustement
   {  timeScale = value * 0.001;
      timeScaleLineEdit->setText(QString::number(timeScale));
   }
   else if (value <= 20)  //10s of millsec adjustement
   {  timeScale = (value - 10) * 0.01;
      timeScaleLineEdit->setText(QString::number(timeScale));
   }
   else if (value <= 30) //100s of millsec adjustement
   {  timeScale = (value - 20) * 0.1;
      timeScaleLineEdit->setText(QString::number(timeScale));
   }
   else if (value <= 40) //1 to 10x adjustement
   {  timeScale = (value - 30);
      timeScaleLineEdit->setText(QString::number(timeScale));
   }
   else if (value <= 100) //multiples of minutes adjustment
   {  timeScale = (value - 40) * 60;
      timeScaleLineEdit->setText(QString::number(timeScale));
   }
   else if (value <= 124) //multiples of hours adjustment
   {  timeScale = (value - 100) * 3600;
      timeScaleLineEdit->setText(QString::number(timeScale));
   }

}


void ControlPanel::setNormalReplaySpeed()
{
   //is this function used? Or a relic from a removed menu option?

   emit setReplayRate(1.0);

}

//replay position GUI functions


void ControlPanel::updateReplayTimeDisplay(const QDateTime &repPos,
      const QDateTime &timeWinEnd)
{
   replayPosSetByUser = false; //to indicate that it was an update, not user
         //input
   replayPosSetBySlider = false; //to avoid race conditions between updating
         //complimentary controls
   replayDateTimeEdit->setDateTime(repPos);
   millisecondsSpinBox->setValue(repPos.time().msec());
   //perhaps update slider postition is updated on a timed basis
   updateReplayTimeSliderPosition(repPos);
   replayPosSetByUser = true;
   replayPosSetBySlider = true;

}


void ControlPanel::updateReplayTimeSliderPosition(const QDateTime &repPos)
{
   timeval tv_ct; //current time
   double d_ct; //current time
   double f; //fraction along the time range (0.0 = at start, 1.0 = at end)

   //calculate the fraction that the current time is between the start time
   //and end time
   replayPosSetByUser = false;
   replayPosSetBySlider = false;
   TimeUtil::qDateTimeToTimeval(tv_ct, repPos);
   d_ct = TimeUtil::timevalToDouble(tv_ct);
   f = (d_ct - startTime) / timeSpan;
   replayPositionSlider->setValue((int)(f * 199.0));  //where max slider
         //position is 999
   replayPosSetByUser = true;
   replayPosSetBySlider = true;

}


void ControlPanel::replayPositionDateTimeChanged()
{
   if (replayPosSetByUser)
   {  if (!replayPosSetBySlider) //slider function would have already paused
         emit pause(); //pause since we want to avoid overprocessing;
      QDateTime repPos = replayDateTimeEdit->dateTime();
      repPos.time().addMSecs(millisecondsSpinBox->text().toInt());  //msec is
            //assumed to be 0
      if (!replayPosSetBySlider) //to avoid a race condition between the
         //controls updating each other
         updateReplayTimeSliderPosition(repPos);
      //wait for apply button before affecting change

   } //else update was not by user

}


void ControlPanel::replayPositionSliderChanged(int val)
{
   //update  the time display that the current slide bar represents as a
   //time between the start and end ranges

   //new time variables
   timeval tv_nt;
   QDateTime qdt_nt;

   //variable to note begining of slider adjustment

   if (replayPosSetByUser)
   {
      //avoid always trying to pause when already paused
      if (replayPositionSliderChangeStarted)
      {  emit pause();
         replayPositionSliderChangeStarted = false;
      }

      //calculate the time value from fraction along the slider
      if (val > 0)
         TimeUtil::doubleToTimeval(tv_nt, (val / 199.0) * timeSpan + startTime);
      else //back to the begining
         TimeUtil::doubleToTimeval(tv_nt, startTime);

      //update the time values
      TimeUtil::timevalToQDateTime(qdt_nt, tv_nt);
      replayDateTimeEdit->setDateTime(qdt_nt);
      millisecondsSpinBox->setValue(qdt_nt.time().msec());

      #ifdef DEBUG_REPLAY_POSITION
         cout << "DEBUG: Replay positon:\n" << "   val=" << val << ", frac="
               << (val / 199.0) << "\n";
      #endif
   }

}


void ControlPanel::replayPoistionSliderReleased()
{
   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] replay postion silder used");

   applyReplayPosition(); //uncomment to effect change as the slider is
      //released, else the check must be selected
   replayPositionSliderChangeStarted = true; //set ready for next time
      //slider is adjusted

}


void ControlPanel::applyReplayPosition()
{
   QDateTime repPos = replayDateTimeEdit->dateTime();
   //msec is assumed to be 0
   repPos.setTime(repPos.time().addMSecs(millisecondsSpinBox->text().toInt()));

   //update UI Log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] replay postion change attempt set at: " 
            + strGetRepPos());

   emit setReplayPosition(repPos);

   //update UI Log - check what the time was set to
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] replay position effected to: " + strGetRepPos());

}


void ControlPanel::setTimeRange(const QDateTime start, const QDateTime end )
{
   timeval tv_s, tv_e; //tempory values for start and  end
   double d_e; //end time
   TimeUtil::qDateTimeToTimeval(tv_s, start);
   TimeUtil::qDateTimeToTimeval(tv_e, end);
   d_e = TimeUtil::timevalToDouble(tv_e);
   startTime = TimeUtil::timevalToDouble(tv_s);
   timeSpan = d_e - startTime;

}


//time window functions

void ControlPanel::updateTimeWindowDisplay(double seconds)
{
   timeWinSetByUser = false;  //to indicate that it was an update, not user input

   timeWindowDaySpinBox->setValue((int)(seconds / 86400.0));
   QTime time;
   TimeUtil::doubleToQTime(time, seconds);
   timeWindowTimeEdit->setTime(time);
   timeWindowMillisecSpinBox->setValue(time.msec());
   //update time slider postition
   if (!timeWindowSetBySlider)
      updateTimeWindowSlider(seconds);

   timeWinSetByUser = true;

}


void ControlPanel::timeWindowChange()
{
   double secs;

   if (timeWinSetByUser)
   {  emit pause(); //allow time to re-read file
      secs = 0.0;
      secs += timeWindowDaySpinBox->value() * 86400.0; //days
      secs += TimeUtil::qTimeToDouble(timeWindowTimeEdit->time());
      secs += timeWindowMillisecSpinBox->value() / 1000.0; //millisec
      updateTimeWindowSlider(secs);
   }

}


void ControlPanel::updateTimeWindowSlider(double sec)
{
   timeWindowSetBySlider = false;

   if (sec <= 1.0) //less than 1 sec
   {  timeWindowSlider->setValue((int)(sec * 10.0)); //100s of millsec incriments
   }
   else if (sec <= 60.0)  //less than 1 min
   {  timeWindowSlider->setValue(10 + (int)(sec)); //second incriments
   }
   else if (sec <= 3600.0) //less than 1 hour
   {  timeWindowSlider->setValue(70 + (int)(sec / 60.0)); //minute incriments
   }
   else if (sec <= 86400.0) //less than 1 day
   {  timeWindowSlider->setValue(130 + (int)(sec / 3600.0)); //hour incriments
   }
   else if (sec <= 604800.0) //less than 1 week
   {  timeWindowSlider->setValue(154 + (int)(sec / 86400.0)); //day incriments
   }
   else if (sec <= 2419200.0) //less than 4 weeks
   {  timeWindowSlider->setValue(161 + (int)(sec / 604800.0));  //week incriments
   }
   else if (sec <= 31449600.0) //less than 53 weeks
   {  timeWindowSlider->setValue(165 + (int)(sec / 2419200.0));  //4 weeks incriments
   }
   else if (sec <= 157248000.0) //less than 5 years
   {  timeWindowSlider->setValue(178 + (int)(sec / 31449600.0));  //52 week incriments
   }

   timeWindowSetBySlider = true;

}


void ControlPanel::timeWindowSliderChanged(int val)
{

   double newTimeWindow;

   if (timeWindowSetBySlider)
   {   //custom increasing adjustment by slider - begins with 100ths a second,
      //then multiples of seconds,  minutes, hours, days, weeks, years.
      emit pause();
      if (val <= 10)  //100s of millsec adjustement
      {   newTimeWindow = (val) * 0.1;
          updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 70)  //second adjustment
      {   newTimeWindow = (val - 10);
          updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 130) //minute adjustment
      {  newTimeWindow = (val - 70) * 60.0; //1 minute
         updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 154) //hour adjustment
      {    newTimeWindow = (val - 130) * 3600.0; //1 hour
           updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 161) //dayly adjustment
      {  newTimeWindow = (val - 154) * 86400.0; //1 day
         updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 165) //weekly adjustment
      {  newTimeWindow = (val - 161) * 604800.0; //1 week
         updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 177) //30 day adjustments
      {  newTimeWindow = (val - 165) * 2592000.0; // 30 days
         updateTimeWindowDisplay(newTimeWindow);
      }
      else if (val <= 183) //yearly adjustment
      {  newTimeWindow = (val - 177) * 31536000.0; // 365 days
         updateTimeWindowDisplay(newTimeWindow);
      }
   }

}


void ControlPanel::applyTimeWindow()
{
   double newTimeWindow; //in seconds

   newTimeWindow = 0.0;
   newTimeWindow += timeWindowDaySpinBox->value() * 86400.0; //days
   newTimeWindow += TimeUtil::qTimeToDouble(timeWindowTimeEdit->time());
   newTimeWindow += timeWindowMillisecSpinBox->value() / 1000.0; //millisec

   //update UI log
   if(LogUI::isEnabled())
   {  int d = timeWindowDaySpinBox->value();
      int h = timeWindowTimeEdit->time().hour();
      int m = timeWindowTimeEdit->time().minute();
      int s = timeWindowTimeEdit->time().second();
      int ms = timeWindowMillisecSpinBox->value();
      LogUI::logEvent(QString("[CP] time window set to: %1d, %2h, %3m, %4s, %5ms").arg(d).arg(h).arg(m).arg(s).arg(ms));
   }

   emit setTimeWindow(newTimeWindow);

}


void ControlPanel::timeWindowSliderReleased()
{

   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] time window slider used");

   applyTimeWindow();

}


//other functions

void ControlPanel::applyFilter()
{
   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] filter expression attempted: \"" + filterLineEdit->text() 
            + "\"");

   emit setFilter(filterLineEdit->text());

}


void ControlPanel::viewPlotterSettings()
{
   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] 'Plotter Settings' opened");

   emit showPlotterSettings();

}


void ControlPanel::viewReferenceFrameSettings()
{

   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] 'Reference Frame Settings' opened");

   emit showReferenceFrameSettings();

}


void ControlPanel::captureFramesToFile(bool activate)
{
   if (captureFramesSetByUser)
   {  
      emit captureFrames(activate);
      //update gui and sync menu with button
      if (activate)
      {  captureFramesSetByUser = false;
         recordCapture_Frames_to_FileAction->setOn(true);
         videoClipToolButton->setOn(true);

         //update UI log
         if(LogUI::isEnabled())
            LogUI::logEvent("[CP] frame capture started at replay time: " 
                + strGetRepPos());

         captureFramesSetByUser = true;
      }
      else
      {  captureFramesSetByUser = false;
         recordCapture_Frames_to_FileAction->setOn(false);
         videoClipToolButton->setOn(false);

         //update UI log
         if(LogUI::isEnabled())
            LogUI::logEvent("[CP] frame capture stoped at replay time: " 
                + strGetRepPos());

         captureFramesSetByUser = true;
      }
   }

}


void ControlPanel::takeSnapshot()
{
   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] image snapshot taken at replay time: " 
            + strGetRepPos());

    emit captureSingleFrame();

}


void ControlPanel::record(bool buttonOn)
{
   if (recordToPcapFileSetByUser)
   {  emit recordToFile(buttonOn);
      //update gui and sync menu with button
      //recordToPcapFileSetByUser is a hack to avoid retriggering the
      //record slot
      if (buttonOn)
      {  recordToPcapFileSetByUser = false;
         recordDump_to_Pcap_FileAction->setOn(true);
         recordToolButton->setOn(true);
         recordToPcapFileSetByUser = true;

         //update UI log
         if(LogUI::isEnabled())
            LogUI::logEvent("[CP] packet recording started at replay time: " 
                + strGetRepPos());
      }
      else
      {  recordToPcapFileSetByUser = false;
         recordDump_to_Pcap_FileAction->setOn(false);
         recordToolButton->setOn(false);
         recordToPcapFileSetByUser = true;

         //update UI log
         if(LogUI::isEnabled())
            LogUI::logEvent("[CP] packet recording stoped at replay time: " 
                + strGetRepPos());
      }
   }

}


void ControlPanel::setRecordButton(bool buttonOn)
{
   recordToPcapFileSetByUser = false;
   recordToolButton->setOn(buttonOn);
   recordDump_to_Pcap_FileAction->setOn(true);
   recordToPcapFileSetByUser = true;

}


void ControlPanel::reportErrorMessage(QString errMsg)
{
    qErrMsg->message(errMsg);

   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[E] error message: {" + errMsg + "}");

}


void ControlPanel::showOnTop()
{
    show();
    showNormal();
    raise();
    setActiveWindow();

}


void ControlPanel::helpDoc()
{
    helpDialog->show();

   //update UI log
   if(LogUI::isEnabled())
      LogUI::logEvent("[CP] menu 'Help->Documentation' selected");

}


void ControlPanel::helpAbout()
{
       aboutDialog->show();

}


QString ControlPanel::strGetRepPos()
{
   return replayDateTimeEdit->dateTime().toString("yyyy/MM/dd-hh:mm:ss") 
         + QString(".%1").arg((millisecondsSpinBox->value()));

}
