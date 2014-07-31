#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>
#include <QTableWidget>
#include <QRadioButton>


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
public slots:
    void checked(int row, int column, QString sts, int id);
    void combox_changed(int);
    void combox_changed_2(int);
    void combox_changed_3(int);
    void combox_changed_4(int);

signals:
    void sig_combox_changed(int, int, QString sts, int id);
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

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_Main_Form_destroyed();


private:
    Ui::Main_Form *ui;
    QTimer *mon_timer;
};

#endif // MAIN_FORM_H
