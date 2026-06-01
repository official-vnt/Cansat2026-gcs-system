#pragma once

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QQuickWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

#include "core/SerialWorker.h"
#include "core/DataParser.h"
#include "storage/Logger.h"
#include "MapWidget.h"

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

    // Mission Timer
    void onMissionTimerTick();
    void onExportCsv();
    void onExportReport();

private:
    // ── Setup helpers ────────────────────────────────────────────────────────
    void setupUiGraphs();
    void setupConnections();
    void setupDashboardCards();
    void setupLeftPanel();
    void setupTopRibbon();
    void setupCommandCenter();
    void setupRecoveryPanel();
    void setupLoggingPanel();
    void setupStatePanel();
    void updateCharts(const TelemetryPacket &packet);
    void styleChart(QChart *chart, const QString &title);
    void showRecoveryPanel(bool visible);

    // ── Helper to show confirm dialog ────────────────────────────────────────
    bool confirmAction(const QString &action);

    Ui::MainWindow *ui;

    // ── Threading & backend ──────────────────────────────────────────────────
    QThread        m_serialThread;
    SerialWorker  *m_worker;
    DataParser    *m_parser;
    Logger        *m_logger;

    // ── Mission timer ────────────────────────────────────────────────────────
    QTimer         m_missionTimer;
    int            m_missionSeconds = 0;

    // ── Packet counter ───────────────────────────────────────────────────────
    int            m_packetCount;
    int            m_packetStored = 0;

    // ── TOP RIBBON ───────────────────────────────────────────────────────────
    QLabel        *m_lblMissionTimer;   // from ui (set in setupTopRibbon)
    QProgressBar  *m_linkQualityBar;
    QLabel        *m_lblRssi;
    QLabel        *m_lblSnr;

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
    QLabel *m_lblOrientRoll;
    QLabel *m_lblOrientPitch;
    QLabel *m_lblOrientYawRate;

    // 2. GPS Location
    QLabel *m_lblLeftGpsLat;
    QLabel *m_lblLeftGpsLon;
    QLabel *m_lblLeftGpsAlt;
    QLabel *m_lblLeftGpsSats;
    MapWidget *m_mapWidget;

    // 3. Mission State (Sequence)
    QLabel *m_lblStateLaunch;
    QLabel *m_lblStateDep1;
    QLabel *m_lblStateDescent;
    QLabel *m_lblStateDep2;
    QLabel *m_lblStateDescent2;
    QLabel *m_lblStateLand;
    int m_currentStateSequence = 0; // 0=Launch, 1=Dep1, 2=Descent1, 3=Dep2, 4=Descent2, 5=Land

    // ── RIGHT BOTTOM: Command Center ─────────────────────────────────────────
    QList<QPushButton*> m_cmdButtons; // List of dynamically generated buttons

    // ── RIGHT BOTTOM: Recovery Panel ─────────────────────────────────────────
    QLabel *m_lblRecoveryLat;
    QLabel *m_lblRecoveryLon;
    QLabel *m_lblRecoveryDistance;
    QLabel *m_lblRecoveryBearing;
    QLabel *m_lblBeaconStatus;
    QLabel *m_lblLastTelemetry;
    double  m_landingLat  = 0.0;
    double  m_landingLon  = 0.0;
    bool    m_landed      = false;

    // ── LEFT BOTTOM: Data Logging Panel ──────────────────────────────────────
    QLabel *m_lblRecordingDot;
    QLabel *m_lblLogFile;
    QLabel *m_lblPacketsStored;
    QLabel *m_lblStorageRemaining;

    // ── Charts ───────────────────────────────────────────────────────────────
    // Chart 1: Altitude & Voltage vs Time
    QChart      *m_altVoltTimeChart;
    QLineSeries *m_altTimeSeries;
    QLineSeries *m_voltTimeSeries;
    QValueAxis  *m_altVoltAxisX;
    QValueAxis  *m_altVoltAxisYAlt;
    QValueAxis  *m_altVoltAxisYVolt;

    // Chart 2: Altitude (X) vs Temp (left-Y) & Pressure (right-Y)
    QChart      *m_altTempPresChart;
    QLineSeries *m_altTempSeries;
    QLineSeries *m_altPresSeries;
    QValueAxis  *m_atpAxisX;
    QValueAxis  *m_atpAxisYTemp;
    QValueAxis  *m_atpAxisYPres;

    // Chart 3: Altitude (X) vs Accelerometer X/Y/Z
    QChart      *m_altAccelChart;
    QLineSeries *m_altAccelX;
    QLineSeries *m_altAccelY;
    QLineSeries *m_altAccelZ;
    QValueAxis  *m_accelAxisX;
    QValueAxis  *m_accelAxisY;

    // Chart 4: Altitude (X) vs Gyroscope X/Y/Z
    QChart      *m_altGyroChart;
    QLineSeries *m_altGyroX;
    QLineSeries *m_altGyroY;
    QLineSeries *m_altGyroZ;
    QValueAxis  *m_gyroAxisX;
    QValueAxis  *m_gyroAxisY;
};
