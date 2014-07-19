#ifndef SYSINFO_FORM_H
#define SYSINFO_FORM_H

#include <QWidget>

namespace Ui {
class Sysinfo_Form;
}

class Sysinfo_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Sysinfo_Form(QWidget *parent = 0);
    ~Sysinfo_Form();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Sysinfo_Form *ui;
};

int ssh_cmd(QString arg);

QString open_read(QString path);

QString cmd_read(QString cmd, QString path);

QString bash_cmd_read(QString cmd, QString path);

QString ssh_read(QString cmd, QString path);

void log_write(QString str);

#endif // SYSINFO_FORM_H
