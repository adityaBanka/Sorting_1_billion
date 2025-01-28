#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int SIZE_OF_ARRAY = 100000000;

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    
    int pivot = arr[high];

    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);  
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        
        int pi = partition(arr, low, high);


        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {

    srand(time(NULL));

    int size = SIZE_OF_ARRAY;
    int *array = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        array[i] = rand()%size;
    }

    if(size <= 10){
        printf("[");
        for(int i = 0; i<size; i++){
            printf("%d, ", array[i]);
        }
        printf("\b\b]\n");
    }

    time_t startTime = time(NULL);
    quickSort(array, 0, size - 1);
    time_t endTime = time(NULL);
    
    if(size <= 10){
        printf("[");
        for(int i = 0; i<size; i++){
            printf("%d, ", array[i]);
        }
        printf("\b\b]\n");
    }

    printf("Sorted %d INTEGERS\n", size);
    printf("Time taken: %ld\n", endTime - startTime);
    
    return 0;
}