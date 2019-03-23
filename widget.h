#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include"dialog.h"
#include<QButtonGroup>
#include<QLabel>
#include<QIcon>
#include<QPixmap>
#include<QTimer>
#include<QTime>
#include<QDebug>
#include<QMessageBox>
/************************************************************************************************************
 *                                          这是主窗体头文件                                                   *
 *          运行程序第一步先调用“重排”按钮的槽函数 on_pushButtonRestart_clicked() 随机排列图片                       *
 *          重排后第一次点击按钮将会打开计时器，每秒刷新计时标签的时间                                                 *
 *          每次点击按钮会转到 RecvClicked(int buttonID)槽函数 交换图片和维护 randA数组并发射信号 sendMsgCompare()    *
 *          转到 RecvIsComplete()槽函数 比较拼图是否完成，若完成则调用 showComplete(void) 槽函数 展示完成信息，否则无动作*
 *                                                                                                          *
 *          点击选图按钮 转到 on_pushButtonSelect_clicked()槽函数，它展示dialog窗体，执行dialog.cpp                 *
 *          主窗体通过Dialog *Dialog1=new Dialog(this) Dialog1指针代表选图窗体                                   *
 *          按住和松开显示按钮分别执行 on_pushButtonShow_pressed() 和  on_pushButtonShow_released()              *
 *                                                                                                          *
 ***********************************************************************************************************/
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void showComplete(void);
    void randArray(void);

signals:
    void sendMsgCompare(void);

public slots:
    void RecvClicked(int buttonID);
    void RecvIsComplete(void);
    void RecvTimeOut(void);

private slots:
    void on_pushButtonRestart_clicked();

    void on_pushButtonSelect_clicked();

    void on_pushButtonShow_pressed();

    void on_pushButtonShow_released();

private:
    Ui::Widget *ui;
    Dialog *Dialog1=new Dialog(this);
    bool f=false;
    int sp;
    int temp;
    int sec=0;
    int min=0;
    int randA[17]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    QTimer *timer;
    QIcon bIco;
    QImage* img,* scaledimg;
    QButtonGroup *buttons;
    QLabel *labShowPic;

};

#endif // WIDGET_H
