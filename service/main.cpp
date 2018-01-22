#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "matrix.h"

using namespace boost::asio;
using std::cout;
using std::endl;
using std::string;
using std::vector;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock)
{
    auto ep = sock->remote_endpoint();//
    std::cout<<ep.address().to_string()<<"连接"<<std::endl;

    //    string data;
    boost::system::error_code ec;
    size_t len;

    while(true)
    {
        char data[512];
        string enterError;

        len = sock->read_some(buffer(data), ec);
        if(ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            break;
        }

        char c;
        int m1_r = 0;
        int m1_c = 0;
        int m2_r = 0;
        int m2_c = 0;

        string tmp = data;

        getRC(tmp,m1_r,m1_c,m2_r,m2_c);


        matrix m1(m1_r, std::vector<double>(m1_c));
        matrix m2(m2_r, std::vector<double>(m2_c));
        cout << tmp << endl;
        read_data(tmp,m1,c,m2);
        matrix res;
        try{
            res = calculate(m1,c,m2);
            string ss = chage(res);
            std::stringstream input(ss);
            input >> data;
        }catch(const char *str){
            enterError = str;
            std::stringstream in(enterError);
            in >> data;
        }

        len = sock->write_some(buffer(data), ec);  //客户输入的消息，重新写到客户端
        if(ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            break;
        }
    }

    std::cout<<ep.address().to_string()<<"关闭"<<std::endl;
}

int main()
{
    char data[512];
    io_service service;   //创建调度器
    ip::tcp::endpoint ep(ip::tcp::v4(),2001);  //监听端口
    ip::tcp::acceptor acc(service,ep); //创建连接器

    //    matrix m1;
    //    matrix m2;


    while(true){
        socket_ptr sock(new ip::tcp::socket(service));
        acc.accept(*sock); //接收新的连接
        boost::thread(boost::bind(client_session,sock)); //开辟线程去处理该连接上的事务
        //前两步可以等价于
        //        acc.async_accept(*sock,boost::bind(client_session,sock));
    }
    return 0;
}
