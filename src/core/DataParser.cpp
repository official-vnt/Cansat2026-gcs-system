#include "DataParser.h"
#include <QStringList>
#include <QMetaType>
#include <QDebug>

DataParser::DataParser(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<TelemetryPacket>("TelemetryPacket");
}

void DataParser::parseData(const QByteArray &data)
{
    m_buffer += QString::fromLatin1(data);

    // Keep extracting lines as long as there is a newline
    int newlineIdx;
    while ((newlineIdx = m_buffer.indexOf('\n')) != -1) {
        QString line = m_buffer.left(newlineIdx).trimmed(); // Extract and remove \r
        m_buffer.remove(0, newlineIdx + 1);

        if (!line.isEmpty()) {
            processLine(line);
        }
    }
}

void DataParser::processLine(const QString &line)
{
    // Expected Format: teamID,time,packetCount,altitude,pressure,temperature,voltage,lat,lon,gpsAlt,sats,ax,ay,az,gx,gy,gz,state
    QStringList parts = line.split(',');
    
    // We expect 18 parts
    if (parts.size() < 18) {
        emit messageReceived(line);
        return;
    }

    TelemetryPacket packet;
    bool ok = true;
    
    packet.teamID = parts[0];
    packet.time = parts[1].toInt(&ok);
    packet.packetCount = parts[2].toUShort(&ok);
    packet.altitude = parts[3].toFloat(&ok);
    packet.pressure = parts[4].toFloat(&ok);
    packet.temperature = parts[5].toFloat(&ok);
    packet.voltage = parts[6].toFloat(&ok);
    packet.latitude = parts[7].toDouble(&ok);
    packet.longitude = parts[8].toDouble(&ok);
    packet.gpsAltitude = parts[9].toFloat(&ok);
    packet.satellites = parts[10].toInt(&ok);
    
    packet.accel.setX(parts[11].toFloat(&ok));
    packet.accel.setY(parts[12].toFloat(&ok));
    packet.accel.setZ(parts[13].toFloat(&ok));
    
    packet.gyro.setX(parts[14].toFloat(&ok));
    packet.gyro.setY(parts[15].toFloat(&ok));
    packet.gyro.setZ(parts[16].toFloat(&ok));
    
    packet.state = parts[17];

    if (ok) {
        emit packetReceived(packet);
    } else {
        qWarning() << "Failed to parse data fields:" << line;
    }
}
