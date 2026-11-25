#include <iostream>
using namespace std;

void maxheapify(int arr[], int n, int i) {
    int largest = i;

    int left = 2*i+1;
    int right = 2*i+2;

    if(left<n && arr[left] > arr[largest]) {
        largest = left;
    }

    if(right<n && arr[right] > arr[largest]) {
        largest = right;
    }

    if(largest!=i) {
        // perform swapping!!

        int t = arr[i];
        arr[i]=arr[largest];
        arr[largest]=t;

        maxheapify(arr, n, largest);
    }
}

void minheapify(int arr[], int n, int i) {
    int smallest = i;

    int left = 2*i+1;
    int right = 2*i+2;

    if(left<n && arr[left] > arr[smallest]) {
        smallest = left;
    }

    if(right<n && arr[right] > arr[smallest]) {
        smallest = right;
    }

    if(smallest!=i) {
        // perform swapping!!

        int t = arr[i];
        arr[i]=arr[smallest];
        arr[smallest]=t;

        minheapify(arr, n, smallest);
    }
}

void decHeapSort(int arr[], int n) {
    for(int i=n/2 - 1; i>=0; i--) {
        minheapify(arr,n,i);
    }
    for(int i = n-1; i>0; i--) {
        int temp = arr[0];
        arr[0]=arr[i];
        arr[i]=temp;

        minheapify(arr,i,0);
    } 
}