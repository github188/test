#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>
#include <QTableWidget>


namespace Ui {
class Main_Form;
}

class Main_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Form(QWidget *parent = 0);
    ~Main_Form();

    void disk_start_local(int row);
    void disk_start_remote(int row);
private slots:

    void on_tabWidget_currentChanged(int index);

    void on_doubleSpinBox_7_valueChanged(const QString &arg1);

    void mon_update();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_3_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();


    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::Main_Form *ui;
    QTimer *mon_timer;
};

#endif // MAIN_FORM_H
