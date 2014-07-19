#include "errorform.h"


errorForm::errorForm(QWidget *parent) :
    QMessageBox(parent)

{


}

void errorForm::set(QString info)
{

}

errorForm::~errorForm()
{
    delete ui;
}

