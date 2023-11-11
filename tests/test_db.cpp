#include <gtest/gtest.h>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QApplication>

#include "../TimeIsMoney-TimeIsMoneyDB_CMake/include/taskdb.hpp"

#include "../serial_db.hpp"
#include "../serialoptions.hpp"


namespace timeismoney
{
namespace serial
{
namespace
{
    class DBFixture : public testing::Test
    {
    public:
        static void SetUpTestSuite()
        {
            if (db_ == nullptr)
            {
                auto tmp_db = TaskDB("test_timeismoney_serial");
                db_ = tmp_db.getDBPtr();
            }
        }
        
        static void TearDownTestSuite()
        {
            if (db_ != nullptr)
            {
                db_->open();
                if (db_->isOpen())
                {
                    db_->exec("drop table serial_setup;");
                }
                db_->close();
                
                db_.reset();
            }
        }
        
        static QSharedPointer<QSqlDatabase> db_;
    };
    
    QSharedPointer<QSqlDatabase> DBFixture::db_ = QSharedPointer<QSqlDatabase>();
    
    TEST_F(DBFixture, insert_new_data_returns_1)
    {
        ASSERT_TRUE(create_serial_setup_entry(db_, 115200, "/dev/ttyACMO"));
        ASSERT_EQ(count_serial_setup(db_), 1);
    }
    
    TEST_F(DBFixture, select_latest_pk_serial_setup)
    {
        ASSERT_TRUE(create_serial_setup_entry(db_, 9600, "/dev/ttyACMZ"));
        
        auto data = select_latest_pk_serial_setup(db_);
        
        ASSERT_EQ(data.baudrate, 9600);
        ASSERT_EQ(data.port_name, "/dev/ttyACMZ");
    }
    
    TEST_F(DBFixture, update_serial_setup)
    {
        ASSERT_TRUE(create_serial_setup_entry(db_, 4800, "/dev/ttyACM9"));
        
        auto data = select_latest_pk_serial_setup(db_);
        
        ASSERT_EQ(data.baudrate, 4800);
        ASSERT_EQ(data.port_name, "/dev/ttyACM9");
        
        ASSERT_TRUE(update_serial_setup(db_, 2400, "/dev/ttyACM5"));
        
        auto data_new = select_latest_pk_serial_setup(db_);
        
        ASSERT_EQ(data_new.baudrate, 2400);
        ASSERT_EQ(data_new.port_name, "/dev/ttyACM5");
        
    }
    
    TEST_F(DBFixture, init_serialoptions_will_load_data_from_db)
    {
        QApplication a(1, );
        SerialOptions serialOptions {db_};
        
        ASSERT_TRUE(serialOptions.getUserBaudrate() != 0);
        ASSERT_FALSE(serialOptions.getSelectedPortName().isEmpty());
    }
}
}
}