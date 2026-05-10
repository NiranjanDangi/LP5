#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

void bubbleSortParallel(vector<int> &arr){
    int n = arr.size();
    for(int i = 0; i < n; i++){
        #pragma omp parallel for
        for(int j = (i%2); j < n-1; j+=2){
            if(arr[j] > arr[j+1]) swap(arr[j],arr[j+1]);
        }
    }
}

void bubbleSort(vector<int>&arr){
    int n = arr.size();
    for(int i = 0; i < n-1; i++){
        bool swapped = false;
        for(int j = 0; j < n - 1 - i; j++){
            if(arr[j] > arr[j+1]) swap(arr[j],arr[j+1]),swapped = true;
        }
        if(!swapped) break;
    }
}

void merge1(vector<int>&arr, int left, int mid,int right){
    vector<int> temp(right-left+1);
    int ptr = 0, i = left, j = mid+1;

    while(i <= mid && j <= right){
        if(arr[i] < arr[j]) temp[ptr++] = arr[i++];
        else temp[ptr++] = arr[j++];
    }

    while(i <= mid){
        temp[ptr++] = arr[i++];
    }

    while(j <= right){
        temp[ptr++] = arr[j++];
    }

    for(int i = 0; i < right-left+1; i++){
        arr[i+left] = temp[i];
    }
}

void mergeSortUtil(vector<int>&arr, int left, int right){
    if(left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortUtil(arr,left,mid);
    mergeSortUtil(arr,mid+1,right);
    merge1(arr,left,mid,right);
}

void mergeSort(vector<int> &arr){
    int n = arr.size();
    mergeSortUtil(arr,0,n-1);
}

void mergeSortParallelUtil(vector<int> &arr,int left, int right){
    if(left >= right) return;
    if(right-left < 1000){
        mergeSortUtil(arr,left,right);
        return;
    }
    int mid = left + (right - left)/2;

    #pragma omp task shared(arr)
    mergeSortParallelUtil(arr,left,mid);

    #pragma omp task shared(arr)
    mergeSortParallelUtil(arr,mid+1,right);

    #pragma omp taskwait
    merge1(arr,left,mid,right);

}

void mergeSortParallel(vector<int> &arr){
    int n = arr.size();
    #pragma omp parallel
    {
        #pragma omp single
        {
            mergeSortParallelUtil(arr,0,n-1);
        }
    }
}

void printArray(vector<int> &arr){
    for(int i = 0; i < arr.size(); i++){
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main(){
    int n = 100000;
    vector<int> arr(n);

    srand(time(0));

    for(int i = 0; i < n; i++) arr[i] = rand() % n;

    double t1,t2;

    t1 = omp_get_wtime();
    vector<int> arr1 = arr;
    bubbleSort(arr1);
    t2 = omp_get_wtime();
    cout << "Time required for sequential Bubble Sort : " << t2 - t1 << " seconds " << endl;

    t1 = omp_get_wtime();
    vector<int> arr2 = arr;
    bubbleSortParallel(arr2);
    t2 = omp_get_wtime();
    cout << "Time required for parallel Bubble Sort : " << t2 - t1 << " seconds " << endl;

    t1 = omp_get_wtime();
    vector<int> arr3 = arr;
    mergeSort(arr3);
    t2 = omp_get_wtime();
    cout << "Time required for sequential Merge Sort : " << t2 - t1 << " seconds " << endl;

    t1 = omp_get_wtime();
    vector<int> arr4 = arr;
    mergeSortParallel(arr4);
    t2 = omp_get_wtime();
    cout << "Time required for parallel Merge Sort : " << t2 - t1 << " seconds " << endl;

    return 0;
}

