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
#include <QThread>
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

int add_function=2;     //0为正在添加，1为添加完成，可以写入数据库
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
    int success=0;
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
                        success=1;
                        //判断被搜索人员状态
                        if(TOOLS.get_days(wYear,wMonth,wDay,DATA_STORAGE.person_data.LAST_ACID.last_time_year,DATA_STORAGE.person_data.LAST_ACID.last_time_month,DATA_STORAGE.person_data.LAST_ACID.last_time_day)<=14&&DATA_STORAGE.person_data.LAST_ACID.last_result==0&&DATA_STORAGE.person_data.STATUS_NOW.last14day_pos==0&&DATA_STORAGE.person_data.STATUS_NOW.health_code==0&&DATA_STORAGE.person_data.STATUS_NOW.is_quarantined==0&&DATA_STORAGE.person_data.STATUS_NOW.position==0)
                        {
                            ui->radioButton->setChecked(true);
                            ui->radioButton_2->setChecked(false);
                            ui->radioButton_3->setChecked(false);
                        }
                        else
                        {
                            ui->radioButton->setChecked(false);
                            ui->radioButton_2->setChecked(true);
                            ui->radioButton_3->setChecked(false);
                        }
                        break;
                    }
                    else
                    {
                    TTTEMP=*new data_storage;
                    DATA_STORAGE.next_person_data_pointer=&TTTEMP;
                    TTTEMP=DATA_STORAGE;
                    }
                }
                //显示查询进度
                ui->progressBar->setValue(100);
                //显示是否查询成功
                if(success==1)
                {
                    ui->lineEdit_15->setText("查询成功");
                }
                else if(success==0)
                {
                    ui->lineEdit_15->setText("查询失败");
                }
                else
                {
                    ui->lineEdit_15->setText("系统错误，请勿修改内存信息");
                    QMessageBox::critical(this,
                                          tr("严重错误"),
                                          tr("内存设定出错，请检查设备或检测是否有程序篡改本程序内存"),
                                          QMessageBox::Ok);
                }
            }
            file.close();
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
    add_function=0;
    bool bOK=false;
    QString personName = QInputDialog::getText(this,
                                               tr("请输入想要修改的姓名(可以用于新增，删除)"),
                                               tr("请输入姓名"),
                                               QLineEdit::Normal,
                                               "",
                                               &bOK);
    qDebug()<<"finished the operator->name";
    if(bOK&&!personName.isEmpty())
    {
        QFile file("data.txt");
        QStringList list;   //用于逐行读取数据
        qDebug()<<"finished the operator->create list";
        if(file.exists())
        {
            qDebug()<<"operating...read files";
            if(!file.open(QFile::ReadOnly|QFile::Text))
            {
               QMessageBox::critical(this,
                                     tr("严重错误"),
                                     tr("无法读取数据文件，请使用管理员身份运行本程序"),
                                     QMessageBox::Ok);
            }
            else
            {
                qDebug()<<"begin to clear list";
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
                QTextStream in(&file);
                QStringList list;   //用于逐行读取数据
                line =0;
                while(!file.atEnd())
                {
                    qDebug()<<"read file ......";
                    QString strline = file.readLine();
                    qDebug()<<strline;
                    if(strline==""||strline=="\n")
                    {
                        break;
                    }
                    list=strline.split(",");
                    QString A=list.at(0);
                    name.append(A);
                    QString B=list.at(1);
                    gender.append(B);
                    QString C=list.at(2);
                    detect_time.append(C);
                    QString D=list.at(3);
                    health_code.append(D);
                    QString E=list.at(4);
                    middle_high_risk_area.append(E);
                    QString F=list.at(5);
                    last_detect_result.append(F);
                    QString G=list.at(6);
                    is_quarantined.append(G);
                    QString H=list.at(7);
                    vaccine_time.append(H);
                    QString I=list.at(8);
                    vaccine_first_time.append(I);
                    QString J=list.at(9);
                    vaccine_second_time.append(J);
                    QString K=list.at(10);
                    vaccine_third_time.append(K);
                    QString L=list.at(11);
                    vaccine_fourth_time.append(L);
                }
                file.close();
                qDebug()<<"successfullly read the database";
                int name_index=name.indexOf(personName);
                qDebug()<<name_index;
                if(name_index==-1)
                {
                    qDebug()<<"please input the text to the lineEdit";
                    QMessageBox::information(this,
                                             tr("未查询到相关信息"),
                                             tr("未查询到相关信息，模式自动更正为添加模式"),
                                             QMessageBox::Ok);
                    /*
                    ui->lineEdit->setText("请输入姓名");
                    ui->lineEdit_2->setText("请输入性别(男，女)");
                    ui->lineEdit_3->setText("请输入上次核酸检测时间（year.month.day）");
                    ui->lineEdit_5->setText("请输入健康码状态(红码，绿码，黄码)");
                    ui->lineEdit_6->setText("请输入是否到过中高风险地区(是，否))");
                    ui->lineEdit_8->setText("请输入上次核酸检测结果(阴性，阳性)");
                    ui->lineEdit_9->setText("请输入是否为隔离人员(是，否)");
                    ui->lineEdit_10->setText("请输入疫苗接种次数(填写数字即可)");
                    ui->lineEdit_11->setText("请输入第一针接种时间(year.month.day)");
                    ui->lineEdit_12->setText("请输入第二针接种时间(year.month.day)");
                    ui->lineEdit_13->setText("请输入第三针接种时间(year.month.day)");
                    ui->lineEdit_14->setText("请输入第四针接种时间(year.month.day)");
                    */
                    /*
                    qDebug()<<"set default information finished";
                    add_function=0;
                    while(add_function==0)
                    {
                        qDebug()<<"add_function="<<add_function;
                        ui->lineEdit_4->setText("禁止写入");
                        ui->lineEdit_7->setText("禁止写入");
                        QThread::msleep(1000);
                    }
                    qDebug()<<"information has successfully received";
                    QString A1,B1,C1,D1,E1,F1,G1,H1,I1,J1,K1,L1;
                    A1=ui->lineEdit->text();
                    B1=ui->lineEdit_2->text();
                    C1=ui->lineEdit_3->text();
                    D1=ui->lineEdit_5->text();
                    E1=ui->lineEdit_6->text();
                    F1=ui->lineEdit_8->text();
                    G1=ui->lineEdit_9->text();
                    H1=ui->lineEdit_10->text();
                    I1=ui->lineEdit_11->text();
                    J1=ui->lineEdit_12->text();
                    K1=ui->lineEdit_13->text();
                    L1=ui->lineEdit_14->text();
                    */
                    QString A1,B1,C1,D1,E1,F1,G1,H1,I1,J1,K1,L1;
                    A1=QInputDialog::getText(this,
                                             tr("请输入姓名"),
                                             tr("请输入姓名，直接填写"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    B1=QInputDialog::getText(this,
                                             tr("请输入性别"),
                                             tr("请输入性别"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    C1=QInputDialog::getText(this,
                                             tr("请输入上次核酸检测时间"),
                                             tr("示例：2022.6.21"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    D1=QInputDialog::getText(this,
                                             tr("请输入健康码状态"),
                                             tr("示例：绿码，黄码，红码"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    E1=QInputDialog::getText(this,
                                             tr("请输入最近14天时候到过中高风险地区"),
                                             tr("示例：是，否"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    F1=QInputDialog::getText(this,
                                             tr("请输入上次核酸检测结果"),
                                             tr("示例：阴性，阳性"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    G1=QInputDialog::getText(this,
                                             tr("请输入是否为隔离人员"),
                                             tr("示例：是，否"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    H1=QInputDialog::getText(this,
                                             tr("请输入疫苗接种次数"),
                                             tr("示例：0，1，2，3，4"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    if(H1=="0")
                    {
                        I1="null";
                        J1="null";
                        K1="null";
                        L1="null";
                    }
                    else if(H1=="1")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1="null";
                        K1="null";
                        L1="null";
                    }
                    else if(H1=="2")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1=QInputDialog::getText(this,
                                                 tr("请输入第二次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        K1="null";
                        L1="null";
                    }
                    else if(H1=="3")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1=QInputDialog::getText(this,
                                                 tr("请输入第二次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        K1=QInputDialog::getText(this,
                                                 tr("请输入第三次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        L1="null";
                    }
                    else if(H1=="4")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1=QInputDialog::getText(this,
                                                 tr("请输入第二次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        K1=QInputDialog::getText(this,
                                                 tr("请输入第三次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        L1=QInputDialog::getText(this,
                                                 tr("请输入第四次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                    }
                    name.append(A1);
                    gender.append(B1);
                    detect_time.append(C1);
                    health_code.append(D1);
                    middle_high_risk_area.append(E1);
                    last_detect_result.append(F1);
                    is_quarantined.append(G1);
                    vaccine_time.append(H1);
                    if(H1=="0")
                    {
                        vaccine_first_time.append("null");
                        vaccine_second_time.append("null");
                        vaccine_third_time.append("null");
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="1")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append("null");
                        vaccine_third_time.append("null");
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="2")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append(J1);
                        vaccine_third_time.append("null");
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="3")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append(J1);
                        vaccine_third_time.append(K1);
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="4")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append(J1);
                        vaccine_third_time.append(K1);
                        vaccine_fourth_time.append(L1);
                    }
                    else
                    {
                        QMessageBox::information(this,
                                                 tr("程序异常"),
                                                 tr("请输入正确的数据"),
                                                 QMessageBox::Ok);
                    }
                }
                else
                {
                    QMessageBox::information(this,
                                             tr("已经查询到相关信息"),
                                             tr("模式自动更正为修改模式"),
                                             QMessageBox::Ok);
                    /*
                    ui->lineEdit->setText(name.at(name_index));
                    ui->lineEdit_2->setText(gender.at(name_index));
                    ui->lineEdit_3->setText(detect_time.at(name_index));
                    ui->lineEdit_5->setText(health_code.at(name_index));
                    ui->lineEdit_6->setText(middle_high_risk_area.at(name_index));
                    ui->lineEdit_8->setText(last_detect_result.at(name_index));
                    ui->lineEdit_9->setText(is_quarantined.at(name_index));
                    ui->lineEdit_10->setText(vaccine_time.at(name_index));
                    ui->lineEdit_11->setText(vaccine_first_time.at(name_index));
                    ui->lineEdit_12->setText(vaccine_second_time.at(name_index));
                    ui->lineEdit_13->setText(vaccine_third_time.at(name_index));
                    ui->lineEdit_14->setText(vaccine_fourth_time.at(name_index));
                    while(add_function==0)
                    {
                        ui->lineEdit_4->setText("禁止写入");
                        ui->lineEdit_7->setText("禁止写入");
                        QThread::msleep(10);
                    }
                    QString A1,B1,C1,D1,E1,F1,G1,H1,I1,J1,K1,L1;
                    A1=ui->lineEdit->text();
                    B1=ui->lineEdit_2->text();
                    C1=ui->lineEdit_3->text();
                    D1=ui->lineEdit_5->text();
                    E1=ui->lineEdit_6->text();
                    F1=ui->lineEdit_8->text();
                    G1=ui->lineEdit_9->text();
                    H1=ui->lineEdit_10->text();
                    I1=ui->lineEdit_11->text();
                    J1=ui->lineEdit_12->text();
                    K1=ui->lineEdit_13->text();
                    L1=ui->lineEdit_14->text();
                    */
                    QString A1,B1,C1,D1,E1,F1,G1,H1,I1,J1,K1,L1;
                    A1=QInputDialog::getText(this,
                                             tr("请输入姓名"),
                                             tr("请输入姓名，直接填写"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    B1=QInputDialog::getText(this,
                                             tr("请输入性别"),
                                             tr("请输入性别"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    C1=QInputDialog::getText(this,
                                             tr("请输入上次核酸检测时间"),
                                             tr("示例：2022.6.21"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    D1=QInputDialog::getText(this,
                                             tr("请输入健康码状态"),
                                             tr("示例：绿码，黄码，红码"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    E1=QInputDialog::getText(this,
                                             tr("请输入最近14天时候到过中高风险地区"),
                                             tr("示例：是，否"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    F1=QInputDialog::getText(this,
                                             tr("请输入上次核酸检测结果"),
                                             tr("示例：阴性，阳性"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    G1=QInputDialog::getText(this,
                                             tr("请输入是否为隔离人员"),
                                             tr("示例：是，否"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    H1=QInputDialog::getText(this,
                                             tr("请输入疫苗接种次数"),
                                             tr("示例：0，1，2，3，4"),
                                             QLineEdit::Normal,
                                             "",
                                             &bOK);
                    if(H1=="0")
                    {
                        I1="null";
                        J1="null";
                        K1="null";
                        L1="null";
                    }
                    else if(H1=="1")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1="null";
                        K1="null";
                        L1="null";
                    }
                    else if(H1=="2")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1=QInputDialog::getText(this,
                                                 tr("请输入第二次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        K1="null";
                        L1="null";
                    }
                    else if(H1=="3")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1=QInputDialog::getText(this,
                                                 tr("请输入第二次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        K1=QInputDialog::getText(this,
                                                 tr("请输入第三次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        L1="null";
                    }
                    else if(H1=="4")
                    {
                        I1=QInputDialog::getText(this,
                                                 tr("请输入第一次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        J1=QInputDialog::getText(this,
                                                 tr("请输入第二次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        K1=QInputDialog::getText(this,
                                                 tr("请输入第三次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                        L1=QInputDialog::getText(this,
                                                 tr("请输入第四次疫苗接种时间"),
                                                 tr("示例：2022.6.21"),
                                                 QLineEdit::Normal,
                                                 "",
                                                 &bOK);
                    }
                    name.append(A1);
                    gender.append(B1);
                    detect_time.append(C1);
                    health_code.append(D1);
                    middle_high_risk_area.append(E1);
                    last_detect_result.append(F1);
                    is_quarantined.append(G1);
                    vaccine_time.append(H1);
                    if(H1=="0")
                    {
                        vaccine_first_time.append("null");
                        vaccine_second_time.append("null");
                        vaccine_third_time.append("null");
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="1")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append("null");
                        vaccine_third_time.append("null");
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="2")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append(J1);
                        vaccine_third_time.append("null");
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="3")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append(J1);
                        vaccine_third_time.append(K1);
                        vaccine_fourth_time.append("null");
                    }
                    else if(H1=="4")
                    {
                        vaccine_first_time.append(I1);
                        vaccine_second_time.append(J1);
                        vaccine_third_time.append(K1);
                        vaccine_fourth_time.append(L1);
                    }
                    else
                    {
                        QMessageBox::information(this,
                                                 tr("程序异常"),
                                                 tr("请输入正确的数据"),
                                                 QMessageBox::Ok);
                    }
                }
                //删除原始数据库文件，并且写一个新的文件
                QFile source_file("data.txt");
                source_file.remove();
                QFile new_file("data.txt");
                new_file.open(QIODevice::ReadWrite | QIODevice::Text);
                int linelines=0;
                for(linelines=0;linelines<name.count();linelines++)
                {
                    if(linelines!=name_index)
                    {
                        new_file.write(name.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(gender.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(detect_time.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(health_code.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(middle_high_risk_area.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(last_detect_result.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(is_quarantined.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(vaccine_time.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(vaccine_first_time.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(vaccine_second_time.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(vaccine_third_time.at(linelines).toUtf8());
                        new_file.write(",");
                        new_file.write(vaccine_fourth_time.at(linelines).toUtf8());
                        new_file.write("");
                    }
                }
                new_file.close();
            }
        }
        else
        {
            QMessageBox::critical(this,
                                     tr("错误"),
                                     tr("未能够找到数据库文件"),
                                     QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("提示"),
                                 tr("本系统检测到用户关闭了本信息选择窗口，或者是输入的信息为空"),
                                 QMessageBox::Ok);
    }
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
                             tr("关于：\n核酸检测管理系统\nversion:3.0.0BETA build22621\ncopyright:all rights reserved ,(c) 2020~2022 by lry，cyx and lqh\n试用版本\n"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_7_clicked()      //打开帮助chm文件
{
    QMessageBox::information(this,tr("帮助"),
                             tr("没有帮助文件,此版本暂不支持"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_5_clicked()      //显示程序信息
{
    QMessageBox::information(this,tr("程序信息"),tr("程序信息：\nbuild time:2022-06-21\nbuild os :windows11\nbuild version 3.0.0"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_6_clicked()      //显示程序版权信息
{
   QMessageBox::information(this,tr("版权信息"),tr("数字签名：刘仁宇\n数字签名到期时间：2040-01-01"),
                            QMessageBox::Ok);
}


void MainWindow::on_pushButton_10_clicked()     //访问我的github代码储存库
{
    system("start https://github.com/lry-123456789/NUCLEIC-ACID-detection-management-system");
}


void MainWindow::on_pushButton_8_clicked()
{
    QMessageBox::information(this,
                             tr("提示"),
                             tr("程序提示:信息保存成功->data.txt"),
                             QMessageBox::Ok);
}


void MainWindow::on_pushButton_9_clicked()      //隐藏彩蛋->俄罗斯方块
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


void MainWindow::on_radioButton_clicked()
{

}


void MainWindow::on_radioButton_2_clicked()
{

}


void MainWindow::on_radioButton_3_clicked()
{

}

