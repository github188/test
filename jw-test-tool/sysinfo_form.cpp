#include "sysinfo_form.h"
#include "widget.h"
#include "main_form.h"
#include "ui_sysinfo_form.h"
#include "warningform.h"
#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QPalette>
#include <QProcess>

extern QString ip_address_remote;
extern QString tmp_file_local;
extern QString tmp_file_remote;
extern QString tmp_file;
extern QString bin_dir;
extern QString serial_num_local;
extern QString serial_num_remote;
extern QString product_name_local;
extern QString product_name_remote;
extern QString eth_num_local;
extern QString eth_num_remote;
extern QString disk_num_local;
extern QString disk_num_remote;
extern QString log_local;
extern QString log_remote;



QString date;

QString serial_num_result_local="unknown";
QString serial_num_result_remote="unknown";
QString product_name_result_local="unknown";
QString product_name_result_remote="unknown";
QString sysinfo_result_local="unknown";
QString sysinfo_result_remote="unknown";


QString system_info_local;
QString system_info_remote;
QString bios_info_local;
QString bios_info_remote;
QString cpu_info_local;
QString cpu_info_remote;
QString chipset_info_local;
QString chipset_info_remote;
QString mem_info_local;
QString mem_info_remote;
QString eth_info_local;
QString eth_info_remote;
QString sata_info_local;
QString sata_info_remote;

int ssh_cmd(QString arg)
{
    QString cmd;


    cmd = "ssh root@" + ip_address_remote + " " + arg;
    return system(cmd.toLatin1());


}

QString open_read(QString path)
{
    QFile outfile(path);
    QString out;

    outfile.open(QFile::ReadWrite);
    out = outfile.readAll();
    return out;
}


QString bash_cmd_read(QString cmd, QString path)
{

    if (bash_cmd(cmd) == 0)
       return open_read(path);
    else
        return "";

}

int bash_cmd(QString cmd)
{
    QString cmd_bash = "bash -c \"" + cmd + "\"";
    return system(cmd_bash.toLatin1());

}

QString ssh_read(QString cmd, QString path)
{if (ssh_cmd(cmd) == 0)
        return open_read(path);
     else

        return "";

}
void log_write_local(QString str)
{
    QString cmd = "echo \\\"" + str + "\\\" >> " + log_local;
    qDebug() << cmd;
    bash_cmd(cmd);
}

void log_write_remote(QString str)
{
    QString cmd = "echo \\\""+str + "\\\" >> " + log_remote;
    bash_cmd(cmd);
}
void get_time()
{

    QDateTime datetime = QDateTime::currentDateTime();
    date = "[" + datetime.toString("yyyy-MM-dd hh:mm:ss") + "]";

}
void sys_type(int product_num, int flag)
{
    QString product_name="Unknown";
    QString eth_num = "0";
    QString disk_num = "0";

    switch (product_num) {
    case 1001:
        product_name = "SYS-6026B-T(2U-B75)";
        eth_num = "2";
        disk_num = "8";
        break;
    case 1002:
        product_name = "SYS-6026N-T(2U-ATOM)";
        eth_num = "2";
        disk_num = "8";
        break;
     case 1003:
        product_name = "SYS-6036B-T(3U-SIMPLE)";
        eth_num = "2";
        disk_num = "16";
        break;
     case 1004:
        product_name = "SYS-6036C-T(3U-C216)";
        eth_num = "5";
        disk_num = "16";
        break;
     case 1005:
        product_name = "SYS-6036Z-T(3U-Z77)";
        eth_num = "2";
        disk_num = "16";
        break;
     default:
        product_name = "Unknonw";
        eth_num = "0";
        disk_num = "0";
        break;
    }

    if (flag) {
        product_name_local = product_name;
        eth_num_local = eth_num;
        disk_num_local = disk_num;
    } else {
        product_name_remote = product_name;
        eth_num_remote = eth_num;
        disk_num_remote = disk_num;
    }
}



Sysinfo_Form::Sysinfo_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sysinfo_Form)
{


    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);


   //System
    warningForm *sys_warning = new warningForm;
    QString errinfo="";

    QString cmd;
    QString out;

   get_time();
   cmd = "dmidecode -s baseboard-product-name >" + tmp_file_local;

   out = bash_cmd_read(cmd, tmp_file_local);
   if (out.length() != 0 ) {
        sys_type(out.toInt(), 1);
   } else {
         errinfo += "获取本地bios信息失败\n";

   }
   cmd = "dmidecode -s baseboard-product-name >" + tmp_file_remote;
    out = ssh_read(cmd, tmp_file_remote);
    if ( out.length() != 0) {
       sys_type(out.toInt(), 0);
    } else {
         errinfo += "获取对测bios信息失败\n";
    }

    ui->lineEdit->setText(product_name_local);
    ui->lineEdit_2->setText(serial_num_local);
    ui->lineEdit_3->setText(product_name_remote);
    ui->lineEdit_4->setText(serial_num_remote);

    cmd=bin_dir + "jw-aging system_info >" + tmp_file_local;
    system_info_local = bash_cmd_read(cmd, tmp_file_local);
    if (system_info_local.length() == 0) {
        errinfo += "获取本地设备系统信息失败\n";
    }
    cmd=bin_dir + "jw-aging system_info >" + tmp_file_remote;
    system_info_remote = ssh_read(cmd, tmp_file_remote);
    if (system_info_remote.length() == 0) {
        errinfo += "获取对测设备系统信息失败\n";
    }

    cmd = bin_dir + "jw-aging bios_info >" + tmp_file_local;
    bios_info_local = bash_cmd_read(cmd, tmp_file_local);
    if (bios_info_local.length() == 0) {
        errinfo += "获取本地设备bios信息失败\n";
    }
    cmd = bin_dir + "jw-aging bios_info >" + tmp_file_remote;
    bios_info_remote = ssh_read(cmd, tmp_file_remote);
    if (bios_info_remote.length() == 0) {
        errinfo += "获取对测设备bios信息失败\n";
    }

    cmd = bin_dir + "jw-aging cpu_info >" + tmp_file_local;
    cpu_info_local = bash_cmd_read(cmd, tmp_file_local);
    if (cpu_info_local.length() == 0) {
        errinfo += "获取本地设备cpu信息失败\n";
    }
     cmd = bin_dir + "jw-aging cpu_info >" + tmp_file_remote;
    cpu_info_remote = ssh_read(cmd, tmp_file_remote);
    if (cpu_info_remote.length() == 0) {
        errinfo += "获取对测设备cpu信息失败\n";
    }

    cmd = bin_dir + "jw-aging chipset_info >" + tmp_file_local;
    chipset_info_local=bash_cmd_read(cmd, tmp_file_local);
    if (chipset_info_local.length() == 0) {
        errinfo += "获取本地设备芯片组信息失败\n";

    }

    cmd = bin_dir + "jw-aging chipset_info >" + tmp_file_remote;
    chipset_info_remote=ssh_read(cmd, tmp_file_remote);
    if (chipset_info_remote.length() == 0) {
         errinfo += "获取本地设备芯片组信息失败\n";
    }

    cmd = bin_dir + "jw-aging memory_info >" + tmp_file_local;
    mem_info_local = bash_cmd_read(cmd, tmp_file_local);
    if (mem_info_local.length() == 0) {
        errinfo += "获取本地设备内存信息失败\n";
    }
    cmd = bin_dir + "jw-aging memory_info >" + tmp_file_remote;
    mem_info_remote = ssh_read(cmd, tmp_file_remote);
    if (mem_info_remote.length() == 0) {
        errinfo += "获取对测设备内存信息失败\n";
    }

    cmd = bin_dir + "jw-aging eth_info >" + tmp_file_local;
    eth_info_local = bash_cmd_read(cmd, tmp_file_local);
    if (eth_info_local.length() == 0){
        errinfo += "获取本地设备网卡信息失败\n";
    }
    cmd = bin_dir + "jw-aging eth_info >" + tmp_file_remote;
    eth_info_remote = ssh_read(cmd, tmp_file_remote);
    if (eth_info_remote.length() == 0) {
        errinfo += "获取对测设备网卡信息失败\n";
    }

    cmd = bin_dir + "jw-aging sata_info >" + tmp_file_local;
    sata_info_local = bash_cmd_read(cmd, tmp_file_local);
    if (sata_info_local.length() == 0) {
        errinfo += "获取本地设备硬盘信息失败\n";
    }
    cmd = bin_dir + "jw-aging sata_info >" + tmp_file_remote;
    sata_info_remote = ssh_read(cmd, tmp_file_remote);
    if (sata_info_remote.length() == 0) {
        errinfo += "获取对测设备硬盘信息失败\n";
    }

/*

    ui->textEdit_1->setTextColor(Qt::red);
    ui->textEdit_2->setTextColor(Qt::red);
    ui->textEdit_3->setTextColor(Qt::red);
    ui->textEdit_4->setTextColor(Qt::red);
    ui->textEdit_5->setTextColor(Qt::red);
    ui->textEdit_6->setTextColor(Qt::red);
    ui->textEdit_7->setTextColor(Qt::red);
    ui->textEdit_11->setTextColor(Qt::red);
    ui->textEdit_12->setTextColor(Qt::red);
    ui->textEdit_13->setTextColor(Qt::red);
    ui->textEdit_14->setTextColor(Qt::red);
    ui->textEdit_15->setTextColor(Qt::red);
    ui->textEdit_16->setTextColor(Qt::red);
    ui->textEdit_17->setTextColor(Qt::red);
*/

    ui->textEdit_1->setText(system_info_local);
    ui->textEdit_2->setText(bios_info_local);
    ui->textEdit_3->setText(cpu_info_local);
    ui->textEdit_4->setText(chipset_info_local);
    ui->textEdit_5->setText(mem_info_local);
    ui->textEdit_6->setText(eth_info_local);
    ui->textEdit_7->setText(sata_info_local);

    ui->textEdit_11->setText(system_info_remote);
    ui->textEdit_12->setText(bios_info_remote);
    ui->textEdit_13->setText(cpu_info_remote);
    ui->textEdit_14->setText(chipset_info_remote);
    ui->textEdit_15->setText(mem_info_remote);
    ui->textEdit_16->setText(eth_info_remote);
    ui->textEdit_17->setText(sata_info_remote);



    QButtonGroup *qbg1 = new QButtonGroup;
    qbg1->addButton(ui->pushButton);
    qbg1->addButton(ui->pushButton_5);

    if (errinfo.length() != 0) {
            sys_warning->setinfo(errinfo);

            sys_warning->show();

    }

}

Sysinfo_Form::~Sysinfo_Form()
{
    delete ui;
}

void Sysinfo_Form::on_pushButton_2_clicked()
{
    warningForm *warning = new warningForm();
    QString errinfo="";
    if (product_name_result_local=="unknown") {
       errinfo += "本地设备设备型号未确认\n";
    }
    if (product_name_result_remote == "unknown") {
        errinfo +="对测设备设备型号未确认\n";
    }
    if(serial_num_result_local=="unknown"){
        errinfo += "本地设备条码未确认\n";
    }
    if (serial_num_result_remote == "unknown") {
        errinfo += "对测设备条码未确认\n";
    }
    if (sysinfo_result_local == "unknown") {
        errinfo += "本地设备系统信息未确认\n";
    }
    if (sysinfo_result_remote == "unknown") {
        errinfo += "对测设备系统信息未确认\n";
    }
    if (errinfo.length() != 0) {
        warning->setinfo(errinfo);
        warning->show();
    } else {
    QString str;
    log_remote.truncate(0);
    str = "System\n" + system_info_local + "\n\nBios\n" + bios_info_local + "\n\nCPU\n" + cpu_info_local + "\n\nChipset\n" \
            + chipset_info_local + "\n\nMEM\n" + mem_info_local + "\n\nNet\n"  + eth_info_local + "\n\nSata\n" + sata_info_local \
            + "\n\n" + product_name_result_local + "\n" + serial_num_result_local+"\n"+ sysinfo_result_local + "\n\n";
    log_write_local(str);

    str = "System\n" + system_info_remote + "\n\nBios\n" + bios_info_remote + "\n\nCPU\n" + cpu_info_remote + "\n\nChipset\n" \
            + chipset_info_remote + "\n\nMEM\n" + mem_info_remote + "\n\nNet\n"  + eth_info_remote + "\n\nSata\n" + sata_info_remote
            + "\n\n" + product_name_result_remote + "\n" + serial_num_result_remote + "\n" + sysinfo_result_remote + "\n\n";
    log_write_remote(str);
    this->close();
    Main_Form *m = new Main_Form;
    m->show();
    }
}



void Sysinfo_Form::on_pushButton_18_clicked()
{
    this->close();
    Widget *widget = new Widget;
    widget->show();
}



void Sysinfo_Form::on_pushButton_clicked()
{
    ui->pushButton->setDisabled(true);
    ui->pushButton_5->setEnabled(true);
    get_time();
    sysinfo_result_local = date+"System:fail";


}

void Sysinfo_Form::on_pushButton_5_clicked()
{
    ui->pushButton_5->setDisabled(true);
    ui->pushButton->setEnabled(true);
    get_time();
    sysinfo_result_local = date+"Syetem:pass";

}

void Sysinfo_Form::on_pushButton_6_clicked()
{
    ui->pushButton_6->setDisabled(true);
    ui->pushButton_7->setEnabled(true);
    get_time();
    sysinfo_result_remote = date+"System:fail";


}

void Sysinfo_Form::on_pushButton_7_clicked()
{
    ui->pushButton_7->setDisabled(true);
    ui->pushButton_6->setEnabled(true);
    get_time();
    sysinfo_result_remote = date+"System:pass";

}


void Sysinfo_Form::on_pushButton_3_clicked()
{
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setEnabled(true);
    get_time();
    product_name_result_local = date+"product name:pass";
}

void Sysinfo_Form::on_pushButton_4_clicked()
{
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_3->setEnabled(true);
    get_time();
    product_name_result_local = date+"product name:fail";
}

void Sysinfo_Form::on_pushButton_13_clicked()
{
    ui->pushButton_13->setDisabled(true);
    ui->pushButton_14
                ->setEnabled(true);
    get_time();
    serial_num_result_local = date+"serial number:pass";
}


void Sysinfo_Form::on_pushButton_14_clicked()
{
    ui->pushButton_14->setDisabled(true);
    ui->pushButton_13->setEnabled(true);
    get_time();
    serial_num_result_local = date+"serial number:fail";
}

void Sysinfo_Form::on_pushButton_19_clicked()
{
    ui->pushButton_19->setDisabled(true);
    ui->pushButton_20->setEnabled(true);
    get_time();
    product_name_result_remote = date+"product name:pass";
}

void Sysinfo_Form::on_pushButton_20_clicked()
{
    ui->pushButton_20->setDisabled(true);
    ui->pushButton_19->setEnabled(true);
    get_time();
    product_name_result_remote = date+"product name:fail";
}

void Sysinfo_Form::on_pushButton_21_clicked()
{
    ui->pushButton_21->setDisabled(true);
    ui->pushButton_22->setEnabled(true);
    get_time();
    serial_num_result_remote = date+"serial number:pass";
}

void Sysinfo_Form::on_pushButton_22_clicked()
{
    ui->pushButton_22->setDisabled(true);
    ui->pushButton_21->setEnabled(true);
    get_time();
    serial_num_result_remote = date+"serial number:fail";
}

