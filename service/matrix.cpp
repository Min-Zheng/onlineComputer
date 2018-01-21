#include "matrix.h"
#include <string>
#include <cctype>
#include <sstream>

using std::stringstream;


matrix add(matrix m1, matrix m2)
{
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size())
        throw "Invalid-matrix\n";
    matrix *m = new matrix(m1);
    for (int i = 0; i != m1.size(); i++)
        for (int j = 0;j != m1[0].size();j++)
            (*m)[i][j] = m1[i][j] + m2[i][j];

    return *m;
}

matrix sub(matrix m1, matrix m2)
{
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size())
        throw "Invalid-matrix\n";
    matrix *m = new matrix(m1);
    for (int i = 0; i != m1.size(); i++)
        for (int j = 0;j != m1[0].size();j++)
            (*m)[i][j] = m1[i][j] - m2[i][j];

    return *m;
}

//matrix numMul(double m1, matrix m2)
//{
//    matrix *m = new matrix(m2);
//    for (int i = 0; i != m2.size(); i++)
//        for (int j = 0;j != m2[0].size();j++)
//            (*m)[i][j] = m1*m2[i][j];
//    return *m;
//}

matrix mul(matrix m1, matrix m2)
{
    if (m1[0].size() != m2.size())
        throw "Invalid-matrix\n";
    double sum = 0.0;
    matrix *m = new matrix(m1.size(), std::vector<double>(m2[0].size()));
    for (int i = 0; i != m1.size(); i++)
        for (int j = 0;j != m2.size();j++)
        {
            for (int k = 0; k < m2.size(); k++)
            {
                sum += m1[i][k]*m2[k][j];
            }
            (*m)[i][j] = sum;
            sum = 0.0;
        }
    return (*m);
}


double getDet(matrix m, int order)
{
    if (m[0].size() != m[0].size()){
        throw "Invalid-matrix\n";
    }
    if (order == 1)
        return m[0][0];
    double res = 0;
    matrix tmp(m.size(), std::vector<double>(m[0].size()));
    for (int i = 0; i < order; i++){
        for (int j = 0; j < order - 1; j++){
            for (int k = 0; k < order - 1; k++)
                tmp[j][k] = m[j+1][(k >= i) ? k+1 : k];
        }
        double t = getDet(tmp, order-1);
        if (i%2 == 0)//shi fou dai fu hao
            res += m[0][i] * t;
        else
            res -= m[0][i] * t;
    }

    return res;
}

matrix getAdjointMatrix(matrix m, int order)
{
    if (m.size() != m[0].size()){
        throw "Invalid-matrix";
    }
    matrix *res = new matrix(m.size(), std::vector<double>(m[0].size()));
    matrix *tmp = new matrix(m.size(), std::vector<double>(m[0].size()));
    if (order == 1){
        (*res)[0][0] = 1;
        return m;
    }

    for (int i = 0; i < order; i++){
        for (int j = 0; j < order; j++){
            for (int k = 0; k < order - 1; k++){
                for (int t = 0; t < order - 1; t++)
                    (*tmp)[k][t] = m[k >= i?k+1:k][t >= j ?t+1:t];
            }

            (*res)[j][i] = getDet((*tmp), order - 1);
            if ((i+j)%2 == 1)
                (*res)[j][i] = -(*res)[j][i];
        }
    }
    return (*res);
}



matrix getInv(matrix m)
{
    double d = getDet(m,m.size());
    matrix *tmp = new matrix(m.size(), std::vector<double>(m[0].size()));
    matrix *res = new matrix(m.size(), std::vector<double>(m[0].size()));
    if (d == 0)
        throw "Invalid-matrix\n";
    else{
        (*tmp) = getAdjointMatrix(m,m.size());
        for (int i = 0; i < m.size(); i++)
            for (int j = 0; j < m.size(); j++)
                (*res)[i][j] = (*tmp)[i][j]/d;
    }

    return (*res);
}

matrix div(matrix m1, matrix m2)
{
    matrix tmp = mul(m1, getInv(m2));
    return tmp;
}


//拆分字符串为 矩阵，操作符
void read_data(std::string line, matrix &m1, char &c, matrix &m2)
{
    int start = 0;
    int end = 0;
    int c_location = 0;
    int row = 0;
    int j = 0;
    for (int i = 0; i < line.size() + 1; i++){
        if (!c_location){
            if (isalnum(line[i]))
                end++;
            else{
                if (line[i] == '+'){
                    c_location = i;
                    c = line[i];
                    row = 0;
                    j = 0;
                    start = i + 1;
                    end = i + 1;

                }else if(line[i] == '-'){
                    c_location = i;
                    c = line[i];
                    row = 0;
                    j = 0;
                    start = i + 1;
                    end = i + 1;

                }else if
                        (line[i] == '*'){
                    c_location = i;
                    c = line[i];
                    row = 0;
                    j = 0;
                    start = i + 1;
                    end = i + 1;

                }
                else if(line[i] == '/'){
                    c_location = i;
                    c = line[i];
                    row = 0;
                    j = 0;
                    start = i + 1;
                    end = i + 1;

                }else{/*(1, std::vector<double>(3))*/
                    if(end > start){
                        std::string s("s",end-start);
                        for(int j = 0; j != end-start; j++)
                        {
                            s[j] = line[start + j];
                        }
                        std::stringstream input(s);
                        double d;
                        input >> d;
                        m1[row][j++]=d;

                        start = end + 1;
                        end = end + 1;
                        if (line[i] == '#'){
                            int k = m1.size();
                            if (row != k-1){
                                row++;
                                //                                m1.push_back(m1[row]);
                            }

                            j = 0;
                        }
                    }
                }
            }
        }else{
            if (isalnum(line[i]))
                end++;
            else if(line[i] == '#' ||line[i] == ','){

                std::string s("s",end-start);
                for(int j = 0; j != end-start; j++)
                {
                    s[j] = line[start + j];
                }
                std::stringstream input(s);
                double d;
                input >> d;
                m2[row][j++] = d;

                start = end + 1;
                if(end + 1 != line.size())
                    end = end + 1;

                if (line[i] == '#'){
                    //                    m2.push_back(m2[row]);
                    row++;
                    j = 0;
                }
            }
        }
    }

}


matrix calculate(matrix m1, char c, matrix m2)
{
    if (c == '+')
        return add(m1,m2);
    if (c == '-')
        return sub(m1,m2);
    if (c == '*')
        return mul(m1,m2);
    if (c == '/')
        return div(m1,m2);
}


std::string chage(matrix m)
{
    std::string res;
    for (int i = 0; i < m.size(); i++){
        for (int j = 0;j < m[i].size();j++){
            if (j < m[i].size() -1)
                res += std::to_string(m[i][j])+',';
            else {
                res += std::to_string(m[i][j])+'#';
            }
        }
    }
    return res;
}


void getRC(std::string tmp, int &m1_r, int &m1_c, int &m2_r, int &m2_c)
{
    auto rel{tmp};
    int j = 0;
    int p1_hang = 0;
    int p1_lie = 0;
    int p2_hang = 0;
    int p2_lie = 0;

    //规定只能一个数字接一个空格的输入
    for(int i = 0; i < rel.size();i++){
        if(rel[i] == ','){
            if(j == 0 && p1_hang < 1)
                p1_lie++;
            else if(j ==  1 && p2_hang < 1)
                p2_lie++;
        }else if(rel[i] =='#'){
            if(j == 0)
                p1_hang++;
            else
                p2_hang++;
        }else if(rel[i] == '+'||rel[i] == '-'||rel[i] == '*'||rel[i] == '/'){
            j = 1;
        }
    }
    m1_r = p1_hang;
    m1_c = p1_lie + 1;
    m2_r = p2_hang;
    m2_c = p2_lie + 1;
}

//void getRC(std::string tmp, int &m1_r, int &m1_c, int &m2_r, int &m2_c)
//{
//    int c_loc = 0;
//    for (int i = 0; i < tmp.size();i++){
//        if (!c_loc){
//            if (!isalnum(tmp[i])){
//                if (tmp[i] == ','){
//                    if (m1_r == 0)
//                        m1_c++;
//                }
//                else if (tmp[i] == '#'){
//                    if (m1_r == 0)
//                        m1_c++;
//                    m1_r++;
//                }
//                else{
//                    c_loc = i;
//                }
//            }
//        }else{
//            if (!isalnum(tmp[i])){
//                if (tmp[i] == ','){
//                    if (m2_r == 0)
//                        m2_c++;
//                }
//                if (tmp[i] == '#'){
//                    if (m2_r == 0)
//                        m2_c++;
//                    m2_r++;
//                }
//            }
//        }
//    }
//    m2_r++;
//}
