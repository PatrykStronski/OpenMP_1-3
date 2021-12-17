#include<iostream>

using namespace std;

int count_words(char* sentence) {
    int count = 1;
    int iter = 0;
    char sign;
    char pre_sign = ' ';
    do {
        if ((int)sentence[iter] < 48 && (int)sentence[iter] > 32) {
            iter++;
            continue;
        }
        pre_sign = sign;
        sign = sentence[iter];
        if (sign == ' ' && pre_sign != ' ' && iter != 0) count++;
        iter++;
    } while (sign != '\0');
    if (pre_sign == ' ' || ((int)pre_sign < 48 && count == 1)) count --;
    return count;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout<<"No string provided"<<endl;
        return 0;
    }

    int count = 0;
    if (argv[0] != "\0") {
        count = count_words(argv[1]);
    }
    cout<<"Word count is: "<<count<<endl;
}