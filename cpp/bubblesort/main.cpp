#include <iostream>
#include <random>
#include <ctime>
#include "bubblesort.h"

using namespace std;
void printArr(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return;
}

main(){
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;
    int *arr = new int[n];

    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 101 + 1;
    }
    //printArr(arr, n);

    cout << "Sorting...\n";

    time_t start, finish;
    time(&start);
    bubblesort(arr, n);
    time(&finish);

    //printArr(arr, n);

	cout << difftime(finish, start) << " seconds to sort " << n << " values.\n";
}