#include "login.h"
#include "ui_login.h"

Login::Login(QDialog *parent) :
    QDialog(parent),
    m_ui(new Ui::Login)
{
    m_ui->setupUi(this);

    connect(m_ui->regBtn,&QPushButton::clicked,this,&Login::register_clicked);
    connect(m_ui->loginBtn,&QPushButton::clicked,this,&Login::login_clicked);
    connect(m_ui->nameCmBox,&QLineEdit::textChanged,this,&Login::getUserInfo);

    usrtableFlag=false;
    rectableFlag = false;

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("database.db");



    //打开数据库
    if(!database.open())
    {
        qDebug()<<database.lastError();
        qFatal("failed to connect.") ;
    }
    else
    {
        qDebug()<<"open seccess";
        QSqlQuery sql_query;        //改变量必须在成功打开数据库后定义才有效
        sql_query.prepare(select_table);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            QString tableName;
            while(sql_query.next())
            {
                tableName = sql_query.value(0).toString();
                qDebug()<<tableName;
                if(tableName.compare("user"))
                {
                    usrtableFlag=false;
                    qDebug()<<"user table is not exist";
                }
                else
                {
                    usrtableFlag=true;
                    qDebug()<<"user table is exist";
                }

                if(tableName.compare("record"))
                {
                    rectableFlag=false;
                    qDebug()<<"record table is not exist";
                }
                else
                {
                    rectableFlag=true;
                    qDebug()<<"record table is exist";
                }
            }
        }

        if(usrtableFlag==false)
        {
            sql_query.prepare(create_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                qDebug()<<"table created!";
            }
        }

        //database.close();
        if (rectableFlag == false){
            sql_query.prepare(create_sql1);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                qDebug()<<"table created!";
            }
        }
    }
}

Login::~Login()
{
    delete m_ui;
}

void Login::register_clicked()
{
    //QApplication b();
    Register r(this);
    this->hide();
    r.show();
    transmitdb(database);
    r.exec();
    this->close();
}

void Login::login_clicked()
{
    if(matchFlag==false)
    {
        //用户名错误
        qDebug()<<"name invalid";
    }
    else
    {
        if(usr_passwd!=m_ui->passwdLineEdit->text())
        {
            //密码错误
            qDebug()<<"passwd not match";
        }
        else
        {
            //用户名和密码均正确
            m_logIn = true;
            this->close();
        }
    }
}

//chatid,passwd,name,email,history
void Login::getUserInfo(QString name)
{

    QSqlQuery sql_query;        //改变量必须在成功打开数据库后定义才有效

    //查询部分数据(name)

    QString tempstring="select * from user where name='"+name+"'";
    qDebug()<<tempstring;
    if(!sql_query.exec(tempstring))
    {
        qDebug()<<sql_query.lastError();
        matchFlag=false;
    }
    else
    {
        while(sql_query.next())
        {
            usr_id = sql_query.value(0).toInt();
            usr_passwd = sql_query.value(1).toString();
            usr_name = sql_query.value(2).toString();
            //            usr_email = sql_query.value(3).toString();
            usr_history = sql_query.value(4).toInt();

            qDebug()<<QString("chatid=%1    passwd=%2     name=%3    history=%4").arg(usr_id).arg(usr_passwd).arg(usr_name).arg(usr_history);

        }
        if(usr_name==name)  matchFlag=true;
        else                matchFlag=false;
    }

    qDebug()<<matchFlag;


}

