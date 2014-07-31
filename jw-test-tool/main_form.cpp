#include "main_form.h"
#include "sysinfo_form.h"
#include "widget.h"
#include "ui_main_form.h"
#include <QTableWidget>
#include <QRadioButton>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>


extern QString bin_dir;
extern QString tmp_file;
extern QString tmp_file_local;
extern QString tmp_file_remote;
extern QString ip_address_remote;
extern int single;
extern QString disk_num_local;
extern QString disk_num_remote;
extern QString eth_num_local;
extern QString eth_num_remote;
extern QString product_name_local;
extern QString product_name_remote;
extern QString log_local;
extern QString log_remote;

int mon_int;
QString ip_address_local;

QStringList disk_list_local;
QStringList disk_list_remote;

QString buzzer_result_local="unknown";
QString buzzer_result_remote="unknown";
QString sysled_result_local="unknown";
QString sysled_result_remote="unknown";
QString diskled_result_local="unknown";
QString diskled_result_remote="unknown";

void Main_Form::combox_changed(int index)
{
    int row;
    int column;
    QString str;
    QComboBox *combox = qobject_cast<QComboBox *>(sender());
    if (combox == 0) {
        return ;
    }
   QModelIndex modelindex = ui->tableWidget_2->indexAt(QPoint(combox->frameGeometry().x(), combox->frameGeometry().y()));
   row = modelindex.row();
   column = modelindex.column();
   if (column == 2) {
        str = get_time() + "disk "+ QString::number(row+1) + " blue led on: ";
   } else if (column == 3) {
        str = get_time() + "disk " + QString::number(row+1) + " blue led blink: ";
   }
   if (index == 1) {
       str += "pass\n";
        emit sig_combox_changed(row, column, str, 1);
    } else if (index == 2){
       str += "fail\n";
       emit sig_combox_changed(row, column, str, 1);
   }
}

void Main_Form::combox_changed_2(int index)
{
    int row;
    int column;
    QString str;
    QComboBox *combox = qobject_cast<QComboBox *>(sender());
    if (combox == 0) {
        return ;
    }
   QModelIndex modelindex = ui->tableWidget_3->indexAt(QPoint(combox->frameGeometry().x(), combox->frameGeometry().y()));
   row = modelindex.row();
   column = modelindex.column();
   if (column == 2) {
        str = get_time() + "disk "+ QString::number(row+1) + " blue led on: ";
   } else if (column == 3) {
        str = get_time() + "disk " + QString::number(row+1) + " blue led blink: ";
   }
   if (index == 1) {
       str += "pass\n";
        emit sig_combox_changed(row, column, str, 2);
    } else if (index == 2){
       str += "fail\n";
       emit sig_combox_changed(row, column, str, 2);
   }
}


void Main_Form::combox_changed_3(int index)
{
    int row;
    int column;
    QString str;
    QComboBox *combox = qobject_cast<QComboBox *>(sender());
    if (combox == 0) {
        return ;
    }
   QModelIndex modelindex = ui->tableWidget_4->indexAt(QPoint(combox->frameGeometry().x(), combox->frameGeometry().y()));
   row = modelindex.row();
   column = modelindex.column();
   if (column == 2) {
        str = get_time() + "eth"+ QString::number(row) + " green led on: ";
   } else if (column == 3) {
        str = get_time() + "eth" + QString::number(row) + " orange led blink: ";
   }
   if (index == 1) {
       str += "pass\n";
        emit sig_combox_changed(row, column, str, 3);
    } else if (index == 2){
       str += "fail\n";
       emit sig_combox_changed(row, column, str, 3);
   }
}

void Main_Form::combox_changed_4(int index)
{
    int row;
    int column;
    QString str;
    QComboBox *combox = qobject_cast<QComboBox *>(sender());
    if (combox == 0) {
        return ;
    }
   QModelIndex modelindex = ui->tableWidget_8->indexAt(QPoint(combox->frameGeometry().x(), combox->frameGeometry().y()));
   row = modelindex.row();
   column = modelindex.column();
   if (column == 2) {
        str = get_time() + "eth"+ QString::number(row) + " green led on: ";
   } else if (column == 3) {
        str = get_time() + "eth" + QString::number(row) + " orange led blink: ";
   }
   if (index == 1) {
       str += "pass\n";
        emit sig_combox_changed(row, column, str, 4);
    } else if (index == 2){
       str += "fail\n";
       emit sig_combox_changed(row, column, str, 4);
   }
}


void Main_Form::checked(int row, int column, QString sts, int id)
{
    //qDebug() << "row:" << row <<"  column:" << column << "  sts:" << sts << "  id:" << id;
    if (id%2) {
        log_write_local(sts);
    } else {
        log_write_remote(sts);
    }
}


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
    ui->tableWidget_2->setColumnWidth(1,350);
    ui->tableWidget_2->setColumnWidth(2,80);
    ui->tableWidget_2->setColumnWidth(3,80);
    ui->tableWidget_2->setColumnWidth(4,120);
    ui->tableWidget_2->setColumnWidth(5,120);

    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    int i;

    connect(this, SIGNAL(sig_combox_changed(int, int, QString, int)), this , SLOT(checked(int, int, QString, int)));

    for(i=0; i<disk_num_local.toInt(); i++) {
        ui->tableWidget_2->insertRow(i);
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        ui->tableWidget_2->setRowHeight(i, 35);
    }


    for (i=0; i<disk_num_local.toInt(); i++) {

       // QButtonGroup *button_group = new QButtonGroup;
      /*
        QPushButton *pass_button = new QPushButton;
        QPushButton *fail_button = new QPushButton;

        pass_button->setText(tr("通过"));
        fail_button->setText(tr("失败"));
        button_group->addButton(pass_button);
        button_group->addButton(fail_button);
        button_group->setExclusive(true);
    */

    QComboBox *comBox = new QComboBox();
     comBox->addItem(QObject::tr(""));
    comBox->addItem(QObject::tr("通过"));
    comBox->addItem(QObject::tr("失败"));
    comBox->setEnabled(true);

    ui->tableWidget_2->setCellWidget(i, 2, comBox);

       // connect(button_start, SIGNAL(clicked()), this, SIGNAL());
       // ui->tableWidget_2->setCellWidget(i, 2, pass_button);
        //ui->tableWidget_2->setCellWidget(i, 2, fail_button);



      connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed(int)));

    }




    for (i=0; i<disk_num_local.toInt(); i++) {

        QComboBox *comBox = new QComboBox();
         comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_2->setCellWidget(i, 3, comBox);

      connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed(int)));

    }

    ui->tableWidget_3->setWindowTitle("disk");
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setColumnWidth(0,50);
    ui->tableWidget_3->setColumnWidth(1,350);
    ui->tableWidget_3->setColumnWidth(2,80);
    ui->tableWidget_3->setColumnWidth(3,80);
    ui->tableWidget_3->setColumnWidth(4,120);
    ui->tableWidget_3->setColumnWidth(5,120);

    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setSelectionMode(QAbstractItemView::SingleSelection);

    for (i=0; i<disk_num_remote.toInt(); i++) {
        ui->tableWidget_3->insertRow(i);
        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
        ui->tableWidget_3->setRowHeight(i, 35);
    }

    for (i=0; i<disk_num_remote.toInt(); i++) {

        QComboBox *comBox = new QComboBox();
        comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_3->setCellWidget(i, 2, comBox);

      connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed_2(int)));

    }

    for (int i=0; i<disk_num_remote.toInt(); i++) {
        QComboBox *comBox = new QComboBox();
        comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_3->setCellWidget(i, 3, comBox);

      connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed_2(int)));
    }





    //net

    ui->tableWidget_4->setColumnWidth(0, 50);
    ui->tableWidget_4->setColumnWidth(1, 120);
    ui->tableWidget_4->setColumnWidth(2, 120);
    ui->tableWidget_4->setColumnWidth(3, 120);
    ui->tableWidget_4->setColumnWidth(4, 120);
    //ui->tableWidget_4->setColumnWidth(5, 120);


    ui->tableWidget_8->setColumnWidth(0, 50);
    ui->tableWidget_8->setColumnWidth(1, 120);
    ui->tableWidget_8->setColumnWidth(2, 120);
    ui->tableWidget_8->setColumnWidth(3, 120);
    ui->tableWidget_8->setColumnWidth(4, 120);
   // ui->tableWidget_8->setColumnWidth(5, 120);


    for (int i=0; i<eth_num_local.toInt(); i++) {
        QString name="eth"+QString::number(i);
        ui->tableWidget_4->insertRow(i);
        ui->tableWidget_4->setItem(i, 0, new QTableWidgetItem(name));


        QComboBox *comBox = new QComboBox();
        comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_4->setCellWidget(i, 2, comBox);
        connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed_3(int)));
    }

    for (int i=0; i<eth_num_local.toInt(); i++) {
        QComboBox *comBox = new QComboBox();
        comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_4->setCellWidget(i, 3, comBox);
        connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed_3(int)));
    }



    for (i=0; i<eth_num_remote.toInt(); i++) {
        QString name="eth"+QString::number(i);
        ui->tableWidget_8->insertRow(i);
        ui->tableWidget_8->setItem(i, 0, new QTableWidgetItem(name));

        QComboBox *comBox = new QComboBox();
        comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_8->setCellWidget(i, 2, comBox);
        connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed_4(int)));

    }
    for (int i=0; i<eth_num_remote.toInt(); i++) {
        QComboBox *comBox = new QComboBox();
        comBox->addItem(QObject::tr(""));
        comBox->addItem(QObject::tr("通过"));
        comBox->addItem(QObject::tr("失败"));
        comBox->setEnabled(true);

        ui->tableWidget_8->setCellWidget(i, 3, comBox);
        connect(comBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combox_changed_4(int)));

    }
    if (product_name_local != "SYS-6036C-T(3U-C216)") {
        ui->tableWidget_2->hideColumn(3);
    }
    if (product_name_remote != "SYS-6036C-T(3U-C216)") {
        ui->tableWidget_3->hideColumn(3);
    }
    if (single) {

        ui->tableWidget->removeColumn(1);
        ui->label_6->setHidden(true);
        ui->pushButton_13->setHidden(true);
        ui->pushButton_14->setHidden(true);
        ui->pushButton_17->setHidden(true);
        ui->pushButton_18->setHidden(true);
        ui->pushButton_21->setHidden(true);
        ui->pushButton_22->setHidden(true);

        ui->pushButton_5->setHidden(true);
        ui->pushButton_6->setHidden(true);

        ui->comboBox->removeItem(4);
        ui->comboBox->removeItem(3);
        ui->comboBox_2->removeItem(4);
        ui->comboBox_2->removeItem(3);
        ui->comboBox_3->removeItem(4);
        ui->comboBox_3->removeItem(3);


        ui->toolBox->removeItem(2);
        ui->toolBox->removeItem(1);
        ui->page_2->setHidden(true);
        ui->dockWidget_2->setHidden(true);

        ui->label_2->setHidden(true);
        ui->textBrowser_2->setHidden(true);
    }

}

Main_Form::~Main_Form()
{
    delete ui;

}


void Main_Form::on_tabWidget_currentChanged(int index)
{

    QString str;
    switch (index) {
    case 0:
        str = "\n\nMonitor\n";
        break;
    case 1:
        str = "\n\nLed and Buzzer\n";
        break;
    case 2:
        setWindowState(Qt::WindowMaximized);
        str = "\n\nDisk\n";
        break;
    case 3:
        str = "\n\nNet\n";
        break;
    case 4:
         str = "\n\nAging\n";
         break;
    case 5:
        str = "\n\nConsole\n";
        break;
    }

    log_write_local(str);
    log_write_remote(str);
}

void Main_Form::on_pushButton_2_clicked()
{

    QString out;
    QString cmd;

    ui->pushButton_2->setDisabled(true);
    ui->pushButton->setEnabled(true);

    QString str = "\n\nMonitor\n"+get_time()+"start monitor test\n";
    log_write_local(str);

    cmd = bin_dir + "jw-aging monitor_test >" + tmp_file_local;
    out = bash_cmd_read(cmd, tmp_file_local);
    QStringList out_list;
    out_list = out.split(" ");



    str = get_time() + "CPU temp: " + out_list.at(0) + "\n"+get_time() +"Case temp: " + out_list.at(1) + "\n" + get_time() + "fan1 speed: " \
            + out_list.at(2)+ "\n" + get_time() + "fan2 speed: " + out_list.at(3) + "\n";
    log_write_local(str);
    if (single) {
        ui->tableWidget->setItem(-2,2, new QTableWidgetItem(out_list.at(0)));
        ui->tableWidget->setItem(1,2, new QTableWidgetItem(out_list.at(2)));
        ui->tableWidget->setItem(2,2, new QTableWidgetItem(out_list.at(3)));
        ui->tableWidget->setItem(3,2, new QTableWidgetItem(out_list.at(1)));
    } else {

    ui->tableWidget->setItem(-1,2, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,2, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,2, new QTableWidgetItem(out_list.at(3)));
    ui->tableWidget->setItem(4,2, new QTableWidgetItem(out_list.at(1)));

    }

    if (!single) {
    str = "\n\nMonitor\n"+get_time()+"start monitor test\n";;
    log_write_remote(str);

    cmd = bin_dir + "jw-aging monitor_test >" + tmp_file_remote;
    out = ssh_read(cmd, tmp_file_remote);
    out_list = out.split(" ");

    ui->tableWidget->setItem(-1,3, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,3, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,3, new QTableWidgetItem(out_list.at(3)));
    ui->tableWidget->setItem(4,3, new QTableWidgetItem(out_list.at(1)));


    str = get_time() + "CPU temp: " + out_list.at(0) + "\n"+get_time() +"Case temp: " + out_list.at(1) + "\n" + get_time() + "fan1 speed: " \
            + out_list.at(2)+ "\n" + get_time() + "fan2 speed: " + out_list.at(3) + "\n";
    log_write_remote(str);
    }
    mon_int = ui->doubleSpinBox_7->value();


    if (!mon_timer->isActive()) {
         mon_timer->start(mon_int * 1000); 
    }
}

void Main_Form::on_pushButton_clicked()
{
    QString str;
    str = get_time() + "monitor stop\n";
    log_write_local(str);
    log_write_remote(str);

    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setDisabled(true);
    if (mon_timer->isActive())
        mon_timer->stop();
}

void Main_Form:: mon_update()
{

    QString out;
    QString cmd;
    QString str;

    cmd = bin_dir + "jw-aging monitor_test >" + tmp_file_local;
    out = bash_cmd_read(cmd, tmp_file_local);
    QStringList out_list;
    out_list = out.split(" ");
    str = get_time() + "CPU temp: " + out_list.at(0) + "\n"+get_time() +"Case temp: " + out_list.at(1) + "\n" + get_time() + "fan1 speed: " \
            + out_list.at(2)+ "\n" + get_time() + "fan2 speed: " + out_list.at(3) + "\n";
    log_write_local(str);

    if (single) {

        ui->tableWidget->setItem(-2,2, new QTableWidgetItem(out_list.at(0)));
        ui->tableWidget->setItem(1,2, new QTableWidgetItem(out_list.at(2)));
        ui->tableWidget->setItem(2,2, new QTableWidgetItem(out_list.at(3)));
        ui->tableWidget->setItem(3,2, new QTableWidgetItem(out_list.at(1)));
    } else {
    ui->tableWidget->setItem(-1,2, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,2, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,2, new QTableWidgetItem(out_list.at(3)));
    ui->tableWidget->setItem(4,2, new QTableWidgetItem(out_list.at(1)));
    }
    if (!single) {
    cmd = bin_dir + "jw-aging monitor_test >" + tmp_file_remote;
    out = ssh_read(cmd, tmp_file_remote);
    out_list = out.split(" ");

    str = get_time() + "CPU temp: " + out_list.at(0) + "\n"+get_time() +"Case temp: " + out_list.at(1) + "\n" + get_time() + "fan1 speed: " \
            + out_list.at(2)+ "\n" + get_time() + "fan2 speed: " + out_list.at(3) + "\n";
    log_write_remote(str);
    ui->tableWidget->setItem(-1,3, new QTableWidgetItem(out_list.at(0)));
    ui->tableWidget->setItem(2,3, new QTableWidgetItem(out_list.at(2)));
    ui->tableWidget->setItem(3,3, new QTableWidgetItem(out_list.at(3)));
    ui->tableWidget->setItem(4,3, new QTableWidgetItem(out_list.at(1)));
    }

}


void Main_Form::on_doubleSpinBox_7_valueChanged(const QString &arg1)
{
    if (mon_timer->isActive()) {
        mon_timer->stop();
    }
    mon_int = ui->doubleSpinBox_7->value();
    QString str = get_time() + "interval changed to " + QString::number(mon_int) + "\n";
    log_write_local(str);
    log_write_remote(str);
    if (!mon_timer->isActive()) {
         mon_timer->start(mon_int * 1000);
    }
}

void Main_Form::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QString  cmd = bin_dir + "jw-aging buzzer_test ";
    int index = ui->comboBox->currentIndex();
    if (index == 1) {
       cmd += "on";
       bash_cmd(cmd);
    } else if (index == 2){
        cmd += "foff";
        bash_cmd(cmd);
    } else if (index == 3) {
        cmd += "on";
        ssh_cmd(cmd);
  } else if (index == 4) {
        cmd += "foff";
        ssh_cmd(cmd);

    }else {
        cmd += "foff";
        bash_cmd(cmd);
        if (!single)
            ssh_cmd(cmd);
    }
}

void Main_Form::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QString cmd = bin_dir + "jw-aging sysled_test ";
    int index = ui->comboBox_2->currentIndex();
    if (index == 1) {
        cmd += "on";
        bash_cmd(cmd);
    } else if (index == 2) {
        cmd += "foff";
        bash_cmd(cmd);
    } else if (index == 3) {
        cmd += "on";
        ssh_cmd(cmd);
    } else if (index == 4) {
        cmd += "foff";
        ssh_cmd(cmd);
    } else {
        cmd += "foff";
        bash_cmd(cmd);
        if (!single)
            ssh_cmd(cmd);
    }

}

void Main_Form::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    QString cmd=bin_dir + "jw-aging diskled_test ";
    int index=ui->comboBox_3->currentIndex();

    if (index == 1) {
        cmd += "on";
        bash_cmd(cmd);
    } else if (index == 2) {
        cmd += "off";
        bash_cmd(cmd);
    } else if (index == 3) {
        cmd += "on";
        ssh_cmd(cmd);
    } else if (index == 4) {
        cmd += "off";
        ssh_cmd(cmd);
    } else {
        cmd += "off";
        bash_cmd(cmd);
        if (!single)
            ssh_cmd(cmd);
    }

}



void Main_Form::on_pushButton_3_clicked()
{
    QString disk_info_local;
    QString disk_info_remote;

    int i;
    QString cmd;
    QString str;

    cmd = bin_dir + "jw-aging disk_test info >" + tmp_file_local;
    disk_info_local = bash_cmd_read(cmd, tmp_file_local);
    disk_list_local = disk_info_local.split("\n");
    str = "disk info\n";
    for (i=0; i<disk_list_local.length()-1; i++) {
       ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(disk_list_local.at(i)));
       str += get_time() + "disk " + QString::number(i+1) + ": " + disk_list_local.at(i) + "\n";
       if (disk_list_local.at(i) == "Null") {
           QComboBox *combox;
           combox=(QComboBox *)ui->tableWidget_2->cellWidget(i, 2);
           combox->setDisabled(true);
           combox=(QComboBox *)ui->tableWidget_2->cellWidget(i, 3);
           combox->setDisabled(true);
       }
    }
    log_write_local(str);

    if (!single) {
    cmd = bin_dir + "jw-aging disk_test info >" + tmp_file_remote;
    disk_info_remote = ssh_read(cmd, tmp_file_remote);
    disk_list_remote = disk_info_remote.split("\n");
    str ="disk info\n";
    for (i=0; i<disk_list_remote.length()-1; i++) {
     //qDebug() << i << disk_list_remote.at(i) << "\n";
         ui->tableWidget_3->setItem(i,1, new QTableWidgetItem(disk_list_remote.at(i)));
         str += get_time() + "disk " + QString::number(i+1) + ": " + disk_list_remote.at(i) + "\n";
         if (disk_list_remote.at(i) == "Null") {
             QComboBox *combox;
             combox=(QComboBox *)ui->tableWidget_3->cellWidget(i, 2);
             combox->setDisabled(true);
             combox=(QComboBox *)ui->tableWidget_3->cellWidget(i, 3);
             combox->setDisabled(true);
         }
        }
    log_write_remote(str);
    }
}


void Main_Form::disk_start_local(int row)
{

    QString cmd;
    QString speed_info;
    QStringList speed_info_list;
    QString str;

        cmd = bin_dir + "jw-aging disk_test speed "+ QString::number(row+1) + " >" + tmp_file_local;
        speed_info = bash_cmd_read(cmd, tmp_file_local);
        speed_info_list = speed_info.split(" ");

        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(speed_info_list.at(0)));
        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(speed_info_list.at(1)));

        str = get_time() + "disk " + QString::number(row+1) + " read: " + speed_info_list.at(0);
        if (speed_info_list.at(0).toInt() > 120) {
            str += " pass\n";
        } else {
            str += " fail\n";
        }
        log_write_local(str);
        str = get_time() + "disk " + QString::number(row+1) + " write: " + speed_info_list.at(1);
        if (speed_info_list.at(1).toInt() > 100) {
            str += " pass\n";
        } else {
            str += " fail\n";
        }
        log_write_local(str);

}

void Main_Form::disk_start_remote(int row)
{

 QString cmd;
 QString speed_info;
 QStringList speed_info_list;
 QString str;


        cmd = bin_dir + "jw-aging disk_test speed " + QString::number(row+1)+ " >" + tmp_file_remote;

        speed_info = ssh_read(cmd, tmp_file_remote);
        speed_info_list = speed_info.split(" ");

        ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(speed_info_list.at(0)));
        ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(speed_info_list.at(1)));

        str = get_time() +"disk " + QString::number(row+1) + " read: " + speed_info_list.at(0) + " MB/s";
        if (speed_info_list.at(0).toInt() > 120) {
            str += " pass\n";
        } else {
            str += " fail\n";
        }
        log_write_remote(str);
        str = get_time() + "disk " + QString::number(row+1) + " write: " + speed_info_list.at(1) + " MB/s";
        if (speed_info_list.at(1).toInt() > 100) {
            str += " pass\n";
        } else {
            str += " fail\n";
        }
        log_write_remote(str);

}

void Main_Form::on_pushButton_4_clicked()
{
    int i;
    QString name;
    QString str;

    str = "\ndisk speed\n";
    log_write_local(str);
    ui->pushButton_4->setDisabled(true);

    for (i=0; i<disk_num_local.toInt(); i++) {
        name = "测试slot：" + QString::number(i+1);
        ui->pushButton_4->setText(name);
        qApp->processEvents();
        if (disk_list_local.at(i).contains("Null")) {
            ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(tr("Null")));
            ui->tableWidget_2->setItem(i, 5, new QTableWidgetItem(tr("Null")));
        } else {
            disk_start_local(i);
        }

        qApp->processEvents();
    }
    ui->pushButton_4->setText("开始测试");
    ui->pushButton_4->setEnabled(true);

}

void Main_Form::on_pushButton_5_clicked()
{
    int i;
    QString name;
    QString str;

    str = "\ndisk speed\n";
    log_write_remote(str);
    ui->pushButton_5->setDisabled(true);

    for (i=0; i<disk_num_remote.toInt(); i++) {
        name = "测试slot：" + QString::number(i+1);
        ui->pushButton_5->setText(name);
        qApp->processEvents();
        if (disk_list_remote.at(i).contains("Null")) {
            ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(tr("Null")));
            ui->tableWidget_3->setItem(i, 5, new QTableWidgetItem(tr("Null")));
        } else {
            disk_start_remote(i);
        }

        qApp->processEvents();
    }
    ui->pushButton_5->setText("开始测试");
    ui->pushButton_5->setEnabled(true);

}

void Main_Form::on_pushButton_6_clicked()
{
    QString cmd;
    QString net_speed_local;
    QString net_speed_remote;
    QStringList net_speed_list_local;
    QStringList net_speed_list_remote;
    QString str;
    int i;

    ui->pushButton_6->setDisabled(true);
    qApp->processEvents();
    cmd = bin_dir + "jw-aging ip_address >" + tmp_file_local;
    ip_address_local = bash_cmd_read(cmd, tmp_file_local);


    cmd = bin_dir + "jw-aging net_test remote " + ip_address_local;
    net_speed_remote = ssh_cmd(cmd);

    /*
    net_speed_list_remote = net_speed_remote.split("\n");
    for (i=0; i<net_speed_list_remote.length(); i++) {
        ui->tableWidget_8->setItem(i, 4, new QTableWidgetItem(net_speed_list_remote.at(i)));
    */

    cmd = bin_dir + "jw-aging net_test local " +ip_address_remote + " >" + tmp_file_local;
    net_speed_local = bash_cmd_read(cmd, tmp_file_local);
    net_speed_list_local = net_speed_local.split("\n");
    str = "\nnet adapter speed\n";
    for (i=0; i<net_speed_list_local.length(); i++) {
        //qDebug() << i<<":" << net_speed_list_local.at(i) << "\n";
        ui->tableWidget_4->setItem(i, 4, new QTableWidgetItem(net_speed_list_local.at(i)));      
        ui->tableWidget_8->setItem(i, 4, new QTableWidgetItem(net_speed_list_local.at(i)));
        str += get_time() + "eth" + QString::number(i) + " speed: " + net_speed_list_local.at(i) + " MB/s\n";
    }
    log_write_local(str);
    log_write_remote(str);

    ui->pushButton_6->setEnabled(true);
}

void Main_Form::on_pushButton_8_clicked()
{
    QString cmd;
    QString net_info_local;
    QString net_info_remote;
    QStringList net_info_list_local;
    QStringList net_info_list_remote;
    QString str;
    int i;

    cmd = bin_dir + "jw-aging net_test info >" + tmp_file_local;
    net_info_local = bash_cmd_read(cmd, tmp_file_local);
    net_info_list_local = net_info_local.split("\n");
    str = "net adpater info\n";
    for (i=0; i<net_info_list_local.length()-1; i++) {
        ui->tableWidget_4->setItem(i, 1, new QTableWidgetItem(net_info_list_local.at(i)));
        str += get_time() + "eth" + QString::number(i) + " link speed: " + net_info_list_local.at(i) + " Mb/s\n";
        if (net_info_list_local.at(i) == "Null" || net_info_list_local.at(i) == "Unknown!") {
            QComboBox *combox;
            combox=(QComboBox *)ui->tableWidget_4->cellWidget(i, 2);
            combox->setDisabled(true);
            combox=(QComboBox *)ui->tableWidget_4->cellWidget(i, 3);
            combox->setDisabled(true);
        }
    }
    log_write_local(str);

    if (!single) {
    cmd = bin_dir + "jw-aging net_test info >" + tmp_file_remote;
    net_info_remote = ssh_read(cmd, tmp_file_remote);
    net_info_list_remote = net_info_remote.split("\n");
    str = "net adpater info\n";
    for (i=0; i<net_info_list_remote.length()-1; i++) {
        ui->tableWidget_8->setItem(i, 1, new QTableWidgetItem(net_info_list_remote.at(i)));
        str += get_time() + "eth" + QString::number(i) + " link speed: " + net_info_list_remote.at(i) + " Mb/s\n";
        if (net_info_list_remote.at(i) == "Null" || net_info_list_remote.at(i) == "Unknown!") {
            QComboBox *combox;
            combox=(QComboBox *)ui->tableWidget_8->cellWidget(i, 2);
            combox->setDisabled(true);
            combox=(QComboBox *)ui->tableWidget_8->cellWidget(i, 3);
            combox->setDisabled(true);
        }
        }
    log_write_remote(str);
    }
}
void Main_Form::on_pushButton_7_clicked()
{
    QString cmd;
    QString str;

    str = get_time() + "start aging test\n";
    ui->pushButton_7->setDisabled(true);

    qApp->processEvents();
    cmd = bin_dir + "jw-aging ip_address >" + tmp_file_local;
    ip_address_local = bash_cmd_read(cmd, tmp_file_local);

    if (single) {
        cmd = bin_dir + "jw-aging aging_test start single";
    } else {
        cmd = bin_dir + "jw-aging aging_test start local " + ip_address_remote + " " + ip_address_local;
    }
    bash_cmd(cmd);



    QString out;
    cmd = bin_dir + "jw-aging aging_test status >" + tmp_file_local;
    out = bash_cmd_read(cmd, tmp_file_local);
    ui->textBrowser->setText(out);
    str += out;
    log_write_local(str);

    if (!single) {
    str = get_time() + "start aging test\n";
    cmd = bin_dir + "jw-aging aging_test status >" + tmp_file_remote;
    out = ssh_read(cmd, tmp_file_remote);
    ui->textBrowser_2->setText(out);
    str += out;
    log_write_remote(str);
    }
    ui->pushButton_9->setEnabled(true);
}

void Main_Form::on_pushButton_9_clicked()
{
    QString cmd;
    QString str;
    ui->pushButton_9->setDisabled(true);
    qApp->processEvents();


    cmd = bin_dir + "jw-aging aging_test stop";
    bash_cmd(cmd);
    str = get_time() + "stop aging test\n";
    log_write_local(str);

    if (!single) {
    cmd = bin_dir + "jw-aging aging_test stop";
    ssh_cmd(cmd);
    str = get_time() + "stop aging test\n";
    log_write_remote(str);
    }
     ui->pushButton_7->setEnabled(true);
}

void Main_Form::on_pushButton_10_clicked()
{
    QString cmd;
    QString out;
    QString str;

    ui->pushButton_10->setDisabled(true);
    qApp->processEvents();
    cmd = bin_dir + "jw-aging aging_test status >" + tmp_file_local;
    out = bash_cmd_read(cmd, tmp_file_local);
    ui->textBrowser->setText(out);
    str = out + "\n\n";
    log_write_local(str);

    if (!single) {
    cmd = bin_dir + "jw-aging aging_test status >" + tmp_file_remote;
    out = ssh_read(cmd, tmp_file_remote);
    ui->textBrowser_2->setText(out);
    str = out + "\n\n";
    log_write_remote(str);
    }
     ui->pushButton_10->setEnabled(true);
 }

void Main_Form::on_pushButton_23_clicked()
{
    QString str = ui->textEdit->toPlainText();
    QString cmd = "echo -e \""+str+ "\" > /dev/ttyS0";
    //qDebug() << "str:" << str << "\n" <<"cmd:" << cmd;
    QString out;
    bash_cmd(cmd);

}

void Main_Form::on_pushButton_24_clicked()
{
    QString cmd = "stty -F /dev/ttyS0 ispeed 115200 ospeed 115200 cs8 -parenb -cstopb clocal cread raw -echo";
    bash_cmd(cmd);
    //ssh_cmd(cmd);
    cmd = "xterm -e cat /dev/ttyS0 &";
    bash_cmd(cmd);
}

void Main_Form::on_pushButton_11_clicked()
{
    ui->pushButton_11->setDisabled(true);
    ui->pushButton_12->setEnabled(true);

    buzzer_result_local = "fail";
    QString str = get_time() + "buzzer: " + buzzer_result_local + "\n";
    log_write_local(str);
}

void Main_Form::on_pushButton_12_clicked()
{
    ui->pushButton_12->setDisabled(true);
    ui->pushButton_11->setEnabled(true);

    buzzer_result_local = "pass";
    QString str = get_time() + "buzzer: " + buzzer_result_local + "\n";
    log_write_local(str);
}

void Main_Form::on_pushButton_13_clicked()
{
    ui->pushButton_13->setDisabled(true);
    ui->pushButton_14->setEnabled(true);

    buzzer_result_remote = "fail";
    QString str = get_time() + "buzzer: " + buzzer_result_remote + "\n";
    log_write_remote(str);
}

void Main_Form::on_pushButton_14_clicked()
{
    ui->pushButton_14->setDisabled(true);
    ui->pushButton_13->setEnabled(true);

    buzzer_result_remote = "pass";
    QString str = get_time() + "buzzer: " + buzzer_result_remote + "\n";
    log_write_remote(str);
}


void Main_Form::on_pushButton_15_clicked()
{
    ui->pushButton_15->setDisabled(true);
    ui->pushButton_16->setEnabled(true);

    sysled_result_local = "fail";
    QString str = get_time() + "sysled: " + sysled_result_local + "\n";
    log_write_local(str);
}

void Main_Form::on_pushButton_16_clicked()
{
    ui->pushButton_16->setDisabled(true);
    ui->pushButton_15->setEnabled(true);

    sysled_result_local = "pass";
    QString str = get_time() + "sysled: " + sysled_result_local + "\n";
    log_write_local(str);
}

void Main_Form::on_pushButton_17_clicked()
{
    ui->pushButton_17->setDisabled(true);
    ui->pushButton_18->setEnabled(true);

    sysled_result_remote = "fail";
    QString str = get_time() + "sysled: " + sysled_result_remote + "\n";
    log_write_remote(str);
}


void Main_Form::on_pushButton_18_clicked()
{
    ui->pushButton_18->setDisabled(true);
    ui->pushButton_17->setEnabled(true);

    sysled_result_remote = "pass";
    QString str = get_time() + "sysled: " + sysled_result_remote + "\n";
    log_write_remote(str);
}

void Main_Form::on_pushButton_19_clicked()
{
    ui->pushButton_19->setDisabled(true);
    ui->pushButton_20->setEnabled(true);

    diskled_result_local = "fail";
    QString str = get_time() +"diskled: " + diskled_result_local + "\n";
    log_write_local(str);
}

void Main_Form::on_pushButton_20_clicked()
{
    ui->pushButton_20->setDisabled(true);
    ui->pushButton_19->setEnabled(true);

    diskled_result_local = "pass";
    QString str = get_time() +"diskled: " + diskled_result_local + "\n";
    log_write_local(str);
}

void Main_Form::on_pushButton_21_clicked()
{
    ui->pushButton_21->setDisabled(true);
    ui->pushButton_22->setEnabled(true);

    diskled_result_remote = "fail";
    QString str = get_time() +"diskled: " + diskled_result_remote + "\n";
    log_write_remote(str);
}

void Main_Form::on_pushButton_22_clicked()
{
    ui->pushButton_22->setDisabled(true);
    ui->pushButton_21->setEnabled(true);

    diskled_result_remote = "pass";
    QString str = get_time() +"diskled: " + diskled_result_remote + "\n";
    log_write_remote(str);
}


void Main_Form::on_Main_Form_destroyed()
{
    QString cmd = bin_dir + "jw-aging store_to_usb " + log_local + " " + log_remote;
    bash_cmd(cmd);
}

