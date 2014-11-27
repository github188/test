#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <unistd.h>

QString ip;
QString gateway;

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("网络设置");
    ui->lineEdit_2->setText("255.255.255.0");
    ui->lineEdit_2->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int ip_cfg(QString ip, QString gateway)
{

        QString cmd;
        int i=0;
        cmd = "killall dhclient";

        system(cmd.toLatin1());
        cmd = "ifconfig eth0 " + ip + " netmask 255.255.255.0 ";

        qDebug() << cmd;
        if (system(cmd.toLatin1()) != 0)
            i= 1;

        cmd = "route add default gw " + gateway;
        qDebug() << cmd;
        if (system(cmd.toLatin1()) != 0)
            i = i | (1 << 1);

        return i;
}
void MainWindow::on_pushButton_clicked()
{
    QString gate;
    int i;

   ip = ui->lineEdit->text();
   gate = ui->lineEdit_3->text();
   i=ip_cfg(ip, gate);
   if(i==0) {
        ui->textBrowser->setText("sucessed.\n");
   } else  if (i == 1) {
        ui->textBrowser->setText("ifconfig failed.\n");
   } else if (i==2) {
        ui->textBrowser->setText("route add failed.\n");
   } else if (i==3) {
       ui->textBrowser->setText("ifconfig failed and route add failed.\n");
   }


}


void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}



void MainWindow::on_lineEdit_returnPressed()
{
    QStringList ip_list;
    QString error="";
    int i, j;
    bool ok;

    ip=ui->lineEdit->text();


    ip_list = ip.split(".");
    if (ip_list.length() != 4) {
        error += "error: ip address 格式错误\n";
        goto error;
    }

     for (i=0; i<4; i++) {
            j = ip_list.at(i).toInt(&ok, 10);


            if ( ok && (j < 256 && j >= 0)){
              continue;

            } else {
                error += "error:ip address 非法\n";
                goto error;
            }

     }

   gateway="";
   for (i=0; i<3; i++) {
        gateway.append(ip_list.at(i));
        gateway.append(".");
   }
    gateway.append("1");

    ui->lineEdit_3->setText(gateway);

error:
     if (error.length() != 0) {
         ui->textBrowser->setText(error);
         ui->lineEdit->clear();
         ui->lineEdit_3->clear();
}
}

void MainWindow::on_pushButton_3_clicked()
{
    QString cmd;
    QString out;

    cmd = "killall dhclient";
    bash_cmd(cmd);

    cmd = "dhclient eth0";
    out = bash_cmd(cmd);
    ui->textBrowser->setText(out);


    cmd = "ip addr show eth0 | grep inet|grep -v inet6";
    out = bash_cmd(cmd);
    ui->textBrowser->setText(out);

}

void MainWindow::on_pushButton_4_clicked()
{
    QString cmd;
    QString out;
    cmd = "ntpdate time.nist.gov";
    out = bash_cmd(cmd);
    ui->textBrowser->setText(out);
    cmd = "date";
    out = bash_cmd(cmd);
    ui->textBrowser->setText(out);
}
