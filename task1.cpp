#include <iostream>
#include "omp.h"
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

const int THREAD_NUMBER = 6;

vector<int> gen_random_vec(int vec_size){
    vector<int> ret(vec_size);
    srand(int(time(nullptr)));
    generate(ret.begin(), ret.end(), rand);
    return ret;
}

int main(int argc, char* argv[]) {
    omp_set_num_threads(THREAD_NUMBER);
    if (argc != 2) return 0;
    const int entities_number = atoi(argv[1]);

    vector<int> vec = gen_random_vec(entities_number);
    //for (int i = 0; i < entities_number; i++) {
    //    cout<<vec[i]<<endl;
    //}
    int batch_size = entities_number/THREAD_NUMBER;

    int max_elem = 0;
    time_t exec_time = clock();

    #pragma omp parallel reduction (max:max_elem)
    {
        int rank = omp_get_thread_num();
        cout<<"thread nr "<<rank<<endl;
        int start = batch_size * rank;
        int end = batch_size * (rank + 1);
        if (rank == THREAD_NUMBER - 1) {
            end = entities_number;
        }

        max_elem = vec[start];
        for (int i = start; i < end; i++) {
            if (vec[i] > max_elem) {
                max_elem = vec[i];
            }
        }
    }
    exec_time = clock() - exec_time;
    cout<<endl<<"Biggest is"<<max_elem<<endl;
    cout<<"Execution time: "<<(float)exec_time/(float)CLOCKS_PER_SEC<<endl;
}
