#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <QMessageBox>
#include <QString>

#ifdef __cplusplus
extern "C" {
#endif

#include "./tdwy_io_board_api.h"

#define VERSION "0.1"
#define SERIAL_C "/dev/ttyS1"
#define SERIAL_B "/dev/ttyS0"
#define USB "/dev/ttyUSB0"
#define MAXSIZE 10240

static char buf[MAXSIZE];
static struct termios saved_usb_termios;
static uint8_t dido_in = 0xff;
static uint16_t dido_out = 0x0;

/*
static int io_board_save_usb(void)
{
    int fd;
    fd = open(USB, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return -1;
    }

    if (tcgetattr(fd, &saved_usb_termios) < 0) {
        return -1;
    }
    close(fd);
    return 0;
}

static int io_board_restore_usb(void)
{
    int fd;
    fd = open(USB, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return -1;
    }
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &saved_usb_termios) != 0) {
        return -1;
    }
    close(fd);
    return 0;

}
*/
static int io_board_init(char *path)
{
    int fd;
    struct termios newtio;
    fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        return -1;
    }
    bzero (&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL|CREAD;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_cflag &= ~PARENB;

    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);

    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 2;
    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) != 0) {
        return -1;
    }
    close(fd);
    return 0;
}


void Widget::work(int idx, int s)
{


    if (s) {
        dido_in = dido_in & ~(1 << (idx));
        // status = status | (1 << (idx * 2 + 1));
    } else {

        dido_in = dido_in | (1 << (idx)) ;
        //status = status ^ ~(1 << (idx * 2 + 1));
    }
   // fprintf(stderr, "dido_in: 0x%x\n", dido_in);


    if (io_board_write_byte_data(GP2_OUT_PORT_REG, dido_in) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("DIDO写入失败！"),
                              QMessageBox::Ok);

    }

    usleep(50000);

    uint8_t gp0_value = 0, gp1_value = 0;
    if (io_board_read_byte_data(GP0_IN_PORT_REG, &gp0_value) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("DIDO读取GP0错误!"),
                              QMessageBox::Ok);
    }
    if (io_board_read_byte_data(GP1_IN_PORT_REG, &gp1_value) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("DIDO读取GP1错误！"),
                              QMessageBox::Ok);
    }
    dido_out = ((gp1_value << 8) | gp0_value);

   // fprintf(stderr, "dido_out: 0x%x\n", dido_out);


    if ((dido_out & (1<<(idx*2)))) {
        ui->tableWidget->setItem(0,idx*2, new QTableWidgetItem("低"));
        ui->tableWidget->item(0, idx*2)->setBackground(Qt::red);
    } else {
        ui->tableWidget->setItem(0, idx*2, new QTableWidgetItem("高"));
        ui->tableWidget->item(0, idx*2)->setBackground(Qt::green);
    }
    if ((dido_out & (1<<(idx*2 + 1)))) {
        ui->tableWidget->setItem(0,idx*2 + 1, new QTableWidgetItem("低"));
        ui->tableWidget->item(0, idx*2+1)->setBackground(Qt::red);
    } else {
        ui->tableWidget->setItem(0, idx*2 + 1, new QTableWidgetItem("高"));
        ui->tableWidget->item(0, idx*2+1)->setBackground(Qt::green);
    }



    /*
    if ((dido_out & (1 << (idx*2))) == (dido_in & (1 << idx))) {
        ui->tableWidget_2->item(0, idx*2)->setBackground(Qt::green);
    } else {
        ui->tableWidget_2->item(0, idx*2)->setBackground(Qt::red);
    }
    if ((dido_out & (1 << (idx*2+1))) == (dido_in & (1 << idx))) {
        ui->tableWidget_2->item(0, idx*2+1)->setBackground(Qt::green);
    } else {
        ui->tableWidget_2->item(0, idx*2+1)->setBackground(Qt::red);
    }
*/

}

static void *read_thread(void *arg)
{
    FILE *fp;

    if ((fp = fopen(USB, "r")) != NULL) {
        memset(buf, sizeof(buf), 0);
        fgets(buf, MAXSIZE-1, fp);
        //fprintf(stderr, "output: %s", buf);

        fclose(fp);
        pthread_exit((void *)0);
    } else {
        pthread_exit((void *)-1);
    }

}

static void *write_thread(void *arg)
{
    FILE *fp;
    char *tmp = (char *)arg;
    if ((fp = fopen(SERIAL_C, "w")) != NULL) {
        fputs(tmp, fp);
       // fprintf(stderr, "input: %s", (char *)arg);

        fclose(fp);
        pthread_exit((void *)0);
    } else {
        pthread_exit((void *)-1);
    }
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString title="继电器测试工具 "+ QString(VERSION);
    //setWindowState(Qt::WindowMaximized);
    setMinimumSize(622, 638);
    setMaximumSize(622, 638);
    setWindowTitle(title);
    int i;
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(16);
    //ui->tableWidget_2->setRowCount(2);
    //ui->tableWidget_2->setColumnCount(16);

    for (i=0; i<16; i++) {
        ui->tableWidget->setItem(0,i, new QTableWidgetItem("高"));
        ui->tableWidget->item(0,i)->setBackgroundColor(Qt::green);
        // ui->tableWidget_2->setItem(0, i, new QTableWidgetItem(QString::number(i+1)));
        //ui->tableWidget_2->item(0,i)->setBackgroundColor(Qt::green);
    }
    /*
    for (i=0; i<16; i+=2) {
        ui->tableWidget_2->setSpan(1,i,1,2);

        ui->tableWidget_2->setItem(1,i,new QTableWidgetItem(QString::number(i/2+1)));
        ui->tableWidget_2->item(1,i)->setBackgroundColor(Qt::green);
    }
*/


    QPalette pal=ui->pushButton->palette();
    pal.setColor(QPalette::ButtonText, QColor(255,0,0));
    ui->pushButton->setPalette(pal);


    /*
    if (io_board_save_usb() != 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("保存USB默认设置失败！"),
                              QMessageBox::Ok);

    }
    */
    if (io_board_init(SERIAL_B) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("串口初始化失败"),
                              QMessageBox::Ok);
    }



    /*
    pthread_t tid_r;
    int err;
    void *ret1;
    err = pthread_create(&tid_r, NULL, read_thread, NULL);
    if (err != 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("创建读线程失败！"),
                              QMessageBox::Ok);
    }
    */
}

Widget::~Widget()
{
    delete ui;
}

//发送
void Widget::on_pushButton_clicked()
{
    pthread_t tid1, tid2;
    int err;
    void  *ret1, *ret2;
    char tmp[MAXSIZE];
    ui->pushButton->setDisabled(true);

    /*
    if (io_board_restore_usb() != 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("设置USB规范模式失败!"),
                              QMessageBox::Ok);

    }
    */
    strncpy(tmp, ui->textEdit->toPlainText().append('\n').toLatin1().data(), MAXSIZE-1);

    err = pthread_create(&tid1, NULL, read_thread, NULL);
    if (err != 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("创建读线程失败！"),
                              QMessageBox::Ok);

        ui->pushButton->setEnabled(true);
        return;
    }
    usleep(20);

    err = pthread_create(&tid2, NULL, write_thread, (void *)tmp);
    if (err != 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("创建写线程失败！"),
                              QMessageBox::Ok);
        ui->pushButton->setEnabled(true);
        return;
    }

    pthread_join(tid1, &ret1);
    pthread_join(tid2, &ret2);
    if (ret1) {
        QMessageBox::critical(this, tr("错误"),
                              tr("打开USB失败！"),
                              QMessageBox::Ok);
        ui->pushButton->setEnabled(true);
        return;
    }

    if (ret2) {
        QMessageBox::critical(this, tr("错误"),
                              tr("打开串口失败！"),
                              QMessageBox::Ok);
        ui->pushButton->setEnabled(true);
        return;
    }

    ui->textEdit_2->setText(buf);

    if (!strcmp(tmp, buf)) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::green);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("通过");
    } else {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

    }

    ui->pushButton->setEnabled(true);

}

//清空
void Widget::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
}

//闭合1
void Widget::on_pushButton_5_clicked()
{
    if (ui->pushButton_5->text() == "闭合") {
        work(0, 1);
        ui->pushButton_5->setText("断开");
    } else {
        work(0, 0);
        ui->pushButton_5->setText("闭合");
    }
}
//闭合2
void Widget::on_pushButton_6_clicked()
{

    if (ui->pushButton_6->text() == "闭合") {
        work(1, 1);
        ui->pushButton_6->setText("断开");
    } else {
        work(1, 0);
        ui->pushButton_6->setText("闭合");
    }
}

//闭合3
void Widget::on_pushButton_7_clicked()
{
    if (ui->pushButton_7->text() == "闭合") {
        work(2, 1);
        ui->pushButton_7->setText("断开");
    } else {
        work(2, 0);
        ui->pushButton_7->setText("闭合");
    }
}

//闭合4
void Widget::on_pushButton_8_clicked()
{
    if (ui->pushButton_8->text() == "闭合") {
        work(3, 1);
        ui->pushButton_8->setText("断开");
    } else {
        work(3, 0);
        ui->pushButton_8->setText("闭合");
    }
}

//闭合5
void Widget::on_pushButton_9_clicked()
{
    if (ui->pushButton_9->text() == "闭合") {
        work(4, 1);
        ui->pushButton_9->setText("断开");
    } else {
        work(4, 0);
        ui->pushButton_9->setText("闭合");
    }
}

//闭合6
void Widget::on_pushButton_10_clicked()
{
    if (ui->pushButton_10->text() == "闭合") {
        work(5, 1);
        ui->pushButton_10->setText("断开");
    } else {
        work(5, 0);
        ui->pushButton_10->setText("闭合");
    }
}

//闭合7
void Widget::on_pushButton_11_clicked()
{
    if (ui->pushButton_11->text() == "闭合") {
        work(6, 1);
        ui->pushButton_11->setText("断开");
    } else {
        work(6, 0);
        ui->pushButton_11->setText("闭合");
    }
}

//闭合8
void Widget::on_pushButton_12_clicked()
{
    if (ui->pushButton_12->text() == "闭合") {
        work(7, 1);
        ui->pushButton_12->setText("断开");
    } else {
        work(7, 0);
        ui->pushButton_12->setText("闭合");
    }
}
#ifdef __cplusplus
}
#endif

void Widget::on_pushButton_3_clicked()
{
    if (ui->pushButton_5->text() == "闭合") {
        work(0, 1);
        ui->pushButton_5->setText("断开");
    } else {
        work(0, 0);
        ui->pushButton_5->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_6->text() == "闭合") {
        work(1, 1);
        ui->pushButton_6->setText("断开");
    } else {
        work(1, 0);
        ui->pushButton_6->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_7->text() == "闭合") {
        work(2, 1);
        ui->pushButton_7->setText("断开");
    } else {
        work(2, 0);
        ui->pushButton_7->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_8->text() == "闭合") {
        work(3, 1);
        ui->pushButton_8->setText("断开");
    } else {
        work(3, 0);
        ui->pushButton_8->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_9->text() == "闭合") {
        work(4, 1);
        ui->pushButton_9->setText("断开");
    } else {
        work(4, 0);
        ui->pushButton_9->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_10->text() == "闭合") {
        work(5, 1);
        ui->pushButton_10->setText("断开");
    } else {
        work(5, 0);
        ui->pushButton_10->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_11->text() == "闭合") {
        work(6, 1);
        ui->pushButton_11->setText("断开");
    } else {
        work(6, 0);
        ui->pushButton_11->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

    if (ui->pushButton_12->text() == "闭合") {
        work(7, 1);
        ui->pushButton_12->setText("断开");
    } else {
        work(7, 0);
        ui->pushButton_12->setText("闭合");
    }
    qApp->processEvents();
    sleep(1);

}
