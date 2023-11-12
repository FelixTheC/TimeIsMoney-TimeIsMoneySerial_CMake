#include "serialreader_qt.hpp"


SerialReader_QT::SerialReader_QT()
{
    initSerialReading();
}

SerialReader_QT::SerialReader_QT(const QString &port_name)
{
    usb_port = port_name;
    initSerialReading();
}

SerialReader_QT::~SerialReader_QT()
{
    serial_port->close();
}

void
SerialReader_QT::initSerialReading()
{
    serial_port = QSharedPointer<QSerialPort>(new QSerialPort(usb_port));
    // first close the port, makes it easier to re-open
    serial_port->close();

    connect(serial_port.data(), &QSerialPort::readyRead, this, &SerialReader_QT::read_line);

    if (serial_port->open(QIODevice::ReadOnly))
    {
        serial_port->setBaudRate(QSerialPort::BaudRate::Baud9600);
        serial_port->setParity(QSerialPort::EvenParity);
        serial_port->setDataBits(QSerialPort::Data7);   // receiving ASCII data
        open = true;
    }
}

void
SerialReader_QT::read_line()
{
    QByteArray raw_bytes = serial_port->read(64);
    std::string received_uuid = raw_bytes.toStdString();
    if (received_uuid[received_uuid.size() - 1] == '\n')
    {
        received_uuid = received_uuid.substr(0, received_uuid.size() - 1);
    }
    emit serialValueReceived(received_uuid);
}

void
SerialReader_QT::changeBaudrate(qint32 &val)
{
    if (serial_port && serial_port->isOpen())
    {
        serial_port->setBaudRate(val);
        serial_port->close();
    }
}

void
SerialReader_QT::changePort(const std::string &val)
{
    usb_port = QString::fromStdString(val);
    initSerialReading();
}

bool
SerialReader_QT::isOpen()
{
    return open;
}
