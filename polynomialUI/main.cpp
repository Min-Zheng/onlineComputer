#include "calculation.h"
#include <QApplication>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <string>
#include <vector>
#include <boost/thread.hpp>
#include <thread>

using std::cout;                using std::endl;
using namespace boost::asio;    using std::vector;
using std::string;

int main(int argc,char *argv[])
{
    QApplication a(argc, argv);
    Calculation w;
    w.show();
    a.exec();

    return 0;

}
