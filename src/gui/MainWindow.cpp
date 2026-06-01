#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QValueAxis>
#include <QQuickWidget>
#include <QQuickItem>
#include <QUrl>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_worker(new SerialWorker())
    , m_parser(new DataParser(this))
    , m_logger(new Logger(this))
    , m_packetCount(0)
{
    ui->setupUi(this);

    // ── Global QSS (GitHub Dark / Cyberpunk) ──────────────────────────────
    this->setStyleSheet(
        "QMainWindow {"
        "  background-color: #0d1117;"
        "}"
        "QWidget#centralwidget {"
        "  background-color: #0d1117;"
        "}"
        "QGroupBox {"
        "  font-family: 'Segoe UI', sans-serif;"
        "  font-size: 11px;"
        "  font-weight: 800;"
        "  text-transform: uppercase;"
        "  letter-spacing: 1px;"
        "  color: #58a6ff;"
        "  border: 1px solid #21262d;"
        "  border-radius: 8px;"
        "  margin-top: 15px;"
        "  padding: 10px;"
        "  background-color: #161b22;"
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  subcontrol-position: top left;"
        "  left: 15px;"
        "  padding: 0 5px;"
        "  color: #58a6ff;"
        "}"
        "QLabel {"
        "  color: #c9d1d9;"
        "  font-family: 'Segoe UI', sans-serif;"
        "  font-size: 12px;"
        "}"
        "QComboBox {"
        "  background-color: #21262d;"
        "  border: 1px solid #30363d;"
        "  border-radius: 6px;"
        "  padding: 5px 12px;"
        "  color: #c9d1d9;"
        "  font-family: 'Segoe UI', sans-serif;"
        "  min-width: 110px;"
        "}"
        "QComboBox:hover {"
        "  border-color: #58a6ff;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "  width: 20px;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #161b22;"
        "  border: 1px solid #30363d;"
        "  color: #c9d1d9;"
        "  selection-background-color: #21262d;"
        "  selection-color: #58a6ff;"
        "}"
        "QPushButton {"
        "  background-color: #21262d;"
        "  border: 1px solid #30363d;"
        "  border-radius: 6px;"
        "  padding: 6px 16px;"
        "  color: #c9d1d9;"
        "  font-weight: bold;"
        "  font-family: 'Segoe UI', sans-serif;"
        "  text-transform: uppercase;"
        "  font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #30363d;"
        "  border-color: #8b949e;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #161b22;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #0d1117;"
        "  border-color: #21262d;"
        "  color: #484f58;"
        "}"
        "QPushButton#connectButton {"
        "  background-color: rgba(35, 134, 54, 0.15);"
        "  border-color: rgba(46, 160, 67, 0.4);"
        "  color: #3fb950;"
        "}"
        "QPushButton#connectButton:hover {"
        "  background-color: rgba(35, 134, 54, 0.25);"
        "  border-color: #2ea043;"
        "}"
        "QPushButton#disconnectButton {"
        "  background-color: rgba(248, 81, 73, 0.1);"
        "  border-color: rgba(248, 81, 73, 0.3);"
        "  color: #f85149;"
        "}"
        "QPushButton#disconnectButton:hover {"
        "  background-color: rgba(248, 81, 73, 0.2);"
        "  border-color: #f85149;"
        "}"
        "QStatusBar {"
        "  background-color: #161b22;"
        "  color: #8b949e;"
        "  border-top: 1px solid #21262d;"
        "}"
    );

    this->setWindowTitle("NAKSHATRA GCS - CanSat Ground Control Station");

    // Populate COM ports
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ui->portComboBox->addItem(info.portName());
    }

    // Baud rates
    ui->baudComboBox->addItem(QStringLiteral("9600"),   QSerialPort::Baud9600);
    ui->baudComboBox->addItem(QStringLiteral("19200"),  QSerialPort::Baud19200);
    ui->baudComboBox->addItem(QStringLiteral("38400"),  QSerialPort::Baud38400);
    ui->baudComboBox->addItem(QStringLiteral("57600"),  QSerialPort::Baud57600);
    ui->baudComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudComboBox->setCurrentIndex(4);

    ui->statusLabel->setText("<span style='color:#f85149; font-weight:bold;'>○ Disconnected</span>");

    setupDashboardCards();
    setupLeftPanel();
    setupUiGraphs();
    setupConnections();

    m_worker->moveToThread(&m_serialThread);
    m_serialThread.start();
}

MainWindow::~MainWindow()
{
    m_serialThread.quit();
    m_serialThread.wait();
    delete m_worker;
    delete ui;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Dashboard Cards (Telemetry readout panel)
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupDashboardCards()
{
    if (ui->groupBox->layout()) {
        QLayout *oldLayout = ui->groupBox->layout();
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QGridLayout *gridLayout = new QGridLayout(ui->groupBox);
    gridLayout->setContentsMargins(10, 15, 10, 10);
    gridLayout->setSpacing(8);

    auto createCard = [](const QString &title, QLabel *&valLabel,
                          const QString &initVal, QWidget *parent) {
        QFrame *card = new QFrame(parent);
        card->setObjectName("telemetryCard");

        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setContentsMargins(10, 8, 10, 8);
        layout->setSpacing(3);

        QLabel *titleLabel = new QLabel(title, card);
        titleLabel->setStyleSheet(
            "font-size: 9px; font-weight: 800; color: #8b949e;"
            " text-transform: uppercase; letter-spacing: 0.5px;");

        valLabel = new QLabel(initVal, card);
        valLabel->setStyleSheet(
            "font-family: 'Consolas', 'Courier New', monospace;"
            " font-size: 15px; font-weight: bold; color: #58a6ff;");

        layout->addWidget(titleLabel);
        layout->addWidget(valLabel);

        card->setStyleSheet(
            "QFrame#telemetryCard {"
            "  background-color: #0d1117;"
            "  border: 1px solid #21262d;"
            "  border-radius: 6px;"
            "}"
            "QFrame#telemetryCard:hover {"
            "  border-color: #58a6ff;"
            "  background-color: #161b22;"
            "}");
        return card;
    };

    QWidget *cardTeamId  = createCard("Team ID",       m_lblTeamId,        "-",          ui->groupBox);
    QWidget *cardPacket  = createCard("Packet ID",     ui->lblPacketId,    "0",          ui->groupBox);
    QWidget *cardUptime  = createCard("System Time",   ui->lblTimestamp,   "0 ms",       ui->groupBox);
    QWidget *cardState   = createCard("State",         m_lblState,         "IDLE",       ui->groupBox);
    QWidget *cardAlt     = createCard("Altitude",      m_lblAltitudeVal,   "0.0 m",      ui->groupBox);
    QWidget *cardGpsAlt  = createCard("GPS Alt",       m_lblGpsAlt,        "0.0 m",      ui->groupBox);
    QWidget *cardLat     = createCard("Latitude",      m_lblLat,           "0.0",        ui->groupBox);
    QWidget *cardLon     = createCard("Longitude",     m_lblLon,           "0.0",        ui->groupBox);
    QWidget *cardTemp    = createCard("Temperature",   m_lblTemperatureVal,"0.0 °C",     ui->groupBox);
    QWidget *cardPres    = createCard("Baro Pressure", m_lblPressureVal,   "101325 Pa",  ui->groupBox);
    QWidget *cardVolt    = createCard("Voltage",       m_lblVoltage,       "0.0 V",      ui->groupBox);
    QWidget *cardSats    = createCard("Satellites",    m_lblSatellites,    "0",          ui->groupBox);

    m_lblTemperatureVal->setStyleSheet(
        "font-family: 'Consolas', 'Courier New', monospace;"
        " font-size: 15px; font-weight: bold; color: #3fb950;");
    m_lblState->setStyleSheet(
        "font-family: 'Consolas', 'Courier New', monospace;"
        " font-size: 15px; font-weight: bold; color: #f85149;");

    // ── Accelerometer card ──────────────────────────────────────────────────
    QFrame *accelCard = new QFrame(ui->groupBox);
    accelCard->setObjectName("telemetryCard");
    accelCard->setStyleSheet(
        "QFrame#telemetryCard {"
        "  background-color: #0d1117; border: 1px solid #21262d; border-radius: 6px;"
        "}"
        "QFrame#telemetryCard:hover {"
        "  border-color: #58a6ff; background-color: #161b22;"
        "}");

    QVBoxLayout *accelLayout = new QVBoxLayout(accelCard);
    accelLayout->setContentsMargins(10, 8, 10, 8);
    accelLayout->setSpacing(3);

    QLabel *accelTitle = new QLabel("ACCELEROMETER (g)", accelCard);
    accelTitle->setStyleSheet(
        "font-size: 9px; font-weight: 800; color: #8b949e;"
        " text-transform: uppercase; letter-spacing: 0.5px;");
    accelLayout->addWidget(accelTitle);

    QHBoxLayout *accelValsLayout = new QHBoxLayout();
    accelValsLayout->setSpacing(6);

    auto createAxisLabel = [](const QString &axis, QLabel *&label,
                               const QString &color, QWidget *parent) {
        QWidget *container = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(2);

        QLabel *axisTag = new QLabel(axis + ":", container);
        axisTag->setStyleSheet(
            QString("font-family: 'Consolas', monospace; font-size: 10px;"
                    " font-weight: bold; color: %1;").arg(color));

        label = new QLabel("0.00", container);
        label->setStyleSheet(
            "font-family: 'Consolas', monospace; font-size: 11px;"
            " font-weight: bold; color: #c9d1d9;");

        layout->addWidget(axisTag);
        layout->addWidget(label);
        return container;
    };

    accelValsLayout->addWidget(createAxisLabel("X", m_lblAccelXVal, "#f85149", accelCard));
    accelValsLayout->addWidget(createAxisLabel("Y", m_lblAccelYVal, "#7ee787", accelCard));
    accelValsLayout->addWidget(createAxisLabel("Z", m_lblAccelZVal, "#d2a8ff", accelCard));
    accelLayout->addLayout(accelValsLayout);

    // ── Gyroscope card ──────────────────────────────────────────────────────
    QFrame *gyroCard = new QFrame(ui->groupBox);
    gyroCard->setObjectName("telemetryCard");
    gyroCard->setStyleSheet(accelCard->styleSheet());

    QVBoxLayout *gyroLayout = new QVBoxLayout(gyroCard);
    gyroLayout->setContentsMargins(10, 8, 10, 8);
    gyroLayout->setSpacing(3);

    QLabel *gyroTitle = new QLabel("GYROSCOPE (°/s)", gyroCard);
    gyroTitle->setStyleSheet(accelTitle->styleSheet());
    gyroLayout->addWidget(gyroTitle);

    QHBoxLayout *gyroValsLayout = new QHBoxLayout();
    gyroValsLayout->setSpacing(6);
    gyroValsLayout->addWidget(createAxisLabel("X", m_lblGyroXVal, "#e3b341", gyroCard));
    gyroValsLayout->addWidget(createAxisLabel("Y", m_lblGyroYVal, "#79c0ff", gyroCard));
    gyroValsLayout->addWidget(createAxisLabel("Z", m_lblGyroZVal, "#f778ba", gyroCard));
    gyroLayout->addLayout(gyroValsLayout);

    // ── Place cards in grid ─────────────────────────────────────────────────
    int row = 0;
    gridLayout->addWidget(cardTeamId,  row, 0); gridLayout->addWidget(cardPacket, row, 1); row++;
    gridLayout->addWidget(cardUptime,  row, 0); gridLayout->addWidget(cardState,  row, 1); row++;
    gridLayout->addWidget(cardAlt,     row, 0); gridLayout->addWidget(cardGpsAlt, row, 1); row++;
    gridLayout->addWidget(cardLat,     row, 0); gridLayout->addWidget(cardLon,    row, 1); row++;
    gridLayout->addWidget(cardTemp,    row, 0); gridLayout->addWidget(cardPres,   row, 1); row++;
    gridLayout->addWidget(cardVolt,    row, 0); gridLayout->addWidget(cardSats,   row, 1); row++;
    gridLayout->addWidget(accelCard,   row, 0); gridLayout->addWidget(gyroCard,   row, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Left Panel Setup
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupLeftPanel()
{
    // Utility for creating sections
    auto createLeftCard = [](const QString &title, QWidget *parent) {
        QGroupBox *group = new QGroupBox(title, parent);
        group->setStyleSheet(
            "QGroupBox {"
            "  font-weight: bold;"
            "  font-size: 14px;"
            "  border: 1px solid #30363d;"
            "  border-radius: 6px;"
            "  margin-top: 10px;"
            "  padding: 10px;"
            "}"
            "QGroupBox::title {"
            "  subcontrol-origin: margin;"
            "  left: 10px;"
            "  color: #58a6ff;"
            "}"
            "QLabel { font-size: 13px; color: #c9d1d9; }"
        );
        QVBoxLayout *layout = new QVBoxLayout(group);
        layout->setSpacing(4);
        return qMakePair(group, layout);
    };

    auto createRow = [](const QString &labelTxt, QLabel *&valLabel, QWidget *parent) {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *nameLbl = new QLabel(labelTxt, parent);
        valLabel = new QLabel("-", parent);
        valLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        valLabel->setStyleSheet("font-weight: bold; color: #58a6ff;");
        row->addWidget(nameLbl);
        row->addWidget(valLabel);
        return row;
    };

    // 1. 3D Orientation Card (Top)
    auto orientPair = createLeftCard("3D ORIENTATION", ui->leftGroupBox);
    
    // QtQuick3D viewer
    m_orientation3DWidget = new QQuickWidget(orientPair.first);
    m_orientation3DWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_orientation3DWidget->setSource(QUrl::fromLocalFile("d:/NAKSHATRA/inspace_cansat_2026/gcs/app.gcs.system/src/gui/Orientation3D.qml"));
    m_orientation3DWidget->setMinimumHeight(150);
    
    // Add to layout with stretch=1 so it shares space equally
    orientPair.second->addWidget(m_orientation3DWidget, 1);
    
    orientPair.second->addLayout(createRow("Roll (°):", m_lblOrientRoll, orientPair.first));
    orientPair.second->addLayout(createRow("Pitch (°):", m_lblOrientPitch, orientPair.first));
    orientPair.second->addLayout(createRow("Yaw Rate (°/s):", m_lblOrientYawRate, orientPair.first));

    ui->leftPanelLayout->addWidget(orientPair.first, 1);


    // 2. GPS Card + Map Placeholder (Middle)
    auto gpsPair = createLeftCard("GPS LOCATION", ui->leftGroupBox);
    gpsPair.second->addLayout(createRow("Latitude:", m_lblLeftGpsLat, gpsPair.first));
    gpsPair.second->addLayout(createRow("Longitude:", m_lblLeftGpsLon, gpsPair.first));
    gpsPair.second->addLayout(createRow("Altitude (m):", m_lblLeftGpsAlt, gpsPair.first));
    gpsPair.second->addLayout(createRow("Satellites:", m_lblLeftGpsSats, gpsPair.first));
    
    m_lblMapPlaceholder = new QLabel("GPS MAP\n(API LINK PENDING)", gpsPair.first);
    m_lblMapPlaceholder->setAlignment(Qt::AlignCenter);
    m_lblMapPlaceholder->setStyleSheet(
        "background-color: #0d1117; "
        "border: 1px dashed #30363d; "
        "border-radius: 4px; "
        "color: #8b949e; "
        "font-weight: bold;"
    );
    m_lblMapPlaceholder->setMinimumHeight(150);
    gpsPair.second->addWidget(m_lblMapPlaceholder, 1);
    
    ui->leftPanelLayout->addWidget(gpsPair.first, 1);


    // 3. Mission State Card (Bottom)
    auto statePair = createLeftCard("CANSAT STATE", ui->leftGroupBox);
    m_lblLeftState = new QLabel("OFFLINE", statePair.first);
    m_lblLeftState->setAlignment(Qt::AlignCenter);
    m_lblLeftState->setStyleSheet("font-size: 28px; font-weight: bold; color: #f85149;");
    statePair.second->addWidget(m_lblLeftState, 1);

    ui->leftPanelLayout->addWidget(statePair.first, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Style helper — applies common GitHub Dark styling to any QChart
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::styleChart(QChart *chart, const QString &title)
{
    QFont titleFont("Segoe UI", 9, QFont::Bold);
    QColor textColor(201, 209, 217);           // #c9d1d9
    QColor bgColor(22, 27, 34);                // #161b22
    QColor plotBgColor(13, 17, 23);            // #0d1117

    chart->setTitle(title);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(textColor));
    chart->setBackgroundBrush(QBrush(bgColor));
    chart->setBackgroundRoundness(6);
    chart->setPlotAreaBackgroundBrush(QBrush(plotBgColor));
    chart->setPlotAreaBackgroundVisible(true);
    chart->setMargins(QMargins(0, 0, 0, 0));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Axis helper — applies common dark-theme styling to a QValueAxis
// ─────────────────────────────────────────────────────────────────────────────
static void styleAxis(QValueAxis *axis, const QString &title,
                      qreal min, qreal max)
{
    QColor textColor(201, 209, 217);
    QPen gridPen(QColor(48, 54, 61, 130));
    gridPen.setWidthF(1.0);

    axis->setTitleText(title);
    axis->setTitleBrush(QBrush(textColor));
    axis->setTitleFont(QFont("Segoe UI", 8));
    axis->setRange(min, max);
    axis->setGridLinePen(gridPen);
    axis->setLabelsColor(textColor);
    axis->setLabelsFont(QFont("Segoe UI", 8));
    axis->setLinePen(QPen(QColor(48, 54, 61)));
}

// ─────────────────────────────────────────────────────────────────────────────
//  setupUiGraphs  — builds all 5 charts and wires them to their chart views
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupUiGraphs()
{
    // ── Color palette ────────────────────────────────────────────────────────
    // Chart 1 — Altitude vs Time
    const QColor colAlt    ("#58a6ff");  // Cyan-Blue

    // Chart 2 — Altitude vs Temp & Pressure
    const QColor colTemp   ("#3fb950");  // Green
    const QColor colPres   ("#e3b341");  // Amber/Orange

    // Chart 3 — Altitude vs Accel X/Y/Z
    const QColor colAccelX ("#f85149");  // Red
    const QColor colAccelY ("#7ee787");  // Lime
    const QColor colAccelZ ("#d2a8ff");  // Purple

    // Chart 4 — Altitude vs Gyro X/Y/Z
    const QColor colGyroX  ("#e3b341");  // Amber
    const QColor colGyroY  ("#79c0ff");  // Sky-Blue
    const QColor colGyroZ  ("#f778ba");  // Pink

    // Chart 5 — Voltage vs Time
    const QColor colVolt   ("#ffa657");  // Gold/Orange

    constexpr qreal lineW = 2.0;

    auto makePen = [&](const QColor &c) {
        QPen p(c);
        p.setWidthF(lineW);
        return p;
    };

    // ── Legend styling helper ────────────────────────────────────────────────
    auto styleLegend = [](QChart *chart) {
        QLegend *leg = chart->legend();
        leg->show();
        leg->setAlignment(Qt::AlignBottom);
        leg->setBackgroundVisible(false);
        leg->setBorderColor(Qt::transparent);
        leg->setColor(Qt::transparent);
        leg->setLabelColor(QColor(201, 209, 217));
        leg->setFont(QFont("Segoe UI", 8));
    };

    // ════════════════════════════════════════════════════════════════════════
    //  CHART 1 — Altitude & Voltage vs Time
    // ════════════════════════════════════════════════════════════════════════
    m_altVoltTimeChart  = new QChart();
    styleChart(m_altVoltTimeChart, "Altitude & Voltage vs Time");

    m_altTimeSeries = new QLineSeries();
    m_altTimeSeries->setName("Altitude (m)  ●");
    m_altTimeSeries->setPen(makePen(colAlt));

    m_voltTimeSeries = new QLineSeries();
    m_voltTimeSeries->setName("Voltage (V)  ●");
    m_voltTimeSeries->setPen(makePen(colVolt));

    m_altVoltTimeChart->addSeries(m_altTimeSeries);
    m_altVoltTimeChart->addSeries(m_voltTimeSeries);

    m_altVoltAxisX = new QValueAxis();
    styleAxis(m_altVoltAxisX, "Time (s)", 0, 100);
    m_altVoltTimeChart->addAxis(m_altVoltAxisX, Qt::AlignBottom);

    m_altVoltAxisYAlt = new QValueAxis();
    styleAxis(m_altVoltAxisYAlt, "Altitude (m)", 0, 600);
    m_altVoltTimeChart->addAxis(m_altVoltAxisYAlt, Qt::AlignLeft);
    
    m_altVoltAxisYVolt = new QValueAxis();
    styleAxis(m_altVoltAxisYVolt, "Voltage (V)", 0, 12);
    m_altVoltTimeChart->addAxis(m_altVoltAxisYVolt, Qt::AlignRight);

    m_altTimeSeries->attachAxis(m_altVoltAxisX);
    m_altTimeSeries->attachAxis(m_altVoltAxisYAlt);
    
    m_voltTimeSeries->attachAxis(m_altVoltAxisX);
    m_voltTimeSeries->attachAxis(m_altVoltAxisYVolt);

    styleLegend(m_altVoltTimeChart);
    for (QLegendMarker *marker : m_altVoltTimeChart->legend()->markers()) {
        marker->setFont(QFont("Segoe UI", 8));
        marker->setLabelBrush(QBrush(QColor(201, 209, 217)));
    }

    ui->altVoltTimeChartView->setChart(m_altVoltTimeChart);
    ui->altVoltTimeChartView->setRenderHint(QPainter::Antialiasing);

    // ════════════════════════════════════════════════════════════════════════
    //  CHART 2 — Altitude (X) vs Temperature (left-Y, Green) &
    //             Pressure (right-Y, Amber)
    // ════════════════════════════════════════════════════════════════════════
    m_altTempPresChart = new QChart();
    styleChart(m_altTempPresChart, "Altitude vs Temp & Pressure");

    m_altTempSeries = new QLineSeries();
    m_altTempSeries->setName("Temp (°C)  ●");
    m_altTempSeries->setPen(makePen(colTemp));

    m_altPresSeries = new QLineSeries();
    m_altPresSeries->setName("Pressure (Pa)  ●");
    m_altPresSeries->setPen(makePen(colPres));

    m_altTempPresChart->addSeries(m_altTempSeries);
    m_altTempPresChart->addSeries(m_altPresSeries);

    m_atpAxisX = new QValueAxis();
    styleAxis(m_atpAxisX, "Altitude (m)", 0, 600);

    m_atpAxisYTemp = new QValueAxis();
    styleAxis(m_atpAxisYTemp, "Temperature (°C)", -10, 60);

    m_atpAxisYPres = new QValueAxis();
    styleAxis(m_atpAxisYPres, "Pressure (Pa)", 90000, 105000);

    m_altTempPresChart->addAxis(m_atpAxisX,     Qt::AlignBottom);
    m_altTempPresChart->addAxis(m_atpAxisYTemp, Qt::AlignLeft);
    m_altTempPresChart->addAxis(m_atpAxisYPres, Qt::AlignRight);

    m_altTempSeries->attachAxis(m_atpAxisX);
    m_altTempSeries->attachAxis(m_atpAxisYTemp);

    m_altPresSeries->attachAxis(m_atpAxisX);
    m_altPresSeries->attachAxis(m_atpAxisYPres);

    styleLegend(m_altTempPresChart);

    // Force legend marker colors to match series pens
    for (QLegendMarker *marker : m_altTempPresChart->legend()->markers()) {
        marker->setFont(QFont("Segoe UI", 8));
        marker->setLabelBrush(QBrush(QColor(201, 209, 217)));
    }

    ui->altTempPresChartView->setChart(m_altTempPresChart);
    ui->altTempPresChartView->setRenderHint(QPainter::Antialiasing);

    // ════════════════════════════════════════════════════════════════════════
    //  CHART 3 — Altitude (X) vs Accelerometer X / Y / Z
    // ════════════════════════════════════════════════════════════════════════
    m_altAccelChart = new QChart();
    styleChart(m_altAccelChart, "Altitude vs Accelerometer");

    m_altAccelX = new QLineSeries(); m_altAccelX->setName("Accel X   ●");   m_altAccelX->setPen(makePen(colAccelX));
    m_altAccelY = new QLineSeries(); m_altAccelY->setName("Accel Y   ●");   m_altAccelY->setPen(makePen(colAccelY));
    m_altAccelZ = new QLineSeries(); m_altAccelZ->setName("Accel Z   ●");  m_altAccelZ->setPen(makePen(colAccelZ));

    m_altAccelChart->addSeries(m_altAccelX);
    m_altAccelChart->addSeries(m_altAccelY);
    m_altAccelChart->addSeries(m_altAccelZ);

    m_accelAxisX = new QValueAxis();
    m_accelAxisY = new QValueAxis();
    styleAxis(m_accelAxisX, "Altitude (m)",     0,  600);
    styleAxis(m_accelAxisY, "Acceleration (g)", -2,  2);
    m_altAccelChart->addAxis(m_accelAxisX, Qt::AlignBottom);
    m_altAccelChart->addAxis(m_accelAxisY, Qt::AlignLeft);

    for (QLineSeries *s : {m_altAccelX, m_altAccelY, m_altAccelZ}) {
        s->attachAxis(m_accelAxisX);
        s->attachAxis(m_accelAxisY);
    }

    styleLegend(m_altAccelChart);
    ui->altAccelChartView->setChart(m_altAccelChart);
    ui->altAccelChartView->setRenderHint(QPainter::Antialiasing);

    // ════════════════════════════════════════════════════════════════════════
    //  CHART 4 — Altitude (X) vs Gyroscope X / Y / Z
    // ════════════════════════════════════════════════════════════════════════
    m_altGyroChart = new QChart();
    styleChart(m_altGyroChart, "Altitude vs Gyroscope");

    m_altGyroX = new QLineSeries(); m_altGyroX->setName("Gyro X ●");    m_altGyroX->setPen(makePen(colGyroX));
    m_altGyroY = new QLineSeries(); m_altGyroY->setName("Gyro Y ●");    m_altGyroY->setPen(makePen(colGyroY));
    m_altGyroZ = new QLineSeries(); m_altGyroZ->setName("Gyro Z ●");    m_altGyroZ->setPen(makePen(colGyroZ));

    m_altGyroChart->addSeries(m_altGyroX);
    m_altGyroChart->addSeries(m_altGyroY);
    m_altGyroChart->addSeries(m_altGyroZ);

    m_gyroAxisX = new QValueAxis();
    m_gyroAxisY = new QValueAxis();
    styleAxis(m_gyroAxisX, "Altitude (m)",       0,   600);
    styleAxis(m_gyroAxisY, "Angular Rate (°/s)", -5,   5);
    m_altGyroChart->addAxis(m_gyroAxisX, Qt::AlignBottom);
    m_altGyroChart->addAxis(m_gyroAxisY, Qt::AlignLeft);

    for (QLineSeries *s : {m_altGyroX, m_altGyroY, m_altGyroZ}) {
        s->attachAxis(m_gyroAxisX);
        s->attachAxis(m_gyroAxisY);
    }

    styleLegend(m_altGyroChart);
    ui->altGyroChartView->setChart(m_altGyroChart);
    ui->altGyroChartView->setRenderHint(QPainter::Antialiasing);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Connections
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupConnections()
{
    connect(ui->connectButton,    &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

    connect(m_worker, &SerialWorker::portOpened,    this, &MainWindow::onPortOpened);
    connect(m_worker, &SerialWorker::portClosed,    this, &MainWindow::onPortClosed);
    connect(m_worker, &SerialWorker::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(m_worker, &SerialWorker::dataReady,   m_parser, &DataParser::parseData);
    connect(m_parser, &DataParser::packetReceived,  this, &MainWindow::onPacketReceived);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Slot Implementations
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onConnectClicked()
{
    QString portName = ui->portComboBox->currentText();
    int baudRate     = ui->baudComboBox->currentData().toInt();

    if (portName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select a valid COM port.");
        return;
    }

    m_logger->startLogging(QCoreApplication::applicationDirPath() + "/logs");

    QMetaObject::invokeMethod(m_worker, "openPort", Qt::QueuedConnection,
                               Q_ARG(QString, portName),
                               Q_ARG(int, baudRate));
}

void MainWindow::onDisconnectClicked()
{
    QMetaObject::invokeMethod(m_worker, "closePort", Qt::QueuedConnection);
    m_logger->stopLogging();
}

void MainWindow::onPortOpened()
{
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->portComboBox->setEnabled(false);
    ui->baudComboBox->setEnabled(false);
    ui->statusLabel->setText("<span style='color:#2ea043; font-weight:bold;'>● Connected</span>");

    m_packetCount = 0;

    // Clear all series on new session
    m_altTimeSeries->clear();
    m_altTempSeries->clear();
    m_altPresSeries->clear();
    m_altAccelX->clear();
    m_altAccelY->clear();
    m_altAccelZ->clear();
    m_altGyroX->clear();
    m_altGyroY->clear();
    m_altGyroZ->clear();
    m_voltTimeSeries->clear();

    // Reset axes
    m_altVoltAxisX->setRange(0, 100);
}

void MainWindow::onPortClosed()
{
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->portComboBox->setEnabled(true);
    ui->baudComboBox->setEnabled(true);
    ui->statusLabel->setText("<span style='color:#f85149; font-weight:bold;'>○ Disconnected</span>");
}

void MainWindow::onPacketReceived(const TelemetryPacket &packet)
{
    m_packetCount++;

    // ── Update telemetry readout cards ──────────────────────────────────────
    if (m_lblTeamId)        m_lblTeamId->setText(packet.teamID);
    if (ui->lblPacketId)    ui->lblPacketId->setText(QString::number(packet.packetCount));
    if (ui->lblTimestamp)   ui->lblTimestamp->setText(QString::number(packet.time) + " ms");
    if (m_lblState)         m_lblState->setText(packet.state);
    if (m_lblAltitudeVal)   m_lblAltitudeVal->setText(QString::number(packet.altitude,     'f', 1) + " m");
    if (m_lblTemperatureVal)m_lblTemperatureVal->setText(QString::number(packet.temperature,'f', 1) + " °C");
    if (m_lblPressureVal)   m_lblPressureVal->setText(QString::number(packet.pressure,     'f', 0) + " Pa");
    if (m_lblVoltage)       m_lblVoltage->setText(QString::number(packet.voltage,           'f', 2) + " V");
    if (m_lblLat)           m_lblLat->setText(QString::number(packet.latitude,              'f', 6));
    if (m_lblLon)           m_lblLon->setText(QString::number(packet.longitude,             'f', 6));
    if (m_lblGpsAlt)        m_lblGpsAlt->setText(QString::number(packet.gpsAltitude,        'f', 1) + " m");
    if (m_lblSatellites)    m_lblSatellites->setText(QString::number(packet.satellites));

    if (m_lblAccelXVal) m_lblAccelXVal->setText(QString::number(packet.accel.x(), 'f', 2));
    if (m_lblAccelYVal) m_lblAccelYVal->setText(QString::number(packet.accel.y(), 'f', 2));
    if (m_lblAccelZVal) m_lblAccelZVal->setText(QString::number(packet.accel.z(), 'f', 2));

    if (m_lblGyroXVal)  m_lblGyroXVal->setText(QString::number(packet.gyro.x(), 'f', 2));
    if (m_lblGyroYVal)  m_lblGyroYVal->setText(QString::number(packet.gyro.y(), 'f', 2));
    if (m_lblGyroZVal)  m_lblGyroZVal->setText(QString::number(packet.gyro.z(), 'f', 2));

    // ── Orientation Math & Left Panel Updates ───────────────────────────────
    if (m_lblLeftState) m_lblLeftState->setText(packet.state);
    
    // Calculate Roll and Pitch
    // Roll = atan2(Y, Z) * 180/PI
    // Pitch = atan2(-X, sqrt(Y*Y + Z*Z)) * 180/PI
    const double PI = 3.14159265358979323846;
    double roll = std::atan2(packet.accel.y(), packet.accel.z()) * 180.0 / PI;
    double pitch = std::atan2(-packet.accel.x(), std::sqrt(packet.accel.y() * packet.accel.y() + packet.accel.z() * packet.accel.z())) * 180.0 / PI;

    // Simple integration of gyro Z for Yaw approximation (dt approx 1.0 for visualization)
    static double staticYaw = 0.0;
    staticYaw += packet.gyro.z() * 0.1;
    if (staticYaw > 360.0) staticYaw -= 360.0;
    if (staticYaw < 0.0)   staticYaw += 360.0;

    // Update 3D QuickWidget
    if (m_orientation3DWidget && m_orientation3DWidget->rootObject()) {
        m_orientation3DWidget->rootObject()->setProperty("modelRoll", roll);
        m_orientation3DWidget->rootObject()->setProperty("modelPitch", pitch);
        m_orientation3DWidget->rootObject()->setProperty("modelYaw", staticYaw);
    }

    if (m_lblOrientRoll) m_lblOrientRoll->setText(QString::number(roll, 'f', 1));
    if (m_lblOrientPitch) m_lblOrientPitch->setText(QString::number(pitch, 'f', 1));
    if (m_lblOrientYawRate) m_lblOrientYawRate->setText(QString::number(packet.gyro.z(), 'f', 1));

    if (m_lblLeftGpsLat) m_lblLeftGpsLat->setText(QString::number(packet.latitude, 'f', 6));
    if (m_lblLeftGpsLon) m_lblLeftGpsLon->setText(QString::number(packet.longitude, 'f', 6));
    if (m_lblLeftGpsAlt) m_lblLeftGpsAlt->setText(QString::number(packet.gpsAltitude, 'f', 1));
    if (m_lblLeftGpsSats) m_lblLeftGpsSats->setText(QString::number(packet.satellites));

    m_logger->logPacket(packet);
    updateCharts(packet);
}

// ─────────────────────────────────────────────────────────────────────────────
//  updateCharts — push new data into every series + auto-scale axes
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::updateCharts(const TelemetryPacket &packet)
{
    qreal pkt = static_cast<qreal>(m_packetCount);
    qreal alt = static_cast<qreal>(packet.altitude);

    // ── Chart 1: Altitude & Voltage vs Time ──────────────────────────────────
    m_altTimeSeries->append(pkt, alt);
    m_voltTimeSeries->append(pkt, packet.voltage);

    if (pkt > m_altVoltAxisX->max())
        m_altVoltAxisX->setRange(pkt - 100, pkt);

    // Auto-scale Altitude (Left Y)
    if (alt > m_altVoltAxisYAlt->max())
        m_altVoltAxisYAlt->setRange(m_altVoltAxisYAlt->min(), alt * 1.15 + 10.0);
    else if (alt < m_altVoltAxisYAlt->min())
        m_altVoltAxisYAlt->setRange(alt * 0.85 - 10.0, m_altVoltAxisYAlt->max());

    // Auto-scale Voltage (Right Y)
    if (packet.voltage > m_altVoltAxisYVolt->max())
        m_altVoltAxisYVolt->setRange(m_altVoltAxisYVolt->min(), packet.voltage + 1.0);
    else if (packet.voltage < m_altVoltAxisYVolt->min())
        m_altVoltAxisYVolt->setRange(packet.voltage - 1.0, m_altVoltAxisYVolt->max());

    // ── Chart 2: Altitude (X) vs Temp & Pressure ────────────────────────────
    m_altTempSeries->append(alt, packet.temperature);
    m_altPresSeries->append(alt, packet.pressure);

    // Expand shared altitude (X) axis
    if (alt > m_atpAxisX->max())
        m_atpAxisX->setRange(m_atpAxisX->min(), alt * 1.1 + 10.0);
    else if (alt < m_atpAxisX->min())
        m_atpAxisX->setRange(alt - 10.0, m_atpAxisX->max());

    // Auto-scale temperature Y (left)
    {
        qreal t = packet.temperature;
        if (t > m_atpAxisYTemp->max())       m_atpAxisYTemp->setRange(m_atpAxisYTemp->min(), t + 5.0);
        else if (t < m_atpAxisYTemp->min())  m_atpAxisYTemp->setRange(t - 5.0, m_atpAxisYTemp->max());
    }
    // Auto-scale pressure Y (right)
    {
        qreal p = packet.pressure;
        if (p > m_atpAxisYPres->max())       m_atpAxisYPres->setRange(m_atpAxisYPres->min(), p + 500.0);
        else if (p < m_atpAxisYPres->min())  m_atpAxisYPres->setRange(p - 500.0, m_atpAxisYPres->max());
    }

    // ── Chart 3: Altitude (X) vs Accelerometer ──────────────────────────────
    m_altAccelX->append(alt, packet.accel.x());
    m_altAccelY->append(alt, packet.accel.y());
    m_altAccelZ->append(alt, packet.accel.z());

    if (alt > m_accelAxisX->max())
        m_accelAxisX->setRange(m_accelAxisX->min(), alt * 1.1 + 10.0);
    else if (alt < m_accelAxisX->min())
        m_accelAxisX->setRange(alt - 10.0, m_accelAxisX->max());

    {
        qreal maxA = qMax(qAbs(packet.accel.x()), qMax(qAbs(packet.accel.y()), qAbs(packet.accel.z())));
        if (maxA > m_accelAxisY->max())
            m_accelAxisY->setRange(-maxA * 1.2, maxA * 1.2);
    }

    // ── Chart 4: Altitude (X) vs Gyroscope ──────────────────────────────────
    m_altGyroX->append(alt, packet.gyro.x());
    m_altGyroY->append(alt, packet.gyro.y());
    m_altGyroZ->append(alt, packet.gyro.z());

    if (alt > m_gyroAxisX->max())
        m_gyroAxisX->setRange(m_gyroAxisX->min(), alt * 1.1 + 10.0);
    else if (alt < m_gyroAxisX->min())
        m_gyroAxisX->setRange(alt - 10.0, m_gyroAxisX->max());

    {
        qreal maxG = qMax(qAbs(packet.gyro.x()), qMax(qAbs(packet.gyro.y()), qAbs(packet.gyro.z())));
        if (maxG > m_gyroAxisY->max())
            m_gyroAxisY->setRange(-maxG * 1.2, maxG * 1.2);
    }
}

void MainWindow::onErrorOccurred(const QString &error)
{
    QMessageBox::critical(this, "Serial Error", error);
    onPortClosed();
}
