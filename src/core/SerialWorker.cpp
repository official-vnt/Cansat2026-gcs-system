#include "SerialWorker.h"

SerialWorker::SerialWorker(QObject *parent)
    : QObject(parent), m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::readyRead, this, &SerialWorker::handleReadyRead);
    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialWorker::handleError);
}

SerialWorker::~SerialWorker()
{
    closePort();
}

void SerialWorker::openPort(const QString &portName, int baudRate)
{
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial->open(QIODevice::ReadOnly)) {
        emit portOpened();
    } else {
        emit errorOccurred(m_serial->errorString());
    }
}

void SerialWorker::closePort()
{
    if (m_serial->isOpen()) {
        m_serial->close();
        emit portClosed();
    }
}

void SerialWorker::handleReadyRead()
{
    QByteArray data = m_serial->readAll();
    emit dataReady(data);
}

void SerialWorker::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        emit errorOccurred(m_serial->errorString());
        closePort();
    }
}
