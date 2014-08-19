#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void mon_update();
    void get_disk_status();
    void get_net_status();
    void get_mem_status();
    void get_disk_speed();
private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_36_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
