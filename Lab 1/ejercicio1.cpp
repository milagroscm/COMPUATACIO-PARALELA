#include <iostream>
#include<chrono>
#include <vector>

using namespace std;

int main()
{
    int MAX = 40000;
	vector<vector<int> > A(MAX, vector<int>(MAX));
	vector<int> x(MAX), y(MAX);

    auto start = std::chrono::system_clock::now();

    for(int i = 0; i < MAX; i++){
        y[i] = 0;
    }

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAX; j++)
            y[i] += A[i][j] * x[j];
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;

    cout <<"primer for time: " <<elapsed.count() << endl;

    auto start1 = std::chrono::system_clock::now();

    for(int i = 0; i < MAX; i++){
        y[i] = 0;
    }

    for(int j = 0; j < MAX; j++){
        for(int i = 0; i < MAX; i++)
            y[i] += A[i][j] * x[j];
    }

    auto end_1 = std::chrono::system_clock::now();
    auto elapsed_1 = end_1 - start1;

    cout <<"Segundo for time: " <<elapsed_1.count() << endl;
}
