#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_worker(new SerialWorker()) // Worker object
    , m_parser(new DataParser(this)) // Parser on main thread
    , m_logger(new Logger(this)) // Logger on main thread
    , m_packetCount(0)
{
    ui->setupUi(this);

    // Apply a professional GitHub Dark / Cyberpunk theme styling via QSS
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
    
    // Add standard baud rates
    ui->baudComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudComboBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudComboBox->setCurrentIndex(4); // Default to 115200

    // Set initial status text with HTML indicator
    ui->statusLabel->setText("<span style='color:#f85149; font-weight:bold;'>○ Disconnected</span>");

    // Setup visual components
    setupDashboardCards();
    setupUiGraphs();
    setupConnections();

    // Move serial worker to dedicated thread
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

void MainWindow::setupDashboardCards()
{
    // Clean up existing layout in ui->groupBox
    if (ui->groupBox->layout()) {
        QLayout *oldLayout = ui->groupBox->layout();
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        delete oldLayout;
    }

    // Set up grid layout inside the card panel
    QGridLayout *gridLayout = new QGridLayout(ui->groupBox);
    gridLayout->setContentsMargins(10, 15, 10, 10);
    gridLayout->setSpacing(10);

    // Lambda helper for building gorgeous telemetry cards
    auto createCard = [](const QString &title, QLabel *&valLabel, const QString &initVal, QWidget *parent) {
        QFrame *card = new QFrame(parent);
        card->setObjectName("telemetryCard");
        
        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setContentsMargins(12, 10, 12, 10);
        layout->setSpacing(4);
        
        QLabel *titleLabel = new QLabel(title, card);
        titleLabel->setStyleSheet("font-size: 10px; font-weight: 800; color: #8b949e; text-transform: uppercase; letter-spacing: 0.5px;");
        
        valLabel = new QLabel(initVal, card);
        valLabel->setStyleSheet("font-family: 'Consolas', 'Courier New', monospace; font-size: 18px; font-weight: bold; color: #58a6ff;");
        
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
            "}"
        );
        
        return card;
    };

    // Instantiate and place primary cards
    QWidget *cardTeamId = createCard("Team ID", m_lblTeamId, "-", ui->groupBox);
    QWidget *cardPacket = createCard("Packet ID", ui->lblPacketId, "0", ui->groupBox);
    QWidget *cardUptime = createCard("System Time", ui->lblTimestamp, "0 ms", ui->groupBox);
    QWidget *cardState = createCard("State", m_lblState, "IDLE", ui->groupBox);
    QWidget *cardAlt = createCard("Altitude", m_lblAltitudeVal, "0.0 m", ui->groupBox);
    QWidget *cardTemp = createCard("Temperature", m_lblTemperatureVal, "0.0 °C", ui->groupBox);
    
    // Style temperature card with custom green accent
    m_lblTemperatureVal->setStyleSheet("font-family: 'Consolas', 'Courier New', monospace; font-size: 18px; font-weight: bold; color: #3fb950;");

    QWidget *cardPres = createCard("Baro Pressure", m_lblPressureVal, "101325 Pa", ui->groupBox);
    QWidget *cardVolt = createCard("Voltage", m_lblVoltage, "0.0 V", ui->groupBox);
    
    // Custom Acceleration Card containing sub-fields
    QFrame *accelCard = new QFrame(ui->groupBox);
    accelCard->setObjectName("telemetryCard");
    accelCard->setStyleSheet(
        "QFrame#telemetryCard {"
        "  background-color: #0d1117;"
        "  border: 1px solid #21262d;"
        "  border-radius: 6px;"
        "}"
        "QFrame#telemetryCard:hover {"
        "  border-color: #58a6ff;"
        "  background-color: #161b22;"
        "}"
    );
    QVBoxLayout *accelLayout = new QVBoxLayout(accelCard);
    accelLayout->setContentsMargins(12, 10, 12, 10);
    accelLayout->setSpacing(4);
    
    QLabel *accelTitle = new QLabel("ACCELEROMETER", accelCard);
    accelTitle->setStyleSheet("font-size: 10px; font-weight: 800; color: #8b949e; text-transform: uppercase; letter-spacing: 0.5px;");
    accelLayout->addWidget(accelTitle);
    
    QHBoxLayout *accelValsLayout = new QHBoxLayout();
    accelValsLayout->setSpacing(6);
    
    auto createAxisLabel = [](const QString &axis, QLabel *&label, QWidget *parent) {
        QWidget *container = new QWidget(parent);
        QHBoxLayout *layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(2);
        
        QLabel *axisTag = new QLabel(axis + ":", container);
        axisTag->setStyleSheet("font-family: 'Consolas', monospace; font-size: 11px; font-weight: bold; color: #d39e00;");
        
        label = new QLabel("0.00", container);
        label->setStyleSheet("font-family: 'Consolas', monospace; font-size: 12px; font-weight: bold; color: #c9d1d9;");
        
        layout->addWidget(axisTag);
        layout->addWidget(label);
        return container;
    };
    
    accelValsLayout->addWidget(createAxisLabel("X", m_lblAccelXVal, accelCard));
    accelValsLayout->addWidget(createAxisLabel("Y", m_lblAccelYVal, accelCard));
    accelValsLayout->addWidget(createAxisLabel("Z", m_lblAccelZVal, accelCard));
    accelLayout->addLayout(accelValsLayout);
    
    // Custom Gyroscope Card containing sub-fields
    QFrame *gyroCard = new QFrame(ui->groupBox);
    gyroCard->setObjectName("telemetryCard");
    gyroCard->setStyleSheet(accelCard->styleSheet());
    QVBoxLayout *gyroLayout = new QVBoxLayout(gyroCard);
    gyroLayout->setContentsMargins(12, 10, 12, 10);
    gyroLayout->setSpacing(4);
    
    QLabel *gyroTitle = new QLabel("GYROSCOPE", gyroCard);
    gyroTitle->setStyleSheet(accelTitle->styleSheet());
    gyroLayout->addWidget(gyroTitle);
    
    QHBoxLayout *gyroValsLayout = new QHBoxLayout();
    gyroValsLayout->setSpacing(6);
    gyroValsLayout->addWidget(createAxisLabel("X", m_lblGyroXVal, gyroCard));
    gyroValsLayout->addWidget(createAxisLabel("Y", m_lblGyroYVal, gyroCard));
    gyroValsLayout->addWidget(createAxisLabel("Z", m_lblGyroZVal, gyroCard));
    gyroLayout->addLayout(gyroValsLayout);

    QWidget *cardLat = createCard("Latitude", m_lblLat, "0.0", ui->groupBox);
    QWidget *cardLon = createCard("Longitude", m_lblLon, "0.0", ui->groupBox);
    QWidget *cardGpsAlt = createCard("GPS Alt", m_lblGpsAlt, "0.0 m", ui->groupBox);
    QWidget *cardSats = createCard("Satellites", m_lblSatellites, "0", ui->groupBox);
    
    // Style status byte label with alert/crimson highlight
    m_lblState->setStyleSheet("font-family: 'Consolas', 'Courier New', monospace; font-size: 18px; font-weight: bold; color: #f85149;");

    // Arrange items in structured grid rows
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

void MainWindow::setupUiGraphs()
{
    // Common visual configurations
    QColor chartBg(22, 27, 34); // #161b22
    QBrush chartBgBrush(chartBg);
    QPen gridPen(QColor(48, 54, 61, 150));
    gridPen.setWidthF(1.0);
    
    QFont titleFont("Segoe UI", 11, QFont::Bold);
    QColor textColor(201, 209, 217);

    // Altitude Chart Visuals
    m_altChart = new QChart();
    m_altSeries = new QLineSeries();
    m_altChart->addSeries(m_altSeries);
    m_altChart->legend()->hide();
    m_altChart->setTitle("Altitude (m)");
    m_altChart->setTitleFont(titleFont);
    m_altChart->setTitleBrush(QBrush(textColor));
    m_altChart->setBackgroundBrush(chartBgBrush);
    m_altChart->setBackgroundRoundness(6);
    m_altChart->setPlotAreaBackgroundBrush(QBrush(QColor(13, 17, 23)));
    m_altChart->setPlotAreaBackgroundVisible(true);

    m_altAxisX = new QValueAxis();
    m_altAxisX->setTitleText("Telemetry Packet Count");
    m_altAxisX->setTitleBrush(QBrush(textColor));
    m_altAxisX->setRange(0, 100);
    m_altAxisX->setGridLinePen(gridPen);
    m_altAxisX->setLabelsColor(textColor);
    m_altAxisX->setLabelsFont(QFont("Segoe UI", 9));
    m_altAxisX->setLinePen(QPen(QColor(48, 54, 61)));
    m_altChart->addAxis(m_altAxisX, Qt::AlignBottom);
    m_altSeries->attachAxis(m_altAxisX);

    m_altAxisY = new QValueAxis();
    m_altAxisY->setTitleText("Meters");
    m_altAxisY->setTitleBrush(QBrush(textColor));
    m_altAxisY->setRange(0, 100); // Dynamic expansion enabled below
    m_altAxisY->setGridLinePen(gridPen);
    m_altAxisY->setLabelsColor(textColor);
    m_altAxisY->setLabelsFont(QFont("Segoe UI", 9));
    m_altAxisY->setLinePen(QPen(QColor(48, 54, 61)));
    m_altChart->addAxis(m_altAxisY, Qt::AlignLeft);
    m_altSeries->attachAxis(m_altAxisY);

    ui->altChartView->setChart(m_altChart);
    ui->altChartView->setRenderHint(QPainter::Antialiasing);

    // Temperature Chart Visuals
    m_tempChart = new QChart();
    m_tempSeries = new QLineSeries();
    m_tempChart->addSeries(m_tempSeries);
    m_tempChart->legend()->hide();
    m_tempChart->setTitle("Temperature (C)");
    m_tempChart->setTitleFont(titleFont);
    m_tempChart->setTitleBrush(QBrush(textColor));
    m_tempChart->setBackgroundBrush(chartBgBrush);
    m_tempChart->setBackgroundRoundness(6);
    m_tempChart->setPlotAreaBackgroundBrush(QBrush(QColor(13, 17, 23)));
    m_tempChart->setPlotAreaBackgroundVisible(true);

    m_tempAxisX = new QValueAxis();
    m_tempAxisX->setTitleText("Telemetry Packet Count");
    m_tempAxisX->setTitleBrush(QBrush(textColor));
    m_tempAxisX->setRange(0, 100);
    m_tempAxisX->setGridLinePen(gridPen);
    m_tempAxisX->setLabelsColor(textColor);
    m_tempAxisX->setLabelsFont(QFont("Segoe UI", 9));
    m_tempAxisX->setLinePen(QPen(QColor(48, 54, 61)));
    m_tempChart->addAxis(m_tempAxisX, Qt::AlignBottom);
    m_tempSeries->attachAxis(m_tempAxisX);

    m_tempAxisY = new QValueAxis();
    m_tempAxisY->setTitleText("Celsius");
    m_tempAxisY->setTitleBrush(QBrush(textColor));
    m_tempAxisY->setRange(0, 50); // Dynamic expansion enabled below
    m_tempAxisY->setGridLinePen(gridPen);
    m_tempAxisY->setLabelsColor(textColor);
    m_tempAxisY->setLabelsFont(QFont("Segoe UI", 9));
    m_tempAxisY->setLinePen(QPen(QColor(48, 54, 61)));
    m_tempChart->addAxis(m_tempAxisY, Qt::AlignLeft);
    m_tempSeries->attachAxis(m_tempAxisY);

    ui->tempChartView->setChart(m_tempChart);
    ui->tempChartView->setRenderHint(QPainter::Antialiasing);

    // Apply high-contrast line series formatting
    QPen altPen(QColor(88, 166, 255)); // Vibrant Cyan/Blue
    altPen.setWidthF(2.5);
    m_altSeries->setPen(altPen);

    QPen tempPen(QColor(56, 220, 113)); // Vibrant Green
    tempPen.setWidthF(2.5);
    m_tempSeries->setPen(tempPen);
}

void MainWindow::setupConnections()
{
    // UI Connections
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

    // Worker signals to MainWindow
    connect(m_worker, &SerialWorker::portOpened, this, &MainWindow::onPortOpened);
    connect(m_worker, &SerialWorker::portClosed, this, &MainWindow::onPortClosed);
    connect(m_worker, &SerialWorker::errorOccurred, this, &MainWindow::onErrorOccurred);

    // Worker data to Parser
    connect(m_worker, &SerialWorker::dataReady, m_parser, &DataParser::parseData);

    // Parser to MainWindow (Main Thread)
    connect(m_parser, &DataParser::packetReceived, this, &MainWindow::onPacketReceived);
}

void MainWindow::onConnectClicked()
{
    QString portName = ui->portComboBox->currentText();
    int baudRate = ui->baudComboBox->currentData().toInt();

    if (portName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select a valid COM port.");
        return;
    }

    // Start Logger
    m_logger->startLogging(QCoreApplication::applicationDirPath() + "/logs");

    // Tell worker to open port (thread-safe invocation)
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
    m_altSeries->clear();
    m_tempSeries->clear();
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

    // Update UI Labels
    if (m_lblTeamId) m_lblTeamId->setText(packet.teamID);
    if (ui->lblPacketId) ui->lblPacketId->setText(QString::number(packet.packetCount));
    if (ui->lblTimestamp) ui->lblTimestamp->setText(QString::number(packet.time) + " ms");
    if (m_lblState) m_lblState->setText(packet.state);
    if (m_lblAltitudeVal) m_lblAltitudeVal->setText(QString::number(packet.altitude, 'f', 1) + " m");
    if (m_lblTemperatureVal) m_lblTemperatureVal->setText(QString::number(packet.temperature, 'f', 1) + " °C");
    if (m_lblPressureVal) m_lblPressureVal->setText(QString::number(packet.pressure, 'f', 0) + " Pa");
    if (m_lblVoltage) m_lblVoltage->setText(QString::number(packet.voltage, 'f', 2) + " V");
    if (m_lblLat) m_lblLat->setText(QString::number(packet.latitude, 'f', 6));
    if (m_lblLon) m_lblLon->setText(QString::number(packet.longitude, 'f', 6));
    if (m_lblGpsAlt) m_lblGpsAlt->setText(QString::number(packet.gpsAltitude, 'f', 1) + " m");
    if (m_lblSatellites) m_lblSatellites->setText(QString::number(packet.satellites));
    
    if (m_lblAccelXVal) m_lblAccelXVal->setText(QString::number(packet.accel.x(), 'f', 2));
    if (m_lblAccelYVal) m_lblAccelYVal->setText(QString::number(packet.accel.y(), 'f', 2));
    if (m_lblAccelZVal) m_lblAccelZVal->setText(QString::number(packet.accel.z(), 'f', 2));

    if (m_lblGyroXVal) m_lblGyroXVal->setText(QString::number(packet.gyro.x(), 'f', 2));
    if (m_lblGyroYVal) m_lblGyroYVal->setText(QString::number(packet.gyro.y(), 'f', 2));
    if (m_lblGyroZVal) m_lblGyroZVal->setText(QString::number(packet.gyro.z(), 'f', 2));
    
    // Log telemetry packet
    m_logger->logPacket(packet);

    // Route coordinates to active chart loops
    updateCharts(packet);
}

void MainWindow::updateCharts(const TelemetryPacket &packet)
{
    qreal x = m_packetCount;
    
    m_altSeries->append(x, packet.altitude);
    m_tempSeries->append(x, packet.temperature);

    // Scroll axis if packet stream exceeds standard horizontal window range
    if (x > m_altAxisX->max()) {
        m_altAxisX->setRange(x - 100, x);
        m_tempAxisX->setRange(x - 100, x);
    }

    // Dynamic Autoscale Altitude Chart Y-Axis to prevent overflow clipping
    if (packet.altitude > m_altAxisY->max()) {
        m_altAxisY->setRange(m_altAxisY->min(), packet.altitude * 1.15 + 10.0);
    } else if (packet.altitude < m_altAxisY->min()) {
        m_altAxisY->setRange(packet.altitude * 0.85 - 10.0, m_altAxisY->max());
    }

    // Dynamic Autoscale Temperature Chart Y-Axis
    if (packet.temperature > m_tempAxisY->max()) {
        m_tempAxisY->setRange(m_tempAxisY->min(), packet.temperature + 5.0);
    } else if (packet.temperature < m_tempAxisY->min()) {
        m_tempAxisY->setRange(packet.temperature - 5.0, m_tempAxisY->max());
    }
}

void MainWindow::onErrorOccurred(const QString &error)
{
    QMessageBox::critical(this, "Serial Error", error);
    onPortClosed();
}

