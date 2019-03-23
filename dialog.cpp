#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

   // ui->pushButtonPrevious->setFlat(true);
   // ui->pushButtonNext->setFlat(true);
    this->setWindowTitle(tr(u8"选图"));
    QPixmap pixmap = QPixmap(":/qrc/0.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButtonPrevious_clicked()
{
    tmp--;
    if(tmp<0)
        tmp=4;
    QPixmap pixmap = QPixmap(":/qrc/"+QString::number(tmp)+".png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
}

void Dialog::on_pushButtonNext_clicked()
{
    tmp++;
    if(tmp>4)
        tmp=0;
    QPixmap pixmap = QPixmap(":/qrc/"+QString::number(tmp)+".png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);
}

void Dialog::on_pushButtonConfirm_clicked()
{
    pics_sp=tmp;
    emit sedMsgConfirm();
    this->close();
}
