#include <log.h>
#include <iostream>

// Re-define static member variables here.
QString Log::stdoutFilename;
QString Log::stderrFilename;
QFile Log::stdoutFile;
QFile Log::stderrFile;
QTextStream Log::stdoutLogStream;
QTextStream Log::stderrLogStream;

bool Log::createDirectories() {
    QDir dir = QDir();

    if (!dir.exists(getLogRootDir())) {
        if (!dir.mkdir(getLogRootDir())) {
            return false;
        }
    }
}

bool Log::openStdoutFile() {
    QString startTime = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
    stdoutFilename = QString(getStdoutFilename() + "-" + startTime + ".log");
    stdoutFile.setFileName(getLogRootDir() + "/" + stdoutFilename);

    if (stdoutFile.open(QFile::WriteOnly)) {
        stdoutLogStream.setDevice(&stdoutFile);
        logOutput("Log file opened");
        return true;
    }
    else {
        return false;
    }
}

bool Log::openStderrFile() {
    QString startTime = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
    stderrFilename = QString(getStderrFilename() + "-" + startTime + ".log");
    stderrFile.setFileName(getLogRootDir() + "/" + stderrFilename);

    if (stderrFile.open(QFile::WriteOnly)) {
        stderrLogStream.setDevice(&stderrFile);
        logError("Log file opened");
        return true;
    }
    else {
        return false;
    }
}

bool Log::openFiles() {
    // TODO: Add handling of return values
    openStdoutFile();
    openStderrFile();
}

void Log::closeStdoutFile() {
    logOutput("log file closed");
    stdoutFile.close();
}

void Log::closeStderrFile() {
    logError("Log file closed");
    stderrFile.close();
}

void Log::closeFiles() {
    closeStdoutFile();
    closeStderrFile();
}

void Log::logOutput(QString msg) {
    QString ts = QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss.zzz");
    stdoutLogStream << ts << " := " << msg << '\n';
    stdoutLogStream.flush();
}

void Log::logError(QString msg) {
    QString ts = QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss.zzz");
    stderrLogStream << ts << " := " << msg << '\n';
    stderrLogStream.flush();
}

QString Log::getLogRootDir() {
    QSettings settings;
    return settings.value(LOG_ROOT_DIR_KEY).toString();
}

void Log::setLogRootDir(QString logRootDir) {
    QSettings settings;
    settings.setValue(LOG_ROOT_DIR_KEY, logRootDir);
}

bool Log::isLogRootDirSet() {
    QSettings settings;
    return settings.contains(LOG_ROOT_DIR_KEY);
}

QString Log::getStdoutFilename() {
    QSettings settings;
    return settings.value(STDOUT_FILENAME_KEY).toString();
}

void Log::setStdoutFilename(QString stdoutFilename) {
    QSettings settings;
    settings.setValue(STDOUT_FILENAME_KEY, stdoutFilename);
}

bool Log::isStdoutFilenameSet() {
    QSettings settings;
    return settings.contains(STDOUT_FILENAME_KEY);
}

QString Log::getStderrFilename() {
    QSettings settings;
    return settings.value(STDERR_FILENAME_KEY).toString();
}

void Log::setStderrFilename(QString stderrFilename) {
    QSettings settings;
    settings.setValue(STDERR_FILENAME_KEY, stderrFilename);
}

bool Log::isStderrFilenameSet() {
    QSettings settings;
    return settings.contains(STDERR_FILENAME_KEY);
}

bool Log::enable() {
    createDirectories();
    openFiles();
}

void Log::disable() {
    closeFiles();
}

void LogQuit::close() {
    //a slot meant to receive qt application call to quit - should be connected in main.
    Log::disable();
}
