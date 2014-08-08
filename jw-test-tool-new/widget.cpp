#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QPalette>

#define  DISK_MIN_READ  120
#define DISK_MIN_WRITE  100

QString product_name;
int eth_num;
int disk_num;
QTimer *mon_timer;
QString bash_cmd(QString cmd)
{
    QString cmd_bash = "bash -c \"" + cmd + "\"";
    QString out="";
    QProcess *process = new QProcess();
    qDebug()<<"cmd:"<<cmd_bash;
    process->start(cmd_bash);
    process->waitForFinished(60000);

    out = process->readAllStandardOutput();
    if (out.endsWith("\n")) {
        out.replace((out.length()-1),1,"");
    }
    qDebug()<<"out:"<<out;
    return out;
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QPalette pal = ui->lineEdit->palette();
    pal.setColor(QPalette::Text,Qt::darkYellow);
    ui->lineEdit->setPalette(pal);

    pal = ui->lineEdit_2->palette();
    pal.setColor(QPalette::Text, Qt::darkYellow);
    ui->lineEdit_2->setPalette(pal);

    pal = ui->lineEdit_3->palette();
    pal.setColor(QPalette::Text, Qt::darkYellow);
    ui->lineEdit_3->setPalette(pal);

    ui->lineEdit->setText("未启动");
    ui->lineEdit_2->setText("未启动");
    ui->lineEdit_3->setText("未启动");

    ui->pushButton_8->setDisabled(true);


    QString cmd;
    mon_timer = new QTimer(this);
    if (!mon_timer->isActive()) {
        mon_timer->start(10*1000);
    }

    connect(mon_timer, SIGNAL(timeout()), this, SLOT(mon_update()));

    cmd = "jw-aging system_info";
    product_name = bash_cmd(cmd);
    ui->lineEdit_4->setText(product_name);
    if (product_name == "SYS-6026B-T(2U-B75)") {
	    eth_num=2;
	    disk_num=8;
    } else if (product_name == "SYS-6026N-T(2U-ATOM)"){
	    eth_num=2;
	    disk_num=8;
    } else if (product_name == "SYS-6036B-T(3U-SIMPLE)") {
	    eth_num=2;
	    disk_num=16;
    } else if (product_name == "SYS-6036C-T(3U-C216)") {
	    eth_num=5;
	    disk_num=16;
    } else if (product_name == "SYS-6036Z-T(3U-Z77)") {
	    eth_num=2;
	    disk_num=16;
    } else {
	    eth_num=0;
	    disk_num=0;
    }
    
    cmd = "jw-aging cpu_info";
    ui->lineEdit_5->setText(bash_cmd(cmd));

    cmd = "jw-aging memory_info";
    ui->textEdit->setText(bash_cmd(cmd));

    cmd = "jw-aging eth_info";
    ui->textEdit_2->setText(bash_cmd(cmd));

    cmd = "jw-aging fireware";
    ui->textEdit_6->setText(bash_cmd(cmd));

	   
    
       
    qDebug() << "eth_num:" << eth_num << "\ndisk_num:" << disk_num;
    ui->tableWidget->setColumnCount(eth_num);
    int i;
    for (i=0; i<eth_num; i++){
	    QString name="eth" + QString::number(i);
	    ui->tableWidget->setItem(0, i, new QTableWidgetItem(name));
    }


    if (disk_num == 16) {
        ui->tableWidget_2->setRowCount(8);
        ui->tableWidget_2->setColumnCount(8);
        int j, m, n;

        for (j=0, m=0; j<8; j+=2, m++) {

        for (i=0, n=0; i<8; i+=2, n++) {
            ui->tableWidget_2->setSpan(i, j, 2, 1);
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(4*m + n + 1)));
            }
        }
    } else if (disk_num == 8) {
        ui->tableWidget_2->setRowCount(4);
        ui->tableWidget_2->setColumnCount(8);
        int j, m, n;

        for (j=0, m=0; j<4; j+=2, m++) {
            for (i=0, n=0; i<8; i+=2, n++) {
                ui->tableWidget_2->setSpan(j,i, 2, 1);
                ui->tableWidget_2->setItem(j, i, new QTableWidgetItem(QString::number(4*m + n +1)));
            }

        }
    }

}

Widget::~Widget()
{
    delete ui;
}

void Widget::mon_update()
{
    QString cmd;
    QString out;
    QStringList out_list;
    cmd = "jw-aging fan";
    out = bash_cmd(cmd);
    ui->textEdit_4->setText(out);
    cmd = "jw-aging power";
    out = bash_cmd(cmd);
    ui->textEdit_5->setText(out);
    cmd = "jw-aging cpu";
    out = bash_cmd(cmd) + "%";
    ui->lineEdit_8->setText(out);
    cmd = "jw-aging mem";
    out = bash_cmd(cmd);
    //out_list = out.split(";");

    ui->lineEdit_7->setText(out);
}

void Widget::on_pushButton_7_clicked()
{

    QPalette pal = ui->lineEdit->palette();
    pal.setColor(QPalette::Text, Qt::green);
    ui->lineEdit->setPalette(pal);

    ui->lineEdit->setText("正在测试");
    ui->pushButton_7->setDisabled(true);
    int i;
    QString cmd;
    QString speed;
    QStringList speed_list;
    cmd = "jw-aging net_speed_test_new";
    speed = bash_cmd(cmd);
    speed_list = speed.split("\n");
    qApp->processEvents();
    for (i=0; i<speed_list.length(); i++) {
      ui->tableWidget->setItem(1, i, new QTableWidgetItem(speed_list.at(i)));
    }
    qApp->processEvents();
    ui->lineEdit->setText("测试完成");
    ui->pushButton_7->setEnabled(true);
}

void Widget::on_pushButton_11_clicked()
{
    ui->pushButton_11->setDisabled(true);

    int i;
    QString cmd;
    QString disk_info;
    QStringList disk_info_list;

    cmd = "jw-aging disk_test info";
    disk_info = bash_cmd(cmd);

    disk_info_list = disk_info.split("\n");
    for(i=0; i< disk_info_list.length(); i++) {
        int m, n;
        if (disk_num == 16) {
            m = (i%4)*2;
            n = (i/4)*2;
        }else if (disk_num == 8) {
            m = (i/4)*2;
            n = (i%4)*2;
        }
        if (disk_info_list.at(i) == "Null") {
            ui->tableWidget_2->item(m, n)->setBackgroundColor(Qt::gray);
        }
        if (disk_info_list.at(i) == "Bad") {
            ui->tableWidget_2->item(m,n)->setBackgroundColor(Qt::red);
        }
    }
    ui->pushButton_8->setEnabled(true);
    ui->pushButton_11->setEnabled(true);

}

void Widget::on_pushButton_8_clicked()
{

    QPalette pal = ui->lineEdit_2->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_2->setPalette(pal);

    ui->lineEdit_2->setText("正在测试");
    ui->pushButton_8->setDisabled(true);
    int i;
    QString cmd;
    QString disk_speed;
    QStringList disk_speed_list;
    QString disk_info;
    QStringList disk_info_list;
    cmd = "jw-aging disk_test info";
    disk_info = bash_cmd(cmd);

    disk_info_list = disk_info.split("\n");

        //QString name= QString::number(i+1) + ":" + disk_info_list.at(i);
        //ui->tableWidget_2->setItem(m, n, new QTableWidgetItem(name));
       // qDebug() << "i" << i << ":" << disk_info_list.at(i) << "abc";

    qApp->processEvents();
    for (i=0; i < disk_info_list.length(); i++) {
        if (disk_info_list.at(i) == "Null" || disk_info_list.at(i) == "Bad") {
            continue;
        } else {
            cmd = "jw-aging disk_test speed " + QString::number(i+1);
            disk_speed=bash_cmd(cmd);
        }
       // qDebug() << "disk_speed:" <<disk_speed;
        disk_speed_list = disk_speed.split(" ");
        //qDebug() << "disk_speed_list[0]" << disk_speed_list.at(0) << " disk_speed_list[1]:" <<disk_speed_list.at(1);
        int m, n;
        if (disk_num == 16){
            m = (i%4)*2;
            n = (i/4)*2+1;
        } else if (disk_num == 8) {
            m = (i/4)*2;
            n = (i%4)*2 + 1;
        }

        QString read_speed="读：" + disk_speed_list.at(0);
        QString write_speed="写：" + disk_speed_list.at(1);
        ui->tableWidget_2->setItem(m, n, new QTableWidgetItem(read_speed));
        ui->tableWidget_2->setItem(m+1, n,new QTableWidgetItem(write_speed));
        ui->pushButton_8->setText("测试：" + QString::number(i+1));

        if (disk_speed_list.at(0).toInt() > DISK_MIN_READ && disk_speed_list.at(1).toInt() < DISK_MIN_WRITE) {
            ui->tableWidget_2->item(m, n-1)->setBackgroundColor(Qt::green);
        } else {
            ui->tableWidget_2->item(m, n-1)->setBackgroundColor(Qt::red);
        }
        qApp->processEvents();
    }
    ui->lineEdit_2->setText("测试完成");

}

void Widget::on_radioButton_clicked()
{
    QString cmd;
    cmd = "jw-aging sysled_test on";
    bash_cmd(cmd);
}

void Widget::on_radioButton_2_clicked()
{
    QString cmd;
    cmd = "jw-aging sysled_test off";
    bash_cmd(cmd);

}

void Widget::on_radioButton_3_clicked()
{
    QString cmd;
    cmd = "jw-aging diskled_test on";
    bash_cmd(cmd);
}

void Widget::on_radioButton_4_clicked()
{
    QString cmd;
    cmd = "jw-aging diskled_test off";
    bash_cmd(cmd);
}

void Widget::on_radioButton_5_clicked()
{
    QString cmd;
    cmd = "jw-aging buzzer_test on";
    bash_cmd(cmd);
}

void Widget::on_radioButton_6_clicked()
{
    QString cmd;
    cmd = "jw-aging buzzer_test off";
    bash_cmd(cmd);

}



void Widget::on_pushButton_12_clicked()
{
    ui->pushButton_12->setDisabled(true);
    QPalette pal = ui->lineEdit_3->palette();
    pal.setColor(QPalette::Text, Qt::green);
    ui->lineEdit_3->setPalette(pal);

    ui->lineEdit_3->setText("正在测试");
    QString cmd;
    QString out;
    int free;
    cmd = "jw-aging free_mem";
    out=bash_cmd(cmd);
    free = out.toInt() - 100;

    cmd = "xterm -T \"Memtester\" -e \"memtester " + QString::number(free) + " 1; read -p 'press any key to exit'\" &";
    system(cmd.toLatin1());
    ui->lineEdit_3->setText("测试完成");
    ui->pushButton_12->setEnabled(true);
}
