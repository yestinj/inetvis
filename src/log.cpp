#include <log.h>
#include <iostream>

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
    QSettings s;
    return s.value("logging/root_dir").toString();
}

QString Log::getStdoutFilename() {
    QSettings s;
    return s.value("logging/stdout_filename").toString();
}

QString Log::getStderrFilename() {
    QSettings s;
    return s.value("logging/stderr_filename").toString();
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
