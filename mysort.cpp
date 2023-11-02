//Jimmy Tran
// Class (CECS 325-02)
// Prog 5 threads
// 11/2/23
//
// I certify that this program is my own original work. I did not copy any part of this program from
// any other source. I further certify that I typed each and every line of code in this program.
#include <thread>
#include <iostream>
#include <unistd.h>
#include <fstream>
using namespace std;

// initialize threads and array size (1 million in this case)
const int numThreads = 8;
const int arr_size = 1000000;

int numArray[arr_size];

// merge function to combine the subarrays
void merge(int a1[], int size1, int a2[], int size2, int mergeId){
    int tempArray[size1 + size2];
    int i = 0;
    int left = 0;
    int right = 0;

    // compares each value from the two subarrays, append to temp array if smaller
    while ((left < size1) && (right < size2)) {
        if (a1[left] <= a2[right]) {
            tempArray[i] = a1[left];
            ++left;
            ++i;
        } else {
            tempArray[i] = a2[right];
            ++right;
            ++i;
        }
    }

    // adds remaining values from left subarray to the temp array
    while (left < size1) {
        tempArray[i] = a1[left];
        ++i;
        ++left;
    }

    // adds remaining values from right subarray to the temp array
    while (right < size2) {
        tempArray[i] = a2[right];
        ++i;
        ++right;
    }

    // copies values from temp array back to main array
    int j = 0;
    for (int i = (mergeId * (size1 + size2)); i < ((mergeId + 1) * (size1 + size2)); i++) {
        numArray[i] = tempArray[0];
        j++;
    }

}

// function adjusted to follow the guidelines of pthread_create
void bubblesort(int array[], int size) {
    for (int i = 0; i < size; ++i) {
        bool swapped = false;

        for (int j = 0; j < size - 1; ++j) {
            if (array[j] > array[j + 1]) {
                swap(array[j], array[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}


int main(int argc, char *argv[] ) {
    //opens numbers.dat to be read, copies the numbers to the array
    ifstream fin;
    fin.open(argv[1], ios::in);
    int num;
    int index = 0;

    while (fin >> num) {
        numArray[index] = num;
        ++index;
    }

    fin.close();

    int chunksize = arr_size/numThreads;

    //create the threads to be performed, 8 in total
    thread thread1(bubblesort, numArray, chunksize);
    thread thread2(bubblesort, numArray + 125000, chunksize);
    thread thread3(bubblesort, numArray + 250000, chunksize);
    thread thread4(bubblesort, numArray + 375000, chunksize);
    thread thread5(bubblesort, numArray + 500000, chunksize);
    thread thread6(bubblesort, numArray + 625000, chunksize);
    thread thread7(bubblesort, numArray + 750000, chunksize);
    thread thread8(bubblesort, numArray + 875000, chunksize);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();

    //merge calls to combine the thread subarrays
    merge(numArray, 125000, numArray + 125000, 125000, 0); 
    merge(numArray + 250000, 125000, numArray + 375000, 125000, 1);
    merge(numArray + 500000, 125000, numArray + 625000, 125000, 2);
    merge(numArray + 750000, 125000, numArray + 875000, 125000, 3);
    merge(numArray, 250000, numArray + 250000, 250000, 0);
    merge(numArray + 500000, 250000, numArray + 750000, 250000, 1);
    merge(numArray, 500000, numArray + 500000, 500000, 0);
    
    ofstream fout;
    fout.open(argv[2], ios::out);
    for (int i = 0; i < arr_size; ++i) {
        fout << numArray[i] << endl;
    }

    fout.close();
}