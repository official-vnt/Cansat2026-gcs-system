#include "MainWindow.h"
#include "CommandDef.h"
#include "MapWidget.h"
#include "ui_MainWindow.h"

#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QQuickItem>
#include <QQuickWidget>
#include <QSerialPortInfo>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>
#include <QValueAxis>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_worker(new SerialWorker()),
      m_parser(new DataParser(this)), m_logger(new Logger(this)),
      m_packetCount(0) {
  ui->setupUi(this);

  // ── Global QSS — Aerospace Professional Light Theme ─────────────────────
  // Inspired by NASA Mission Control, SpaceX GCS, ISRO dashboards:
  //   • Near-white base with cool-gray tint (not pure white)
  //   • Deep navy accent (#003580) for structural elements
  //   • Electric blue (#0057D9) for live data values
  //   • Amber-orange (#E07B00) for warnings / mission ID
  //   • Clean Segoe UI throughout
  this->setStyleSheet(
      "QMainWindow {"
      "  background-color: #EEF1F5;"
      "}"
      "QWidget#centralwidget {"
      "  background-color: #EEF1F5;"
      "}"
      // Section panels — white cards with a left navy accent stripe
      "QGroupBox {"
      "  font-family: 'Segoe UI', sans-serif;"
      "  font-size: 10px;"
      "  font-weight: 800;"
      "  text-transform: uppercase;"
      "  letter-spacing: 1.5px;"
      "  color: #003580;"
      "  border: 1px solid #C8D0DC;"
      "  border-left: 3px solid #003580;"
      "  border-radius: 6px;"
      "  margin-top: 16px;"
      "  padding: 10px 8px 8px 8px;"
      "  background-color: #FFFFFF;"
      "}"
      "QGroupBox::title {"
      "  subcontrol-origin: margin;"
      "  subcontrol-position: top left;"
      "  left: 12px;"
      "  padding: 0 6px;"
      "  color: #003580;"
      "  background: #FFFFFF;"
      "}"
      "QLabel {"
      "  color: #1A2238;"
      "  font-family: 'Segoe UI', sans-serif;"
      "  font-size: 11px;"
      "}"
      // Combo boxes
      "QComboBox {"
      "  background-color: #FFFFFF;"
      "  border: 1px solid #B0BBCC;"
      "  border-radius: 4px;"
      "  padding: 4px 10px;"
      "  color: #1A2238;"
      "  font-family: 'Segoe UI', sans-serif;"
      "  font-size: 11px;"
      "  min-width: 100px;"
      "}"
      "QComboBox:hover {"
      "  border-color: #0057D9;"
      "}"
      "QComboBox::drop-down {"
      "  border: none;"
      "  width: 18px;"
      "}"
      "QComboBox QAbstractItemView {"
      "  background-color: #FFFFFF;"
      "  border: 1px solid #B0BBCC;"
      "  color: #1A2238;"
      "  selection-background-color: #E8F0FE;"
      "  selection-color: #003580;"
      "}"
      // Default buttons
      "QPushButton {"
      "  background-color: #FFFFFF;"
      "  border: 1px solid #B0BBCC;"
      "  border-radius: 4px;"
      "  padding: 5px 14px;"
      "  color: #1A2238;"
      "  font-weight: 600;"
      "  font-family: 'Segoe UI', sans-serif;"
      "  text-transform: uppercase;"
      "  font-size: 10px;"
      "  letter-spacing: 0.5px;"
      "}"
      "QPushButton:hover {"
      "  background-color: #E8F0FE;"
      "  border-color: #0057D9;"
      "  color: #003580;"
      "}"
      "QPushButton:pressed {"
      "  background-color: #D0E2FF;"
      "}"
      "QPushButton:disabled {"
      "  background-color: #F3F5F7;"
      "  border-color: #D8DDE5;"
      "  color: #9AA3B0;"
      "}"
      // CONNECT — green
      "QPushButton#connectButton {"
      "  background-color: #EAF6ED;"
      "  border: 1px solid #2DA44E;"
      "  color: #1A6B34;"
      "  font-weight: 700;"
      "}"
      "QPushButton#connectButton:hover {"
      "  background-color: #D4EDDA;"
      "  border-color: #1A6B34;"
      "}"
      // DISCONNECT — red
      "QPushButton#disconnectButton {"
      "  background-color: #FEF0EE;"
      "  border: 1px solid #CF222E;"
      "  color: #9E1B28;"
      "  font-weight: 700;"
      "}"
      "QPushButton#disconnectButton:hover {"
      "  background-color: #FADDDA;"
      "  border-color: #9E1B28;"
      "}"
      // Status bar
      "QStatusBar {"
      "  background-color: #1A2238;"
      "  color: #8A9BB5;"
      "  font-family: 'Consolas', monospace;"
      "  font-size: 10px;"
      "  border-top: 2px solid #003580;"
      "}"
      // Scroll areas
      "QScrollArea {"
      "  background: transparent;"
      "  border: none;"
      "}"
      "QScrollArea > QWidget > QWidget {"
      "  background: transparent;"
      "}"
      "QScrollBar:vertical {"
      "  background: #EEF1F5;"
      "  width: 5px;"
      "  border-radius: 3px;"
      "}"
      "QScrollBar::handle:vertical {"
      "  background: #B0BBCC;"
      "  border-radius: 3px;"
      "  min-height: 20px;"
      "}"
      "QScrollBar::handle:vertical:hover {"
      "  background: #0057D9;"
      "}"
      "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
      "  height: 0px;"
      "}"
      // Chart views — same card style
      "QChartView {"
      "  border: 1px solid #C8D0DC;"
      "  border-left: 3px solid #003580;"
      "  border-radius: 6px;"
      "  background: #FFFFFF;"
      "}"
      "QChartView:hover {"
      "  border-color: #0057D9;"
      "  border-left-color: #0057D9;"
      "}"
      "QGraphicsView {"
      "  background: transparent;"
      "}");

  this->setWindowTitle("NAKSHATRA GCS - CanSat Ground Control Station");

  // Populate COM ports
  const auto infos = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo &info : infos) {
    ui->portComboBox->addItem(info.portName());
  }

  // Baud rates
  ui->baudComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
  ui->baudComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
  ui->baudComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
  ui->baudComboBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
  ui->baudComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
  ui->baudComboBox->setCurrentIndex(4);

  ui->statusLabel->setText(
      "<span style='color:#cf222e; font-weight:bold;'>○ Disconnected</span>");

  setupTopRibbon();
  setupDashboardCards();
  setupLeftPanel();
  setupUiGraphs();
  setupCommandCenter();
  setupRecoveryPanel();
  setupLoggingPanel();
  setupStatePanel();
  setupSystemStatus();
  setupConnections();

  ui->statusbar->setVisible(false);

  m_worker->moveToThread(&m_serialThread);
  m_serialThread.start();
}

MainWindow::~MainWindow() {
  m_serialThread.quit();
  m_serialThread.wait();
  delete m_worker;
  delete ui;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Dashboard Cards (Telemetry readout panel)
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupDashboardCards() {
  if (ui->groupBox->layout()) {
    QLayout *oldLayout = ui->groupBox->layout();
    QLayoutItem *item;
    while ((item = oldLayout->takeAt(0)) != nullptr) {
      if (item->widget())
        delete item->widget();
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
    layout->setContentsMargins(8, 6, 8, 6);
    layout->setSpacing(2);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(
        "font-size: 9px; font-weight: 800; color: #57606a;"
        " letter-spacing: 0.5px;");

    valLabel = new QLabel(initVal, card);
    valLabel->setStyleSheet(
        "font-family: 'Consolas', 'Courier New', monospace;"
        " font-size: 13px; font-weight: bold; color: #0969da;");
    // Prevent the label from stretching the card horizontally
    valLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    valLabel->setMinimumWidth(0);

    layout->addWidget(titleLabel);
    layout->addWidget(valLabel);

    card->setStyleSheet("QFrame#telemetryCard {"
                        "  background-color: #ffffff;"
                        "  border: 1px solid #d0d7de;"
                        "  border-radius: 6px;"
                        "}"
                        "QFrame#telemetryCard:hover {"
                        "  border-color: #0969da;"
                        "  background-color: #ffffff;"
                        "}");
    return card;
  };

  QWidget *cardTeamId = createCard("Team ID", m_lblTeamId, "-", ui->groupBox);
  QWidget *cardPacket =
      createCard("Packet ID", ui->lblPacketId, "0", ui->groupBox);
  QWidget *cardUptime =
      createCard("System Time", ui->lblTimestamp, "0 ms", ui->groupBox);
  QWidget *cardState = createCard("State", m_lblState, "IDLE", ui->groupBox);
  QWidget *cardAlt =
      createCard("Altitude", m_lblAltitudeVal, "0.0 m", ui->groupBox);
  QWidget *cardGpsAlt =
      createCard("GPS Alt", m_lblGpsAlt, "0.0 m", ui->groupBox);
  QWidget *cardLat = createCard("Latitude", m_lblLat, "0.0", ui->groupBox);
  QWidget *cardLon = createCard("Longitude", m_lblLon, "0.0", ui->groupBox);
  QWidget *cardTemp =
      createCard("Temperature", m_lblTemperatureVal, "0.0 °C", ui->groupBox);
  QWidget *cardPres =
      createCard("Baro Pressure", m_lblPressureVal, "101325 Pa", ui->groupBox);
  QWidget *cardVolt =
      createCard("Voltage", m_lblVoltage, "0.0 V", ui->groupBox);
  QWidget *cardSats =
      createCard("Satellites", m_lblSatellites, "0", ui->groupBox);

  m_lblTemperatureVal->setStyleSheet(
      "font-family: 'Consolas', 'Courier New', monospace;"
      " font-size: 15px; font-weight: bold; color: #1a7f37;");
  m_lblState->setStyleSheet(
      "font-family: 'Consolas', 'Courier New', monospace;"
      " font-size: 15px; font-weight: bold; color: #cf222e;");

  // ── Accelerometer card ──────────────────────────────────────────────────
  QFrame *accelCard = new QFrame(ui->groupBox);
  accelCard->setObjectName("telemetryCard");
  accelCard->setStyleSheet("QFrame#telemetryCard {"
                           "  background-color: #ffffff; border: 1px solid "
                           "#d0d7de; border-radius: 6px;"
                           "}"
                           "QFrame#telemetryCard:hover {"
                           "  border-color: #0969da; background-color: #ffffff;"
                           "}");

  QVBoxLayout *accelLayout = new QVBoxLayout(accelCard);
  accelLayout->setContentsMargins(10, 8, 10, 8);
  accelLayout->setSpacing(3);

  QLabel *accelTitle = new QLabel("ACCELEROMETER (g)", accelCard);
  accelTitle->setStyleSheet(
      "font-size: 9px; font-weight: 800; color: #57606a;"
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
                " font-weight: bold; color: %1;")
            .arg(color));

    label = new QLabel("0.00", container);
    label->setStyleSheet("font-family: 'Consolas', monospace; font-size: 11px;"
                         " font-weight: bold; color: #24292f;");

    layout->addWidget(axisTag);
    layout->addWidget(label);
    return container;
  };

  accelValsLayout->addWidget(
      createAxisLabel("X", m_lblAccelXVal, "#cf222e", accelCard));
  accelValsLayout->addWidget(
      createAxisLabel("Y", m_lblAccelYVal, "#7ee787", accelCard));
  accelValsLayout->addWidget(
      createAxisLabel("Z", m_lblAccelZVal, "#8250df", accelCard));
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
  gyroValsLayout->addWidget(
      createAxisLabel("X", m_lblGyroXVal, "#9a6700", gyroCard));
  gyroValsLayout->addWidget(
      createAxisLabel("Y", m_lblGyroYVal, "#79c0ff", gyroCard));
  gyroValsLayout->addWidget(
      createAxisLabel("Z", m_lblGyroZVal, "#f778ba", gyroCard));
  gyroLayout->addLayout(gyroValsLayout);

  // ── Place cards in grid ─────────────────────────────────────────────────
  int row = 0;
  gridLayout->addWidget(cardTeamId, row, 0);
  gridLayout->addWidget(cardPacket, row, 1);
  row++;
  gridLayout->addWidget(cardUptime, row, 0);
  gridLayout->addWidget(cardState, row, 1);
  row++;
  gridLayout->addWidget(cardAlt, row, 0);
  gridLayout->addWidget(cardGpsAlt, row, 1);
  row++;
  gridLayout->addWidget(cardLat, row, 0);
  gridLayout->addWidget(cardLon, row, 1);
  row++;
  gridLayout->addWidget(cardTemp, row, 0);
  gridLayout->addWidget(cardPres, row, 1);
  row++;
  gridLayout->addWidget(cardVolt, row, 0);
  gridLayout->addWidget(cardSats, row, 1);
  row++;
  gridLayout->addWidget(accelCard, row, 0);
  gridLayout->addWidget(gyroCard, row, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Left Panel Setup
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupLeftPanel() {
  // Utility for creating sections
  auto createLeftCard = [](const QString &title, QWidget *parent) {
    QGroupBox *group = new QGroupBox(title, parent);
    group->setStyleSheet("QGroupBox {"
                         "  font-weight: bold;"
                         "  font-size: 14px;"
                         "  border: 1px solid #afb8c1;"
                         "  border-radius: 6px;"
                         "  margin-top: 10px;"
                         "  padding: 10px;"
                         "  background-color: #ffffff;"
                         "}"
                         "QGroupBox::title {"
                         "  subcontrol-origin: margin;"
                         "  left: 10px;"
                         "  color: #0969da;"
                         "}"
                         "QLabel { font-size: 13px; color: #24292f; }");
    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(4);
    return qMakePair(group, layout);
  };

  // 1. 3D Orientation Card (Top)
  auto orientPair = createLeftCard("3D ORIENTATION", ui->leftGroupBox);

  QHBoxLayout *orientLayout = new QHBoxLayout();

  // QtQuick3D viewer
  m_orientation3DWidget = new QQuickWidget(orientPair.first);
  m_orientation3DWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
  m_orientation3DWidget->setSource(
      QUrl::fromLocalFile("d:/NAKSHATRA/inspace_cansat_2026/gcs/app.gcs.system/"
                          "src/gui/Orientation3D.qml"));
  m_orientation3DWidget->setMinimumHeight(150);

  orientLayout->addWidget(m_orientation3DWidget, 2);

  QVBoxLayout *orientTextLayout = new QVBoxLayout();

  auto createValLabel = [](const QString &text, QLabel *&valLabel,
                           QWidget *parent) {
    QLabel *nameLbl = new QLabel(text, parent);
    nameLbl->setStyleSheet("color: #57606a; font-size: 10px; font-weight: "
                           "bold; background: transparent;");
    valLabel = new QLabel("-", parent);
    valLabel->setStyleSheet("color: #0969da; font-weight: bold; font-size: "
                            "12px; background: transparent;");
    QVBoxLayout *v = new QVBoxLayout();
    v->setSpacing(0);
    v->addWidget(nameLbl);
    v->addWidget(valLabel);
    return v;
  };

  orientTextLayout->addLayout(
      createValLabel("Roll (°)", m_lblOrientRoll, orientPair.first));
  orientTextLayout->addLayout(
      createValLabel("Pitch (°)", m_lblOrientPitch, orientPair.first));
  orientTextLayout->addLayout(
      createValLabel("Yaw Rate (°/s)", m_lblOrientYawRate, orientPair.first));
  orientTextLayout->addStretch();

  orientLayout->addLayout(orientTextLayout, 1);
  orientPair.second->addLayout(orientLayout);
  ui->leftPanelLayout->addWidget(orientPair.first, 1);

  // 2. GPS Card + Map (Middle)
  auto gpsPair = createLeftCard("GPS LOCATION", ui->leftGroupBox);

  m_mapWidget = new MapWidget(gpsPair.first);
  m_mapWidget->setMinimumHeight(150);

  // Overlay for GPS data
  QWidget *gpsOverlay = new QWidget(m_mapWidget);
  gpsOverlay->setStyleSheet(
      "background-color: rgba(255, 255, 255, 200); border-radius: 4px;");
  QHBoxLayout *gpsOverlayLayout = new QHBoxLayout(gpsOverlay);
  gpsOverlayLayout->setContentsMargins(5, 5, 5, 5);

  auto createGpsLabel = [](const QString &text, QLabel *&valLabel,
                           QWidget *parent) {
    QLabel *nameLbl = new QLabel(text, parent);
    nameLbl->setStyleSheet("color: #57606a; font-size: 9px; font-weight: bold; "
                           "background: transparent; border: none;");
    valLabel = new QLabel("-", parent);
    valLabel->setStyleSheet("color: #0969da; font-weight: bold; font-size: "
                            "11px; background: transparent; border: none;");
    QVBoxLayout *v = new QVBoxLayout();
    v->setAlignment(Qt::AlignTop);
    v->setSpacing(2);
    v->addWidget(nameLbl);
    v->addWidget(valLabel);
    return v;
  };

  gpsOverlayLayout->addLayout(
      createGpsLabel("Lat", m_lblLeftGpsLat, gpsOverlay));
  gpsOverlayLayout->addLayout(
      createGpsLabel("Lon", m_lblLeftGpsLon, gpsOverlay));
  gpsOverlayLayout->addLayout(
      createGpsLabel("Alt", m_lblLeftGpsAlt, gpsOverlay));
  gpsOverlayLayout->addLayout(
      createGpsLabel("Sat", m_lblLeftGpsSats, gpsOverlay));

  // Create a layout in MapWidget to position the overlay at the top
  QVBoxLayout *mapLayout = new QVBoxLayout(m_mapWidget);
  mapLayout->setContentsMargins(5, 5, 5, 5);
  mapLayout->addWidget(gpsOverlay, 0, Qt::AlignTop);

  gpsPair.second->addWidget(m_mapWidget, 1);
  ui->leftPanelLayout->addWidget(gpsPair.first, 1);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Style helper — applies common GitHub Dark styling to any QChart
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::styleChart(QChart *chart, const QString &title) {
  QFont titleFont("Segoe UI", 9, QFont::Bold);
  QColor textColor(36, 41, 47);       // dark text
  QColor bgColor(255, 255, 255);      // pure white — matches container

  chart->setTitle(title);
  chart->setTitleFont(titleFont);
  chart->setTitleBrush(QBrush(textColor));
  chart->setBackgroundBrush(QBrush(bgColor));
  chart->setBackgroundRoundness(0);          // no rounding — border comes from QGroupBox
  chart->setBackgroundVisible(true);
  chart->setPlotAreaBackgroundBrush(QBrush(QColor(250, 251, 252)));
  chart->setPlotAreaBackgroundVisible(true);
  chart->setMargins(QMargins(6, 4, 6, 4));
}

// ─────────────────────────────────────────────────────────────────────────────
//  Axis helper — applies common dark-theme styling to a QValueAxis
// ─────────────────────────────────────────────────────────────────────────────
static void styleAxis(QValueAxis *axis, const QString &title, qreal min,
                      qreal max) {
  QColor textColor(36, 41, 47);                    // dark — readable on white
  QColor titleColor(87, 96, 106);                  // slightly muted for titles
  QPen gridPen(QColor(208, 215, 222, 160));
  gridPen.setWidthF(0.8);
  QPen axisPen(QColor(175, 184, 193));
  axisPen.setWidthF(1.0);

  axis->setTitleText(title);
  axis->setTitleBrush(QBrush(titleColor));
  axis->setTitleFont(QFont("Segoe UI", 7, QFont::DemiBold));
  axis->setRange(min, max);
  axis->setGridLinePen(gridPen);
  axis->setLabelsColor(textColor);
  axis->setLabelsFont(QFont("Consolas", 8));
  axis->setLinePen(axisPen);
  axis->setTickCount(5);
}

// ─────────────────────────────────────────────────────────────────────────────
//  setupUiGraphs  — builds all 5 charts and wires them to their chart views
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupUiGraphs() {
  // ── Color palette ────────────────────────────────────────────────────────
  // Chart 1 — Altitude vs Time
  const QColor colAlt("#0969da"); // Cyan-Blue

  // Chart 2 — Altitude vs Temp & Pressure
  const QColor colTemp("#1a7f37"); // Green
  const QColor colPres("#9a6700"); // Amber/Orange

  // Chart 3 — Altitude vs Accel X/Y/Z
  const QColor colAccelX("#cf222e"); // Red
  const QColor colAccelY("#7ee787"); // Lime
  const QColor colAccelZ("#8250df"); // Purple

  // Chart 4 — Altitude vs Gyro X/Y/Z
  const QColor colGyroX("#9a6700"); // Amber
  const QColor colGyroY("#79c0ff"); // Sky-Blue
  const QColor colGyroZ("#f778ba"); // Pink

  // Chart 5 — Voltage vs Time
  const QColor colVolt("#ffa657"); // Gold/Orange

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
    leg->setLabelColor(QColor(36, 41, 47));   // dark — visible on white background
    leg->setFont(QFont("Segoe UI", 8));
  };

  // ════════════════════════════════════════════════════════════════════════
  //  CHART 1 — Altitude & Voltage vs Time
  // ════════════════════════════════════════════════════════════════════════
  m_altVoltTimeChart = new QChart();
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
    marker->setLabelBrush(QBrush(QColor(36, 41, 47)));
  }

  ui->altVoltTimeChartView->setChart(m_altVoltTimeChart);
  ui->altVoltTimeChartView->setRenderHint(QPainter::Antialiasing);
  ui->altVoltTimeChartView->setFrameStyle(QFrame::NoFrame);
  ui->altVoltTimeChartView->setBackgroundBrush(Qt::white);

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

  m_altTempPresChart->addAxis(m_atpAxisX, Qt::AlignBottom);
  m_altTempPresChart->addAxis(m_atpAxisYTemp, Qt::AlignLeft);
  m_altTempPresChart->addAxis(m_atpAxisYPres, Qt::AlignRight);

  m_altTempSeries->attachAxis(m_atpAxisX);
  m_altTempSeries->attachAxis(m_atpAxisYTemp);

  m_altPresSeries->attachAxis(m_atpAxisX);
  m_altPresSeries->attachAxis(m_atpAxisYPres);

  styleLegend(m_altTempPresChart);

  // Force legend marker colors to be dark — readable on white
  for (QLegendMarker *marker : m_altTempPresChart->legend()->markers()) {
    marker->setFont(QFont("Segoe UI", 8));
    marker->setLabelBrush(QBrush(QColor(36, 41, 47)));
  }

  ui->altTempPresChartView->setChart(m_altTempPresChart);
  ui->altTempPresChartView->setRenderHint(QPainter::Antialiasing);
  ui->altTempPresChartView->setFrameStyle(QFrame::NoFrame);
  ui->altTempPresChartView->setBackgroundBrush(Qt::white);

  // ════════════════════════════════════════════════════════════════════════
  //  CHART 3 — Altitude (X) vs Accelerometer X / Y / Z
  // ════════════════════════════════════════════════════════════════════════
  m_altAccelChart = new QChart();
  styleChart(m_altAccelChart, "Altitude vs Accelerometer");

  m_altAccelX = new QLineSeries();
  m_altAccelX->setName("Accel X   ●");
  m_altAccelX->setPen(makePen(colAccelX));
  m_altAccelY = new QLineSeries();
  m_altAccelY->setName("Accel Y   ●");
  m_altAccelY->setPen(makePen(colAccelY));
  m_altAccelZ = new QLineSeries();
  m_altAccelZ->setName("Accel Z   ●");
  m_altAccelZ->setPen(makePen(colAccelZ));

  m_altAccelChart->addSeries(m_altAccelX);
  m_altAccelChart->addSeries(m_altAccelY);
  m_altAccelChart->addSeries(m_altAccelZ);

  m_accelAxisX = new QValueAxis();
  m_accelAxisY = new QValueAxis();
  styleAxis(m_accelAxisX, "Altitude (m)", 0, 600);
  styleAxis(m_accelAxisY, "Acceleration (g)", -2, 2);
  m_altAccelChart->addAxis(m_accelAxisX, Qt::AlignBottom);
  m_altAccelChart->addAxis(m_accelAxisY, Qt::AlignLeft);

  for (QLineSeries *s : {m_altAccelX, m_altAccelY, m_altAccelZ}) {
    s->attachAxis(m_accelAxisX);
    s->attachAxis(m_accelAxisY);
  }

  styleLegend(m_altAccelChart);
  ui->altAccelChartView->setChart(m_altAccelChart);
  ui->altAccelChartView->setRenderHint(QPainter::Antialiasing);
  ui->altAccelChartView->setFrameStyle(QFrame::NoFrame);
  ui->altAccelChartView->setBackgroundBrush(Qt::white);

  // ════════════════════════════════════════════════════════════════════════
  //  CHART 4 — Altitude (X) vs Gyroscope X / Y / Z
  // ════════════════════════════════════════════════════════════════════════
  m_altGyroChart = new QChart();
  styleChart(m_altGyroChart, "Altitude vs Gyroscope");

  m_altGyroX = new QLineSeries();
  m_altGyroX->setName("Gyro X ●");
  m_altGyroX->setPen(makePen(colGyroX));
  m_altGyroY = new QLineSeries();
  m_altGyroY->setName("Gyro Y ●");
  m_altGyroY->setPen(makePen(colGyroY));
  m_altGyroZ = new QLineSeries();
  m_altGyroZ->setName("Gyro Z ●");
  m_altGyroZ->setPen(makePen(colGyroZ));

  m_altGyroChart->addSeries(m_altGyroX);
  m_altGyroChart->addSeries(m_altGyroY);
  m_altGyroChart->addSeries(m_altGyroZ);

  m_gyroAxisX = new QValueAxis();
  m_gyroAxisY = new QValueAxis();
  styleAxis(m_gyroAxisX, "Altitude (m)", 0, 600);
  styleAxis(m_gyroAxisY, "Angular Rate (°/s)", -5, 5);
  m_altGyroChart->addAxis(m_gyroAxisX, Qt::AlignBottom);
  m_altGyroChart->addAxis(m_gyroAxisY, Qt::AlignLeft);

  for (QLineSeries *s : {m_altGyroX, m_altGyroY, m_altGyroZ}) {
    s->attachAxis(m_gyroAxisX);
    s->attachAxis(m_gyroAxisY);
  }

  styleLegend(m_altGyroChart);
  ui->altGyroChartView->setChart(m_altGyroChart);
  ui->altGyroChartView->setRenderHint(QPainter::Antialiasing);
  ui->altGyroChartView->setFrameStyle(QFrame::NoFrame);
  ui->altGyroChartView->setBackgroundBrush(Qt::white);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Connections
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupConnections() {
  connect(ui->connectButton, &QPushButton::clicked, this,
          &MainWindow::onConnectClicked);
  connect(ui->disconnectButton, &QPushButton::clicked, this,
          &MainWindow::onDisconnectClicked);

  connect(m_worker, &SerialWorker::portOpened, this, &MainWindow::onPortOpened);
  connect(m_worker, &SerialWorker::portClosed, this, &MainWindow::onPortClosed);
  connect(m_worker, &SerialWorker::errorOccurred, this,
          &MainWindow::onErrorOccurred);
  connect(m_worker, &SerialWorker::dataReady, m_parser, &DataParser::parseData);
  connect(m_parser, &DataParser::packetReceived, this,
          &MainWindow::onPacketReceived);
  connect(m_parser, &DataParser::messageReceived, this,
          &MainWindow::onMessageReceived);

  // Mission timer
  connect(&m_missionTimer, &QTimer::timeout, this,
          &MainWindow::onMissionTimerTick);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Slot Implementations
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onConnectClicked() {
  QString portName = ui->portComboBox->currentText();
  int baudRate = ui->baudComboBox->currentData().toInt();

  if (portName.isEmpty()) {
    QMessageBox::warning(this, "Error", "Select a valid COM port.");
    return;
  }

  m_logger->startLogging(
      "D:/NAKSHATRA/inspace_cansat_2026/gcs/app.gcs.system/datalogging");

  QMetaObject::invokeMethod(m_worker, "openPort", Qt::QueuedConnection,
                            Q_ARG(QString, portName), Q_ARG(int, baudRate));
}

void MainWindow::onDisconnectClicked() {
  QMetaObject::invokeMethod(m_worker, "closePort", Qt::QueuedConnection);
  m_logger->stopLogging();
}

void MainWindow::onPortOpened() {
  ui->connectButton->setEnabled(false);
  ui->disconnectButton->setEnabled(true);
  ui->portComboBox->setEnabled(false);
  ui->baudComboBox->setEnabled(false);
  ui->statusLabel->setText(
      "<span style='color:#2ea043; font-weight:bold;'>● Connected</span>");

  m_packetCount = 0;
  m_packetStored = 0;

  // Start mission timer
  m_missionSeconds = 0;
  m_missionTimer.start(1000);

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

  // Update logging panel
  if (m_lblRecordingDot)
    m_lblRecordingDot->setText("● RECORDING");
  if (m_lblPacketsStored)
    m_lblPacketsStored->setText("0");
  if (m_lblStorageRemaining)
    m_lblStorageRemaining->setText("--");

  // Reset trackers and alert
  m_lastPacketTimer.invalidate();
  m_lastImuUpdate.invalidate();
  m_lastBmpUpdate.invalidate();
  m_lastGpsUpdate.invalidate();
  m_prevValuesInitialized = false;
  m_sdLoggingConfirmed = false;
  m_firstPacketCount = -1;
  m_expectedPacketCount = 0;
  m_lostPacketsCount = 0;

  addAlert("Serial port connected", "INFO");
  updateSystemStatus();
}

void MainWindow::onPortClosed() {
  ui->connectButton->setEnabled(true);
  ui->disconnectButton->setEnabled(false);
  ui->portComboBox->setEnabled(true);
  ui->baudComboBox->setEnabled(true);
  ui->statusLabel->setText(
      "<span style='color:#cf222e; font-weight:bold;'>○ Disconnected</span>");

  // Stop mission timer
  m_missionTimer.stop();

  // Update recording indicator
  if (m_lblRecordingDot)
    m_lblRecordingDot->setText("◌ STOPPED");

  addAlert("Serial port disconnected", "WARNING");
  updateSystemStatus();
}

void MainWindow::onPacketReceived(const TelemetryPacket &packet) {
  m_packetCount++;

  m_lastPacketTimer.restart();

  if (m_firstPacketCount == -1) {
    m_firstPacketCount = packet.packetCount;
    addAlert("Telemetry stream active", "INFO");
  }

  // Track expected vs received for packet loss
  m_expectedPacketCount = packet.packetCount - m_firstPacketCount + 1;
  if (m_expectedPacketCount < 1) m_expectedPacketCount = 1;
  m_lostPacketsCount = m_expectedPacketCount - m_packetCount;
  if (m_lostPacketsCount < 0) m_lostPacketsCount = 0;

  // Track sensor updates
  if (!m_prevValuesInitialized) {
    m_prevAccel = packet.accel;
    m_prevGyro = packet.gyro;
    m_prevPressure = packet.pressure;
    m_prevTemperature = packet.temperature;
    m_prevLatitude = packet.latitude;
    m_prevLongitude = packet.longitude;
    m_prevGpsAltitude = packet.gpsAltitude;
    m_prevValuesInitialized = true;

    m_lastImuUpdate.start();
    m_lastBmpUpdate.start();
    m_lastGpsUpdate.start();
  } else {
    if (packet.accel != m_prevAccel || packet.gyro != m_prevGyro) {
      m_lastImuUpdate.restart();
      m_prevAccel = packet.accel;
      m_prevGyro = packet.gyro;
    }
    if (packet.pressure != m_prevPressure || packet.temperature != m_prevTemperature) {
      m_lastBmpUpdate.restart();
      m_prevPressure = packet.pressure;
      m_prevTemperature = packet.temperature;
    }
    if (packet.latitude != m_prevLatitude || packet.longitude != m_prevLongitude || packet.gpsAltitude != m_prevGpsAltitude) {
      m_lastGpsUpdate.restart();
      m_prevLatitude = packet.latitude;
      m_prevLongitude = packet.longitude;
      m_prevGpsAltitude = packet.gpsAltitude;
    }
  }

  // Alerts logic
  static bool gpsFixAlerted = false;
  bool currentGpsFix = (packet.latitude != 0.0 && packet.longitude != 0.0 && packet.satellites > 3);
  if (!currentGpsFix && !gpsFixAlerted && m_packetCount > 5) {
    addAlert("GPS fix lost / insufficient satellites!", "WARNING");
    gpsFixAlerted = true;
  } else if (currentGpsFix && gpsFixAlerted) {
    addAlert("GPS fix re-established", "INFO");
    gpsFixAlerted = false;
  }

  static bool battAlerted = false;
  if (packet.voltage < 6.5f && !battAlerted && packet.voltage > 0.0f) {
    addAlert(QString("Low battery voltage: %1 V!").arg(packet.voltage, 0, 'f', 2), "ERROR");
    battAlerted = true;
  } else if (packet.voltage >= 6.5f && battAlerted) {
    battAlerted = false;
  }

  // SD card confirmation from packet state or flight
  if (packet.state.contains("SD") || packet.state == "FLIGHT") {
    if (!m_sdLoggingConfirmed) {
      m_sdLoggingConfirmed = true;
      addAlert("SD Card active (flight state)", "INFO");
    }
  }

  // ── Update telemetry readout cards ──────────────────────────────────────
  if (m_lblTeamId)
    m_lblTeamId->setText(packet.teamID);
  if (ui->lblPacketId)
    ui->lblPacketId->setText(QString::number(packet.packetCount));
  if (ui->lblTimestamp)
    ui->lblTimestamp->setText(QString::number(packet.time) + " ms");
  if (m_lblAltitudeVal)
    m_lblAltitudeVal->setText(QString::number(packet.altitude, 'f', 1) + " m");
  if (m_lblTemperatureVal)
    m_lblTemperatureVal->setText(QString::number(packet.temperature, 'f', 1) +
                                 " °C");
  if (m_lblPressureVal)
    m_lblPressureVal->setText(QString::number(packet.pressure, 'f', 0) + " Pa");
  if (m_lblVoltage)
    m_lblVoltage->setText(QString::number(packet.voltage, 'f', 2) + " V");
  if (m_lblLat)
    m_lblLat->setText(QString::number(packet.latitude, 'f', 6));
  if (m_lblLon)
    m_lblLon->setText(QString::number(packet.longitude, 'f', 6));
  if (m_lblGpsAlt)
    m_lblGpsAlt->setText(QString::number(packet.gpsAltitude, 'f', 1) + " m");
  if (m_lblSatellites)
    m_lblSatellites->setText(QString::number(packet.satellites));

  if (m_lblAccelXVal)
    m_lblAccelXVal->setText(QString::number(packet.accel.x(), 'f', 2));
  if (m_lblAccelYVal)
    m_lblAccelYVal->setText(QString::number(packet.accel.y(), 'f', 2));
  if (m_lblAccelZVal)
    m_lblAccelZVal->setText(QString::number(packet.accel.z(), 'f', 2));

  if (m_lblGyroXVal)
    m_lblGyroXVal->setText(QString::number(packet.gyro.x(), 'f', 2));
  if (m_lblGyroYVal)
    m_lblGyroYVal->setText(QString::number(packet.gyro.y(), 'f', 2));
  if (m_lblGyroZVal)
    m_lblGyroZVal->setText(QString::number(packet.gyro.z(), 'f', 2));

  // ── Orientation Math & Left Panel Updates ───────────────────────────────

  // State machine logic
  if (m_currentStateSequence == 0 && packet.altitude >= 1000.0) {
    m_currentStateSequence = 1;
  } else if (m_currentStateSequence == 1 && packet.accel.z() < 0.0) {
    m_currentStateSequence = 2;
  } else if (m_currentStateSequence == 2 && packet.altitude <= 600.0) {
    m_currentStateSequence = 3;
  } else if (m_currentStateSequence == 3 && packet.accel.z() < 0.0) {
    m_currentStateSequence = 4;
  } else if (m_currentStateSequence == 4 && qAbs(packet.accel.z()) < 0.1 &&
             qAbs(packet.accel.x()) < 0.1 && qAbs(packet.accel.y()) < 0.1) {
    m_currentStateSequence = 5;
  }

  auto updateStateStyle = [this](QLabel *lbl, int index) {
    if (!lbl)
      return;
    if (m_currentStateSequence == index) {
      lbl->setStyleSheet("font-size: 13px; font-weight: bold; color: #1a7f37; "
                         "border: 1px solid #1a7f37; border-radius: 4px; "
                         "padding: 4px; background: rgba(63,185,80,0.1);");
    } else {
      lbl->setStyleSheet(
          "font-size: 11px; font-weight: bold; color: #8c959f; padding: 2px; "
          "border: none; background: transparent;");
    }
  };

  updateStateStyle(m_lblStateLaunch, 0);
  updateStateStyle(m_lblStateDep1, 1);
  updateStateStyle(m_lblStateDescent, 2);
  updateStateStyle(m_lblStateDep2, 3);
  updateStateStyle(m_lblStateDescent2, 4);
  updateStateStyle(m_lblStateLand, 5);

  const QString stateNames[] = {"LAUNCH",       "DEPLOYMENT 1", "DESCENT",
                                "DEPLOYMENT 2", "DESCENT",      "LAND"};
  if (m_lblState)
    m_lblState->setText(stateNames[m_currentStateSequence]);

  // Calculate Roll and Pitch
  // Roll = atan2(Y, Z) * 180/PI
  // Pitch = atan2(-X, sqrt(Y*Y + Z*Z)) * 180/PI
  const double PI = 3.14159265358979323846;
  double roll = std::atan2(packet.accel.y(), packet.accel.z()) * 180.0 / PI;
  double pitch = std::atan2(-packet.accel.x(),
                            std::sqrt(packet.accel.y() * packet.accel.y() +
                                      packet.accel.z() * packet.accel.z())) *
                 180.0 / PI;

  // Simple integration of gyro Z for Yaw approximation (dt approx 1.0 for
  // visualization)
  static double staticYaw = 0.0;
  staticYaw += packet.gyro.z() * 0.1;
  if (staticYaw > 360.0)
    staticYaw -= 360.0;
  if (staticYaw < 0.0)
    staticYaw += 360.0;

  // Update 3D QuickWidget
  if (m_orientation3DWidget && m_orientation3DWidget->rootObject()) {
    m_orientation3DWidget->rootObject()->setProperty("modelRoll", roll);
    m_orientation3DWidget->rootObject()->setProperty("modelPitch", pitch);
    m_orientation3DWidget->rootObject()->setProperty("modelYaw", staticYaw);
  }

  if (m_lblOrientRoll)
    m_lblOrientRoll->setText(QString::number(roll, 'f', 1));
  if (m_lblOrientPitch)
    m_lblOrientPitch->setText(QString::number(pitch, 'f', 1));
  if (m_lblOrientYawRate)
    m_lblOrientYawRate->setText(QString::number(packet.gyro.z(), 'f', 1));

  if (m_lblLeftGpsLat)
    m_lblLeftGpsLat->setText(QString::number(packet.latitude, 'f', 6));
  if (m_lblLeftGpsLon)
    m_lblLeftGpsLon->setText(QString::number(packet.longitude, 'f', 6));
  if (m_lblLeftGpsAlt)
    m_lblLeftGpsAlt->setText(QString::number(packet.gpsAltitude, 'f', 1));
  if (m_lblLeftGpsSats)
    m_lblLeftGpsSats->setText(QString::number(packet.satellites));

  if (m_mapWidget) {
    m_mapWidget->setCoordinate(packet.latitude, packet.longitude);
  }

  // ── State indicator + recovery detection ────────────────────────────────
  if ((packet.state == "LANDED" || packet.state == "RECOVERY MODE") &&
      !m_landed) {
    m_landed = true;
    m_landingLat = packet.latitude;
    m_landingLon = packet.longitude;
    showRecoveryPanel(true);
  }
  if (m_lblRecoveryLat)
    m_lblRecoveryLat->setText(QString::number(packet.latitude, 'f', 6));
  if (m_lblRecoveryLon)
    m_lblRecoveryLon->setText(QString::number(packet.longitude, 'f', 6));
  if (m_lblLastTelemetry)
    m_lblLastTelemetry->setText(
        QDateTime::currentDateTime().toString("hh:mm:ss"));

  // ── Update logging panel counts ──────────────────────────────────────────
  m_packetStored++;
  if (m_lblPacketsStored)
    m_lblPacketsStored->setText(QString::number(m_packetStored));

  m_logger->logPacket(packet);
  updateCharts(packet);
  updateSystemStatus();
}

// ─────────────────────────────────────────────────────────────────────────────
//  updateCharts — push new data into every series + auto-scale axes
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::updateCharts(const TelemetryPacket &packet) {
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
    m_altVoltAxisYVolt->setRange(m_altVoltAxisYVolt->min(),
                                 packet.voltage + 1.0);
  else if (packet.voltage < m_altVoltAxisYVolt->min())
    m_altVoltAxisYVolt->setRange(packet.voltage - 1.0,
                                 m_altVoltAxisYVolt->max());

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
    if (t > m_atpAxisYTemp->max())
      m_atpAxisYTemp->setRange(m_atpAxisYTemp->min(), t + 5.0);
    else if (t < m_atpAxisYTemp->min())
      m_atpAxisYTemp->setRange(t - 5.0, m_atpAxisYTemp->max());
  }
  // Auto-scale pressure Y (right)
  {
    qreal p = packet.pressure;
    if (p > m_atpAxisYPres->max())
      m_atpAxisYPres->setRange(m_atpAxisYPres->min(), p + 500.0);
    else if (p < m_atpAxisYPres->min())
      m_atpAxisYPres->setRange(p - 500.0, m_atpAxisYPres->max());
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
    qreal maxA = qMax(qAbs(packet.accel.x()),
                      qMax(qAbs(packet.accel.y()), qAbs(packet.accel.z())));
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
    qreal maxG = qMax(qAbs(packet.gyro.x()),
                      qMax(qAbs(packet.gyro.y()), qAbs(packet.gyro.z())));
    if (maxG > m_gyroAxisY->max())
      m_gyroAxisY->setRange(-maxG * 1.2, maxG * 1.2);
  }
}

void MainWindow::onErrorOccurred(const QString &error) {
  QMessageBox::critical(this, "Serial Error", error);
  onPortClosed();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Top Ribbon — style the pre-existing UI widgets from the .ui file
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupTopRibbon() {
  // ── Dark aerospace mission-control header bar ──────────────────────────
  const QString ribbonStyle =
      // The ribbon bar itself: clean white
      "QWidget#topRibbon {"
      "  background: #FFFFFF;"
      "  border-bottom: 2px solid #003580;"
      "}"
      // Mission badge
      "QLabel#lblTeamIdBadge {"
      "  color: #D35400;"
      "  font-family: 'Consolas', monospace;"
      "  font-size: 12px;"
      "  font-weight: bold;"
      "  letter-spacing: 1.5px;"
      "  padding: 0 10px;"
      "  border-right: 1px solid #C8D0DC;"
      "}"
      // All caption labels inside the ribbon
      "QLabel#lblTimerCaption, QLabel#lblLinkCaption, "
      "QLabel#lblRssiCaption, QLabel#lblSnrCaption {"
      "  color: #57606a;"
      "  font-size: 8px;"
      "  font-weight: bold;"
      "  text-transform: uppercase;"
      "  letter-spacing: 1.5px;"
      "}"
      // Mission elapsed timer — large digital readout
      "QLabel#lblMissionTimer {"
      "  color: #003580;"
      "  font-family: 'Consolas', monospace;"
      "  font-size: 18px;"
      "  font-weight: bold;"
      "  letter-spacing: 3px;"
      "  min-width: 100px;"
      "}"
      // RSSI / SNR telemetry values
      "QLabel#lblRssiVal, QLabel#lblSnrVal {"
      "  color: #1A6B34;"
      "  font-family: 'Consolas', monospace;"
      "  font-size: 12px;"
      "  font-weight: bold;"
      "  min-width: 55px;"
      "}"
      // Generic labels inside ribbon get dark text
      "QLabel {"
      "  color: #1A2238;"
      "  font-family: 'Segoe UI', sans-serif;"
      "}"
      // Vertical separators
      "QFrame[frameShape='5'] {"
      "  color: #C8D0DC;"
      "  max-width: 1px;"
      "}"
      // Link quality progress bar
      "QProgressBar#linkQualityBar {"
      "  background: #EEF1F5;"
      "  border: 1px solid #C8D0DC;"
      "  border-radius: 3px;"
      "  height: 6px;"
      "}"
      "QProgressBar#linkQualityBar::chunk {"
      "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
      "    stop:0 #2DA44E, stop:1 #4ADE80);"
      "  border-radius: 3px;"
      "}"
      // Status label
      "QLabel#statusLabel {"
      "  color: #57606a;"
      "  font-family: 'Consolas', monospace;"
      "  font-size: 11px;"
      "}";

  ui->topRibbon->setStyleSheet(ribbonStyle);

  // Cache pointer to the mission timer label
  m_lblMissionTimer = ui->lblMissionTimer;
  m_linkQualityBar = ui->linkQualityBar;
  m_lblRssi = ui->lblRssiVal;
  m_lblSnr = ui->lblSnrVal;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Command Center — 3×3 compact grid inside commandGroupBox
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupCommandCenter() {
  QGridLayout *grid = ui->commandPanelLayout;
  grid->setSpacing(4);

  // Aerospace-style: solid filled buttons with strong color identity
  const QString btnBase =
      "QPushButton {"
      "  border-radius: 3px;"
      "  padding: 6px 4px;"
      "  font-size: 9px;"
      "  font-weight: 700;"
      "  font-family: 'Segoe UI';"
      "  letter-spacing: 0.5px;"
      "  text-align: center;"
      "  text-transform: uppercase;"
      "}"
      "QPushButton:hover { opacity: 0.9; }"
      "QPushButton:pressed { padding-top: 7px; padding-bottom: 5px; }"
      "QPushButton:disabled {"
      "  color: #9AA3B0;"
      "  border: 1px solid #D8DDE5;"
      "  background: #F3F5F7;"
      "}";

  const auto &cmds = CommandRegistry::commands();
  int col = 0, row = 0;

  for (const CommandDefinition &d : cmds) {
    QPushButton *btn = new QPushButton(d.label, ui->commandGroupBox);
    btn->setEnabled(d.enabled);
    btn->setStyleSheet(btnBase);

    m_cmdButtons.append(btn);
    grid->addWidget(btn, row, col);

    connect(btn, &QPushButton::clicked, this, [this, d]() {
      if (d.needsConfirm) {
        if (!confirmAction(d.label.trimmed()))
          return;
      }
      QByteArray cmdBytes = (d.serialCmd + "\n").toUtf8();
      QMetaObject::invokeMethod(m_worker, "sendData", Qt::QueuedConnection,
                                Q_ARG(QByteArray, cmdBytes));
      QMessageBox::information(this, "Command Sent",
                               QString("Sent: %1").arg(d.serialCmd));
    });

    if (++col == 3) {
      col = 0;
      ++row;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Recovery Panel — built into recoveryGroupBox, hidden until landing
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupRecoveryPanel() {
  QVBoxLayout *layout = ui->recoveryPanelLayout;

  const QString cardStyle = "QLabel { color: #24292f; font-size: 12px; }"
                            "QLabel[class='val'] { color: #0969da; "
                            "font-family: 'Consolas'; font-weight: bold; }";

  auto makeRow = [&](const QString &label, QLabel *&val) {
    QHBoxLayout *row = new QHBoxLayout();
    QLabel *lbl = new QLabel(label, ui->recoveryGroupBox);
    lbl->setStyleSheet("color:#57606a; font-size:11px;");
    val = new QLabel("--", ui->recoveryGroupBox);
    val->setStyleSheet("color:#0969da; font-family:'Consolas'; "
                       "font-weight:bold; font-size:12px;");
    row->addWidget(lbl);
    row->addStretch();
    row->addWidget(val);
    layout->addLayout(row);
  };

  // Beacon status — big label at top
  QLabel *beaconHeader = new QLabel("BEACON STATUS", ui->recoveryGroupBox);
  beaconHeader->setStyleSheet(
      "color:#57606a; font-size:9px; font-weight:bold; letter-spacing:1px;");
  layout->addWidget(beaconHeader);

  m_lblBeaconStatus = new QLabel("INACTIVE", ui->recoveryGroupBox);
  m_lblBeaconStatus->setStyleSheet("color:#cf222e; font-size:16px; "
                                   "font-weight:bold; font-family:'Consolas';");
  m_lblBeaconStatus->setAlignment(Qt::AlignCenter);
  layout->addWidget(m_lblBeaconStatus);

  makeRow("Landing Lat:", m_lblRecoveryLat);
  makeRow("Landing Lon:", m_lblRecoveryLon);
  makeRow("Distance:", m_lblRecoveryDistance);
  makeRow("Direction:", m_lblRecoveryBearing);
  makeRow("Last Telemetry:", m_lblLastTelemetry);

  // Start hidden — revealed on landing
  ui->recoveryGroupBox->setVisible(false);
  Q_UNUSED(cardStyle);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Data Logging Panel — built into loggingGroupBox
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupLoggingPanel() {
  QVBoxLayout *layout = ui->loggingPanelLayout;

  // Recording status indicator
  m_lblRecordingDot = new QLabel("◌  IDLE", ui->loggingGroupBox);
  m_lblRecordingDot->setStyleSheet(
      "color: #9E1B28;"
      " font-family: 'Consolas', monospace;"
      " font-weight: bold;"
      " font-size: 11px;"
      " letter-spacing: 1.5px;"
      " padding: 3px 6px;"
      " background: #FFFFFF;"
      " border: 1px solid #CF222E;"
      " border-radius: 3px;");
  layout->addWidget(m_lblRecordingDot);

  auto makeRow = [&](const QString &label, QLabel *&val, const QString &init) {
    QHBoxLayout *row = new QHBoxLayout();
    QLabel *lbl = new QLabel(label, ui->loggingGroupBox);
    lbl->setStyleSheet("color: #5A7399; font-size: 10px; font-weight: 600;");
    val = new QLabel(init, ui->loggingGroupBox);
    val->setStyleSheet(
        "color: #003580;"
        " font-family: 'Consolas', monospace;"
        " font-size: 10px;"
        " font-weight: bold;");
    val->setWordWrap(true);
    row->addWidget(lbl);
    row->addStretch();
    row->addWidget(val);
    layout->addLayout(row);
  };

  makeRow("Log File:", m_lblLogFile, "mission_01.csv");
  makeRow("Packets Stored:", m_lblPacketsStored, "0");
  makeRow("Storage Remaining:", m_lblStorageRemaining, "--");

  // Export buttons — aerospace blue and green
  QPushButton *btnCsv = new QPushButton("Export CSV", ui->loggingGroupBox);
  btnCsv->setStyleSheet(
      "QPushButton {"
      "  color: #003580;"
      "  background: #FFFFFF;"
      "  border: 1px solid #0057D9;"
      "  border-radius: 3px;"
      "  padding: 5px;"
      "  font-size: 9px;"
      "  font-weight: 700;"
      "  letter-spacing: 0.5px;"
      "}"
      "QPushButton:hover { background: #E8F0FE; }");
  connect(btnCsv, &QPushButton::clicked, this, &MainWindow::onExportCsv);
  layout->addWidget(btnCsv);

  QPushButton *btnReport =
      new QPushButton("Export Report", ui->loggingGroupBox);
  btnReport->setStyleSheet(
      "QPushButton {"
      "  color: #1A6B34;"
      "  background: #FFFFFF;"
      "  border: 1px solid #2DA44E;"
      "  border-radius: 3px;"
      "  padding: 5px;"
      "  font-size: 9px;"
      "  font-weight: 700;"
      "  letter-spacing: 0.5px;"
      "}"
      "QPushButton:hover { background: #EAF6ED; }");
  connect(btnReport, &QPushButton::clicked, this, &MainWindow::onExportReport);
  layout->addWidget(btnReport);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Mission Timer Tick
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onMissionTimerTick() {
  m_missionSeconds++;
  int h = m_missionSeconds / 3600;
  int m = (m_missionSeconds % 3600) / 60;
  int s = m_missionSeconds % 60;
  if (m_lblMissionTimer)
    m_lblMissionTimer->setText(QString("%1:%2:%3")
                                   .arg(h, 2, 10, QChar('0'))
                                   .arg(m, 2, 10, QChar('0'))
                                   .arg(s, 2, 10, QChar('0')));
  updateSystemStatus();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Recovery Panel visibility toggle
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::showRecoveryPanel(bool visible) {
  ui->recoveryGroupBox->setVisible(visible);
  if (visible && m_lblBeaconStatus)
    m_lblBeaconStatus->setText("ACTIVE");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Confirmation Dialog helper
// ─────────────────────────────────────────────────────────────────────────────
bool MainWindow::confirmAction(const QString &action) {
  QMessageBox dlg(this);
  dlg.setWindowTitle("Confirm Command");
  dlg.setText(QString("<b>Are you sure you want to: %1?</b>").arg(action));
  dlg.setInformativeText("This action will be sent to the CanSat immediately.");
  dlg.setIcon(QMessageBox::Warning);
  dlg.setStyleSheet(
      "QMessageBox { background:#f6f8fa; color:#24292f; }"
      "QLabel { color:#24292f; }"
      "QPushButton { background:#d0d7de; border:1px solid #afb8c1;"
      " border-radius:4px; padding:6px 16px; color:#24292f; font-weight:bold; "
      "}");
  dlg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
  dlg.setDefaultButton(QMessageBox::Cancel);
  return dlg.exec() == QMessageBox::Yes;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Command Slots
// ─────────────────────────────────────────────────────────────────────────────

// ─────────────────────────────────────────────────────────────────────────────
//  Export Slots
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::onExportCsv() {
  QString currentFile = m_logger->currentLogFile();
  if (currentFile.isEmpty() || !QFile::exists(currentFile)) {
    QMessageBox::warning(this, "Export Error",
                         "No active or existing log file to export.");
    return;
  }

  QDir dir("D:/NAKSHATRA/inspace_cansat_2026/gcs/app.gcs.system/datalogging");
  if (!dir.exists()) {
    dir.mkpath(".");
  }

  QString defaultName = QFileInfo(currentFile).fileName();
  QString path = QFileDialog::getSaveFileName(this, "Export CSV",
                                              dir.absoluteFilePath(defaultName),
                                              "CSV Files (*.csv)");

  if (!path.isEmpty()) {
    if (QFile::exists(path))
      QFile::remove(path);
    if (QFile::copy(currentFile, path)) {
      QMessageBox::information(this, "Export",
                               "CSV exported successfully to:\n" + path);
    } else {
      QMessageBox::warning(this, "Export Error", "Failed to copy CSV file.");
    }
  }
}

void MainWindow::onExportReport() {
  QString path = QFileDialog::getSaveFileName(
      this, "Export Mission Report",
      QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) +
          "/mission_report.txt",
      "Text Files (*.txt)");
  if (!path.isEmpty()) {
    QFile f(path);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&f);
      out << "=== NAKSHATRA GCS Mission Report ===\n";
      out << "Generated: " << QDateTime::currentDateTime().toString(Qt::ISODate)
          << "\n";
      out << "Mission Duration: " << m_missionSeconds << " seconds\n";
      out << "Packets Received: " << m_packetCount << "\n";
      out << "Packets Stored:   " << m_packetStored << "\n";
      f.close();
    }
    QMessageBox::information(this, "Export", "Report exported to:\n" + path);
  }
}

// ─────────────────────────────────────────────────────────────────────────────
//  State Panel — horizontal states
// ─────────────────────────────────────────────────────────────────────────────
void MainWindow::setupStatePanel() {
  QVBoxLayout *layout = ui->statePanelLayout;
  if (!layout)
    return;

  auto createSeqLabel = [](const QString &text, QWidget *parent) {
    QLabel *lbl = new QLabel(text, parent);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet(
        "font-size: 11px; font-weight: bold; color: #8c959f; padding: 2px;");
    return lbl;
  };

  m_lblStateLaunch = createSeqLabel("LAUNCH", ui->stateGroupBox);
  m_lblStateDep1 = createSeqLabel("DEP 1", ui->stateGroupBox);
  m_lblStateDescent = createSeqLabel("DESCENT", ui->stateGroupBox);
  m_lblStateDep2 = createSeqLabel("DEP 2", ui->stateGroupBox);
  m_lblStateDescent2 = createSeqLabel("DESCENT", ui->stateGroupBox);
  m_lblStateLand = createSeqLabel("LAND", ui->stateGroupBox);

  QHBoxLayout *hLayout = new QHBoxLayout();
  hLayout->setSpacing(2);
  hLayout->addWidget(m_lblStateLaunch);

  // Add arrows between states
  auto addArrow = [&]() {
    QLabel *arr = new QLabel("→", ui->stateGroupBox);
    arr->setStyleSheet("color: #57606a; font-size: 10px; font-weight: bold;");
    arr->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(arr);
  };

  addArrow();
  hLayout->addWidget(m_lblStateDep1);
  addArrow();
  hLayout->addWidget(m_lblStateDescent);
  addArrow();
  hLayout->addWidget(m_lblStateDep2);
  addArrow();
  hLayout->addWidget(m_lblStateDescent2);
  addArrow();
  hLayout->addWidget(m_lblStateLand);

  layout->addLayout(hLayout);
}

void MainWindow::setupSystemStatus() {
  QVBoxLayout *layout = ui->systemStatusLayout;
  if (!layout) return;

  // Clear existing items in layout just in case
  QLayoutItem *item;
  while ((item = layout->takeAt(0)) != nullptr) {
    if (item->widget()) delete item->widget();
    delete item;
  }

  auto createStatusRow = [this, layout](const QString &compName) {
    QWidget *row = new QWidget(ui->systemStatusBox);
    QHBoxLayout *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(8);

    QLabel *dot = new QLabel("●", row);
    dot->setStyleSheet("color: #9AA3B0; font-size: 14px; font-weight: bold;");
    dot->setFixedWidth(15);

    QLabel *name = new QLabel(compName, row);
    name->setStyleSheet("font-family: 'Segoe UI'; font-size: 11px; font-weight: bold; color: #5A7399; text-transform: uppercase;");
    name->setFixedWidth(90);

    QLabel *val = new QLabel("Waiting...", row);
    val->setStyleSheet("font-family: 'Segoe UI'; font-size: 11px; color: #1A2238;");

    h->addWidget(dot);
    h->addWidget(name);
    h->addWidget(val);
    h->addStretch();

    layout->addWidget(row);

    StatusItem sItem;
    sItem.dot = dot;
    sItem.name = name;
    sItem.value = val;
    m_statusItems[compName] = sItem;
  };

  createStatusRow("Sensors");
  createStatusRow("GPS");
  createStatusRow("Telemetry");
  createStatusRow("SD Card");
  createStatusRow("Battery");
  createStatusRow("Radio Link");
}

void MainWindow::updateSystemStatus() {
  bool isConnected = ui->disconnectButton->isEnabled();

  // 1. Telemetry Status
  bool telemetryOk = false;
  if (isConnected && m_lastPacketTimer.isValid() && m_lastPacketTimer.elapsed() < 3000) {
    telemetryOk = true;
  }

  // 2. Sensors Status
  bool sensorsOk = false;
  QString sensorDetails = "Offline";
  if (isConnected && telemetryOk) {
    bool imuOk = m_lastImuUpdate.isValid() && m_lastImuUpdate.elapsed() < 5000;
    bool bmpOk = m_lastBmpUpdate.isValid() && m_lastBmpUpdate.elapsed() < 5000;
    bool gpsOk = m_lastGpsUpdate.isValid() && m_lastGpsUpdate.elapsed() < 5000;
    sensorsOk = imuOk && bmpOk && gpsOk;
    if (sensorsOk) {
      sensorDetails = "IMU, BMP, GPS active";
    } else {
      QStringList failed;
      if (!imuOk) failed.append("IMU");
      if (!bmpOk) failed.append("BMP");
      if (!gpsOk) failed.append("GPS");
      sensorDetails = failed.join(",") + " stale";
    }
  }

  // 3. GPS Status
  bool gpsOk = false;
  QString gpsDetails = "No Fix";
  if (isConnected && telemetryOk) {
    int sats = m_lblSatellites ? m_lblSatellites->text().toInt() : 0;
    double lat = m_lblLat ? m_lblLat->text().toDouble() : 0.0;
    gpsOk = (lat != 0.0 && sats > 3);
    if (gpsOk) {
      gpsDetails = QString("Fix OK (%1 Sats)").arg(sats);
    } else {
      gpsDetails = QString("No Fix (%1 Sats)").arg(sats);
    }
  }

  // 4. SD Card
  bool sdOk = m_sdLoggingConfirmed;

  // 5. Battery
  bool batteryOk = false;
  QString batteryDetails = "Offline";
  if (isConnected && telemetryOk) {
    float volt = m_lblVoltage ? m_lblVoltage->text().replace(" V", "").toFloat() : 0.0f;
    batteryOk = (volt >= 6.5f);
    batteryDetails = QString("%1 V (%2)").arg(volt, 0, 'f', 2).arg(batteryOk ? "Safe" : "LOW");
  }

  // 6. Radio Link
  bool radioOk = false;
  QString radioDetails = "Offline";
  if (isConnected) {
    int expected = m_expectedPacketCount;
    int lost = m_lostPacketsCount;
    float lossRate = 0.0f;
    if (expected > 0) {
      lossRate = (lost / (float)expected) * 100.0f;
    }
    radioOk = (m_rssiVal >= -85.0f && lossRate < 5.0f);
    radioDetails = QString("RSSI: %1 dBm (Loss: %2%)").arg(static_cast<int>(m_rssiVal)).arg(lossRate, 0, 'f', 1);
  }

  // Helper to update visual row
  auto updateRow = [this](const QString &name, bool ok, const QString &valText, bool warn = false) {
    if (!m_statusItems.contains(name)) return;
    StatusItem item = m_statusItems[name];
    if (ok) {
      item.dot->setStyleSheet("color: #2DA44E; font-size: 14px; font-weight: bold;"); // Green
      item.value->setStyleSheet("font-family: 'Segoe UI'; font-size: 11px; color: #1A2238;");
    } else if (warn) {
      item.dot->setStyleSheet("color: #E07B00; font-size: 14px; font-weight: bold;"); // Amber
      item.value->setStyleSheet("font-family: 'Segoe UI'; font-size: 11px; color: #E07B00; font-weight: bold;");
    } else {
      item.dot->setStyleSheet("color: #CF222E; font-size: 14px; font-weight: bold;"); // Red
      item.value->setStyleSheet("font-family: 'Segoe UI'; font-size: 11px; color: #CF222E; font-weight: bold;");
    }
    item.value->setText(valText);
  };

  updateRow("Sensors", sensorsOk, isConnected ? (telemetryOk ? sensorDetails : "Stale") : "Offline");
  updateRow("GPS", gpsOk, isConnected ? (telemetryOk ? gpsDetails : "No Fix") : "Offline");
  updateRow("Telemetry", telemetryOk, isConnected ? (telemetryOk ? "Active" : "No Packets (3s+)") : "Offline");
  updateRow("SD Card", sdOk, isConnected ? (sdOk ? "Confirmed" : "Not Confirmed") : "Offline");
  updateRow("Battery", batteryOk, isConnected ? (telemetryOk ? batteryDetails : "Low") : "Offline");
  updateRow("Radio Link", radioOk, isConnected ? radioDetails : "Offline");
}

void MainWindow::addAlert(const QString &text, const QString &type) {
  QVBoxLayout *layout = ui->alertsLayout;
  if (!layout) return;

  QString timeStr = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
  QLabel *lbl = new QLabel(timeStr + text, ui->alertsBox);

  if (type == "ERROR") {
    lbl->setStyleSheet("color: #CF222E; font-family: 'Consolas', monospace; font-size: 10px; font-weight: bold;");
  } else if (type == "WARNING") {
    lbl->setStyleSheet("color: #E07B00; font-family: 'Consolas', monospace; font-size: 10px; font-weight: bold;");
  } else {
    lbl->setStyleSheet("color: #24292f; font-family: 'Consolas', monospace; font-size: 10px;");
  }

  layout->addWidget(lbl);

  // Keep max 5 items
  QList<QLabel*> labels;
  for (int i = 0; i < layout->count(); ++i) {
    if (QLabel *l = qobject_cast<QLabel*>(layout->itemAt(i)->widget())) {
      labels.append(l);
    }
  }

  if (labels.size() > 5) {
    QLabel *oldest = labels.first();
    layout->removeWidget(oldest);
    delete oldest;
  }
}

void MainWindow::onMessageReceived(const QString &message) {
  QString msg = message.trimmed();
  if (msg.isEmpty()) return;

  addAlert("MSG: " + msg, "INFO");

  if (msg.contains("SD_OK") || msg.contains("SD_INITIALIZED") || msg.contains("SD_WRITE") || msg.contains("SD_LOG_OK")) {
    m_sdLoggingConfirmed = true;
    addAlert("SD Card logging confirmation received from CanSat", "INFO");
    updateSystemStatus();
  }
}
