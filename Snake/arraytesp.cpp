#include <iostream>
#include <vector>
using namespace std;
int ** allocateDynamicArray(int &order){
    int ** arr = new int *[order];
    int cols = order+1;
    int *pool = new int [order * cols];
    for(int i = 0;i < order; i++, pool += cols){
        arr[i] = pool;
    }
    return arr;
}

void deallocateDynamicArray(int **arr){
        delete [] arr[0];
        delete [] arr;
}
int main()
{
    /*
    int order;
    cin >> order;
    int **dynArray = allocateDynamicArray(order);
    deallocateDynamicArray(dynArray);
    */
    vector<int> vect;
    cout << vect[1];
    return 0;
}