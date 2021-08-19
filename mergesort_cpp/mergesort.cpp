#include "headers/mergesort.h"

using namespace std;
void merge(int *arr, int lo, int mid, int hi) {
    int i, j, k, l_sz, r_sz;

    // Get the size of the left and right sub-arrays
    l_sz = mid - lo + 1;
    r_sz = hi - mid;

    // Create the sub-arrays
    int *l_sub = new int[l_sz];
    int *r_sub = new int[r_sz];

    // Populate the sub-arrays
    for (i = 0; i < l_sz; i++) {
        l_sub[i] = arr[lo + i];
    }
    for (j = 0; j < r_sz; j++) {
        r_sub[j] = arr[mid + j + 1];
    }

    // Merge both sub-arrays back into their main piece
    i = 0;
    j = 0;
    k = lo;
    while (i < l_sz && j < r_sz) {
        if (l_sub[i] <= r_sub[j])
            arr[k] = l_sub[i++];
        else
            arr[k] = r_sub[j++];
        k++;
    }
    while (i < l_sz)
        arr[k++] = l_sub[i++];
    while (j < r_sz)
        arr[k++] = r_sub[j++];  
}

void mergesort(int *arr, int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo)/2;
        mergesort(arr, lo, mid);
        mergesort(arr, mid+1, hi);   
        merge(arr, lo, mid, hi);     
    }
}

void mergesort(int *arr, int n) {
    cout << "Sorting..." << endl;
    int lo = 0;
    int hi = n-1;
    mergesort(arr, lo, hi);
    return;
}