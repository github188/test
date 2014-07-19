#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QtDebug>
namespace common {
class info;
}

class info
{
private:
    QString tester;     //测试热源
    QString serial_num_local;   //本地设备号
    QString serial_num_remote;  //对端设备号
    QString ip_address_remote;  //对端设备ip
    QString password_remote;
    QString tmp_file = "/tmp/out";      //临时文件
    QString log_local;          //本地log
    QString log_remote;         //对测log


    QString bios_product_name;        //


};
#endif // COMMON_H
