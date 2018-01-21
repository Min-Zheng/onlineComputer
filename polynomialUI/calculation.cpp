#include "calculation.h"
#include "ui_calculation.h"
#include "login.h"
#include <iostream>
#include <string>
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>

using std::cout;                using std::endl;
using namespace boost::asio;    using std::vector;
using std::string;


boost::asio::io_service service;
boost::asio::ip::tcp::socket sock(service);

Calculation::Calculation(QDialog *parent) :
    QDialog(parent),
    m_ui(new Ui::Calculation)
{
    m_ui->setupUi(this);
    m_ui->recordlist->hide();
    if(m_isLogIn == false)
        m_ui->recordButton->setEnabled(false);

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    //    m_ui->polynomialGridLayout->SetMinimumSize(m_ui->recordlist->sizeHint());
    //设置为系统试图认同的理想大小，随着部件的改变而改变
    m_ui->operator1->addItem((QString('+')));
    m_ui->operator1->addItem((QString('-')));
    m_ui->operator1->addItem((QString('*')));
    m_ui->operator1->addItem((QString('/')));

    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"),2001);

    sock.async_connect(ep,[](const boost::system::error_code &e){
        if(e){
            std::cout << e.message() << std::endl;
            return;
        }
        std::cout <<"connect succeed " << endl;
    });
    service.run();


    connect(m_ui->loginButton,&QPushButton::clicked,this,&Calculation::login_clicked);
    connect(m_ui->recordButton,&QPushButton::clicked,this,&Calculation::recordButtonClicked);
    connect(m_ui->okButton,&QPushButton::clicked,this,&Calculation::okBUttonClocked);
    connect(this,&Calculation::authorityError,this,&Calculation::onAuthorityError);
    if(m_isLogIn == true){
        m_ui->loginButton->setText("Log Out");
        m_ui->recordButton->setEnabled(true);
    }
}

Calculation::~Calculation()
{
    delete m_ui;
}

void Calculation::clientTransmit()
{
    boost::system::error_code ec;
    size_t len;
    char data[512];
//    string stl;

    //发送 如果不传入 ec(error_code)参数，则出错时会抛出异常，此时可以用try-catch进行捕获，如果也没捕获，程序会直接终止
    len = sock.write_some(buffer(this->m_p), ec);
    if(ec)
        std::cout << boost::system::system_error(ec).what() << std::endl;

    //du
    len = sock.read_some(buffer(data),ec);
    if(ec)
        std::cout << boost::system::system_error(ec).what() << std::endl;
    string str = string(data);
    ServiceSplit(str);

}

void Calculation::deleteSpace(std::string &line)
{
    bool flag = false;
    for (auto it = line.begin(); it != line.end(); it++)
    {
        if(*it == ' '){
            if (!flag){
                it = line.erase(it);
                it--;
            }
            else {
                if (isalnum(*(it-1)))
                    *it = ',';
                else{
                    it = line.erase(it);
                    it--;
                }
            }
        }
        else {
            flag = true;
        }
    }
    //    strcpy(m_p,line);

}

void Calculation::ServiceSplit(std::string &line)
{
    bool flag = false;
    for(auto it = line.begin(); it != line.end(); it++)
    {
        if(*it == ','){
            *it = ' ';
        }else if(*it == '#')
            *it = '\n';
    }

    auto s = QString::fromStdString(line);
    m_ui->resultTextEdit->setText(s);
}

void Calculation::login_clicked()
{
    Login log(this);
    log.show();
    log.exec();

    if(log.m_logIn == true)
        m_isLogIn = true;
}

void Calculation::closeButtonClicked()
{
    this->close();
}

void Calculation::split(string str)
{
    string rel{str};
    deleteSpace(rel);
    char data[512];
    int j = 0;
    int p1_hang = 0;
    int p1_lie = 0;
    int p2_hang = 0;
    int p2_lie = 0;

    //规定只能一个数字接一个空格的输入
    for(int i = 0; i < rel.size();i++){
        if(rel[i] == ','){
            data[i] = rel[i];
            if(j == 0 && p1_hang < 1)
                p1_lie++;
            else if(j ==  1 && p2_hang < 1)
                p2_lie++;
        }else if(rel[i] =='\n'){
            data[i] = '#';
            if(j == 0)
                p1_hang++;
            else
                p2_hang++;
        }else if(rel[i] == '+'||rel[i] == '-'||rel[i] == '*'||rel[i] == '/'){
            data[i] = rel[i];
            j = 1;
        }
        else{
            data[i] = rel[i];
        }
    }

    p1_lie++;
    p2_lie++;
    if(p1_hang >= 5 && p1_lie >= 5){
        if(m_isLogIn == false){
            emit authorityError();
            m_isAuthorityError = true;
        }
    }
    else if(p2_hang >= 5 && p2_lie >= 5){
        if(m_isLogIn == false){
            emit authorityError();
            m_isAuthorityError = true;
        }
    }

    strcpy(this->m_p,data);

}

//删除字符串前面的空格
void deleteBeforeSpace(string &str)
{
    for(auto i = str.begin();i < str.end();i++){
        if(*i == ' '){
            str.erase(i);
            i = str.begin()-1;
        }
        else
            break;
    }
}

void Calculation::okBUttonClocked()
{
    auto str = m_ui->polynomial1->document();
    //    auto s = str->toPlainText();
    auto p1 = str->toPlainText().toStdString();

    str = m_ui->polynomial2->document();
    auto p2 = str->toPlainText().toStdString();

    auto ostr = m_ui->operator1->currentText();
    auto operatorNumber = ostr.toStdString();


    //    deleteSpace(p1);
    //    deleteSpace(p2);
    //    split(p1 + operatorNumber+ p2);
    //    boost::thread(client);
    split(p1 + operatorNumber+ p2);
    if(m_isAuthorityError == false || m_isLogIn == true)
        clientTransmit();

}

void Calculation::recordButtonClicked()
{

    m_ui->recordlist->setVisible(!m_ui->recordlist->isVisible());
}

void Calculation::onAuthorityError()
{
    this->login_clicked();
}

















