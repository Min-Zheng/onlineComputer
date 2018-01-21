#ifndef CALCULATION_H
#define CALCULATION_H

#include <QDialog>

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
    void clientTransmit();
    void deleteSpace(std::string line);
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
    bool m_isLogIn = false;
    bool m_isAuthorityError = false;
    void split(std::string str);
};

#endif // CALCULATION_H
