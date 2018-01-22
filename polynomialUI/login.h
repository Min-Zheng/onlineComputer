#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget>
#include "register.h"
#include <QPainter>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    QString usr_name;
    explicit Login(QDialog *parent = 0);
    ~Login();

    bool m_logIn = false;

signals:
    void transmitdb(QSqlDatabase db);

private slots:
    void register_clicked();    //注册按键槽函数
    void getUserInfo(QString name);
    void login_clicked();   //登录按键槽函数

private:
    Ui::Login *m_ui;
    QSqlDatabase database;
    bool usrtableFlag;
    bool rectableFlag;

    int usr_id;
    QString usr_passwd;

//    QString usr_email;
    int usr_history;
    bool matchFlag;

    QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
    QString create_sql = "create table user (chatid int primary key, passwd varchar(30), name varchar(30),  history int)";
    QString create_sql1 = "create table record (name varchar(30), record varchar(512))";
    QString select_max_sql = "select max(chatid) from user";
    QString insert_sql = "insert into user values (?, ?, ?, ?)";
    //QString update_sql = "update user set name = :name where chatid = :chatid";
    QString select_sql = "select name from user";
    //QString select_all_sql = "select * from user";
    //QString delete_sql = "delete from user where chatid = ?";
    //QString clear_sql = "delete from user";

    QString select_nameInfo = "selcet * from user wher name=";


};

#endif // LOGIN_H
