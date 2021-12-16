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
    if (argc != 2) return 0;
    const int entities_number = atoi(argv[1]);

    vector<int> vec = gen_random_vec(entities_number);
    for (int i = 0; i < entities_number; i++) {
        cout<<vec[i]<<endl;
    }
    int batch_size = entities_number/THREAD_NUMBER;

    int max_elem = 0;
    #pragma omp parallel reduction (max:max_elem) num_threads(6)
    {
        int rank = omp_get_thread_num();
        int start = batch_size * rank;
        int end = batch_size * (rank + 1);
        if (rank == THREAD_NUMBER - 1) {
            end = entities_number;
        }

        max_elem = vec[start];
        for (int i = start + 1; i < end; i++) {
            if (vec[i] > max_elem) {
                max_elem = vec[i];
            }
        }
    }
    cout<<endl<<"Biggest is"<<max_elem<<endl;
}
