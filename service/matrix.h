#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include<string>


typedef std::vector<std::vector<double>> matrix;

void read_data(std::string line, matrix &m1, char &c, matrix &m2);//拆分客户端字符串为 矩阵，操作符
void getRC(std::string tmp, int &m1_r, int &m1_c, int &m2_r, int &m2_c);//获取矩阵行列数
matrix add(matrix m1, matrix m2);// 加法
matrix sub(matrix m1, matrix m2);//减法
matrix div(matrix m1, matrix m2);//除法
matrix mul(matrix m1, matrix m2);// 乘法
double getDet(matrix m, int order);// 求行列式
matrix getAdjointMatrix(matrix m, int order);// 求伴随矩阵
matrix getInv(matrix m);// 求逆

matrix calculate(matrix m1, char c, matrix m2);
std::string chage(matrix m);//将结果变为字符串

#endif // MATRIX_H

