#include "main_form.h"
#include "sysinfo_form.h"
#include "widget.h"
#include "ui_main_form.h"
#include <QTableWidget>
#include <QRadioButton>
#include <QDebug>
#include <QTimer>

extern QString tmp_dir;
extern QString tmp_file;
extern QString ip_address_remote;
int mon_int;
QString ip_address_local;

int disk_index_local=0;
int disk_index_remote=0;

Main_Form::Main_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Form)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle(tr("jw-test-tool"));

    //mon
    mon_timer = new QTimer(this);
    connect(mon_timer, SIGNAL(timeout()), this, SLOT(mon_update()));


    //disk
    ui->tableWidget_2->setWindowTitle("disk");
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setColumnWidth(0,50);
    ui->tableWidget_2->setColumnWidth(1,250);
    ui->tableWidget_2->setColumnWidth(2,80);
    ui->tableWidget_2->setColumnWidth(3,80);
    ui->tableWidget_2->setColumnWidth(4,80);
    ui->tableWidget_2->setColumnWidth(5,80);



    for (int i=0; i<16; i++) {

        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));


        button_group->addButton(pass_radioButton);


        /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */
       // connect(button_start, SIGNAL(clicked()), this, SIGNAL());
        ui->tableWidget_2->setCellWidget(i, 2, pass_radioButton);

    }

    for (int i=0; i<16; i++) {
        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));

        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */

        ui->tableWidget_2->setCellWidget(i, 3, pass_radioButton);

    }


    ui->tableWidget_3->setWindowTitle("disk");
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setColumnWidth(0,50);
    ui->tableWidget_3->setColumnWidth(1,250);
    ui->tableWidget_3->setColumnWidth(2,80);
    ui->tableWidget_3->setColumnWidth(3,80);
    ui->tableWidget_3->setColumnWidth(4,80);
    ui->tableWidget_3->setColumnWidth(5,80);



    for (int i=0; i<16; i++) {

        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));


        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */

        ui->tableWidget_3->setCellWidget(i, 2, pass_radioButton);

    }

    for (int i=0; i<16; i++) {
        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;



        pass_radioButton->setText(tr("通过"));

        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */



        ui->tableWidget_3->setCellWidget(i, 3, pass_radioButton);

    }





    //net

    ui->tableWidget_4->setColumnWidth(0, 50);
    ui->tableWidget_4->setColumnWidth(1, 80);
    ui->tableWidget_4->setColumnWidth(2, 80);
    ui->tableWidget_4->setColumnWidth(3, 80);
    ui->tableWidget_4->setColumnWidth(4, 80);
    ui->tableWidget_4->setColumnWidth(5, 80);


    ui->tableWidget_8->setColumnWidth(0, 50);
    ui->tableWidget_8->setColumnWidth(1, 80);
    ui->tableWidget_8->setColumnWidth(2, 80);
    ui->tableWidget_8->setColumnWidth(3, 80);
    ui->tableWidget_8->setColumnWidth(4, 80);
    ui->tableWidget_8->setColumnWidth(5, 80);


    for (int i=0; i<16; i++) {
        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));

        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */

        ui->tableWidget_4->setCellWidget(i, 2, pass_radioButton);

    }

    for (int i=0; i<16; i++) {
        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));

        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */

        ui->tableWidget_4->setCellWidget(i, 3, pass_radioButton);

    }



    for (int i=0; i<16; i++) {
        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));

        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */

        ui->tableWidget_8->setCellWidget(i, 2, pass_radioButton);

    }
    for (int i=0; i<16; i++) {
        QButtonGroup *button_group = new QButtonGroup;
        QRadioButton *pass_radioButton = new QRadioButton;

        pass_radioButton->setText(tr("通过"));

        button_group->addButton(pass_radioButton);

    /*
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("pass"));
    comBox->addItem(QObject::tr("fail"));
    comBox->setEnabled(true);

    comBox->baseSize();

    ui->tableWidget_2->setCellWidget(i, 2, comBox);
       */

        ui->tableWidget_8->setCellWidget(i, 3, pass_radioButton);

    }




}

Main_Form::~Main_Form()
{
    delete ui;
}

void Main_Form::on_tabWidget_currentChanged(int index)
{
    if (index == 2)
        setWindowState(Qt::WindowMaximized);
}

void Main_Form::on_pushButton_2_clicked()
{

    QString out;
    QString cmd;
    cmd = tmp_dir + "jw-aging monitor_test >" + tmp_file;
    out = bash_cmd_read(cmd, tmp_file);
    QStringList out_list;
    out_list = out.split(" ");

    ui->tableWidget->setItem(-1,2, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,2, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,2, new QTableWidgetItem(out_list.at(3)));

    out = ssh_read(cmd, tmp_file);
    out_list = out.split(" ");

    qDebug() << out;
    ui->tableWidget->setItem(-1,3, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,3, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,3, new QTableWidgetItem(out_list.at(3)));

    mon_int = ui->doubleSpinBox_7->value();


    if (!mon_timer->isActive()) {
         mon_timer->start(mon_int * 1000);
         qDebug() << "timer started.";
    }
}

void Main_Form::on_pushButton_clicked()
{
    if (mon_timer->isActive())
        mon_timer->stop();
}

void Main_Form:: mon_update()
{

    QString out;
    QString cmd;
    cmd = tmp_dir + "jw-aging monitor_test >" + tmp_file;
    out = bash_cmd_read(cmd, tmp_file);
    QStringList out_list;
    out_list = out.split(" ");

    ui->tableWidget->setItem(-1,2, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,2, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,2, new QTableWidgetItem(out_list.at(3)));

    out = ssh_read(cmd, tmp_file);
    out_list = out.split(" ");

    ui->tableWidget->setItem(-1,3, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,3, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,3, new QTableWidgetItem(out_list.at(3)));


}


void Main_Form::on_doubleSpinBox_7_valueChanged(const QString &arg1)
{

}

void Main_Form::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QString  cmd = tmp_dir + "jw-aging buzzer_test ";
    int index = ui->comboBox->currentIndex();
    if (index == 1) {
       cmd += "on";
       bash_cmd_read(cmd, tmp_file);
    } else if (index == 2){
        cmd += "off";
        bash_cmd_read(cmd, tmp_file);
    } else if (index == 3) {
        cmd += "on";
        ssh_cmd(cmd);
  } else if (index == 4) {
        cmd += "off";
        ssh_cmd(cmd);

    }else {
        cmd += "off";
        bash_cmd_read(cmd, tmp_file);
        ssh_cmd(cmd);
    }
}

void Main_Form::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QString cmd = tmp_dir + "jw-aging sysled_test ";
    int index = ui->comboBox_2->currentIndex();
    if (index == 1) {
        cmd += "on";
        bash_cmd_read(cmd, tmp_file);
    } else if (index == 2) {
        cmd += "off";
        bash_cmd_read(cmd, tmp_file);
    } else if (index == 3) {
        cmd += "on";
        ssh_cmd(cmd);
    } else if (index == 4) {
        cmd += "off";
        ssh_cmd(cmd);
    } else {
        cmd += "off";
        bash_cmd_read(cmd, tmp_file);
        ssh_cmd(cmd);
    }

}

void Main_Form::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    QString cmd=tmp_dir + "jw-aging diskled_test ";
    int index=ui->comboBox_3->currentIndex();

    if (index == 1) {
        cmd += "on";
        bash_cmd_read(cmd, tmp_file);
    } else if (index == 2) {
        cmd += "off";
        bash_cmd_read(cmd, tmp_file);
    } else if (index == 3) {
        cmd += "on";
        ssh_cmd(cmd);
    } else if (index == 4) {
        cmd += "off";
        ssh_cmd(cmd);
    } else {
        cmd += "off";
        bash_cmd_read(cmd, tmp_file);
        ssh_cmd(cmd);
    }

}



void Main_Form::on_pushButton_3_clicked()
{
    QString disk_info_local;
    QString disk_info_remote;
    QStringList disk_list_local;
    QStringList disk_list_remote;
    int i;
    QString cmd = tmp_dir + "jw-aging disk_test info >" + tmp_file;
    disk_info_local = bash_cmd_read(cmd, tmp_file);
    disk_list_local = disk_info_local.split("\n");

    for (i=0; i<disk_list_local.length(); i++)
       ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(disk_list_local.at(i)));

    disk_info_remote = ssh_read(cmd, tmp_file);
    disk_list_remote = disk_info_remote.split("\n");
    for (i=0; i<disk_list_remote.length(); i++)
     //qDebug() << i << disk_list_remote.at(i) << "\n";
         ui->tableWidget_3->setItem(i,1, new QTableWidgetItem(disk_list_remote.at(i)));
}


void Main_Form::disk_start_local(int row)
{
    qDebug() << "local" << row;
    QString cmd = tmp_dir + "jw-aging disk_test speed "+ QString::number(row+1) + " >" + tmp_file;
    QString speed_info;
    QStringList speed_info_list;

    speed_info = bash_cmd_read(cmd, tmp_file);
    qDebug() <<"speed_info:" << speed_info;
    speed_info_list = speed_info.split(" ");

    ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(speed_info_list.at(0)));
    ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(speed_info_list.at(1)));

}

void Main_Form::disk_start_remote(int row)
{
 qDebug() << "remote:" << row;
 QString cmd = tmp_dir + "jw-aging disk_test speed " + QString::number(row+1) + " >" + tmp_file;
 QString speed_info;
 QStringList speed_info_list;

 qDebug() << "cmd" << cmd;
 speed_info = ssh_read(cmd, tmp_file);
 qDebug() <<"speed_info:" << speed_info;
 speed_info_list = speed_info.split(" ");

 ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(speed_info_list.at(0)));
 ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(speed_info_list.at(1)));

}

void Main_Form::on_pushButton_4_clicked()
{

    if (disk_index_local < 16) {
        disk_start_local(disk_index_local);
        disk_index_local++;
        QString name = "测试slot:" + QString::number(disk_index_local+1);
        ui->pushButton_4->setText(name);
    } else {
        disk_index_local=0;
        ui->pushButton_4->setText("开始测试");
    }
}

void Main_Form::on_pushButton_5_clicked()
{
    if (disk_index_remote < 16) {
            disk_start_remote(disk_index_remote);
            disk_index_remote++;
            QString name = "测试slot:"+ QString::number(disk_index_remote+1);
            ui->pushButton_5->setText(name);
    } else {
        disk_index_remote = 0;
        ui->pushButton_5->setText("开始测试");
    }
}

void Main_Form::on_pushButton_6_clicked()
{
    QString cmd;
    QString net_speed_local;
    QString net_speed_remote;
    QStringList net_speed_list_local;
    QStringList net_speed_list_remote;
    int i;

    cmd = tmp_dir + "jw-aging ip_address >" + tmp_file;
    ip_address_local = bash_cmd_read(cmd, tmp_file);
    qDebug() << ip_address_local;

    cmd = tmp_dir + "jw-aging net_test remote " + ip_address_local + " >" + tmp_file;
    qDebug() << "cmd:" <<cmd << "\n";
    net_speed_remote = ssh_cmd(cmd);
    /*
    net_speed_list_remote = net_speed_remote.split("\n");
    for (i=0; i<net_speed_list_remote.length(); i++) {
        ui->tableWidget_8->setItem(i, 4, new QTableWidgetItem(net_speed_list_remote.at(i)));
    */

    cmd = tmp_dir + "jw-aging net_test local " +ip_address_remote + " >" + tmp_file;
    net_speed_local = bash_cmd_read(cmd, tmp_file);
    net_speed_list_local = net_speed_local.split("\n");

    qDebug() << "length:"<<net_speed_list_local.length() << "\n";
    for (i=0; i<net_speed_list_local.length(); i++) {
        qDebug() << i<<":" << net_speed_list_local.at(i) << "\n";
        ui->tableWidget_4->setItem(i, 4, new QTableWidgetItem(net_speed_list_local.at(i)));      
        ui->tableWidget_8->setItem(i, 4, new QTableWidgetItem(net_speed_list_local.at(i)));
    }
}

void Main_Form::on_pushButton_8_clicked()
{
    QString cmd = tmp_dir + "jw-aging net_test info >" + tmp_file;
    QString net_info_local;
    QString net_info_remote;
    QStringList net_info_list_local;
    QStringList net_info_list_remote;
    int i;
    net_info_local = bash_cmd_read(cmd, tmp_file);
    net_info_list_local = net_info_local.split("\n");
    for (i=0; i<net_info_list_local.length(); i++)
        ui->tableWidget_4->setItem(i, 1, new QTableWidgetItem(net_info_list_local.at(i)));

    net_info_remote = ssh_read(cmd, tmp_file);
    net_info_list_remote = net_info_remote.split("\n");
    for (i=0; i<net_info_list_remote.length(); i++)
        ui->tableWidget_8->setItem(i, 1, new QTableWidgetItem(net_info_list_remote.at(i)));
}

void Main_Form::on_pushButton_7_clicked()
{
    QString cmd = tmp_dir + "jw-aging ip_address >" + tmp_file;
    ip_address_local = bash_cmd_read(cmd, tmp_file);

    cmd = tmp_dir + "jw-aging aging_test start local " + ip_address_remote + " >"  + tmp_file;
    bash_cmd_read(cmd, tmp_file);
    /*cmd = tmp_dir + "jw-aging aging_test start remote " + ip_address_local + " >" + tmp_file;
    ssh_read(cmd, tmp_file);
    */
    cmd = "ssh -f root@"+ip_address_remote + " " + tmp_dir + "jw-aging aging_test start remote " + ip_address_local;\
    system(cmd.toLatin1());

}

void Main_Form::on_pushButton_9_clicked()
{
    QString cmd = tmp_dir + "jw-aging aging_test stop >" + tmp_file;
    bash_cmd_read(cmd, tmp_file);
    ssh_read(cmd, tmp_file);
}

void Main_Form::on_pushButton_10_clicked()
{
    QString cmd = tmp_dir + "jw-aging aging_test status >/tmp/out_local";
    QString out;
    out = bash_cmd_read(cmd, "/tmp/out_local");
    ui->textBrowser->setText(out);
    cmd = tmp_dir + "jw-aging aging_test status > /tmp/out_remote";
    out = ssh_read(cmd, "/tmp/out_remote");
    ui->textBrowser_2->setText(out);
}
