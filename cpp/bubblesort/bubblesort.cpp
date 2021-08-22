#include "bubblesort.h"

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void bubblesort(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (array[j] > array[j+1]) {
                swap(array[j], array[j+1]);
            }
        }
    }
}