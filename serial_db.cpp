//
// Created by felix on 11.11.23.
//

#include "serial_db.hpp"

[[ nodiscard ]] int count_serial_setup(QSharedPointer<QSqlDatabase> &db) noexcept
{
    int result = -1;
    db->open();
    
    if (db->isOpen())
    {
        QSqlQuery query;
        auto res = query.exec("select count(*) from serial_setup;");
        
        if (res)
        {
            query.first();
            result = query.value(0).toInt();
        }
    }
    
    db->close();
    
    return result;
}

bool create_serial_setup_entry(QSharedPointer<QSqlDatabase> &db, unsigned int baudrate, const std::string &port_name) noexcept
{
    bool is_valid = false;
    db->open();
    
    if (db->isOpen())
    {
        QSqlQuery query;
        query.prepare("insert into serial_setup (baudrate, port_name) values (:baudrate, :port_name)");
        query.bindValue(":baudrate", baudrate);
        query.bindValue(":port_name", QString::fromStdString(port_name));
        
        is_valid = query.exec();
    }
    
    db->close();
    
    return is_valid;
}

bool update_serial_setup(QSharedPointer<QSqlDatabase> &db, unsigned int baudrate, const std::string &port_name) noexcept
{
    bool is_valid = false;
    db->open();
    
    if (db->isOpen())
    {
        QSqlQuery query;
        query.prepare("update serial_setup set baudrate=:baudrate, port_name=:port_name;");
        query.bindValue(":baudrate", baudrate);
        query.bindValue(":port_name", QString::fromStdString(port_name));
        
        is_valid = query.exec();
    }
    
    db->close();
    
    return is_valid;
}

SerialSetup select_latest_pk_serial_setup(QSharedPointer<QSqlDatabase> &db) noexcept
{
    SerialSetup result {};
    db->open();
    
    if (db->isOpen())
    {
        QSqlQuery query;
        query.prepare("select baudrate, port_name from serial_setup order by pk desc limit 1;");
        
        auto res = query.exec();
        
        if (res)
        {
            query.first();
            result.baudrate = query.value(0).toUInt();
            result.port_name = query.value(1).toString().toStdString();
        }
    }
    
    db->close();
    
    return result;
}
