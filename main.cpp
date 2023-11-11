#include <QApplication>

#include "../TimeIsMoney-TimeIsMoneyDB_CMake/include/taskdb.hpp"

#include "serialoptions.hpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    auto tmp_db = TaskDB("test_timeismoney_serial");
    auto db_ = tmp_db.getDBPtr();
    
    SerialOptions w {db_};
    w.show();
    
    return a.exec();
}
