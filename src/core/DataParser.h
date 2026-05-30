#pragma once

#include <QObject>
#include <QByteArray>
#include <QString>
#include "TelemetryDef.h"

Q_DECLARE_METATYPE(TelemetryPacket)

class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);

    // Feed raw text string bytes into the parser's internal buffer
    void parseData(const QByteArray &data);

signals:
    // Emitted when a valid CSV line is parsed
    void packetReceived(const TelemetryPacket &packet);

private:
    QString m_buffer;
    
    void processLine(const QString &line);
};
