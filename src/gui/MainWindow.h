#pragma once

#include <QMainWindow>
#include <QThread>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QQuickWidget>

#include <QLabel>
#include "core/SerialWorker.h"
#include "core/DataParser.h"
#include "storage/Logger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectClicked();
    void onDisconnectClicked();
    void onPortOpened();
    void onPortClosed();
    void onPacketReceived(const TelemetryPacket &packet);
    void onErrorOccurred(const QString &error);

private:
    void setupUiGraphs();
    void setupConnections();
    void setupDashboardCards();
    void setupLeftPanel();
    void updateCharts(const TelemetryPacket &packet);
    void styleChart(QChart *chart, const QString &title);

    Ui::MainWindow *ui;

    // Threading & backend
    QThread m_serialThread;
    SerialWorker *m_worker;
    DataParser *m_parser;
    Logger *m_logger;

    // ── RIGHT PANEL: Telemetry readout labels ────────────────────────────────
    QLabel *m_lblTeamId;
    QLabel *m_lblVoltage;
    QLabel *m_lblAltitudeVal;
    QLabel *m_lblTemperatureVal;
    QLabel *m_lblPressureVal;
    QLabel *m_lblAccelXVal;
    QLabel *m_lblAccelYVal;
    QLabel *m_lblAccelZVal;
    QLabel *m_lblGyroXVal;
    QLabel *m_lblGyroYVal;
    QLabel *m_lblGyroZVal;
    QLabel *m_lblLat;
    QLabel *m_lblLon;
    QLabel *m_lblGpsAlt;
    QLabel *m_lblSatellites;
    QLabel *m_lblState;

    // ── LEFT PANEL: Mission overview, orientation, GPS ───────────────────────
    // 1. 3D Orientation
    QQuickWidget *m_orientation3DWidget;
    QLabel *m_lblOrientRoll;       // Roll  computed from accel (°)
    QLabel *m_lblOrientPitch;      // Pitch computed from accel (°)
    QLabel *m_lblOrientYawRate;    // Yaw rate = Gyro Z (°/s)
    
    // 2. GPS Location
    QLabel *m_lblLeftGpsLat;
    QLabel *m_lblLeftGpsLon;
    QLabel *m_lblLeftGpsAlt;
    QLabel *m_lblLeftGpsSats;
    QLabel *m_lblMapPlaceholder; // Map visual element

    // 3. Mission State
    QLabel *m_lblLeftState;        // Large mission-state indicator

    // ── Chart 1: Altitude & Voltage vs Time ─────────────────────────────────
    QChart      *m_altVoltTimeChart;
    QLineSeries *m_altTimeSeries;   // Cyan  (#58a6ff)
    QLineSeries *m_voltTimeSeries;  // Gold  (#ffa657)
    QValueAxis  *m_altVoltAxisX;    // Time (s)
    QValueAxis  *m_altVoltAxisYAlt; // Altitude (m)  [Left-Y]
    QValueAxis  *m_altVoltAxisYVolt;// Voltage (V)   [Right-Y]

    // ── Chart 2: Altitude (X) vs Temp (left-Y) & Pressure (right-Y) ─────────
    QChart      *m_altTempPresChart;
    QLineSeries *m_altTempSeries;   // Green (#3fb950)
    QLineSeries *m_altPresSeries;   // Amber (#e3b341)
    QValueAxis  *m_atpAxisX;        // Altitude (m) – shared X
    QValueAxis  *m_atpAxisYTemp;    // Temperature (°C) – left Y
    QValueAxis  *m_atpAxisYPres;    // Pressure (Pa) – right Y

    // ── Chart 3: Altitude (X) vs Accelerometer X/Y/Z ────────────────────────
    QChart      *m_altAccelChart;
    QLineSeries *m_altAccelX;       // Red    (#f85149)
    QLineSeries *m_altAccelY;       // Lime   (#7ee787)
    QLineSeries *m_altAccelZ;       // Purple (#d2a8ff)
    QValueAxis  *m_accelAxisX;      // Altitude (m)
    QValueAxis  *m_accelAxisY;      // Acceleration (g)

    // ── Chart 4: Altitude (X) vs Gyroscope X/Y/Z ────────────────────────────
    QChart      *m_altGyroChart;
    QLineSeries *m_altGyroX;        // Amber (#e3b341)
    QLineSeries *m_altGyroY;        // Sky   (#79c0ff)
    QLineSeries *m_altGyroZ;        // Pink  (#f778ba)
    QValueAxis  *m_gyroAxisX;       // Altitude (m)
    QValueAxis  *m_gyroAxisY;       // Angular rate (°/s)

    int m_packetCount;
};
