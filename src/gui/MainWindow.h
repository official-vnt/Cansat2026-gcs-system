#pragma once

#include <QMainWindow>
#include <QThread>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

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
    void updateCharts(const TelemetryPacket &packet);

    Ui::MainWindow *ui;

    // Threads and Workers
    QThread m_serialThread;
    SerialWorker *m_worker;
    DataParser *m_parser;
    Logger *m_logger;

    // Custom Telemetry Readout Labels
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

    // Charts
    QChart *m_altChart;
    QLineSeries *m_altSeries;
    QValueAxis *m_altAxisX;
    QValueAxis *m_altAxisY;

    QChart *m_tempChart;
    QLineSeries *m_tempSeries;
    QValueAxis *m_tempAxisX;
    QValueAxis *m_tempAxisY;
    
    int m_packetCount;
};

