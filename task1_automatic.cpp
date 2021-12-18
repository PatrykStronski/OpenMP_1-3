#include <iostream>
#include "omp.h"
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

int* gen_random_vec(int vec_size){
    int* ret = new int[vec_size];
    srand(0);
    for (int i = 0; i < vec_size; i++){
        ret[i] = (int)(rand() % 100000);
    }
    return ret;
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 0;
    const int entities_number = atoi(argv[1]);

    int* vec = gen_random_vec(entities_number);
    //for (int i = 0; i < entities_number; i++) {
    //    cout<<vec[i]<<',';
    //}
    //cout<<endl;

    int max_elem = 0;
    for (int thread_num = 1; thread_num < 11; thread_num++) {
        omp_set_num_threads(thread_num);
        auto time_start = high_resolution_clock::now();
        #pragma omp parallel for reduction (max:max_elem)
            for (int i = 0; i < entities_number; i++) 
                if (vec[i] > max_elem)
                max_elem = vec[i];
        auto time_stop = high_resolution_clock::now();
        auto exectime = duration_cast<microseconds>(time_stop - time_start);
        cout<<endl<<"Biggest is"<<max_elem<<endl;
        cout<<"Execution time for "<<thread_num<<" is: "<<(float)exectime.count()<<endl;
    }
}
