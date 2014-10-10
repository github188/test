#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QPalette>
#include <QThread>
#include <QMessageBox>
#include <unistd.h>

#define  VERSION   0.5
#define  DISK_MIN_READ  120
#define  DISK_MIN_WRITE  100
#define LOCKFILE "/run/lock/jw-aging.lock"

QString product_name;
int eth_num;
int disk_num;
QTimer *time_timer;
QTimer *mon_timer;
QTimer *disk_timer;
QTimer *mem_timer;
QTimer *net_timer;
QTimer *disk_speed_timer;
QStringList disk_info_list;
int aging_start=0;
int net_start=0;
int mem_start=0;
int disk_start=0;

long disk_time = 0;
long net_time = 0;
long mem_time = 0;

int disk_speed_now=0;

QString time_tostr(int time)
{
    int hour = 0;
    int min = 0;
    int sec = 0;

    hour = time / 3600;
    min = (time  - 3600*hour) / 60;
    sec = time - 3600*hour - 60*min;
    return ""+QString::number(hour) + "小时" +QString::number(min)+"分钟"+QString::number(sec)+"秒";
}

QString bash_cmd(QString cmd)
{
    QString cmd_bash = "bash -c \"" + cmd + "\"";
    QString out="";
    QProcess *process = new QProcess();
    //qDebug()<<"cmd:"<<cmd_bash;
    process->start(cmd_bash);

    process->waitForFinished(60000);

    out = process->readAllStandardOutput();
    if (out.endsWith("\n")) {
        out.replace((out.length()-1),1,"");
    }
    //qDebug()<<"out:"<<out;
    return out;
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString cmd;

    QPalette pal = ui->lineEdit_9->palette();
    pal.setColor(QPalette::Text,Qt::darkYellow);
    ui->lineEdit_9->setPalette(pal);

    pal = ui->lineEdit_10->palette();
    pal.setColor(QPalette::Text, Qt::darkYellow);
    ui->lineEdit_10->setPalette(pal);

    pal = ui->lineEdit_11->palette();
    pal.setColor(QPalette::Text, Qt::darkYellow);
    ui->lineEdit_11->setPalette(pal);


    ui->lineEdit_9->setText("未启动");
    ui->lineEdit_10->setText("未启动");
    ui->lineEdit_11->setText("未启动");

    ui->pushButton_8->setDisabled(true);

    QString title="JW测试工具 " + QString::number(VERSION);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle(title);


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
    } else if (product_name == "SYS-6036C-S(3U-C216)") {
            eth_num=5;
            disk_num=16;
      } else if (product_name == "SYS-6036Z-S(3U-Z77)") {
            eth_num=2;
            disk_num=16;
      }  else if (product_name == "CMS1100G2") {
        eth_num = 2;
        disk_num = 16;
    } else {

        QMessageBox::critical(this, tr("error"),
                                       tr("程序不支持当前设备的类型，请检测bios版本，程序即将退出..."),
                                       QMessageBox::Ok );
        exit(-1);

        eth_num=0;
        disk_num=0;                 
    }
    
    cmd = "jw-aging cpu_info";
    ui->lineEdit_5->setText(bash_cmd(cmd));

    cmd = "jw-aging memory_info";
    ui->textEdit->setText(bash_cmd(cmd));


    cmd = "jw-aging fireware";
    ui->textEdit_6->setText(bash_cmd(cmd));

    if (product_name != "SYS-6036C-T(3U-C216)" && product_name != "SYS-6036Z-T(3U-Z77") {
        ui->label_15->setHidden(true);
        ui->textEdit_6->setHidden(true);
        ui->pushButton_17->setHidden(true);
        ui->pushButton_18->setHidden(true);
    }

    if (product_name == "SYS-6036C-S(3U-C216)" || product_name == "SYS-6036Z-S(3U-Z77)") {
        QString raid_cmd = "init_raid init";

        QMessageBox msgBox;
        msgBox.setWindowTitle("警告");
        msgBox.setText("警告：在测试sas设备时， 可能会引起raid的丢失！");
        msgBox.setInformativeText("请选择是否继续?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Ok:
              // Save was clicked
              bash_cmd(raid_cmd);
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked
              exit (-1);
              break;
          default:
              // should never be reached
              break;
        }
      }

    cmd = "net_init &";
    bash_cmd(cmd);

    if (product_name == "SYS-6036C-S(3U-C216)" || product_name == "SYS-6036Z-S(3U-Z77)" || product_name == "CMS1100G2") {

        ui->label_21->setHidden(true);
        ui->radioButton_3->setHidden(true);
        ui->radioButton_4->setHidden(true);
        ui->pushButton_23->setHidden(true);
        ui->pushButton_24->setHidden(true);
    }

    ui->tableWidget->setColumnCount(eth_num);
    int i;
    for (i=0; i<eth_num; i++){
        QString name="eth" + QString::number(i);
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(name));
    }

    ui->tableWidget_4->setColumnCount(eth_num);
    for (i=0; i<eth_num; i++){
        QString name="eth" + QString::number(i);
        ui->tableWidget->setColumnWidth(i, 150);
        ui->tableWidget_4->setItem(0, i, new QTableWidgetItem(name));
    }


    if (disk_num == 16) {
        ui->tableWidget_2->setRowCount(8);
        ui->tableWidget_2->setColumnCount(8);

        ui->tableWidget_2->setColumnWidth(0,50);
        ui->tableWidget_2->setColumnWidth(2, 50);
        ui->tableWidget_2->setColumnWidth(4, 50);
        ui->tableWidget_2->setColumnWidth(6,50);
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




    if (disk_num == 16) {
        ui->tableWidget_3->setRowCount(8);
        ui->tableWidget_3->setColumnCount(8);

        ui->tableWidget_3->setColumnWidth(0,50);
        ui->tableWidget_3->setColumnWidth(2, 50);
        ui->tableWidget_3->setColumnWidth(4, 50);
        ui->tableWidget_3->setColumnWidth(6,50);
        int j, m, n;

        for (j=0, m=0; j<8; j+=2, m++) {

        for (i=0, n=0; i<8; i+=2, n++) {
            ui->tableWidget_3->setSpan(i, j, 2, 1);
            ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(4*m + n + 1)));
            }
        }
    } else if (disk_num == 8) {
        ui->tableWidget_3->setRowCount(4);
        ui->tableWidget_3->setColumnCount(8);
        int j, m, n;

        for (j=0, m=0; j<4; j+=2, m++) {
            for (i=0, n=0; i<8; i+=2, n++) {
                ui->tableWidget_3->setSpan(j,i, 2, 1);
                ui->tableWidget_3->setItem(j, i, new QTableWidgetItem(QString::number(4*m + n +1)));
            }

        }
    }

    time_timer = new QTimer(this);
    if (!time_timer->isActive()) {
        time_timer->start(1*1000);
    }

    connect(time_timer, SIGNAL(timeout()), this, SLOT(time_update()));

    //禁用内存老化（暂时）
    ui->pushButton_35->setDisabled(true);
}

Widget::~Widget()
{
    QString cmd;



    cmd = "killall iperf >/dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "killall net_init > /dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "killall nc > /dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "killall dd > /dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "killall memtester > /dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "killall .aging_* > /dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "jw-aging net_restory";
    bash_cmd(cmd);

    cmd = "rm -rf /tmp/old/* > /dev/null 2>&1";
    bash_cmd(cmd);

    cmd = "rm -rf /tmp/route > /dev/null 2>&1";
    bash_cmd (cmd);

    if (product_name == "SYS-6036C-S(3U-C216)" || product_name == "SYS-6036Z-S(3U-Z77)") {
       cmd = "init_raid restore &";
       bash_cmd(cmd);
     }
    delete ui;
}

void Widget::time_update()
{
    if ( disk_start == 2) {
        disk_time += 1;
        ui->lineEdit_10->setText(time_tostr(disk_time));
    }
    if ( net_start == 2 || net_start == 3) {
        net_time += 1;
        ui->lineEdit_9->setText(time_tostr(net_time));
    }
    if ( mem_start == 2) {
        mem_time += 1;
        ui->lineEdit_11->setText(time_tostr(mem_time));
    }
    qApp->processEvents();
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
    if(net_start==2 || net_start == 3) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("网络测试已经开始运行，请检查是否开启了老化测试！\n"),
                                       QMessageBox::Ok );
        return ;
    } else  {


    ui->pushButton_7->setDisabled(true);
    qApp->processEvents();

    int i;
    QString cmd;
    QString speed;
    QStringList speed_list;
    net_start=1;
    cmd = "jw-aging net_info";
    speed = bash_cmd(cmd);
    speed_list = speed.split("\n");
    for (i=0; i<eth_num; i++) {
        ui->tableWidget->setItem(2, i, new QTableWidgetItem(speed_list.at(i)));
    }

    cmd = "jw-aging net_mac";
    speed = bash_cmd(cmd);
    speed_list = speed.split("\n");
    for (i=0; i<eth_num; i++) {
        ui->tableWidget->setItem(1, i, new QTableWidgetItem(speed_list.at(i)));
    }

    cmd = "jw-aging net_speed_test_new";
    speed = bash_cmd(cmd);
    if (speed == "remote") {
        QMessageBox::critical(this, tr("Error"),
                              tr("网络测试已经由对测设备启动!\n"),
                              QMessageBox::Ok);

        ui->pushButton_7->setEnabled(true);
        net_start = 0;
        return ;
    }
    if (speed == "Null") {
        QMessageBox::critical(this, tr("Error"),
                              tr("启动网络测试失败!\n"),
                              QMessageBox::Ok);

        ui->pushButton_7->setEnabled(true);
        net_start = 0;
        return;
    }
    speed_list = speed.split("\n");
    qApp->processEvents();
    for (i=0; i<eth_num; i++) {
      ui->tableWidget->setItem(3, i, new QTableWidgetItem(speed_list.at(i)));
    }
    qApp->processEvents();

    ui->pushButton_7->setEnabled(true);
    net_start = 0;
    }
}

void Widget::on_pushButton_11_clicked()
{

    if(disk_start) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("磁盘测试已经开始运行，请检查是否开启了老化测试！\n"),
                                       QMessageBox::Ok );
        return ;
    } else {


    disk_start = 1;
    ui->pushButton_11->setDisabled(true);

    ui->tableWidget_2->clearContents();
    qApp->processEvents();
    int i;
    QString cmd;
    QString disk_info;

    if (product_name == "SYS-6036C-S(3U-C216)" || product_name == "SYS-6036Z-S(3U-Z77)") {
          cmd = "jw-aging get_raid";
          QString raid_info = bash_cmd(cmd);
          if (raid_info == "Null") {
              QMessageBox::warning(this, tr("Warning"),
                                             tr("没有检测到raid卡！\n"),
                                             QMessageBox::Ok );

              ui->pushButton_11->setEnabled(true);
              disk_start = 0;
              return;
            }

     }

    cmd = "jw-aging disk_test info";
    disk_info = bash_cmd(cmd);

    disk_info_list = disk_info.split("\n");
    for(i=0; i< disk_num && i < disk_info_list.length() ; i++) {
        int m, n;
        if (disk_num == 16) {
            m = (i%4)*2;
            n = (i/4)*2;
        }else if (disk_num == 8) {
            m = (i/4)*2;
            n = (i%4)*2;
        }

        QString name = QString::number(i+1) + ":"+disk_info_list.at(i);
        ui->tableWidget_2->setItem(m, n, new QTableWidgetItem(name));
        if (disk_info_list.at(i) == "Null") {
            ui->tableWidget_2->item(m, n)->setBackgroundColor(Qt::gray);
        }
        if (disk_info_list.at(i) == "Bad") {
            ui->tableWidget_2->item(m,n)->setBackgroundColor(Qt::red);
        }

    }
    ui->pushButton_8->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    disk_start = 0;
}
}

void Widget::on_pushButton_8_clicked()
{
    if(disk_start) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("磁盘测试已经开始运行，请检查是否开启了老化测试！\n"),
                                       QMessageBox::Ok );
        return ;
    } else {
       disk_start = 1;
     ui->pushButton_8->setDisabled(true);
     ui->pushButton_11->setDisabled(true);


   /*
    QString disk_info;
    QStringList disk_info_list;
    cmd = "jw-aging disk_test info";
    disk_info = bash_cmd(cmd);

    disk_info_list = disk_info.split("\n");

        //QString name= QString::number(i+1) + ":" + disk_info_list.at(i);
        //ui->tableWidget_2->setItem(m, n, new QTableWidgetItem(name));
      //  qDebug() << "i" << i << ":" << disk_info_list.at(i) << "abc";
    */

    ui->tableWidget_2->setDisabled(true);
    QString cmd = "jw-aging disk_test speed";
    system(cmd.toLatin1());

    disk_speed_timer = new QTimer(this);
    if (!disk_speed_timer->isActive()) {
        disk_speed_timer->start(20*1000);
    }

    connect(disk_speed_timer, SIGNAL(timeout()), this, SLOT(get_disk_speed()));

}
}

void Widget::get_disk_speed()
{


    while (disk_speed_now < disk_num && disk_speed_now < disk_info_list.length() && disk_info_list.at(disk_speed_now) == "Null") {
        int m, n;
        if (disk_num == 16){
            m = (disk_speed_now%4)*2;
            n = (disk_speed_now/4)*2+1;
        } else if (disk_num == 8) {
            m = (disk_speed_now/4)*2;
            n = (disk_speed_now%4)*2 + 1;
        }
        ui->tableWidget_2->setItem(m, n, new QTableWidgetItem("Null"));
        ui->tableWidget_2->setItem(m+1, n,new QTableWidgetItem("Null"));
        disk_speed_now++;
    }
    if (disk_speed_now >= disk_num || disk_speed_now >= disk_info_list.length()) {
        if(disk_speed_timer->isActive()) {
            disk_speed_timer->stop();
        }
        disk_speed_now = 0;
        ui->tableWidget_2->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_11->setEnabled(true);

        disk_start = 0;
        return ;
    }

    QString cmd = "jw-aging disk_test get_speed " + QString::number(disk_speed_now+1);
    QString out = bash_cmd(cmd);
    QStringList speed = out.split(" ");

        int m, n;
        if (disk_num == 16){
            m = (disk_speed_now%4)*2;
            n = (disk_speed_now/4)*2+1;
        } else if (disk_num == 8) {
            m = (disk_speed_now/4)*2;
            n = (disk_speed_now%4)*2 + 1;
        }

        QString read_speed="读：" + speed.at(0);
        QString write_speed="写：" + speed.at(1);


        ui->tableWidget_2->setItem(m, n, new QTableWidgetItem(read_speed));
        ui->tableWidget_2->setItem(m+1, n,new QTableWidgetItem(write_speed));
        if (speed.at(0).toInt() < DISK_MIN_READ ) {
             ui->tableWidget_2->item(m,n)->setBackgroundColor(Qt::red);
        } else {
             ui->tableWidget_2->item(m,n)->setBackgroundColor(Qt::green);
        }

        if (speed.at(1).toInt() < DISK_MIN_WRITE ) {
             ui->tableWidget_2->item(m+1,n)->setBackgroundColor(Qt::red);
        } else {
             ui->tableWidget_2->item(m+1,n)->setBackgroundColor(Qt::green);
        }
        disk_speed_now++;

        while (disk_speed_now < disk_num && disk_speed_now < disk_info_list.length() && disk_info_list.at(disk_speed_now) == "Null") {
            int m, n;
            if (disk_num == 16){
                m = (disk_speed_now%4)*2;
                n = (disk_speed_now/4)*2+1;
            } else if (disk_num == 8) {
                m = (disk_speed_now/4)*2;
                n = (disk_speed_now%4)*2 + 1;
            }
            ui->tableWidget_2->setItem(m, n, new QTableWidgetItem("Null"));
            ui->tableWidget_2->setItem(m+1, n,new QTableWidgetItem("Null"));
            disk_speed_now++;
        }
        if (disk_speed_now >= disk_num || disk_speed_now >= disk_info_list.length()) {
            if(disk_speed_timer->isActive()) {
                disk_speed_timer->stop();
            }
            disk_speed_now = 0;
            ui->tableWidget_2->setEnabled(true);
            ui->pushButton_8->setEnabled(true);
            ui->pushButton_11->setEnabled(true);

            disk_start = 0;
            return ;
        }

        qApp->processEvents();
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
    cmd = "jw-aging sysled_test foff";
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
    cmd = "jw-aging buzzer_test foff";
    bash_cmd(cmd);

}

void Widget::on_tabWidget_currentChanged(int index)
{
    if (index == 1) {

    }
}

void Widget::on_pushButton_19_clicked()
{
    ui->pushButton_19->setDisabled(true);
    if (disk_start) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("磁盘老化测试启动失败，请检查是否开启了磁盘的功能测试！\n"),
                                       QMessageBox::Ok );
        ui->pushButton_19->setEnabled(true);
        return ;
    }

    disk_start=2;
    QString cmd;
    if (product_name == "SYS-6036C-S(3U-C216)" || product_name == "SYS-6036Z-S(3U-Z77)") {
          cmd = "jw-aging get_raid";
          QString raid_info = bash_cmd(cmd);
          if (raid_info == "Null") {
              QMessageBox::warning(this, tr("Warning"),
                                             tr("没有检测到raid卡！\n"),
                                             QMessageBox::Ok );

              ui->pushButton_19->setEnabled(true);
              disk_start = 0;
              return;
            }

     }

    cmd = "jw-aging aging_io_start";
    bash_cmd(cmd);
    disk_timer = new QTimer(this);
    if (!disk_timer->isActive()) {
        disk_timer->start(30*1000);
    }

    connect(disk_timer, SIGNAL(timeout()), this, SLOT(get_disk_status()));
    QPalette pal = ui->lineEdit_10->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_10->setPalette(pal);

}

void Widget::on_pushButton_20_clicked()
{
    ui->pushButton_20->setDisabled(true);
   if (disk_start != 2) {
       QMessageBox::warning(this, tr("Warning"),
                                      tr("未启动磁盘老化测试\n"),
                                      QMessageBox::Ok );
       ui->pushButton_20->setEnabled(true);
       return ;
   }

    QString cmd = "jw-aging  aging_io_stop";
    bash_cmd(cmd);

    if (disk_timer->isActive()) {
        disk_timer->stop();
    }
    disk_start=0;
    disk_time = 0;
    QPalette pal = ui->lineEdit_10->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_10->setPalette(pal);
    ui->lineEdit_10->setText("已经停止");

    ui->pushButton_19->setEnabled(true);
    ui->pushButton_20->setEnabled(true);

}

void Widget::on_pushButton_33_clicked()
{
    ui->pushButton_33->setDisabled(true);

    if (net_start==1) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("网络老化测试启动失败，请检查是否开启了网络的功能测试！\n"),
                                       QMessageBox::Ok );
        ui->pushButton_33->setEnabled(true);
        return ;
    }
    qApp->processEvents();

    QString cmd = "jw-aging aging_net_start";
    QString out;
    out = bash_cmd(cmd);

    if (out == "Null") {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("网络老化测试启动失败！\n"),
                                       QMessageBox::Ok );
        ui->pushButton_33->setEnabled(true);
        return ;
    }

    if (out == "remote") {
        time_t t;
        time(&t);
        FILE *fp = NULL;
        char buf[20];
        if ( (fp = fopen(LOCKFILE, "r+")) != NULL) {
           if(fgets(buf, sizeof(buf)-1, fp) != NULL) {
               net_time = t - atol(buf);
           }
              fclose(fp);
        }
        net_start = 3;
    } else {
        net_start = 2;
    }

    net_timer = new QTimer(this);
    if (!net_timer->isActive()) {
        net_timer->start(10*1000);
    }

    connect(net_timer, SIGNAL(timeout()), this, SLOT(get_net_status()));
    QPalette pal = ui->lineEdit_9->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_9->setPalette(pal);

}

void Widget::on_pushButton_34_clicked()
{
    ui->pushButton_34->setDisabled(true);
    if (net_start == 0 || net_start == 1) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("未启动网络老化测试!\n"),
                                       QMessageBox::Ok );
        ui->pushButton_34->setEnabled(true);
        return;
    }
    if (net_start == 3) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("网络老化测试由对测设备启动，请在对测设备上关闭!\n"),
                                       QMessageBox::Ok );
        ui->pushButton_34->setEnabled(true);
        ui->pushButton_33->setEnabled(true);
        return;
    }

    QString cmd = "jw-aging aging_net_stop";
    bash_cmd(cmd);

    if (net_timer->isActive()) {
        net_timer->stop();
    }
    net_start=0;
    net_time = 0;
    QPalette pal = ui->lineEdit_9->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_9->setPalette(pal);
    ui->lineEdit_9->setText("已经停止");


    ui->pushButton_33->setEnabled(true);
    ui->pushButton_34->setEnabled(true);
}

void Widget::on_pushButton_35_clicked()
{
    ui->pushButton_35->setDisabled(true);
    if (mem_start) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("内存老化测试启动失败，请检查是否开启了内存的功能测试！\n"),
                                       QMessageBox::Ok );
        ui->pushButton_35->setEnabled(true);
        return ;
    }
    mem_start=2;
    QString cmd = "jw-aging aging_mem_start";
    bash_cmd(cmd);
    mem_timer = new QTimer(this);
    if (!mem_timer->isActive()) {
        mem_timer->start(10*1000);
    }

    connect(mem_timer, SIGNAL(timeout()), this, SLOT(get_mem_status()));
    QPalette pal = ui->lineEdit_11->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_11->setPalette(pal);

}


void Widget::on_pushButton_36_clicked()
{
    ui->pushButton_36->setDisabled(true);
    if (mem_start != 2) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("未启动内存老化测试!\n"),
                                       QMessageBox::Ok );
        ui->pushButton_36->setEnabled(true);
        return;
    }
    QString cmd = "jw-aging aging_mem_stop";
    bash_cmd(cmd);
    if (mem_timer->isActive()) {
        mem_timer->stop();
    }
    mem_start=0;
    mem_time = 0;
    QPalette pal = ui->lineEdit_11->palette();
    pal.setColor(QPalette::Text,Qt::green);
    ui->lineEdit_11->setPalette(pal);
    ui->lineEdit_11->setText("已经停止");

    ui->pushButton_35->setEnabled(true);
    ui->pushButton_36->setEnabled(true);
}


void Widget::get_disk_status()
{
    int i = 0;
    QString cmd = "jw-aging aging_io_status";
    QString out = bash_cmd(cmd);
    QStringList out_list = out.split("\n");
    for (i=0; i < disk_num && i < out_list.length(); i++) {


        if (out_list.at(i) == "Null Null") {
            int m, n;
            if (disk_num == 16) {
                m = (i%4)*2;
                n = (i/4)*2;
            }else if (disk_num == 8) {
                m = (i/4)*2;
                n = (i%4)*2;
            }

           ui->tableWidget_3->item(m, n)->setBackgroundColor(Qt::gray);

        } else {
        QStringList speed = out_list.at(i).split(" ");
        int m, n;
        if (disk_num == 16){
            m = (i%4)*2;
            n = (i/4)*2+1;
        } else if (disk_num == 8) {
            m = (i/4)*2;
            n = (i%4)*2 + 1;
        }
        QString read_speed="读：" + speed.at(0);
        QString write_speed="写：" + speed.at(1);
        ui->tableWidget_3->setItem(m, n, new QTableWidgetItem(read_speed));
        ui->tableWidget_3->setItem(m+1, n,new QTableWidgetItem(write_speed));
        }
        qApp->processEvents();
    }

}


void Widget::get_net_status()
{
    QString cmd = "jw-aging aging_net_status";
    QString out = bash_cmd(cmd);
    QStringList out_list = out.split("\n");

    for (int i=0; i<eth_num && i < out_list.length(); i++) {
        QStringList speed = out_list.at(i).split(" ");        

        ui->tableWidget_4->setItem(1, i, new QTableWidgetItem(speed.at(0)));
        ui->tableWidget_4->setItem(2, i,new QTableWidgetItem(speed.at(1)));
        qApp->processEvents();
    }

}
void Widget::get_mem_status()
{
    QString cmd = "jw-aging aging_mem_status";
    QString out = bash_cmd(cmd);
    ui->lineEdit_6->setText(out);



}
