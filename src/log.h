#ifndef LOG_H
#define LOG_H
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>

// Settings keys
#define LOG_ROOT_DIR_KEY "logging/root_dir"
#define STDOUT_FILENAME_KEY "logging/stdout_filename"
#define STDERR_FILENAME_KEY "logging/stderr_filename"

// Settings default values
#define LOG_ROOT_DIR_DEFAULT "logs"
#define STDOUT_FILENAME_DEFAULT "stdout"
#define STDERR_FILENAME_DEFAULT "stderr"

class Log {
private:

    static QString stdoutFilename;
    static QString stderrFilename;
    static QFile stdoutFile;  //object to manage log file - file handle
    static QFile stderrFile;  //object to manage log file - file handle
    static QTextStream stdoutLogStream;  //stream to write into log file
    static QTextStream stderrLogStream;  //stream to write into log fil    
    static QSettings settings;

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
    static void setLogRootDir(QString logRootDir);
    static bool isLogRootDirSet();

    static QString getStdoutFilename();
    static void setStdoutFilename(QString stdoutFilename);
    static bool isStdoutFilenameSet();

    static QString getStderrFilename();    
    static void setStderrFilename(QString stderrFilename);
    static bool isStderrFilenameSet();
};

class LogQuit : public QObject {
    Q_OBJECT //macro for use of signals and slots
    //implemented to receive quit signal and close any open logging sessions.
    // This will be called by main.

public slots:
    void close();
};
#endif // LOG_H
