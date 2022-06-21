#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QWidget"
#include "QMessageBox"
#include <stdio.h>
#include <QInputDialog>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include <String>
#pragma comment(lib,"coredll.lib")
int line=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//数据存放结构体
struct PERSON
{
    QString name;       //姓名
    int age;            //年龄
    int gender;         //性别<男为1，女为0>
    struct last_acid
    {
        int last_time_year;         //上一次检测--年
        int last_time_month;        //上一次检测--月
        int last_time_day;          //上一次检测--日
        int last_result;            //上一次检测结果<阴性为0，阳性为1>
    }LAST_ACID;
    struct vaccine
    {
        int vaccine_time;                   //疫苗接种次数
        int vaccine_year[4];      //疫苗接种时间--年
        int vaccine_month[4];     //疫苗接种时间--月
        int vaccine_day[4];       //疫苗接种时间--日
        int vaccine_result[4];    //疫苗接种结果
    }VACCINE;
    struct status_now
    {
        int health_code;            //健康码状态<绿码=0，黄码=1，红码=2>
        int position;               //所处位置<高风险=1，中风险=2，低风险=3>
        int last14day_pos;          //过去14天是否经过中高风险地区<经过=1，未经过=0>
        int is_quarantined;         //是否被隔离（是=1，否=0）
    }STATUS_NOW;
};

struct data_storage
{
    struct PERSON person_data;                          //保存单个人员的信息
    struct data_storage *next_person_data_pointer;      //保存下一个人员的指针位置
};

struct data
{
    struct data_storage person_data;                //保存数据的指针
    int data_num;                                   //保存数据的个数
    int MAXSIZE;                                    //保存数据的最大值
};

class tools
{
public:
    int get_days(int year1,int month1,int day1,int year2,int month2,int day2)
    {
        return daydis(year1,month1,day1,year2,month2,day2);
    }
private:
    int getDays(int year,int month,int day)
        {
            int days;
            int month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
            if(year%4==0&&year%100!=0||year%400==0)
            {
                month_days[1]=29;
            }
            days=day;
            for(int i=0;i<month-1;i++)
            {
                days+=month_days[i];
            }
            return days;
        }
    int daydis(int year1,int month1,int day1,int year2,int month2,int day2)
    {
        return abs(getDays(year1,month1,day1)-getDays(year2,month2,day2));
    }
}TOOLS;

//全局指针变量
data data_pointer;

void MainWindow::on_pushButton_clicked()        //按照姓名查询
{
    bool bOK=false;
    QString personName = QInputDialog::getText(this,
                                               tr("请输入想要查询的姓名"),
                                               tr("请输入姓名"),
                                               QLineEdit::Normal,
                                               "",
                                               &bOK);
    qDebug()<<personName;
    if(bOK&&!personName.isEmpty())
    {
        QFile file("data.txt");
        if(file.exists())
        {
            if(!file.open(QFile::ReadOnly|QFile::Text))
            {
               QMessageBox::critical(this,
                                     tr("严重错误"),
                                     tr("无法读取数据文件，请使用管理员身份运行本程序"),
                                     QMessageBox::Ok);
            }
            else
            {
                name.clear();
                gender.clear();
                detect_time.clear();
                health_code.clear();
                middle_high_risk_area.clear();
                last_detect_result.clear();
                is_quarantined.clear();
                vaccine_time.clear();
                vaccine_first_time.clear();
                vaccine_second_time.clear();
                vaccine_third_time.clear();
                vaccine_fourth_time.clear();
                //重置变量
                //新建一个文本流对象
                QTextStream in(&file);
                QStringList list;   //用于逐行读取数据
                line =0;
                data_storage DATA_STORAGE,TTTEMP,FOUNT;
                //逐行读取数据，并且将读取的数据保存
                QMessageBox::information(this,
                                         tr("操作成功"),
                                         tr("数据逐行读取成功"),
                                         QMessageBox::Ok);
                DATA_STORAGE=data_pointer.person_data;
                FOUNT=DATA_STORAGE;
                while(1)
                {
                    QString fileLine=in.readLine();         //逐行读取数据
                    if(fileLine=="")
                    {
                        break;
                    }
                    list=fileLine.split(",");
                    QString A=list.at(0);                   //姓名
                    name.append(A);
                    qDebug()<<"find the name:";
                    qDebug()<<A;
                    //将数据写入结构体中
                    DATA_STORAGE.person_data.name=A;
                    qDebug()<<"DATA_STORAGE.person_data.name="<<DATA_STORAGE.person_data.name;
                    QString B=list.at(1);                   //性别
                    gender.append(B);
                    qDebug()<<"find the gender:";
                    if(B=="男")
                    {
                        qDebug()<<"1";
                        DATA_STORAGE.person_data.gender=1;
                    }
                    else
                    {
                        qDebug()<<"0";
                        DATA_STORAGE.person_data.gender=0;
                    }
                    QString C=list.at(2);        //上次检测时间
                    qDebug()<<"last detect time";
                    detect_time.append(C);
                    QStringList list0=C.split(".");
                    int temp;
                    for(int ii=0;ii<list0.count();ii++)
                    {
                        switch(ii)
                        {
                            case 0:
                            {
                                temp=list0.at(0).toInt();
                                DATA_STORAGE.person_data.LAST_ACID.last_time_year=temp;
                            }
                            break;
                            case 1:
                            {
                                temp=list0.at(1).toInt();
                                DATA_STORAGE.person_data.LAST_ACID.last_time_month=temp;
                            }
                            break;
                            case 2:
                            {
                                temp=list0.at(2).toInt();
                                DATA_STORAGE.person_data.LAST_ACID.last_time_day=temp;
                            }
                            break;
                        }
                    }
                    QString D=list.at(3);        //健康码状态
                    qDebug()<<"health_code";
                    health_code.append(D);
                    if(D=="绿马")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.health_code=0;
                    }
                    else if(D=="黄码")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.health_code=1;
                    }
                    else if(D=="红码")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.health_code=2;
                    }
                    QString E=list.at(4);        //最近14天是否去过中高风险地区
                    qDebug()<<"middle_high_risk_area";
                    middle_high_risk_area.append(E);
                    if(E=="否")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.last14day_pos=0;
                    }
                    else if(E=="是")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.last14day_pos=1;
                    }
                    QString F=list.at(5);        //上一次核酸检测结果
                    qDebug()<<"last_detect_result";
                    last_detect_result.append(F);
                    if(F=="阴性")
                    {
                        DATA_STORAGE.person_data.LAST_ACID.last_result=0;
                    }
                    else if(F=="阳性")
                    {
                        DATA_STORAGE.person_data.LAST_ACID.last_result=1;
                    }
                    QString G=list.at(6);        //是否被隔离
                    qDebug()<<"is_quarantined";
                    is_quarantined.append(G);
                    if(G=="是")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.is_quarantined=1;
                    }
                    else if(G=="否")
                    {
                        DATA_STORAGE.person_data.STATUS_NOW.is_quarantined=0;
                    }
                    QString H=list.at(7);        //疫苗接种次数
                    vaccine_time.append(H);
                    qDebug()<<"vaccine_time";
                    temp=H.toInt();
                    DATA_STORAGE.person_data.VACCINE.vaccine_time=temp;
                    QString I=list.at(8);        //第一次接种疫苗的时间
                    vaccine_first_time.append(I);
                    if(I=="null"||I=="NULL")
                    {
                        DATA_STORAGE.person_data.VACCINE.vaccine_year[0]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_month[0]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_day[0]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[0]=-1;
                    }
                    else
                    {
                        QStringList list1=I.split(".");
                        for(int iii=0;iii<list1.count();iii++)
                        {
                            switch(iii)
                            {
                                case 0:
                                {
                                    temp=list1.at(0).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_year[0]=temp;
                                }
                                break;
                                case 1:
                                {
                                    temp=list1.at(1).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_month[0]=temp;
                                }
                                break;
                                case 2:
                                {
                                    temp=list1.at(2).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_day[0]=temp;
                                }
                                break;
                            }
                        }
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[0]=0;
                    }
                    QString J=list.at(9);        //第二次接种疫苗的时间
                    vaccine_second_time.append(J);
                    if(J=="null"||J=="NULL")
                    {
                        DATA_STORAGE.person_data.VACCINE.vaccine_year[1]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_month[1]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_day[1]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[1]=-1;
                    }
                    else
                    {
                        QStringList list2=J.split(".");
                        for(int iii=0;iii<list2.count();iii++)
                        {
                            switch(iii)
                            {
                                case 0:
                                {
                                    temp=list2.at(0).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_year[1]=temp;
                                }
                                break;
                                case 1:
                                {
                                    temp=list2.at(1).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_month[1]=temp;
                                }
                                break;
                                case 2:
                                {
                                    temp=list2.at(2).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_day[1]=temp;
                                }
                                break;
                            }
                        }
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[1]=0;
                    }
                    QString K=list.at(10);       //第三次接种疫苗的时间
                    vaccine_third_time.append(K);
                    if(K=="null"||K=="NULL")
                    {
                        DATA_STORAGE.person_data.VACCINE.vaccine_year[2]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_month[2]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_day[2]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[2]=-1;
                    }
                    else
                    {
                        QStringList list3=K.split(".");
                        for(int iii=0;iii<list3.count();iii++)
                        {
                            switch(iii)
                            {
                                case 0:
                                {
                                    temp=list3.at(0).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_year[2]=temp;
                                }
                                break;
                                case 1:
                                {
                                    temp=list3.at(1).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_month[2]=temp;
                                }
                                break;
                                case 2:
                                {
                                    temp=list3.at(2).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_day[2]=temp;
                                }
                                break;
                            }
                        }
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[2]=0;
                    }
                    QString L=list.at(11);       //第四次接种疫苗的时间
                    vaccine_fourth_time.append(L);
                    if(L=="null"||L=="NULL")
                    {
                        DATA_STORAGE.person_data.VACCINE.vaccine_year[3]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_month[3]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_day[3]=0;
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[3]=-1;
                    }
                    else
                    {
                        QStringList list4=L.split(".");
                        for(int iii=0;iii<list4.count();iii++)
                        {
                            switch(iii)
                            {
                                case 0:
                                {
                                    temp=list4.at(0).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_year[3]=temp;
                                }
                                break;
                                case 1:
                                {
                                    temp=list4.at(1).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_month[3]=temp;
                                }
                                break;
                                case 2:
                                {
                                    temp=list4.at(2).toInt();
                                    DATA_STORAGE.person_data.VACCINE.vaccine_day[3]=temp;
                                }
                                break;
                            }
                        }
                        DATA_STORAGE.person_data.VACCINE.vaccine_result[3]=0;
                    }
                    if(DATA_STORAGE.person_data.name==personName)
                    {
                        qDebug()<<"matched";
                        ui->lineEdit->setText(DATA_STORAGE.person_data.name);
                        qDebug()<<"person_name="<<DATA_STORAGE.person_data.name;
                        qDebug()<<"person_gender"<<DATA_STORAGE.person_data.gender;
                        if(DATA_STORAGE.person_data.gender==0)
                        {
                            ui->lineEdit_2->setText("女");
                        }
                        else if(DATA_STORAGE.person_data.gender==1)
                        {
                            ui->lineEdit_2->setText("男");
                        }
                        else
                        {
                            ui->lineEdit_2->setText("未知错误，请检查数据库文件");
                            QMessageBox::critical(this,
                                                  tr("严重错误"),
                                                  tr("数据库文件出错，请检查数据库文件"),
                                                  QMessageBox::Ok);
                        }
                        qDebug()<<"person_gender"<<DATA_STORAGE.person_data.gender;
                        ui->lineEdit_3->setText(QString::number(DATA_STORAGE.person_data.LAST_ACID.last_time_year));
                        ui->lineEdit_3->insert(".");
                        ui->lineEdit_3->insert(QString::number(DATA_STORAGE.person_data.LAST_ACID.last_time_month));
                        ui->lineEdit_3->insert(".");
                        ui->lineEdit_3->insert(QString::number(DATA_STORAGE.person_data.LAST_ACID.last_time_day));
                        qDebug()<<DATA_STORAGE.person_data.LAST_ACID.last_time_year<<DATA_STORAGE.person_data.LAST_ACID.last_time_month<<DATA_STORAGE.person_data.LAST_ACID.last_time_day;
                        //获取当前时间
                        /*
                        SYSTEMTIME stTime;
                        GetLocalTime(&stTime);
                        int wYear,wMonth,wDay;
                        wYear=stTime.wYear;
                        wMonth=stTime.wMonth;
                        wDay=stTime.wDay;
                        */
                        //使用QT获取当前时间
                        int wYear,wMonth,wDay;
                        QDateTime current_date_time=QDateTime::currentDateTime();
                        QString QYear,QMonth,QDay;
                        QYear=current_date_time.toString("yyyy");
                        QMonth=current_date_time.toString("MM");
                        QDay=current_date_time.toString("dd");
                        wYear=QYear.toInt();
                        wMonth=QMonth.toInt();
                        wDay=QDay.toInt();
                        int days_1;
                        days_1=TOOLS.get_days(wYear,wMonth,wDay,DATA_STORAGE.person_data.LAST_ACID.last_time_year,DATA_STORAGE.person_data.LAST_ACID.last_time_month,DATA_STORAGE.person_data.LAST_ACID.last_time_day);
                        ui->lineEdit_4->setText(QString::number(days_1));
                        ui->lineEdit_4->insert("天");
                        //健康码状态
                        if(DATA_STORAGE.person_data.STATUS_NOW.health_code==0)
                        {
                            ui->lineEdit_5->setText("绿码");
                        }
                        else if(DATA_STORAGE.person_data.STATUS_NOW.health_code==1)
                        {
                            ui->lineEdit_5->setText("黄码");
                        }
                        else if(DATA_STORAGE.person_data.STATUS_NOW.health_code==2)
                        {
                            ui->lineEdit_5->setText("红码");
                        }
                        else
                        {
                            ui->lineEdit_5->setText("未知错误，请检查数据库文件");
                            QMessageBox::critical(this,
                                                  tr("严重错误"),
                                                  tr("数据库文件出错，请检查数据库文件"),
                                                  QMessageBox::Ok);
                        }
                        //最近14天是否去过中高风险地区
                        if(DATA_STORAGE.person_data.STATUS_NOW.last14day_pos==0)
                        {
                            ui->lineEdit_6->setText("此人未在最近14天到访过中高风险地区");
                        }
                        else if(DATA_STORAGE.person_data.STATUS_NOW.last14day_pos==1)
                        {
                            ui->lineEdit_6->setText("此人在最近14天内到访过中高风险地区");
                        }
                        else
                        {
                            ui->lineEdit_6->setText("未知错误，请检查数据库文件");
                            QMessageBox::critical(this,
                                                  tr("严重错误"),
                                                  tr("数据库文件出错，请检查数据库文件"),
                                                  QMessageBox::Ok);
                        }
                        //是否有48小时核酸阴性证明
                        int day_2;
                        day_2=TOOLS.get_days(wYear,wMonth,wDay,DATA_STORAGE.person_data.LAST_ACID.last_time_year,DATA_STORAGE.person_data.LAST_ACID.last_time_month,DATA_STORAGE.person_data.LAST_ACID.last_time_day);
                        if(day_2<=2&&DATA_STORAGE.person_data.LAST_ACID.last_result==0)
                        {
                            ui->lineEdit_7->setText("此人持有48小时核酸阴性证明");
                        }
                        else
                        {
                            ui->lineEdit_7->setText("此人未持有48小时核酸阴性证明");
                        }
                        //上次核酸检测结果
                        if(DATA_STORAGE.person_data.LAST_ACID.last_result==0)
                        {
                            ui->lineEdit_8->setText("阴性");
                        }
                        else if(DATA_STORAGE.person_data.LAST_ACID.last_result==1)
                        {
                            ui->lineEdit_8->setText("阳性");
                            QMessageBox::information(this,
                                                     tr("提示"),
                                                     tr("核酸阳性"),
                                                     QMessageBox::Ok);
                        }
                        else
                        {
                            ui->lineEdit_8->setText("未知错误，请检查数据库文件");
                            QMessageBox::critical(this,
                                                  tr("严重错误"),
                                                  tr("数据库文件出错，请检查数据库文件"),
                                                  QMessageBox::Ok);
                        }
                        //是否为隔离人员
                        if(DATA_STORAGE.person_data.STATUS_NOW.is_quarantined==0)
                        {
                            ui->lineEdit_9->setText("未被隔离");
                        }
                        else if(DATA_STORAGE.person_data.STATUS_NOW.is_quarantined==1)
                        {
                            ui->lineEdit_9->setText("已被隔离");
                        }
                        else
                        {
                            ui->lineEdit_9->setText("未知错误，请检查数据库文件");
                            QMessageBox::critical(this,
                                                  tr("严重错误"),
                                                  tr("数据库文件出错，请检查数据库文件"),
                                                  QMessageBox::Ok);
                        }
                        //疫苗接种次数
                        ui->lineEdit_10->setText(QString::number(DATA_STORAGE.person_data.VACCINE.vaccine_time));
                        //第一针接种时间
                        if(DATA_STORAGE.person_data.VACCINE.vaccine_result[0]==-1)
                        {
                            ui->lineEdit_11->setText("未接种第一针疫苗");
                        }
                        else
                        {
                            int ttttemp;
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_year[0];
                            ui->lineEdit_11->setText(QString::number(ttttemp));
                            ui->lineEdit_11->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_month[0];
                            ui->lineEdit_11->insert(QString::number(ttttemp));
                            ui->lineEdit_11->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_day[0];
                            ui->lineEdit_11->insert(QString::number(ttttemp));
                        }
                        //第二针接种时间
                        if(DATA_STORAGE.person_data.VACCINE.vaccine_result[1]==-1)
                        {
                            ui->lineEdit_12->setText("未接种第二针疫苗");
                        }
                        else
                        {
                            int ttttemp;
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_year[1];
                            ui->lineEdit_12->setText(QString::number(ttttemp));
                            ui->lineEdit_12->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_month[1];
                            ui->lineEdit_12->insert(QString::number(ttttemp));
                            ui->lineEdit_12->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_day[1];
                            ui->lineEdit_12->insert(QString::number(ttttemp));
                        }
                        //第三针接种时间
                        if(DATA_STORAGE.person_data.VACCINE.vaccine_result[2]==-1)
                        {
                            ui->lineEdit_13->setText("未接种第三针疫苗");
                        }
                        else
                        {
                            int ttttemp;
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_year[2];
                            ui->lineEdit_13->setText(QString::number(ttttemp));
                            ui->lineEdit_13->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_month[2];
                            ui->lineEdit_13->insert(QString::number(ttttemp));
                            ui->lineEdit_13->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_day[2];
                            ui->lineEdit_13->insert(QString::number(ttttemp));
                        }
                        //第四针接种时间
                        if(DATA_STORAGE.person_data.VACCINE.vaccine_result[3]==-1)
                        {
                            ui->lineEdit_14->setText("未接种第四针疫苗");
                        }
                        else
                        {
                            int ttttemp;
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_year[3];
                            ui->lineEdit_14->setText(QString::number(ttttemp));
                            ui->lineEdit_14->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_month[3];
                            ui->lineEdit_14->insert(QString::number(ttttemp));
                            ui->lineEdit_14->insert(".");
                            ttttemp=DATA_STORAGE.person_data.VACCINE.vaccine_day[3];
                            ui->lineEdit_14->insert(QString::number(ttttemp));
                        }
                        QMessageBox::information(this,tr("程序信息"),tr("程序信息：\nbuild time:2022-06-20\nbuild os :windows11\nbuild version 2.0.0"),
                                                 QMessageBox::Ok);
                        break;
                    }
                    else
                    {
                    TTTEMP=*new data_storage;
                    DATA_STORAGE.next_person_data_pointer=&TTTEMP;
                    TTTEMP=DATA_STORAGE;
                    }
                }    
            }
        }
        else
        {
            //文件不存在
            QMessageBox::critical(this,
                                 tr("严重错误"),
                                  tr("程序调用的数据库文件不存在,重新安装本程序可能会解决本问题"),
                                  QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::critical(this,
                              tr("严重警告"),
                              tr("没有检测到用户输入或者是用户启动输入框后强制关闭了弹窗"),
                              QMessageBox::Ok);
    }

}


void MainWindow::on_pushButton_11_clicked()     //修改(按照姓名修改)
{

}


void MainWindow::on_pushButton_2_clicked()      //退出本程序
{
    exit(0);
}


void MainWindow::on_pushButton_3_clicked()      //程序初始化（所有信息全部置空）
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");
    ui->lineEdit_12->setText("");
    ui->lineEdit_13->setText("");
    ui->lineEdit_14->setText("");
    ui->lineEdit_15->setText("");
    ui->progressBar->setValue(0);
    int a=QMessageBox::information(this,tr("程序初始化完成"),
                                   tr("程序初始化完成"),
                                   QMessageBox::Ok);
}


void MainWindow::on_pushButton_4_clicked()      //显示程序版本
{
    QMessageBox::information(this,tr("版权信息"),
                             tr("关于：\n核酸检测管理系统\nversion:2.0.0BETA build22620\ncopyright:all rights reserved ,(c) 2020~2022 by lry and cyx\n试用版本\n"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_7_clicked()      //打开帮助chm文件
{
    QMessageBox::information(this,tr("帮助"),
                             tr("没有找到帮助文件"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_5_clicked()      //显示程序信息
{
    QMessageBox::information(this,tr("程序信息"),tr("程序信息：\nbuild time:2022-06-20\nbuild os :windows11\nbuild version 2.0.0"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_6_clicked()      //显示程序版权信息
{
   QMessageBox::information(this,tr("版权信息"),tr("数字签名：刘仁宇\n数字签名到期时间：2040-01-01"),
                            QMessageBox::Ok);
}


void MainWindow::on_pushButton_10_clicked()     //访问我的github代码储存库
{
    system("start https://github.com/lry-123456789");
}


void MainWindow::on_pushButton_8_clicked()      //隐藏彩蛋0->贪吃蛇
{
    QMessageBox::information(this,tr("提示"),tr("此处的彩蛋已经移动到右侧，请使用右侧按钮"),QMessageBox::Ok);
}


void MainWindow::on_pushButton_9_clicked()      //隐藏彩蛋1->俄罗斯方块
{
    MainWindow_tetris *win =new MainWindow_tetris;
    win->show();
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_8_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_9_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_10_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_11_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_12_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_13_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_14_textChanged(const QString &arg1)
{

}


void MainWindow::on_progressBar_valueChanged(int value)
{

}


void MainWindow::on_lineEdit_15_textChanged(const QString &arg1)
{

}


void MainWindow::on_lineEdit_9_textEdited(const QString &arg1)
{

}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{

}


void MainWindow::on_lineEdit_2_editingFinished()
{

}

