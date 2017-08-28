#ifndef LOG_H
#define LOG_H
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>

class Log {
private:

    static QString stdoutFilename;
    static QString stderrFilename;
    static QFile stdoutFile;  //object to manage log file - file handle
    static QFile stderrFile;  //object to manage log file - file handle
    static QTextStream stdoutLogStream;  //stream to write into log file
    static QTextStream stderrLogStream;  //stream to write into log fil

    static bool openStdoutFile();
    static bool openStderrFile();
    static bool openFiles();
    static void closeStdoutFile();
    static void closeStderrFile();
    static void closeFiles();

public:

    static bool enable();
    static void disable();
    static bool createDirectories();
    static void logOutput(QString);
    static void logError(QString);
    static QString getLogRootDir();
    static QString getStdoutFilename();
    static QString getStderrFilename();    
};

class LogQuit : public QObject {
    Q_OBJECT //macro for use of signals and slots
    //implemented to receive quit signal and close any open logging sessions.
    // This will be called by main.

public slots:
    void close();
};
#endif // LOG_H
