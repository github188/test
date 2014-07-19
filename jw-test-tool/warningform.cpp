#include "warningform.h"
#include "ui_warningform.h"

warningForm::warningForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warningForm)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    setParent(parent);

}

void warningForm::setinfo(QString info)
{
    ui->label->setText(info);


}

warningForm::~warningForm()
{
    delete ui;
}

void warningForm::on_pushButton_clicked()
{
    this->close();
}
