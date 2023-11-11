//
// Created by felix on 11.11.23.
//

#ifndef TIMEISMONEY_CMAKE_SERIAL_DB_HPP
#define TIMEISMONEY_CMAKE_SERIAL_DB_HPP

#include <string>
#include <QSqlDatabase>
#include <QSharedPointer>
#include <QSqlQuery>

struct SerialSetup
{
    unsigned int baudrate = 0;
    std::string port_name {};
};

[[ nodiscard ]] int count_serial_setup(QSharedPointer<QSqlDatabase> &db) noexcept;
bool create_serial_setup_entry(QSharedPointer<QSqlDatabase> &db, unsigned int baudrate, const std::string &port_name) noexcept;
bool update_serial_setup(QSharedPointer<QSqlDatabase> &db, unsigned int baudrate, const std::string &port_name) noexcept;
SerialSetup select_latest_pk_serial_setup(QSharedPointer<QSqlDatabase> &db) noexcept;

#endif //TIMEISMONEY_CMAKE_SERIAL_DB_HPP
