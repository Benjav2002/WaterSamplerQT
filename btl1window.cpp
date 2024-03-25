#include "btl1window.h"
#include "ui_btl1window.h"

btl1window::btl1window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::btl1window)
{
    ui->setupUi(this);
}

btl1window::~btl1window()
{
    delete ui;
}
