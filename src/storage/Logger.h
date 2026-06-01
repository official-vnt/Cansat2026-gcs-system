#pragma once

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "TelemetryDef.h"

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    bool startLogging(const QString &dirPath);
    void stopLogging();
    QString currentLogFile() const { return m_logFile.fileName(); }

public slots:
    void logPacket(const TelemetryPacket &packet);

private:
    QFile m_logFile;
    QTextStream m_stream;
};
