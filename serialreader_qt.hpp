#ifndef SERIALREADER_QT_H
#define SERIALREADER_QT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSharedPointer>
#include <QtWidgets/QWidget>


class SerialReader_QT : public QWidget
{
    Q_OBJECT

public:
    SerialReader_QT();
    SerialReader_QT(const QString &port_name);
    ~SerialReader_QT();

Q_SIGNALS:
    void serialValueReceived(std::string &val);

public Q_SLOTS:
    bool isOpen();
    void read_line();
    void changeBaudrate(qint32 &val);

private:
    bool open = false;
    QString usb_port = "ttyACM0";
    QSharedPointer<QSerialPort> serial_port;
    QByteArray received_uuid = QByteArray();
    void initSerialReading();
};

#endif // SERIALREADER_QT_H
