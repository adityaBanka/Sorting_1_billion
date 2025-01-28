#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int SIZEOF_ARRAY = 268435456;
int USE_FORKS = 1;

void print(int *array, int size){
    if(size <= 10){
        printf("[");
        for(int i = 0; i<size; i++){
            printf("%d, ", array[i]);
        }
        printf("\b\b]\n");
    }
}

int* getMemoryAddress(int size){
    key_t key = ftok("shmfile.dat", 65);
    if (key == -1) {
        perror("ftok failed");
        return NULL;
    }

    int shmid = shmget(key, size * sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        return NULL;
    }

    int *data = (int *)shmat(shmid, NULL, 0); 
    if (data == (int *)-1) {
        perror("shmat failed");
        return NULL;
    }

    srand(time(NULL));
    for(int i = 0; i < size; i++){
        data[i] = rand()%size;
    }
    
    return(data);
}

void deleteMemoryAddress(int size){
    key_t key = ftok("shmfile.dat", 65);
    if (key == -1) {
        perror("ftok failed");
        return;
    }

    int shmid = shmget(key, size * sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        return;
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        return;
    }
}

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1e9 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

int partition(int *arr, int low, int high){
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;

    return(i+1);
}

void quickSort(int *arr, int low, int high){
    if(low < high){
        int pi = partition(arr, low, high);

        if(high - low > SIZEOF_ARRAY/32 && USE_FORKS)//(high - low > 100000)
        {
            int pid = fork();
            if(pid == 0){
                quickSort(arr, low, pi-1);
                exit(0);
            }
            else{
                quickSort(arr, pi+1, high);
            }
            waitpid(pid, NULL, 0);
        }
        else{
            quickSort(arr, low, pi-1);
            quickSort(arr, pi+1, high);
        }
    }
}

void main()
{
    int *arr = getMemoryAddress(SIZEOF_ARRAY);
    if(arr == NULL){
        printf("Failed to get memory address\n");
        return;
    }
    printf("Memory segment initialization complete, sorting now..\n");
    
    print(arr, SIZEOF_ARRAY);

    struct timespec startWall, endWall;
    clock_gettime(CLOCK_MONOTONIC_RAW, &startWall);
    quickSort(arr, 0, SIZEOF_ARRAY-1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &endWall);

    print(arr, SIZEOF_ARRAY);

    struct timespec deltaWall = diff(startWall, endWall);

    long wallNSec = deltaWall.tv_sec * 1e9 + deltaWall.tv_nsec;

    printf("Sorted %d integer values.\n", SIZEOF_ARRAY);
    printf("Time taken:\n");
    printf("%ld nanoseconds\n", wallNSec);
    printf("%.3f microseconds\n", wallNSec / 1e3);
    printf("%.3f milliseconds\n", wallNSec / 1e6);
    printf("%.3f seconds\n", wallNSec/1e9);
    printf("\n");

    deleteMemoryAddress(SIZEOF_ARRAY);
}
