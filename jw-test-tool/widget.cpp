#include "widget.h"
#include "sysinfo_form.h"
#include "ui_widget.h"
#include "warningform.h"
#include <QProcess>
#include <QIODevice>

QString tester;     //测试热源
QString serial_num_local;   //本地设备号
QString serial_num_remote;  //对端设备号
QString ip_address_remote;  //对端设备ip
QString password_remote;
QString tmp_file = "/tmp/out";      //临时文件
QString tmp_dir="/usr/local/bin/";
QString log_local;          //本地log
QString log_remote;         //对测log


QString product_name_local;        //
QString product_name_remote;
QString eth_num_local;
QString eth_num_remote;
QString disk_num_local;
QString disk_num_remote;



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()
{
    this->close();
}

void Widget::on_pushButton_clicked()
{




    QFile outfile(tmp_file);
    QString cmd;
    QString out;
    QString errinfo;

    warningForm *warning = new warningForm();


   //方便测试
    tester = ui->lineEdit->text();
    serial_num_local = ui->lineEdit_2->text();
    serial_num_remote = ui->lineEdit_6->text();
    ip_address_remote = ui->lineEdit_3->text();
    password_remote = ui->lineEdit_4->text();
/*
    tester = "lyt";
    serial_num_local = "123";
    serial_num_remote = "456";
    ip_address_remote = "192.168.70.36";
*/
    errinfo = "";
    if (tester.length() == 0) {
        errinfo += "测试人员为空\n";

    }
    if (serial_num_local.length() == 0) {

        errinfo += "本地设备条码为空\n";
    }
    if (serial_num_remote.length() == 0) {
        errinfo += "对测设备条码为空\n";
    }
    if (ip_address_remote.length() == 0) {

        errinfo += "对测设备ip未指定\n";
    }

    if (password_remote.length() == 0) {

        errinfo += "对测设备密码未指定\n";
    }



    outfile.remove();
    cmd = "ping -t 2 -c 2 " + ip_address_remote + " >"+ tmp_file;
    system(cmd.toLatin1());
    outfile.open(QFile::ReadWrite);
    out = outfile.readAll();

    if (!out.contains("ttl=",  Qt::CaseInsensitive)) {
        errinfo += "与对测设备网络连接不通\n";
    }
    outfile.remove();
    outfile.close();



        cmd = tmp_dir + "jw-aging auto_ssh " + ip_address_remote + " "+ password_remote + " >" +tmp_file;
        qDebug() << cmd;
        out = bash_cmd_read(cmd, tmp_file);
        qDebug() << out;
        if (out.length() == 1) {
            errinfo +=  "ssh 失败\n";
         }


    if(errinfo.length() != 0) {
        warning->setinfo(errinfo);
        warning->show();
    } else {

    log_local = "./jw-aging-"+tester+"-"+serial_num_local+".log";
    log_remote = "./jw-aging-"+tester+"-"+serial_num_remote + ".log";
    this->close();
    Sysinfo_Form *s= new Sysinfo_Form;
    s->show();

    }

}

