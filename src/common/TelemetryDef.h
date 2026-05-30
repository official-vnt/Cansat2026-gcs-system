#pragma once
#include <QString>
#include <QVector3D>
#include <cstdint>

struct TelemetryPacket {
    QString teamID;
    int time;
    quint16 packetCount;
    float altitude;
    float pressure;
    float temperature;
    float voltage;
    double latitude;
    double longitude;
    float gpsAltitude;
    int satellites;
    QVector3D accel;
    QVector3D gyro;
    QString state;
};
