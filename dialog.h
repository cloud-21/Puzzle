#ifndef DIALOG1_H
#define DIALOG1_H
#include <QDialog>
#include<QIcon>
#include<QPixmap>
/************************************************************************************************************
 *                            这是点击 “选图” 按钮后显示的窗体头文件                                               *
 *                                                                                                          *
 *                      点击些窗体的“确认”按钮会发送  sedMsgConfirm(void)信号与主窗体关联                           *
 *                                                                                                          *
 *   “上一个”按钮(Previous)和“下一个”按钮(Next)共同维护 int型变量 “pics_sp”表示主窗体显示哪副图片                      *
 *                                                                                                          *
 *                                                                                                          *
 ***********************************************************************************************************/
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    int pics_sp=0;
    int tmp=0;
signals:
    void sedMsgConfirm(void);
private slots:
    void on_pushButtonPrevious_clicked();

    void on_pushButtonNext_clicked();

    void on_pushButtonConfirm_clicked();

private:
    Ui::Dialog *ui;


};

#endif // DIALOG1_H
