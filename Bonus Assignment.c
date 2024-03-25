//
//  main.c
//  Bonus program
//
//  Created by Axel Diaz Bringuez on 3/22/24.
//

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
    extraMemoryAllocated += sz;
    size_t* ret = malloc(sizeof(size_t) + sz);
    *ret = sz;
    printf("Extra memory allocated, size: %ld\n", sz);
    return &ret[1];
}

//start here

void DeAlloc(void* ptr)
{
    size_t* pSz = (size_t*)ptr - 1;
    extraMemoryAllocated -= *pSz;
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
    return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void before_heapsort(int arr[], int n, int i) {
    
    int large = i;
    int L_placeholder = 2*i + 1;
    int placeholder = 2*i + 2;

    if (L_placeholder < n && arr[L_placeholder] > arr[large])
        large = L_placeholder;
    else if (placeholder < n && arr[placeholder] > arr[large])
        large = placeholder;
    else if (large != i){
        int swap = arr[i];
        arr[i] = arr[large];
        arr[large] = swap;
    }
    
    before_heapsort(arr, n, large);
}

void heapSort (int arr[], int n, int i){
    
    
    
    for (int i= n/2 -1; i >= 0; i--) {
        before_heapsort (arr, n, i);
    }
    for (int i= n-1; i >= 0; i--){
        int placeholder = arr[0];
        arr[0] = arr[i];
        arr[i] = placeholder;
        
        before_heapsort (arr, n, 0);
    }
        
    
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated

void mergeSort(int arr[], int l, int r) {
    
    int i, placeholder, temp;
    
    int temp2 = l+(r-l)/2;
    
    int n1 = temp2 - l + 1;
    
    int n2 = r - temp2;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        
        L[i] = arr[l + i];
    
    for (placeholder = 0; placeholder < n2; placeholder++)
        R[placeholder] = arr[temp2 + 1+ placeholder];

    i = 0; placeholder = 0; temp = l;
    while (i < n1 && placeholder < n2) {
        if (L[i] <= R[placeholder]) {
            arr[temp] = L[i];
            i++;
        }
        else {
            arr[temp] = R[placeholder];
            placeholder++;
        }
        temp++;
    }

    while (i < n1) {
        arr[temp] = L[i];
        i++;
        temp++;
    }
    while (placeholder < n2) {
        arr[temp] = R[placeholder];
        placeholder++;
        temp++;
    }
    
    
    
}
    

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int arr[], int n) {
    int i, placeholder, temp;
    for (i = 1; i < n; i++) {
        placeholder = arr[i];
        temp = i - 1;

        while (temp >= 0 && arr[temp] > placeholder) {
            arr[temp + 1] = arr[temp];
            temp = temp - 1;
        }
        arr[temp + 1] = placeholder;
    }
}


// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    int i, temp, placeholder;
    for (i = 0; i < n-1; i++)
        for (temp = 0; temp < n-i-1; temp++)
            if (pData[temp] > pData[temp+1]) {
                placeholder = pData[temp];
                pData[temp] = pData[temp+1];
                pData[temp+1] = placeholder;
            }
}


// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    int i, j, index, temp;
    for (i = 0; i < n-1; i++) {
        index = i;
        for (j = i+1; j < n; j++)
          if (pData[j] < pData[index])
              index = j;

        temp = pData[index];
        pData[index] = pData[i];
        pData[i] = temp;
    }
}


// end here

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
    int dataSz = 0;
    *ppData = NULL;

    FILE* inFile = fopen(inputFileName,"r");
    
    if (inFile)
    {
        fscanf(inFile,"%d\n",&dataSz);
        *ppData = (int *)Alloc(sizeof(int) * dataSz);
        // Implement parse data block
        
        if (inFile == NULL) {
            printf ("file failed to open");
            return -1;
        }
    }
    
    fclose (inFile);
    return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i=0;i<100;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");
    
    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
    
        for (i=0;i<3;++i)
        {
            int counter = 0;
            int *pDataSrc, *pDataCopy;
            int dataSz = parseData(fileNames[i], &pDataSrc);
            
            if (dataSz <= 0)
                continue;
            
            pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
            
            printf("---------------------------\n");
            printf("Dataset Size : %d\n",dataSz);
            printf("---------------------------\n");
            
            printf("Selection Sort:\n");
            memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
            extraMemoryAllocated = 0;
            start = clock();
            selectionSort(pDataCopy, dataSz);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
            printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
            printArray(pDataCopy, dataSz);
            
            printf("Insertion Sort:\n");
            memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
            extraMemoryAllocated = 0;
            start = clock();
            insertionSort(pDataCopy, dataSz);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
            printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
            printArray(pDataCopy, dataSz);
            
            printf("Bubble Sort:\n");
            memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
            extraMemoryAllocated = 0;
            start = clock();
            bubbleSort(pDataCopy, dataSz);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
            printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
            printArray(pDataCopy, dataSz);
            
            printf("Merge Sort:\n");
            memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
            extraMemoryAllocated = 0;
            start = clock();
            mergeSort(pDataCopy, 0, dataSz - 1);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
            printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
            printArray(pDataCopy, dataSz);
            
            printf("Heap Sort:\n");
            memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
            extraMemoryAllocated = 0;
            start = clock();
            heapSort(pDataCopy, 0, dataSz - 1);
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
            printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
            printArray(pDataCopy, dataSz);
            
            DeAlloc(pDataCopy);
            DeAlloc(pDataSrc);
            
            if (counter == 3)
                break;
            else
                counter++;
        }
    
    return 0;
    
}
