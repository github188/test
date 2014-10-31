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
    int work(int, int);
private slots:
    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
