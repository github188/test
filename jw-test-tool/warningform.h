#ifndef WARNINGFORM_H
#define WARNINGFORM_H

#include <QWidget>
#include <QString>
#include <QDialog>


namespace Ui {
class warningForm;
}

class warningForm : public QDialog
{
    Q_OBJECT

public:
    explicit warningForm(QWidget *parent = 0);
    void setinfo(QString info);
    ~warningForm();

private slots:
    void on_pushButton_clicked();

private:
    Ui::warningForm *ui;


};

#endif // WARNINGFORM_H
