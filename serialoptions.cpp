#include "serialoptions.hpp"
#include "serial_db.hpp"
#include "ui_serialoptions.h"


SerialOptions::SerialOptions(QSharedPointer<QSqlDatabase> &db_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialOptions)
{
    ui->setupUi(this);
    
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SerialOptions::handle_accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SerialOptions::close);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &SerialOptions::handlePortChanged);
    
    this->db = db_;
    
    if (count_serial_setup(db) > 0)
    {
        auto last_setup = select_latest_pk_serial_setup(db);
        this->user_baudrate = static_cast<qint32>(last_setup.baudrate);
        this->selected_port_name = QString::fromStdString(last_setup.port_name);
    }

    initPortOptions();
    initBaudRateOptions();
}

SerialOptions::~SerialOptions()
{
    delete ui;
}

QString
SerialOptions::getFirstPortName()
{
    auto available_ports = QSerialPortInfo::availablePorts();
    if (available_ports.isEmpty())
        return "";
    else
        return available_ports.first().portName();
}

void
SerialOptions::initPortOptions()
{
    auto availablePorts = QSerialPortInfo::availablePorts();
    std::for_each(availablePorts.begin(),
                  availablePorts.end(),
                  [&](const QSerialPortInfo &portInfo){
                     ui->comboBox->addItem(portInfo.portName());
                  });
    if (!availablePorts.empty())
    {
        if (selected_port_name.isEmpty())
        {
            selected_port_name = availablePorts[0].portName();
        }
        else
        {
            auto text_index = ui->comboBox->findText(selected_port_name);
            ui->comboBox->setCurrentIndex(text_index);
        }
    }
}

void
SerialOptions::initBaudRateOptions()
{
    std::for_each(all_rates.begin(),
                  all_rates.end(),
                  [&](const qint64 &rate){
                      ui->comboBox_2->addItem(QString::number(rate));
                  });

    if (user_baudrate == -1)
    {
        auto current_index = ui->comboBox_2->findText(QString::number(default_baudrate));
        ui->comboBox_2->setCurrentIndex(current_index);
    }
    else
    {
        auto current_index = ui->comboBox_2->findText(QString::number(user_baudrate));
        ui->comboBox_2->setCurrentIndex(current_index);
    }
}

void
SerialOptions::handle_accepted()
{
    auto new_rate = ui->comboBox_2->itemText(ui->comboBox_2->currentIndex()).toInt();
    auto new_port = ui->comboBox->itemText(ui->comboBox->currentIndex());
    
    if (new_rate != user_baudrate)
    {
        user_baudrate = new_rate;
        emit baudrateChanged(new_rate);
    }
    
    if (new_port != selected_port_name)
    {
        selected_port_name = new_port;
        emit portChanged(selected_port_name.toStdString());
    }
    
    if (count_serial_setup(db) == 0)
    {
        create_serial_setup_entry(db, user_baudrate, selected_port_name.toStdString());
    }
    else
    {
        update_serial_setup(db, user_baudrate, selected_port_name.toStdString());
    }
}

void
SerialOptions::handlePortChanged(const QString &val)
{
    if (val != selected_port_name)
    {
        selected_port_name = val;
        emit portChanged(val.toStdString());
    }
}

qint32 SerialOptions::getUserBaudrate() const noexcept
{
    return user_baudrate;
}

QString SerialOptions::getSelectedPortName() noexcept
{
    return selected_port_name;
}
