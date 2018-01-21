#ifndef CALCULATION_H
#define CALCULATION_H

#include <QDialog>
#include "login.h"

namespace Ui {
class Calculation;
}

class Calculation : public QDialog
{
    Q_OBJECT

public:
    explicit Calculation(QDialog *parent = 0);
    ~Calculation();

    char m_p[512];
    bool m_isLogIn = false;
    void clientTransmit();
    void deleteSpace(std::string &line);
    void ServiceSplit(std::string &line);
    bool isLogIn(){
        return m_isLogIn;
    }

signals:
    void authorityError();

private slots:
    void login_clicked();   //登录按键槽函数
    void closeButtonClicked();
    void okBUttonClocked();
    void recordButtonClicked();
    void onAuthorityError();

private:
    Ui::Calculation *m_ui;
    bool m_isAuthorityError = false;
    void split(std::string str);
//    Login m_log;
};

#endif // CALCULATION_H
