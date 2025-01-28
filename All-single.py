import time
import random

SIZE_OF_ARRAY = 1000000

def bubbleSort(array):
    size = len(array)
    for i in range(size):
        for j in range(size):
            if (array[i] < array[j]):
                array[i], array[j] = array[j], array[i]
    return(array)

def selectionSort(array):
    size = len(array)
    for i in range(size):
        smallIndex = i
        for j in range (i, size):
            if(array[j] < array[smallIndex]):
                smallIndex = j
        array[i], array[smallIndex] = array[smallIndex], array[i]
    return(array)

def merge(arr, left, mid, right):
    n1 = mid - left + 1
    n2 = right - mid

    L = [0] * n1
    R = [0] * n2

    for i in range(n1):
        L[i] = arr[left + i]
    for j in range(n2):
        R[j] = arr[mid + 1 + j]

    i = 0  
    j = 0 
    k = left 

    while i < n1 and j < n2:
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1

    while i < n1:
        arr[k] = L[i]
        i += 1
        k += 1

    while j < n2:
        arr[k] = R[j]
        j += 1
        k += 1
def MergeSort(arr, left, right):
    if left < right:
        mid = (left + right) // 2
        MergeSort(arr, left, mid)
        MergeSort(arr, mid + 1, right)
        merge(arr, left, mid, right)
    return(arr)

def partition(arr, low, high):
    
    pivot = arr[high]

    i = low - 1

    for j in range(low, high):
        if arr[j] < pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]

    arr[i+ 1], arr[high] = arr[high], arr[i + 1]
    return i + 1
def quickSort(arr, low, high):
    if low < high:
        
        pi = partition(arr, low, high)

        quickSort(arr, low, pi - 1)
        quickSort(arr, pi + 1, high)
    return(arr)

def main():
    size = SIZE_OF_ARRAY
    array = []
    for i in range(size):
        array.append(random.randint(0, size))

    if(size <= 10):
        print(array)

    startTime = time.time()

    # array = bubbleSort(array)
    # array = selectionSort(array)
    # array = MergeSort(array, 0, size - 1)
    # array = quickSort(array, 0, size - 1)
    
    endTime =  time.time()

    if(size <= 10):
        print(array)

    print("Sorted", size, "INTEGERS")
    print("Time taken: ", endTime - startTime)
    
if __name__ == "__main__":
    main()