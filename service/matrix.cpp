#include "matrix.h"
#include <string>
#include <cctype>
#include <sstream>

matrix add(matrix m1, matrix m2)
{
    if (m1.size() != m2.size() && m1[0].size() != m2[0].size())
        throw "can't calculate\n";
    matrix *m = new matrix(m1);
    for (int i = 0; i != m1.size(); i++)
        for (int j = 0;j != m1[0].size();j++)
            (*m)[i][j] = m1[i][j] + m2[i][j];

    return *m;
}

matrix sub(matrix m1, matrix m2)
{
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size())
        throw "can't calculate\n";
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
        throw "can't calculate\n";
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
    if (m.size() != m[0].size())
        throw "The number of rows is not equal. Can't calculate!\n";
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
    if (m.size() != m[0].size())
        throw "The number of rows is not equal. Can't calculate!\n";
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
        throw "can't calculate!\n";
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

matrix s_add(std::string s1, std::string s2)
{

}


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
                                m1.push_back(m1[row]);
                            }

                            j = 0;
                        }
                    }
                }
            }
        }else{
            if (isalnum(line[i]))
                end++;
            else{

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
                end = end + 1;

                if (line[i] == '#'){
                    m2.push_back(m2[row]);
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
    for (int i = 0; i != m.size(); i++){
        for (int j = 0;j != m[0].size();j++){
            res += std::to_string(m[i][j])+',';
        }
        res += '#';
    }
    return res;
}
