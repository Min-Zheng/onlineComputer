#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include<string>


typedef std::vector<std::vector<double>> matrix;

void read_data(std::string line, matrix &m1, char &c, matrix &m2);

matrix add(matrix m1, matrix m2);// jia fa
matrix sub(matrix m1, matrix m2);//jian fa
matrix div(matrix m1, matrix m2);//chu fa
matrix mul(matrix m1, matrix m2);// cheng fa
double getDet(matrix m, int order);// order jie shu
matrix getAdjointMatrix(matrix m, int order);// ban sui ju zhen
matrix getInv(matrix m);// qiu ni

matrix calculate(matrix m1, char c, matrix m2);
std::string chage(matrix m);

#endif // MATRIX_H

