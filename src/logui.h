#ifndef LOGUI_H
#define LOGUI_H
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
#include "log.h";

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
