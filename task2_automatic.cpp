#include <iostream>
#include "omp.h"
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_ITERS = 5;

int ret_0(){
    return 0;
}

int gen_random_num(){
    return rand() % 10000;
}

vector<vector<int>> gen_random_matrix(int matrix_size){
    vector<vector<int>> ret(matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        vector<int> row(matrix_size);
        generate(row.begin(), row.end(), gen_random_num);
        ret[i] = row;
    }
    return ret;
}

vector<vector<int>> gen_empty_matrix(int matrix_size){
    vector<vector<int>> ret(matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        vector<int> row(matrix_size);
        srand(int(time(nullptr)));
        generate(row.begin(), row.end(), ret_0);
        ret[i] = row;
    }
    return ret;
}

void print_matrix(vector<vector<int>> mat, int entities_number) {
    for (int i = 0; i < entities_number; i++) {      
        for (int j = 0; j < entities_number; j++) {
            cout<<mat[i][j]<<",";
        }
        cout<<endl;
    }
}

int main(int argc, char* argv[]) {
    srand(0);
    if (argc != 2) return 0;
    const int entities_number = atoi(argv[1]);

    vector<vector<int>> mat1 = gen_random_matrix(entities_number);
    vector<vector<int>> mat2 = gen_random_matrix(entities_number);
    vector<vector<int>> res = gen_empty_matrix(entities_number);

    float ijk_base = 0;
    float jki_base = 0;
    float ikj_base = 0;

    //print_matrix(mat1, entities_number);
    //cout<<"---"<<endl;
    //print_matrix(mat2, entities_number);


    for (int core_numb = 1; core_numb < 11; core_numb++) {
        omp_set_num_threads(core_numb);
        cout<<endl<<core_numb<<" cores running"<<endl;
        cout<<"IJK ORDER"<<endl;
        float sum_runtime = 0;
        auto time_start = high_resolution_clock::now();
        auto time_stop = high_resolution_clock::now();
        auto exectime = duration_cast<microseconds>(time_stop - time_start);
        for (int i = 0; i < MAX_ITERS; i++) {
            res = gen_empty_matrix(entities_number);
            time_start = high_resolution_clock::now();
            #pragma omp parallel for collapse(3)
                for (int i = 0; i < entities_number; i++)
                        for (int j = 0; j < entities_number; j++)
                                for (int k = 0; k < entities_number; k++)
                                        res[i][j] += mat1[i][k] * mat2[k][j];
            time_stop = high_resolution_clock::now();
            exectime = duration_cast<microseconds>(time_stop - time_start);
            sum_runtime += (float)exectime.count();
        }

        //print_matrix(res, entities_number);
        if (core_numb == 1) ijk_base = sum_runtime / MAX_ITERS /1000;
        cout << "Time ijk loops for " << core_numb<<" threads is " << sum_runtime / MAX_ITERS /1000 << " miliseconds; Efficiency is: " << ijk_base / (sum_runtime / MAX_ITERS /1000)<<  endl;


        cout<<"JKI ORDER"<<endl;
        sum_runtime = 0;
        for (int i = 0; i < MAX_ITERS; i++) {
            res = gen_empty_matrix(entities_number);
            time_start = high_resolution_clock::now();
            #pragma omp parallel for collapse(3)
                for (int j = 0; j < entities_number; j++)
                        for (int k = 0; k < entities_number; k++)
                                for (int i = 0; i < entities_number; i++)
                                        res[i][j] += mat1[i][k] * mat2[k][j];
            time_stop = high_resolution_clock::now();
            exectime = duration_cast<microseconds>(time_stop - time_start);
            sum_runtime += (float)exectime.count();
        }

        //print_matrix(res, entities_number);
        if (core_numb == 1) jki_base = sum_runtime / MAX_ITERS /1000;
        cout << "Time jki loops for "<< core_numb<<" threads is " << sum_runtime / MAX_ITERS /1000 << " miliseconds; Efficiency is: " << jki_base / (sum_runtime / MAX_ITERS /1000)<< endl;

        cout<<"IKJ ORDER"<<endl;
        sum_runtime = 0;
        for (int i = 0; i < MAX_ITERS; i++) {
            res = gen_empty_matrix(entities_number);
            time_start = high_resolution_clock::now();
            #pragma omp parallel for collapse(3)
                for (int i = 0; i < entities_number; i++)
                        for (int k = 0; k < entities_number; k++)
                                for (int j = 0; j < entities_number; j++)
                                        res[i][j] += mat1[i][k] * mat2[k][j];
            time_stop = high_resolution_clock::now();
            exectime = duration_cast<microseconds>(time_stop - time_start);
            sum_runtime += (float)exectime.count();
        }

        //print_matrix(res, entities_number);
        if (core_numb == 1) ikj_base = sum_runtime / MAX_ITERS /1000;
        cout << "Time ikj loops for "<< core_numb<<" threads is " << sum_runtime / MAX_ITERS /1000 << " miliseconds; Efficiency is: " << ikj_base / (sum_runtime / MAX_ITERS /1000)<<  endl;

    }
}