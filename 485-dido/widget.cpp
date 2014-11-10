#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <QMessageBox>
#include <QString>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "./tdwy_io_board_api.h"

#define VERSION "0.9"
#define USB "/dev/ttyUSB0"
#define MSG "abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n1234567890"
#define MAXSIZE 1024
#define TIMEOUT 5

char SERIAL_CHAR[16] = "/dev/ttyS0";
char gCommPort[16] = "/dev/ttyS1";
static char COM[16] =  "/dev/ttyS0";

static uint8_t dido_in = 0xff;
static uint16_t dido_out = 0x0;


struct msg {
    size_t len;
    char path[128];
    char data[MAXSIZE];
};

static int io_board_init_c(const char *path)
{
    int fd;
    struct termios newtio;
    uint32_t status;

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
    newtio.c_cc[VMIN] =  1;
    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) != 0) {
        return -1;
    }

    if (strcmp(path, SERIAL_CHAR) == 0) {
        ioctl ( fd, TIOCMGET, ( long unsigned int )&status );
        status &= ~TIOCM_RTS;
        //status |= TIOCM_RTS;
        ioctl ( fd, TIOCMSET, ( long unsigned int )&status );
    }
    close(fd);
    return 0;
}

static int io_board_init(const char *path)
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


int Widget::work(int idx, int s)
{
    int ret = 0;


    if (s) {
        dido_in = dido_in | (1 << (idx)) ;

       // status = status | (1 << (idx * 2 + 1));
   } else {
         dido_in = dido_in & ~(1 << (idx));
       //status = status ^ ~(1 << (idx * 2 + 1));
   }



   if (io_board_write_byte_data(GP2_OUT_PORT_REG, dido_in) < 0) {

       /*QMessageBox::critical(this, tr("错误"),
                              tr("DIDO写入失败！"),
                              QMessageBox::Ok); */
        ret = -1;
    }

    usleep(50000);

    uint8_t gp0_value = 0, gp1_value = 0;
    if (io_board_read_byte_data(GP0_IN_PORT_REG, &gp0_value) < 0) {
       /* QMessageBox::critical(this, tr("错误"),
                              tr("DIDO读取GP0错误!"),
                              QMessageBox::Ok); */
        ret = -1;
    }
    if (io_board_read_byte_data(GP1_IN_PORT_REG, &gp1_value) < 0) {
       /* QMessageBox::critical(this, tr("错误"),
                              tr("DIDO读取GP1错误！"),
                              QMessageBox::Ok); */
        ret = -1;
    }
    dido_out = ((gp1_value << 8) | gp0_value);
    //fprintf(stderr, "dido_in: 0x%x\n", dido_in);
    //fprintf(stderr, "dido_out: 0x%x\n", dido_out);


    if (dido_out & (1<<(idx*2))) {
        ui->tableWidget->setItem(0,idx*2, new QTableWidgetItem("低"));
        ui->tableWidget->item(0, idx*2)->setBackground(Qt::green);

    } else {
        ui->tableWidget->setItem(0, idx*2, new QTableWidgetItem("高"));
        ui->tableWidget->item(0, idx*2)->setBackground(Qt::yellow);
    }
    if (dido_out & (1<<(idx*2 + 1))) {
        ui->tableWidget->setItem(0,idx*2 + 1, new QTableWidgetItem("低"));
        ui->tableWidget->item(0, idx*2+1)->setBackground(Qt::green);

    } else {
        ui->tableWidget->setItem(0, idx*2 + 1, new QTableWidgetItem("高"));
        ui->tableWidget->item(0, idx*2+1)->setBackground(Qt::yellow);
    }

    //return ret;


    if ( (((dido_out & (1 << (idx*2))) == 0) && ((dido_in & (1 << idx)) == 0)) ||
         (((dido_out & (1 << (idx*2))) != 0) && ((dido_in & (1 << idx)) != 0)) ) {
        ui->tableWidget->item(0, idx*2)->setBackground(Qt::red);
        ret = -1;
    }

    if ( (((dido_out & (1 << (idx*2+1))) == 0) && ((dido_in & (1 << idx)) == 0)) ||
         (((dido_out & (1 << (idx*2+1))) != 0) && ((dido_in & (1 << idx)) != 0)) )  {
        ui->tableWidget->item(0, idx*2+1)->setBackground(Qt::red);
        ret = -1;
    }

    return ret;


}

static void *read_thread(void *arg)
{
    int fd;
    size_t n=0;
    ssize_t i;
    struct timeval oldtime, newtime;

    struct msg *msg = (struct msg *)arg;
    unsigned int status;

    if ((fd = open(msg->path, O_RDONLY|O_NONBLOCK)) > 0) {
        if (strcmp(msg->path, SERIAL_CHAR) == 0) {
            ioctl ( fd, TIOCMGET, ( long unsigned int )&status );
            status &= ~TIOCM_RTS;
            ioctl ( fd, TIOCMSET, ( long unsigned int )&status );
        }
        gettimeofday(&oldtime, NULL);
        while (n < msg->len) {
            if ((i = read(fd, &msg->data[n], msg->len)) > 0) {
                n += i;
                //fprintf(stderr, "read:%s \nlen: %d\nmsg_len: %d\n\n", msg->data, strlen(msg->data), msg->len);
            }
            gettimeofday(&newtime, NULL);
            if ((newtime.tv_sec * 1000000 + newtime.tv_usec - oldtime.tv_sec * 1000000 - oldtime.tv_usec) > TIMEOUT * 1000000) {
                pthread_exit((void *)-2);
            }
        }
        msg->data[msg->len] = '\0';
        close(fd);
        pthread_exit((void *)0);
    } else {
        pthread_exit((void *)-1);
    }

}

static void *write_thread(void *arg)
{
    int fd;
    uint32_t status;
    struct msg *msg = (struct msg *)arg;


    if ((fd = open(msg->path, O_WRONLY)) > 0) {

        if (strcmp(msg->path, SERIAL_CHAR) == 0) {
            /* Set RTS Pin to High level to enable the RS485 send */
            ioctl ( fd, TIOCMGET, ( long unsigned int )&status );
            status |= TIOCM_RTS;
            ioctl ( fd, TIOCMSET, ( long unsigned int )&status );
        }

        if (write(fd, msg->data, msg->len) != (ssize_t)msg->len) {
            fprintf(stderr, "%s: write error\n", msg->path);
        }

       // fprintf(stderr, "sended msg: %s\nlen: %d\nmsg_len: %d\n", msg->data, strlen(msg->data), msg->len);

        usleep(50*1000);

        /* Set RTS Pin to Low level to enable the RS485 recv */
        if (strcmp(msg->path, SERIAL_CHAR) == 0) {
            ioctl ( fd, TIOCMGET, ( long unsigned int )&status );
            status &= ~TIOCM_RTS;
            ioctl ( fd, TIOCMSET, ( long unsigned int )&status );
        }

        close(fd);
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

    QString title="DIDO及串口测试工具 "+ QString(VERSION);
    //setWindowState(Qt::WindowMaximized);
    setMinimumSize(590, 512);
    setMaximumSize(590, 512);
    setWindowTitle(title);
    int i;
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(16);
    //ui->tableWidget_2->setRowCount(2);
    //ui->tableWidget_2->setColumnCount(16);

    for (i=0; i<16; i++) {
        ui->tableWidget->setItem(0,i, new QTableWidgetItem("高"));
        ui->tableWidget->item(0,i)->setBackgroundColor(Qt::yellow);
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
    /*
    if (io_board_init_c(SERIAL_CHAR) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("串口初始化失败"),
                              QMessageBox::Ok);
    }
    if (io_board_init_c(USB) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("USB初始化失败"),
                              QMessageBox::Ok);
    }

    if (io_board_init(gCommPort) < 0) {
        QMessageBox::critical(this, tr("错误"),
                              tr("串口B初始化失败"),
                              QMessageBox::Ok);
    }
    */
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_3_clicked()
{

    int i, flag = 0;

    if (io_board_init(gCommPort) < 0) {
       /* QMessageBox::critical(this, tr("错误"),
                              tr("串口B初始化失败"),
                              QMessageBox::Ok); */

        QPalette pal = ui->lineEdit_2->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_2->setPalette(pal);
        ui->lineEdit_2->setText("失败");
        return;
    }

    for (i=0; i<8; i++) {
        if (work(i, 0) < 0) {
            flag = 1;
        }
        qApp->processEvents();
        sleep(1);
    }

    for (i=0; i<8; i++) {

        if (work(i, 1) < 0) {
            flag = 1;
        }
        qApp->processEvents();
        sleep(1);
    }

    if (flag) {

        QPalette pal = ui->lineEdit_2->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_2->setPalette(pal);
        ui->lineEdit_2->setText("失败");

    } else {
        QPalette pal = ui->lineEdit_2->palette();
        pal.setColor(QPalette::Text, Qt::green);
        ui->lineEdit_2->setPalette(pal);
        ui->lineEdit_2->setText("通过");
    }
}


void Widget::on_pushButton_13_clicked()
{
    pthread_t tid1, tid2;
    int err, flag = 0;
    void  *ret1, *ret2;
    struct msg write_msg, read_msg;

    ui->pushButton_13->setDisabled(true);

    if (io_board_init_c(SERIAL_CHAR) < 0) {
     /*   QMessageBox::critical(this, tr("错误"),
                              tr("串口C初始化失败"),
                              QMessageBox::Ok); */

        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

        ui->pushButton_13->setEnabled(true);
        return;
    }
    if (io_board_init_c(USB) < 0) {

        /* QMessageBox::critical(this, tr("错误"),
                              tr("USB初始化失败"),
                              QMessageBox::Ok); */

        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

        ui->pushButton_13->setEnabled(true);
        return;
    }

    write_msg.len = strlen(MSG);
    strncpy(write_msg.data, MSG, MAXSIZE-1);
    strncpy(write_msg.path, SERIAL_CHAR, 127);

    read_msg.len = write_msg.len;
    strncpy(read_msg.path, USB, 127);


    err = pthread_create(&tid1, NULL, read_thread, (void *)&read_msg);
    if (err != 0) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

        ui->pushButton_13->setEnabled(true);
        return;
    }

    usleep(20*1000);
    err = pthread_create(&tid2, NULL, write_thread, (void *)&write_msg);
    if (err != 0) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");
        ui->pushButton_13->setEnabled(true);
        return;
    }


    pthread_join(tid1, &ret1);
    pthread_join(tid2, &ret2);
    if (ret2) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");
        ui->pushButton_13->setEnabled(true);
        return;
    }

    if ((long)ret1 == -1) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");
        ui->pushButton_13->setEnabled(true);
        return;
    }
    if ((long)ret1 == -2) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");
        ui->pushButton_13->setEnabled(true);
        return;
    }
    if (!strcmp(read_msg.data, write_msg.data)) {
        flag = 0;
    } else {
        flag = 1;
    }

    usleep(500*1000);

    memset(&write_msg, 0, sizeof(write_msg));
    memset(&read_msg, 0, sizeof(read_msg));

    write_msg.len = strlen(MSG);
    strncpy(write_msg.data, MSG, MAXSIZE-1);
    strncpy(write_msg.path, USB, 127);

    read_msg.len = write_msg.len;
    strncpy(read_msg.path, SERIAL_CHAR, 127);

    err = pthread_create(&tid1, NULL, read_thread, (void *)&read_msg);
    if (err != 0) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

        ui->pushButton_13->setEnabled(true);
        return;
    }
    usleep(20*1000);

    err = pthread_create(&tid2, NULL, write_thread, (void *)&write_msg);
    if (err != 0) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

        ui->pushButton_13->setEnabled(true);
        return;
    }

    pthread_join(tid1, &ret1);
    pthread_join(tid2, &ret2);
    if (ret2) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");
        ui->pushButton_13->setEnabled(true);
        return;
    }

    if ((long)ret1 == -1) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");

        ui->pushButton_13->setEnabled(true);
        return;
    }

    if ((long)ret1 == -2) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit->setPalette(pal);
        ui->lineEdit->setText("失败");
        ui->pushButton_13->setEnabled(true);
        return;
    }


    if ((flag == 0 )&&!strcmp(read_msg.data, write_msg.data)) {
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
    ui->pushButton_13->setEnabled(true);

}


void Widget::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        strncpy(COM, "/dev/ttyS0", 16);
        break;
    case 1:
        strncpy(COM, "/dev/ttyS1", 16);
        break;
    case 2:
        strncpy(COM, "/dev/ttyS2", 16);
        break;
    case 3:
        strncpy(COM, "/dev/ttyS3", 16);
        break;
    case 4:
        strncpy(COM, "/dev/ttyS4", 16);
        break;
    case 5:
        strncpy(COM, "/dev/ttyS5", 16);
        break;
    default:
        strncpy(COM, "/dev/ttyS0", 16);
    }
    ui->lineEdit_3->clear();
}

void Widget::on_pushButton_14_clicked()
{
    pthread_t tid1, tid2;
    int err;
    void  *ret1, *ret2;
    struct msg write_msg, read_msg;

    ui->pushButton_14->setDisabled(true);

    if (io_board_init_c(COM) < 0) {

        /* QMessageBox::critical(this, tr("错误"),
                              tr("COM初始化失败"),
                              QMessageBox::Ok); */

        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");

        ui->pushButton_14->setEnabled(true);
        return;
    }

    write_msg.len = strlen(MSG);
    strncpy(write_msg.data, MSG, MAXSIZE-1);
    strncpy(write_msg.path, COM, 127);

    read_msg.len = write_msg.len;
    strncpy(read_msg.path, COM, 127);


    err = pthread_create(&tid1, NULL, read_thread, (void *)&read_msg);
    if (err != 0) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");

        ui->pushButton_14->setEnabled(true);
        return;
    }

    usleep(20*1000);
    err = pthread_create(&tid2, NULL, write_thread, (void *)&write_msg);
    if (err != 0) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");
        ui->pushButton_14->setEnabled(true);
        return;
    }


    pthread_join(tid1, &ret1);
    pthread_join(tid2, &ret2);
    if (ret2) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");
        ui->pushButton_14->setEnabled(true);
        return;
    }

    if ((long)ret1 == -1) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");
        ui->pushButton_14->setEnabled(true);
        return;
    }

    if ((long)ret1 == -2) {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");
        ui->pushButton_14->setEnabled(true);
        return;
    }
    if (!strcmp(read_msg.data, write_msg.data)) {
        QPalette pal = ui->lineEdit_3->palette();
        pal.setColor(QPalette::Text, Qt::green);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("通过");
    } else {
        QPalette pal = ui->lineEdit->palette();
        pal.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_3->setPalette(pal);
        ui->lineEdit_3->setText("失败");

    }
    ui->pushButton_14->setEnabled(true);
}


void Widget::on_comboBox_2_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        strncpy(gCommPort, "/dev/ttyS0", 16);
        break;
    case 1:
        strncpy(gCommPort, "/dev/ttyS1", 16);
        break;
    case 2:
        strncpy(gCommPort, "/dev/ttyS2", 16);
        break;
    case 3:
        strncpy(gCommPort, "/dev/ttyS3", 16);
        break;
    case 4:
         strncpy(gCommPort, "/dev/ttyS4", 16);
        break;
    case 5:
         strncpy(gCommPort, "/dev/ttyS5", 16);
        break;
    default:
         strncpy(gCommPort, "/dev/ttyS1", 16);
    }

    ui->lineEdit_2->clear();
}

void Widget::on_comboBox_3_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        strncpy(SERIAL_CHAR, "/dev/ttyS0", 16);
        break;
    case 1:
        strncpy(SERIAL_CHAR, "/dev/ttyS1", 16);
        break;
    case 2:
        strncpy(SERIAL_CHAR, "/dev/ttyS2", 16);
        break;
    case 3:
        strncpy(SERIAL_CHAR, "/dev/ttyS3", 16);
        break;
    case 4:
         strncpy(SERIAL_CHAR, "/dev/ttyS4", 16);
        break;
    case 5:
         strncpy(SERIAL_CHAR, "/dev/ttyS5", 16);
        break;
    default:
         strncpy(SERIAL_CHAR, "/dev/ttyS0", 16);
    }
    ui->lineEdit->clear();
}


#ifdef __cplusplus
}
#endif




