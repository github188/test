#include "widget.h"
#include "sysinfo_form.h"
#include "ui_widget.h"
#include "warningform.h"
#include <QProcess>
#include <QIODevice>


int single;
QString tester;     //测试热源
QString serial_num_local;   //本地设备号
QString serial_num_remote;  //对端设备号
QString ip_address_remote;  //对端设备ip
QString password_remote;
QString tmp_file = "/tmp/out";
QString tmp_file_local = "/tmp/out_local";      //临时文件
QString tmp_file_remote = "/tmp/out_remote";
QString bin_dir="/usr/local/bin/";
QString log_local;          //本地log
QString log_remote;         //对测log

QString product_name_local;        //
QString product_name_remote;
QString eth_num_local="0";
QString eth_num_remote="0";
QString disk_num_local="0";
QString disk_num_remote="0";



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton->setFocus();
    ui->pushButton->setDefault(true);
    ui->lineEdit->setText("jw");
    ui->lineEdit_2->setText("123");
    ui->lineEdit_6->setText("456");

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

    single = 0;

   //方便测试
    tester = ui->lineEdit->text();
    serial_num_local = ui->lineEdit_2->text();
    serial_num_remote = ui->lineEdit_6->text();
    ip_address_remote = ui->lineEdit_3->text();
    password_remote = "123456";
    //password_remote = ui->lineEdit_4->text();

    errinfo = "";
    if (tester.length() == 0) {
        errinfo += "测试人员为空\n";

    }
    if (serial_num_local.length() == 0) {

        errinfo += "本地设备条码为空\n";
    }
    if (ip_address_remote == "single") {
        single = 1;
    } else {
         if (serial_num_remote.length() == 0) {
            errinfo += "对测设备条码为空\n";
        }
         if (ip_address_remote.length() == 0) {

            errinfo += "对测设备ip未指定\n";
        }

        if (password_remote.length() == 0) {

            errinfo += "对测设备密码未指定\n";
        }
    }
    if (errinfo.length() != 0) {
        goto error;
    }



    if (!single) {
    outfile.remove();

    cmd = "ping -t 30 -c 2 " + ip_address_remote + " >"+ tmp_file;
    out = bash_cmd_read(cmd, tmp_file);

    if (!out.contains("ttl=",  Qt::CaseInsensitive)) {
        errinfo += "与对测设备网络连接不通\n";
    }




    cmd = bin_dir + "jw-aging auto_ssh " + ip_address_remote + " "+ password_remote + " >" +tmp_file;
    out = bash_cmd_read(cmd, tmp_file);
    if (out.length() == 1) {
            errinfo +=  "ssh 失败\n";
    }
    }

error:
    if(errinfo.length() != 0) {
        warning->setinfo(errinfo);
        warning->show();
    } else {

        log_local = "/tmp/jw-aging-"+tester+"-"+serial_num_local+".log";
        log_remote = "/tmp/jw-aging-"+tester+"-"+serial_num_remote + ".log";
        QFile log_file_local(log_local);
        log_file_local.remove();

        QFile log_file_remote(log_remote);
        log_file_remote.remove();

    this->close();
    Sysinfo_Form *s= new Sysinfo_Form;
    s->show();

    }

}

