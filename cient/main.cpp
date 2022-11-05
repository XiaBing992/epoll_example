#include<iostream>
#include<thread>
#include<vector>
#include"Cient.h"

using namespace std;

int main()
{
    int n=1;
    Cient cient[n];
    vector<thread> threads;


    for(int i=0;i<n;i++)
    {
        threads.push_back(thread(&Cient::sendData,cient[i]));
    }
    for(int i=0;i<n;i++)
    {
        threads[i].join();
    }
}