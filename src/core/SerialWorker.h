#pragma once

#include <QObject>
#include <QSerialPort>
#include <QThread>

class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

public slots:
    void openPort(const QString &portName, int baudRate);
    void closePort();
    void sendData(const QByteArray &data);

signals:
    void dataReady(const QByteArray &data);
    void errorOccurred(const QString &error);
    void portOpened();
    void portClosed();

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serial;
};
