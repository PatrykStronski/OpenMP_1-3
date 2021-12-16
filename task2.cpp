#include <iostream>
#include "omp.h"
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

const int THREAD_NUMBER = 6;

int ret_0(){
    return 0;
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

int main(int argc, char* argv[]) {
    if (argc != 2) return 0;
    const int entities_number = atoi(argv[1]);

    vector<vector<int>> mat = gen_random_matrix(entities_number);
    vector<vector<int>> res = gen_empty_matrix(entities_number);
    for (int i = 0; i < entities_number; i++) {      
        for (int j = 0; j < entities_number; j++) {
            cout<<mat[i][j]<<",";
        }
        cout<<endl;
    }

    int batch_size = entities_number/THREAD_NUMBER;

    int max_elem = 0;

}