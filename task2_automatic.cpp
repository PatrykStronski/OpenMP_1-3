#include <iostream>
#include "omp.h"
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

const int THREAD_NUMBER = 5;
const int MAX_ITERS = 5;
const bool TEST = false;

int ret_0(){
    return 0;
}

vector<vector<int>> gen_training_matrix(int matrix_size){
    vector<vector<int>> ret(matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        vector<int> row(matrix_size);
        for (int j = 0; j < matrix_size; j++) {
            row[j] = 1;
        }
        ret[i] = row;
    }
    return ret;
}

vector<vector<int>> gen_random_matrix(int matrix_size){
    vector<vector<int>> ret(matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        vector<int> row(matrix_size);
        srand(int(time(nullptr)));
        generate(row.begin(), row.end(), rand);
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
    omp_set_num_threads(THREAD_NUMBER);
    if (argc != 2) return 0;
    const int entities_number = atoi(argv[1]);

    vector<vector<int>> mat1 = gen_random_matrix(entities_number);
    vector<vector<int>> mat2 = gen_random_matrix(entities_number);
    vector<vector<int>> res = gen_empty_matrix(entities_number);

    if (TEST) {
        mat1 = gen_training_matrix(entities_number);
        mat2 = gen_training_matrix(entities_number);
    }

    //print_matrix(res, entities_number);

    int batch_size = entities_number/THREAD_NUMBER;
    time_t exec_time = 0;
    int max_elem = 0;

    cout<<THREAD_NUMBER<<" cores running"<<endl;
    cout<<"IJK ORDER"<<endl;
    for (int i = 0; i < MAX_ITERS; i++) {
        clock_t t = clock();
        #pragma omp parallel for collapse(3)
            for (int i = 0; i < entities_number; i++)
                    for (int j = 0; j < entities_number; j++)
                            for (int k = 0; k < entities_number; k++)
                                    res[i][j] += mat1[i][k] * mat2[k][j];
        exec_time += (clock() - t);
        //print_matrix(res, entities_number);
        //cout << "Time ijk loops is " << t / CLOCKS_PER_SEC << " seconds" <<  endl;
    }
    exec_time = exec_time/MAX_ITERS;
    cout<<exec_time/ CLOCKS_PER_SEC;


    cout << "Time ijk loops is " << exec_time / CLOCKS_PER_SEC << " seconds" <<  endl;
    exec_time = 0;

    cout<<endl<<"JKI ORDER"<<endl;
    for (int i = 0; i < MAX_ITERS; i++) {
        clock_t t = clock();
        #pragma omp parallel for collapse(3)
            for (int j = 0; j < entities_number; j++)
                    for (int k = 0; k < entities_number; k++)
                            for (int i = 0; i < entities_number; i++)
                                    res[i][j] += mat1[i][k] * mat2[k][j];
        exec_time += (clock() - t)/MAX_ITERS;

        //print_matrix(res, entities_number);

        //cout << "Time jki loops is " << t / CLOCKS_PER_SEC << " seconds" <<  endl;
    }

    cout << "Time jki loops is " << exec_time / CLOCKS_PER_SEC << " seconds" <<  endl;
    exec_time = 0;

    cout<<endl<<"IKJ ORDER"<<endl;
    for (int i = 0; i < MAX_ITERS; i++) {
        clock_t t = clock();
        #pragma omp parallel for collapse(3)
            for (int i = 0; i < entities_number; i++)
                    for (int k = 0; k < entities_number; k++)
                            for (int j = 0; j < entities_number; j++)
                                    res[i][j] += mat1[i][k] * mat2[k][j];
        exec_time += (clock() - t)/MAX_ITERS;

        //print_matrix(res, entities_number);

        //cout << "Time ikj loops is " << t / CLOCKS_PER_SEC << " seconds" <<  endl;
    }

    cout << "Time ikj loops is " << exec_time / CLOCKS_PER_SEC << " seconds" <<  endl;
    exec_time = 0;
}