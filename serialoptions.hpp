#ifndef SERIALOPTIONS_H
#define SERIALOPTIONS_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSharedPointer>


namespace Ui {
class SerialOptions;
}

class SerialOptions : public QDialog
{
    Q_OBJECT

public:
    explicit SerialOptions(QWidget *parent = nullptr);
    ~SerialOptions() override;
    static QString getFirstPortName();

signals:
    void baudrateChanged(qint32 &new_rate);
    void portChanged(const std::string &val);


private slots:
    void handle_accepted();
    void handlePortChanged(const QString &val);

private:
    Ui::SerialOptions *ui;
    void initPortOptions();
    void initBaudRateOptions();

    qint32 default_baudrate = 9600;
    qint32 user_baudrate = -1;
    QString selected_port_name = "";
    static constexpr std::initializer_list<qint32> all_rates = {QSerialPort::Baud1200,
                                                                QSerialPort::Baud2400,
                                                                QSerialPort::Baud4800,
                                                                QSerialPort::Baud9600,
                                                                QSerialPort::Baud19200,
                                                                QSerialPort::Baud38400,
                                                                QSerialPort::Baud57600,
                                                                QSerialPort::Baud115200};
};

#endif // SERIALOPTIONS_H
