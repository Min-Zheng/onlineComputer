#include "register.h"
#include "ui_register.h"

Register::Register(QDialog *parent) :
    QDialog(parent),
    m_ui(new Ui::Register)
{
    m_ui->setupUi(this);

    connect(m_ui->okBtn,&QPushButton::clicked,this,&Register::okBtn_clicked);
    connect(m_ui->closeBtn,&QPushButton::clicked,this,&Register::closeBtn_clicked);

    connect((Login *)parent,&Login::transmitdb,this,&Register::receivedb);
    dboperation();
}

Register::~Register()
{
    delete m_ui;
}

QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
QString create_sql = "create table user (chatid int primary key, passwd varchar(30), name varchar(30), history int)";
QString select_max_sql = "select max(chatid) from user";
QString insert_sql = "insert into user values (?, ?, ?, ?)";
QString select_sql = "select name from user";

//chatid,passwd,name
void Register::dboperation()
{

}

void Register::okBtn_clicked()
{
    bool nameFlag=false;    //用户名有效标志
    bool passwdFlag=false;  //密码有效标志
    int newchatid=max_id+1;
    QString newpasswd=NULL;
    QString newname=NULL;
//    QString newemail=ui->emailLineEdit->text();

    if(m_ui->passwd1LineEdit->text()==""||m_ui->passwd2LineEdit->text()=="")
    {
        passwdFlag=false;
    }
    else if(m_ui->passwd1LineEdit->text()==m_ui->passwd2LineEdit->text())    //两次密码相同
    {
        //newpasswd=ui->passwd1LineEdit->text();
        passwdFlag=true;
        qDebug() << "password right";
    }
    else
    {
        qDebug()<<"passwd err";
        passwdFlag=false;
        //return;
    }

    //以下为数据库的操作
    QSqlQuery sql_query;

    //查询最大id
    max_id = 0;
    sql_query.prepare(select_max_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            max_id = sql_query.value(0).toInt();
            qDebug()<<QString("max chatid:%1").arg(max_id);
        }
    }


    //查询部分数据(name)
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(1)
        {
            if(sql_query.next())    //name有数据
            {
                QString name = sql_query.value(0).toString();
                qDebug()<<QString("name=%1").arg(name);

                if(m_ui->nameLineEdit->text()==name)    //用户名已经存在
                {
                    qDebug()<<"name existed";
                    nameFlag=false;
                    break;
                }
                else
                {
                    //newname=ui->nameLineEdit->text();
                    nameFlag=true;
                }
            }
            else
            {       //name列为空
                nameFlag=true;
                break;
            }
        }
    }

    newchatid=max_id+1;
    if(nameFlag==true) newname=m_ui->nameLineEdit->text();
    else                return;
    if(passwdFlag==true)    newpasswd=m_ui->passwd1LineEdit->text();
    else                    return;

    //插入数据
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(newchatid);              //chatid
    sql_query.addBindValue(newpasswd);              //passwd
    sql_query.addBindValue(newname);                //name
//    sql_query.addBindValue(newemail);               //email
    sql_query.addBindValue(0);                      //history
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"inserted!";
    }

    this->close();
}

void Register::receivedb(QSqlDatabase db)
{
    qDebug()<<"received db";
    database=db;
    if(!database.isOpen())
    {
        if(!database.open())
        {
            qDebug()<<database.lastError();
            qFatal("failed to connect.") ;
            return;
        }
        else
        {
        }
    }

}

void Register::closeBtn_clicked()
{
    this->close();
}
