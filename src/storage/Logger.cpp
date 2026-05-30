#include "Logger.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{
}

Logger::~Logger()
{
    stopLogging();
}

bool Logger::startLogging(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString filename = QString("telemetry_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    m_logFile.setFileName(dir.absoluteFilePath(filename));

    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qWarning() << "Failed to open log file" << m_logFile.fileName();
        return false;
    }

    m_stream.setDevice(&m_logFile);
    // Write CSV Header
    m_stream << "TeamID,Time,PacketCount,Altitude,Pressure,Temperature,Voltage,Latitude,Longitude,GPS_Alt,Satellites,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,State\n";
    m_stream.flush();

    return true;
}

void Logger::stopLogging()
{
    if (m_logFile.isOpen()) {
        m_stream.flush();
        m_logFile.close();
    }
}

void Logger::logPacket(const TelemetryPacket &packet)
{
    if (!m_logFile.isOpen()) return;

    m_stream << packet.teamID << ","
             << packet.time << ","
             << packet.packetCount << ","
             << packet.altitude << ","
             << packet.pressure << ","
             << packet.temperature << ","
             << packet.voltage << ","
             << packet.latitude << ","
             << packet.longitude << ","
             << packet.gpsAltitude << ","
             << packet.satellites << ","
             << packet.accel.x() << ","
             << packet.accel.y() << ","
             << packet.accel.z() << ","
             << packet.gyro.x() << ","
             << packet.gyro.y() << ","
             << packet.gyro.z() << ","
             << packet.state << "\n";
             
    // Flush dynamically to prevent data loss on crashes
    m_stream.flush();
}
