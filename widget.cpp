#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButtonRestart->setFlat(true);
    ui->pushButtonSelect->setFlat(true);
    ui->pushButtonShow->setFlat(true);

    QPixmap pixmap = QPixmap(":/qrc/timg.jpg").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

    timer =new QTimer(this);
    timer->setInterval(1000);

   on_pushButtonRestart_clicked();      //手动调用槽，随机放置图片(打乱拼图棋盘)

   buttons=new QButtonGroup(this);
   buttons->addButton(ui->pushButton01,1);
   buttons->addButton(ui->pushButton02,2);
   buttons->addButton(ui->pushButton03,3);
   buttons->addButton(ui->pushButton04,4);
   buttons->addButton(ui->pushButton05,5);
   buttons->addButton(ui->pushButton06,6);
   buttons->addButton(ui->pushButton07,7);
   buttons->addButton(ui->pushButton08,8);
   buttons->addButton(ui->pushButton09,9);
   buttons->addButton(ui->pushButton10,10);
   buttons->addButton(ui->pushButton11,11);
   buttons->addButton(ui->pushButton12,12);
   buttons->addButton(ui->pushButton13,13);
   buttons->addButton(ui->pushButton14,14);
   buttons->addButton(ui->pushButton15,15);
   buttons->addButton(ui->pushButton16,16);

   connect(buttons,SIGNAL(buttonClicked(int)),this,SLOT(RecvClicked(int)));
   //点击按钮组中的按钮将会发送点击信号并此按钮在组中的序号 ，带着序号转到槽
   connect(this,SIGNAL(sendMsgCompare(void)),this,SLOT(RecvIsComplete(void)));
   //RecvClicked(int)槽发射此信号，转到些槽比较拼图是否完成
   connect(timer,SIGNAL(timeout()),this,SLOT(RecvTimeOut()));
   //计时器中断信号和槽
   connect(Dialog1,SIGNAL(sedMsgConfirm(void)),this,SLOT(on_pushButtonRestart_clicked()));
   //选图窗体的 确认 信号关联到主窗体的 重排 槽
}

Widget::~Widget()
{
    delete ui;
}

void Widget::RecvClicked(int buttonID)
{
    bool f=0;
    if(buttons->button(buttonID)->objectName().toStdString()
            !=buttons->button(sp)->objectName().toStdString())
    {
        bIco=buttons->button(buttonID)->icon();     //暂存被点击的块的图片
        temp=randA[buttonID];                       //暂存被点击的块的序号
        if(sp==buttonID+1&&buttonID%4!=0)
        {

            buttons->button(buttonID)->setIcon(buttons->button(sp)->icon());
            buttons->button(sp)->setIcon(bIco);
            randA[buttonID]=randA[sp];
            randA[sp]=temp;
            sp=buttonID;
            f=1;
        }
            //交换白块和被点击的块的图标和位置，更新sp位置
        else if(sp==buttonID-1&&(buttonID-1)%4!=0)
        {
             buttons->button(buttonID)->setIcon(buttons->button(sp)->icon());
             buttons->button(sp)->setIcon(bIco);
             randA[buttonID]=randA[sp];
             randA[sp]=temp;
             sp=buttonID;
             f=1;
        }
        else if(buttonID+4==sp)
        {
            buttons->button(buttonID)->setIcon(buttons->button(sp)->icon());
            buttons->button(sp)->setIcon(bIco);
            randA[buttonID]=randA[sp];
            randA[sp]=temp;
            sp=buttonID;
            f=1;
        }
        else if(buttonID-4==sp)
        {
            buttons->button(buttonID)->setIcon(buttons->button(sp)->icon());
            buttons->button(sp)->setIcon(bIco);
            randA[buttonID]=randA[sp];
            randA[sp]=temp;
            sp=buttonID;
            f=1;
        }
        if(f)
        {
            if(!timer->isActive())
                timer->start();
            emit sendMsgCompare();      //每次交换都比较是否完成拼图
        }
    }

}
void Widget::RecvIsComplete(void)
{
    int i=1;
    for(;i<=16;i++)
    {
        if(randA[i]!=i)
        {
           break;
        }
    }
//数组有序说明完成了拼图
    if(i>=17)
    {
        f=1;
        timer->stop();

        disconnect(buttons,SIGNAL(buttonClicked(int)),this,SLOT(RecvClicked(int)));
        //取消按钮组的点击信号和槽的关联，表示拼图完成后无法移动图片
        showComplete();//展示胜利窗口
    }
}
void Widget::showComplete(void)
{
    ui->pushButton16->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+".last.png"));
    //白块换成拼图的最后一块
    QString str;
    str=QObject::tr(u8"%1分%2秒\r\n").arg(min, 2, 10, QChar('0')).arg(sec,2,10,QChar('0'));
    str+=tr(u8"你做到了！");
    QIcon complete_title(":/qrc/complete.ico");
    QIcon complete_yes(":/qrc/yes.jpg");
    QPixmap pix=complete_yes.pixmap(QSize(40,40));
    QMessageBox MsgComplete;
    MsgComplete.setWindowTitle(tr(u8"真棒！"));
    MsgComplete.setText(str);
    MsgComplete.setStandardButtons(QMessageBox::Ok);
    MsgComplete.setWindowIcon(complete_title);
    MsgComplete.setIconPixmap(pix);
    MsgComplete.exec();
}
void Widget::RecvTimeOut(void)
{
    if(++sec>=60)
    {
        sec=0;
        if(++min>=60)
        {
            sec=0;
            min=0;
        }
    }
    QString str;
    str=QObject::tr(u8"已用时：%1:%2").arg(min, 2, 10, QChar('0')).arg(sec,2,10,QChar('0'));
    ui->label->setText(str);
}

void Widget::randArray(void)
{
    unsigned int tm=uint(QTime(0, 0, 0).secsTo((QTime::currentTime())));//转换为无符号类型
    qsrand(tm);//初始化随机数种子

    int temp,r;
    for(int i=1;i<=16;i++)
    {
        temp=randA[i];      //下标为1的先和随机下标（1-16）交换数值，最后到下标为16的
        r=randA[qrand()%16+1];
        randA[i]=randA[r];
        randA[r]=temp;
    }
    for(int i=1;i<=16;i++)
    {
        if(randA[i]==16) sp=i;//找出白块所在位置
    }
}
void Widget::on_pushButtonRestart_clicked()
{
    if(timer->isActive())
    timer->stop();

    min=0;
    sec=0;
    ui->label->setText(tr(u8"已用时：00:00"));

    randArray();
    //以下 QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[1])+".png")
    //合成字符串如":/qrc/1.1.png"
    //Dialog1->pics_sp是第一个数字 表示选图选中的那张图默认是0
    //randA[x]是第二个数字 表示选中图片的第 x 块
    ui->pushButton01->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[1])+".png"));
    ui->pushButton02->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[2])+".png"));
    ui->pushButton03->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[3])+".png"));
    ui->pushButton04->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[4])+".png"));
    ui->pushButton05->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[5])+".png"));
    ui->pushButton06->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[6])+".png"));
    ui->pushButton07->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[7])+".png"));
    ui->pushButton08->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[8])+".png"));
    ui->pushButton09->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[9])+".png"));
    ui->pushButton10->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[10])+".png"));
    ui->pushButton11->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[11])+".png"));
    ui->pushButton12->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[12])+".png"));
    ui->pushButton13->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[13])+".png"));
    ui->pushButton14->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[14])+".png"));
    ui->pushButton15->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[15])+".png"));
    ui->pushButton16->setIcon(QIcon(":/qrc/"+QString::number(Dialog1->pics_sp)+"."+QString::number(randA[16])+".png"));

    if(f)
    {
    f=0;    //如果拼完点击重排才重新关联信号和槽，信号和槽已经在开头关联过了,不能再次关联！
    connect(buttons,SIGNAL(buttonClicked(int)),this,SLOT(RecvClicked(int)));
    }
}


void Widget::on_pushButtonSelect_clicked()
{
    Dialog1->show();
}

void Widget::on_pushButtonShow_pressed()
{
   // qDebug()<<"按下";
     labShowPic=new QLabel("",this);
    QString filename(":/qrc/"+QString::number(Dialog1->pics_sp)+".png");
    img=new QImage;
    scaledimg=new QImage;
    if(! ( img->load(filename) ) ) //加载图像
    {
        QMessageBox::information(this,
                     tr("打开图像失败"),
                     tr("打开图像失败!"));
        delete img;
        return;
    }

    *scaledimg = img->scaled(800, 600).scaled(350, 350, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    labShowPic->setPixmap(QPixmap::fromImage(*scaledimg));
    labShowPic->setGeometry(75,75,scaledimg->width(),scaledimg->height());
    labShowPic->show();


}

void Widget::on_pushButtonShow_released()
{
    //qDebug()<<"松开";
    labShowPic->close();
    delete img;
    img=nullptr;
    delete scaledimg;
    scaledimg=nullptr;
}



