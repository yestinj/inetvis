#ifndef LOGUI_H
#define LOGUI_H

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

/* logui.h & logui.cpp
 *
 * A class that faciliates recording interface interaction to a text log file -
 * useful for performing usability studies
 *
 */

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "log.h"

class LogUI {

private:

    //members
    static bool enabled;
    static QString logFileName;
    static QFile logFile;  //object to manage log file - file handle
    static QTextStream logStream;  //stream to write into log file

    //functions
    static bool openFile();
    static void closeFile();

public:

    static bool enable(); //toggle loggin on
    static void disable();
    static bool isEnabled();
    static void logEvent(QString msg);  //funtion automatically prepends
    //timestamp
};


class LogUIQuit : public QObject {
    Q_OBJECT //macro for use of signals and slots

    //implemented to receive quit signal and close any open logging sessions.
    // This will be called by main.

public slots:
    void close();
};
#endif // LOGUI_H
